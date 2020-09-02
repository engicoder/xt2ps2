/* =======================================================================
 * ps2d_xcvr.c
 *
 * Purpose:
 *  Transmit and receive data using the PS/2 device bus.
 *
 * Operational Summary:
 *  Transmitting a byte on the PS/2 bus:
 *   When Ps2dXcvr_TransmitByteAsync() is called, the provided data
 *   is copied into the transmit buffer and then into the transmit
 *   register. The RTS interrupt is disabled and the timer started.
 *   Timer interrupts trigger the CLOCK and DATA interrupts that 
 *   transmit the data.
 *   The CLOCK interrupt is responsible for driving the CLOCK line to 
 *   generate the PS/2 CLOCK signal at the correct interval. 
 *   The DATA ISR is triggered at the half pulse to ensure the data 
 *   will be available when the clock ISR transitions the CLOCK line state.
 *   The DATA ISR reads the data from the send register and transmits
 *   it one bit at a time. Transmission, including start and stop bits,
 *   is controlled using a state machine that advances every time the
 *   DATA ISR is called.  After each CLOCK pulse, the CLOCK ISR also checks to
 *   for a LOW state on the CLOCK line, which indicates a host initiated 
 *   INHIBIT state, i.e. the PS/2 host it trying to send a command. If this
 *   state is detected transmission of data is halted and an error set.
 *
 * Receiving a byte from the PS/2 bus:
 *   The PS/2 host will indicate the desire to send data by holding
 *   the clock low for > 100 us. This condition is detected by this
 *   subsystem in either the periodic update or during transmission of
 *   data. When the condition is detected, the receive function is
 *   called, which waits for the data line to be pulled low and the
 *   the clock line released, which indicates the Ready to Send(RTS)
 *   state. The host will then transmit the a single byte. The device
 *   is responsible for generating the clock, so the timer is started
 *   to trigger clock and data interrupts.
 *   Data bits are read by the DATA ISR when the CLOCK is high. Reception
 *   of the data bits, including the start, stop bits, and validation
 *   of the parity bit is managed by a state machine. If an error occurs
 *   and error status is set and the timer stopped. Once the entire byte
 *   has been successfully received, it is loaded into the receive buffer.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 
#include "ps2d_xcvr_config.h"
#include "ps2d_xcvr.h"
#include "ps2d_xcvr_hal.h"
#include "ps2_command.h"
#include "atomic_hal.h"

#include "console.h"

#include "con_msg_ps2d_xcvr.h"




typedef enum 
{
    DISABLED,
    IDLE,
    INHIBIT,
	RECEIVING,
	TRANSMITTING,
} XcvrState;


/* Provides status to the caller regarding the state of Ps2dXcvr */
volatile uint8_t _ps2dXcvrStatus = (uint8_t)PS2D_XCVR_STATUS_NONE;

/* Provides a time base for calculating durations */
volatile uint16_t _ps2dXcvrClockCount = 0;

/* Clock count since the bus last became idle */
volatile uint16_t _ps2dXcvrIdleCount = 0;

/* Tracks the state of the PS/2 bus */
static volatile XcvrState _xcvrState = DISABLED;

/* Used to transfer data in an out of the interrupt driven 
 * transmission routines */
static volatile uint8_t _xmitBuffer;
static volatile uint8_t _xmitRegister;
static volatile uint8_t _recvBuffer;
static volatile uint8_t _recvRegister;
static volatile uint8_t _lastXmit;


/* Convenience functions to manage state local variables */
static inline bool StatusIsSet(Ps2dXcvrStatus status)  __attribute__((always_inline));
static inline void StatusSet(Ps2dXcvrStatus status)  __attribute__((always_inline)); 
static inline void StatusClear(Ps2dXcvrStatus status)  __attribute__((always_inline));
static inline void StatusReset(void)  __attribute__((always_inline));
static inline void StatusResetRecv(void)  __attribute__((always_inline));

static inline void XcvrStateSet(XcvrState state)  __attribute__((always_inline));
static inline bool XcvrStateIs(XcvrState state)  __attribute__((always_inline));
static inline XcvrState XcvrStateGet(void) __attribute__((always_inline));


/* ------------------------------------------------------------------------
 *  Initialize the PS/2 Device Trancevier
 *   - Initialize the bus timer
 *   - Set bus state to IDLE
 *   - Start the bus timer
 *  TODO: add options support
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dXcvr_Init(void)
{
    XcvrStateSet(DISABLED);

    /* Initialize the bus timer and clock */
    Ps2dXcvrHal_BusTimerInit();

    /* Set bus state to IDLE */
    Ps2dXcvrHal_ClockHigh();
    Ps2dXcvrHal_DataHigh();

    StatusReset();

    _ps2dXcvrClockCount = 0;
    _ps2dXcvrIdleCount = 0;
	Ps2dXcvrHal_BusTimerStart();        

    CONSOLE_SEND8(CON_SRC_PS2D_XCVR, CON_SEV_TRACE_INFO, CON_MSG_PS2D_XCVR_CLK_PERIOD, PS2_CLOCK_PERIOD);
}

/* ------------------------------------------------------------------------
 *  Returns the last received byte of data to the user and resets state to 
 *  IDLE.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
uint8_t Ps2dXcvr_ReadReceivedData(void)
{
    uint8_t data;
    data = _recvBuffer;

#ifdef USE_CONSOLE
    if (Ps2dXcvr_StatusIsSet(PS2D_XCVR_RECV_FRAME_ERROR)) {
        CONSOLE_SEND0(CON_SRC_PS2D_XCVR, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_XCVR_RECV_ERROR);
    }
#endif

    CONSOLE_SEND8(CON_SRC_PS2D_XCVR, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_XCVR_RECV, data);

    StatusResetRecv();

    return data;
}

/* ------------------------------------------------------------------------
 *  Eanbles the Transceiver. Clears all status and buffers. Sets bus to IDLE.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dXcvr_Enable(void)
{
    StatusReset();
    Ps2dXcvrHal_ClockHigh();
    Ps2dXcvrHal_DataHigh();

    if (!Ps2dXcvrHal_ClockIsHigh())
    {
        XcvrStateSet(INHIBIT);
    }
    else
    {
        XcvrStateSet(IDLE);
    }
  
}

void Ps2dXcvr_Disable(void)
{
    XcvrStateSet(DISABLED);
    StatusReset();

    Ps2dXcvrHal_ClockHigh();
    Ps2dXcvrHal_DataHigh();

    Ps2dXcvrHal_BusTimerStop();
}



/* ------------------------------------------------------------------------
 *  Transmits the byte of data most recently transmitted.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dXcvr_RetransmitLastByte(void)
{
    if (XcvrStateIs(DISABLED))
        return;

	StatusSet(PS2D_XCVR_XMIT_BUFFER_FULL);

	CONSOLE_SEND8(CON_SRC_PS2D_XCVR, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_XCVR_REXMIT, _xmitBuffer);
    
}

/* ------------------------------------------------------------------------
 *  Determins if the bus is idle
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool Ps2dXcvr_BusIdle(void)
{
    return (XcvrStateIs(IDLE));
}

 /* ------------------------------------------------------------------------
 * Initiate the transmission of a byte of data across the PS/2 bus.
 *    - Reset existing status
 *    - Load the data to be send into the send register
 *    - Set the BUFFER_FULL status flag to indicate there is data to send.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool Ps2dXcvr_TransmitDataAsync (uint8_t data)
{
    bool result = false;

    switch (XcvrStateGet())
    {
        case DISABLED:
            break;

        case IDLE:
            if (!StatusIsSet(PS2D_XCVR_XMIT_BUFFER_FULL)) {
                StatusClear(PS2D_XCVR_XMIT_COMPLETE | PS2D_XCVR_XMIT_INTERRUPTED);
                _lastXmit = data;
                _xmitBuffer = data;
                StatusSet(PS2D_XCVR_XMIT_BUFFER_FULL);
            	CONSOLE_SEND8(CON_SRC_PS2D_XCVR, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_XCVR_XMIT, data);
                result = true;
            } else {
           	    CONSOLE_SEND0(CON_SRC_PS2D_XCVR, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_XCVR_XMIT_BUFFER_FULL);

            }

            break;

        default:
    	    CONSOLE_SEND8(CON_SRC_PS2D_XCVR, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_XCVR_XMIT_BUSY, XcvrStateGet());
            break;
    }

    return result;
}

/* ------------------------------------------------------------------------
 *  Convenience function to check if a flag is set in the status.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline bool StatusIsSet(Ps2dXcvrStatus status)
{
    return (bool)(_ps2dXcvrStatus & (uint8_t)status);
}

/* ------------------------------------------------------------------------
 *  Convenience function to set a flag in status. The specified status flag
 *  is added to the status, no previous status is cleared.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void StatusSet(Ps2dXcvrStatus status)
{
    _ps2dXcvrStatus |= (uint8_t)status;
}

/* ------------------------------------------------------------------------
 *  Convenience function to clear a flag in status. The specified status 
 *  flag is removed from the status, but other status flags are undisturbed.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
 static inline void StatusClear(Ps2dXcvrStatus status)
{
    _ps2dXcvrStatus &= ~status;
}

/* ------------------------------------------------------------------------
 *  Convenience function to reset status to STATUS_NONE.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void StatusReset(void)
{
	_ps2dXcvrStatus = PS2D_XCVR_STATUS_NONE;
}

/* ------------------------------------------------------------------------
 *  Convenience function to reset recv status.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void StatusResetRecv(void)
{
	_ps2dXcvrStatus &= ~(PS2D_XCVR_RECV_BUFFER_FULL | 
                         PS2D_XCVR_RECV_BUFFER_OVERFLOW |
                         PS2D_XCVR_RECV_FRAME_ERROR);
}



static inline void XcvrStateSet(XcvrState state) 
{
    _xcvrState = state;
}

static inline bool XcvrStateIs(XcvrState state) 
{
    return (_xcvrState == state);
}

static inline XcvrState XcvrStateGet(void)
{
    return _xcvrState;
}

 /* ------------------------------------------------------------------------
 *  ISR used to manage the PS/2 bus, including transmitting and receiving  
 *  data as well as checking bus status.
 *  Trigger timing:
 *   This ISR is triggered by a timer Compare Match interrupt.  It is 
 *   triggered 4 times per cycle of the CLOCK line. 
 *   The host latches/reads data on the rising edge of the clock pulse, 
 *   and the device does the same on falling edge.    
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
PS2D_XCVR_CLOCK_ISR()
{
    static enum 
    {
        DATA_BIT,
        DATA_CLK_HIGH_LOW,
        DATA_NOP,
        DATA_CLK_LOW_HIGH
    } dataState;

    static uint8_t bit = 0;
    static uint8_t parity = 0;

#ifdef DEBUG_TIMING
    Ps2dXcvrHal_DbgTimingLow();
#endif

    _ps2dXcvrClockCount++;

    Ps2BusState busState = Ps2dXcvrHal_BusState();

    switch(_xcvrState)
    {
        case DISABLED:
            break;

        case IDLE:
            if (busState == PS2_BUS_STATE_IDLE)
            {
                /* If data is ready to be sent, iniate transmission */
                if (StatusIsSet(PS2D_XCVR_XMIT_BUFFER_FULL))
                {
                    _xmitRegister = _xmitBuffer;
                    XcvrStateSet(TRANSMITTING);
                    dataState = DATA_BIT;
                    bit = 0;
                    _ps2dXcvrIdleCount = 0;
                    break;
                } else {
                    if (_ps2dXcvrIdleCount < UINT16_MAX)
                        _ps2dXcvrIdleCount++;
                }       
            } else {
                _ps2dXcvrIdleCount = 0;
                XcvrStateSet(INHIBIT);
            }

        case INHIBIT:
            switch (busState)
            {
                case PS2_BUS_STATE_IDLE:
                    XcvrStateSet(IDLE);
                    break;
                case PS2_BUS_STATE_INHIBIT:
                case PS2_BUS_STATE_INHIBIT_RTS:
                    break;
                case PS2_BUS_STATE_RTS:
                    XcvrStateSet(RECEIVING);
                    StatusResetRecv();
                    dataState = DATA_BIT;;
                    bit = 0;
                    break;
            }
            break;
        
        case RECEIVING:
        {
            switch(dataState)
            {
                case DATA_BIT:
                {
                    if (bit == 0) { /* start bit */
                        parity = 0;
                        _recvRegister = 0;
                    } else if (bit < 9) {
                        uint8_t bitValue = Ps2BusState_DataIsHigh(busState) ? 1 : 0;
                        parity += bitValue;
                        _recvRegister |= (bitValue << (bit-1));
                    } else if (bit == 9) {
                        uint8_t bitValue = Ps2BusState_DataIsHigh(busState) ? 1 : 0;
                        if (bitValue == (parity & 1))
                            StatusSet(PS2D_XCVR_RECV_FRAME_ERROR);
                    } else if (bit == 10) {
                        Ps2dXcvrHal_DataLow(); /* Acknowledge(ACK) by holding DATA low */
                    }
                }
                break;

                case DATA_CLK_HIGH_LOW:
                {
                    Ps2dXcvrHal_ClockLow();
                }
                break;

                case DATA_NOP:
                break;

                case DATA_CLK_LOW_HIGH:
                {
                    Ps2dXcvrHal_ClockHigh();

                    if (bit > 9) {
                        if (_ps2dXcvrStatus & PS2D_XCVR_RECV_BUFFER_FULL) {
                            StatusSet(PS2D_XCVR_RECV_BUFFER_OVERFLOW);
                        } else if (_recvRegister == PS2_CMD_RESEND) {
                            _xmitBuffer = _lastXmit;
                            StatusSet(PS2D_XCVR_XMIT_BUFFER_FULL);     
                        } else {    
                            _recvBuffer = _recvRegister;
                            StatusSet(PS2D_XCVR_RECV_BUFFER_FULL);
                        }
                        Ps2dXcvrHal_DataHigh();
                        XcvrStateSet(INHIBIT);                        
                    }
                }                            
                bit++;
                
            } // switch(DataState_Get())

            /* Cycle data state */
            dataState = (dataState + 1) & 3;
            break;
        } // case RECEIVING:
        
        case TRANSMITTING:
        {
            switch(dataState) 
            {
                case DATA_BIT:
                {
                    if (bit == 0) { /* start bit */
                        Ps2dXcvrHal_DataLow();
                        parity = 0;
                    } else if (bit < 9) { /* data bits */
                        if (_xmitRegister & ( 1 << (bit - 1))) {
                            Ps2dXcvrHal_DataHigh();
                            parity++;
                        } else {
                            Ps2dXcvrHal_DataLow();
                        }
                    } else if (bit == 9) { /* parity bit */
                        if (parity & 1)
                            Ps2dXcvrHal_DataLow();
                        else
                            Ps2dXcvrHal_DataHigh();
                    } else { /* stop bit */
                        Ps2dXcvrHal_DataHigh();
                    }
                }
                break;

                /* The clock line should be high. 
                 * - If CLK LOW, the host may be issuing a request to send.
                 *   Cancel transmit and set status to interrupted.
                 * - If CLK HIGH, transition clock to LOW
                 * --------------------------------------------------------- */
                case DATA_CLK_HIGH_LOW:
                {
                    if (!Ps2BusState_ClockIsHigh(busState)) {   
                        StatusClear(PS2D_XCVR_XMIT_BUFFER_FULL); 
				        StatusSet(PS2D_XCVR_XMIT_COMPLETE | PS2D_XCVR_XMIT_INTERRUPTED);
                        Ps2dXcvrHal_DataHigh();          
                        XcvrStateSet(INHIBIT);                                  
                    } else {
                        Ps2dXcvrHal_ClockLow();
                    }
                }
                break;                  


                case DATA_NOP:
                break;


                case DATA_CLK_LOW_HIGH:
                {
                    Ps2dXcvrHal_ClockHigh();

                    if (bit > 9) {
                        Ps2dXcvrHal_DataHigh();     
        			    StatusClear(PS2D_XCVR_XMIT_BUFFER_FULL); 
                        StatusSet(PS2D_XCVR_XMIT_COMPLETE);
                        XcvrStateSet(INHIBIT);                        
                    }                            
                    bit++;
                }
            } // switch(dataState)

            dataState = (dataState + 1) & 3;

        } // case TRANSMITTING:
        break;
    }
#ifdef DEBUG_TIMING
    Ps2dXcvrHal_DbgTimingHigh;
#endif

}