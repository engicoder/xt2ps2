/* ========================================================================
 * xth_xcvr_xcvr.c
 * 
 * Purpose:
 *  Handles detection and reception of scan codes from a PC/XT keyboard.
 *
 * Operational Summary:
 *  The XT clock line is connected to a pin having external interrupt
 *  functionality. The interrupt is triggered on the signal's falling edge.
 *  Each time the interrupt is triggered, the receive state is advanced.
 *  For states corresponding to data bits, the state of the data line is
 *  read and the value of the corresponding bit in the received data is 
 *  updated. When the entire frame has been received, the received scan code 
 *  is added to the receive queue. Queued scan codes are retrieved from 
 *  the receive queue using the XthXcvr_DataReceeved() and 
 *  XthXcvr_ReadReceivedData() functions.
 * 
 * Special Error Handling:
 *  When the START bit is received an error task is scheduled.
 *  The period of the task should be greater than the longest possible time
 *  required to completely receive a single scan code from the keyboard.
 *  The purpose of this task is to re-sync the clock line interrupt and
 *  reset the receive state so that future scan codes can be properly
 *  received.
 *  Upon successful reception of a scan code, the error task is canceled.
 * 
 * Synchronizing data frame
 *  1) Keyboard is held in reset (reset low, clock low) for 20ms. Compliant
 *     keyboards will not send data until clock and/or reset is released.
 *     This should sync start of frame.
 *  2) The transceiver maintains a free running timer such that when the 
 *     counter hits 255, it rolls over to 0 (no interrupt is generated).
 *     The count of this timer is recorded and then reset on the falling 
 *     edge of the clock line. In this way the timer count indicates the 
 *     time that has elapsed since the last falling edge of the clock line.
 *     If the time is greater than a specified threshold or the overflow 
 *     flag has been set, a start of frame has been detected. This threshold 
 *     must be twice the period of the clock.
 * 
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE file for license details.
 * ------------------------------------------------------------------------ */ 

#include "common.h"
#include "xth_xcvr_config.h"
#include "xth_xcvr_hal.h"
#include "xth_xcvr.h"
#include "con_msg_xth_xcvr.h"

#include "console.h"

#define XTH_XCVR_STATUS_RECV_MASK (XTH_XCVR_STATUS_RECV_BUFFER_FULL | \
                                   XTH_XCVR_STATUS_RECV_FRAME_ERROR )

/* Receive State - the state of the frame currently being received. Used by 
 * the XT clock line ISR to coordinate across interrupt calls. */
typedef enum {
    IDLE, START1, START2, DATA0, DATA1, DATA2, DATA3, DATA4, DATA5, DATA6, DATA7, 
} ReceiveState;

typedef enum {
    XCVR_STATE_DISABLED,
    XCVR_STATE_POR,
    XCVR_STATE_SOFT_RESET,
    XCVR_STATE_IDLE,
} XcvrState;

static volatile uint8_t _receiveRegister;
static volatile uint8_t _receiveBuffer;
volatile XthXcvrStatus _xthXcvrStatus = 0;
static ReceiveState _receiveState = IDLE;
static XcvrState _xcvrState = XCVR_STATE_DISABLED;
volatile uint16_t _timerCount = 0;

static inline void StatusClear(XthXcvrStatus status)
{
    _xthXcvrStatus &= ~status;
}

static inline void StatusReset(void)
{
	_xthXcvrStatus = 0;
}

static inline void StatusResetRecv(void)
{
	_xthXcvrStatus &= ~XTH_XCVR_STATUS_RECV_MASK;
}

static inline void StatusSet(XthXcvrStatus status)
{
    _xthXcvrStatus |= status;
}

/* ------------------------------------------------------------------------
 *  Initialize XT transceiver
 *   - Clear error code
 *   - Initialize XT clock and data lines
 *   - Initialize reset line if enabled
 *   - Set inital state and status 
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthXcvr_Init(void)
{
    /* Initialize the data bus */
    XthXcvrHal_ClockInit();
    XthXcvrHal_DataInit();    

    if (XTH_XCVR_RESET_LINE_ENABLE) {
        XthXcvrHal_ResetInit();
    }

    _receiveState = IDLE;
    _xcvrState = XCVR_STATE_DISABLED;

    StatusReset();
}


/* ------------------------------------------------------------------------
 *  Read received data
 *   - Copy the contents of the receive buffer. 
 *   - Reset RECV status and clear start frame timer count.
 *   - Release the clock line to allow the keyboard to send more data
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
uint8_t XthXcvr_ReadReceivedData(void)
{
    uint8_t data = _receiveBuffer;

    if (XthXcvr_StatusIsSet(XTH_XCVR_STATUS_RECV_BUFFER_FULL)) {
        StatusResetRecv();

        if (!XthXcvr_StatusIsSet(XTH_XCVR_STATUS_KBD_DETECTED)) {
            StatusSet(XTH_XCVR_STATUS_KBD_DETECTED);
        }

        XthXcvrHal_TimerResetCount();
        XthXcvrHal_ClockRelease();

        CONSOLE_SEND8(CON_SRC_XTH_XCVR, CON_SEV_TRACE_EVENT, CON_MSG_XTH_XCVR_RECV_SCODE, data);
    }

    return data;
}

/* ------------------------------------------------------------------------
 *  Initiates a soft reset of the keyboard
 *   - Hold keyboard in reset for 20ms by holding clock and reset low
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthXcvr_SoftReset(void)
{
    if (XTH_XCVR_SOFT_RESET_ENABLE) {
        XthXcvrHal_DisableClockInterrupt();
        XthXcvrHal_TimerStop();    

        StatusReset();
        _xcvrState = XCVR_STATE_SOFT_RESET;

        XthXcvrHal_ClockHoldLow();

        CONSOLE_SEND0(CON_SRC_XTH_XCVR, CON_SEV_TRACE_EVENT, CON_MSG_XTH_XCVR_SOFT_RESET);

        _timerCount = 0;
        XthXcvrHal_TimerStartTimeout();
    }
}


/* ------------------------------------------------------------------------
 *  Handles the POR reset of the keyboard
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthXcvr_HardReset(void)
{
    XthXcvrHal_DisableClockInterrupt();
    XthXcvrHal_TimerStop();    
    XthXcvrHal_ClockRelease();

    _xcvrState = XCVR_STATE_POR;

    if (XTH_XCVR_RESET_LINE_ENABLE)
        XthXcvrHal_ResetHoldLow();

    CONSOLE_SEND0(CON_SRC_XTH_XCVR, CON_SEV_TRACE_EVENT, CON_MSG_XTH_XCVR_HARD_RESET);

    _timerCount = 0;
    XthXcvrHal_TimerStartTimeout();
}

/* ------------------------------------------------------------------------
 *  Initiates execution of the XT host subsystem
 *   - Set initial state
 *   - Issue reset to keyboard
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthXcvr_Enable(void)
{
    _receiveState = IDLE;
    StatusReset();

    if (XTH_XCVR_POR_ON_ENABLE) {
        XthXcvr_HardReset();
    } else {
        XthXcvrHal_EnableClockInterrupt();
        _timerCount = 0;
        XthXcvrHal_TimerStartDetectSof();
        _xcvrState = XCVR_STATE_IDLE;        
    }
}

/* ------------------------------------------------------------------------
 *  Halts execution of the XT host subsystem

 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthXcvr_Disable(void)
{
    XthXcvrHal_DisableClockInterrupt();
    XthXcvrHal_TimerStop();
}

/* ------------------------------------------------------------------------
 *  Timeout timer interrupt service routing
 *
 *  Invoked when the timeout timer interval has expired.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
XTH_XCVR_RESET_TIMEOUT_ISR()
{
    _timerCount++;
    switch (_xcvrState)
    {
        case XCVR_STATE_SOFT_RESET:
        {
            if (_timerCount >= XTH_XCVR_SOFT_RESET_DURATION) {
                XthXcvrHal_ClockRelease();

                XthXcvrHal_EnableClockInterrupt();
                _timerCount = 0;
                XthXcvrHal_TimerStartDetectSof();
                _xcvrState = XCVR_STATE_IDLE;
                StatusSet(XTH_XCVR_STATUS_READY);
            }
        }
        break;

        case XCVR_STATE_POR:
        {
            if (_timerCount == XTH_XCVR_HARD_RESET_DURATION) {
                if (XTH_XCVR_RESET_LINE_ENABLE) {
                    XthXcvrHal_ResetRelease(XTH_XCVR_RESET_LINE_HAS_PULLUP);
                }    
            } else if (_timerCount >= XTH_XCVR_POR_DURATION) {
                if (XTH_XCVR_SOFT_RESET_ENABLE){
                   XthXcvrHal_ClockHoldLow();
                    _timerCount = 0;
                } else {
                    _timerCount = XTH_XCVR_SOFT_RESET_DURATION;
                }
                _xcvrState = XCVR_STATE_SOFT_RESET;
            }
        }
        break;

        default:
        {
            // Error
        }
        break;

    }


    return;
}

/* ------------------------------------------------------------------------
 *  XT Clock line interrupt service routine
 *
 *  Triggered on the rising edge of the XT clock line. Manages reception of
 *  a single scan code frame from the XT device. 
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
XTH_XCVR_CLOCK_ISR()
{
	static bool frameError = false;

    bool timerOverflow = XthXcvrHal_TimerOverflow();
    uint8_t timerCount = XthXcvrHal_TimerCount();
    XthXcvrHal_TimerResetCount();

    if (timerCount > XTH_XCVR_SOF_THRESHOLD || timerOverflow) {
        StatusResetRecv();
        _receiveState = IDLE;
    }

    /* Read the state of the data line. This will be used by several states. */
    bool dataLineHigh = XthXcvrHal_DataIsHigh();

    /* Advance the frame state */
    _receiveState++;

    /* Process based on frame state */
    switch (_receiveState) 
    {
        case IDLE: /* Should never happen */
            break;
        case START1:
            _receiveRegister = 0;
            frameError = false;
            if (XTH_XCVR_1_START_BIT) {
                _receiveState = START2;
                /* !!! FALLTHROUGH to START2 case !!! */
            } else
                break;
            
        case START2:
            if (!dataLineHigh) {
                frameError = true;
            }
            break;

        case DATA0:
        case DATA1:
        case DATA2:
        case DATA3:
        case DATA4:
        case DATA5:
        case DATA6:
        case DATA7:
            
            /* Record the data line state 
             * in the next bit of the scan code */
            _receiveRegister = _receiveRegister >> 1;
            if (dataLineHigh) {
                _receiveRegister |= (1 << 7);
            }

            if (_receiveState == DATA7) {
                /* Hold clock low until host can record data and update status
                 * in XthXcvr_ReadReceivedData() */
                XthXcvrHal_ClockHoldLow();

                /* If the receive buffer is full, set BUFFER_OVERFLOW status
                * and discard data */
                if (frameError) {
                    StatusSet(XTH_XCVR_STATUS_RECV_FRAME_ERROR);
                } else if (_receiveRegister == 0xAA && 
                           !XthXcvr_StatusIsSet(XTH_XCVR_STATUS_KBD_DETECTED)) {
                    StatusSet(XTH_XCVR_STATUS_KBD_DETECTED);
                    XthXcvrHal_ClockRelease();
                } else {
                    /* Copy received data into receive buffer and set 
                    * BUFFER_FULL status. */
                    _receiveBuffer = _receiveRegister;
                    
                    StatusSet(XTH_XCVR_STATUS_RECV_BUFFER_FULL);
                }

                /* Reset the frame state to IDLE */
                _receiveState = IDLE;

            }
            break;
    }

    return;
}

