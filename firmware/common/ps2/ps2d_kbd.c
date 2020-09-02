/* ========================================================================
 * ps2d_kbd.c
 * 
 * Purpose:
 *  Handles sending scan code sequences to a PS/2 host as well as 
 *  reception and proper handling of PS/2 commands from that host.
 *
 * Operational Summary:
 *   PS/2 scan code sequences to be sent are placed in the send queue.
 *   When Update() is called, if a the send queue has data available, 
 *   a byte is read from the queue state will be set to XMIT_SCANCODE and 
 *   transmission requested via Ps2dXcvr_TransmitDataAsync(). 
 *
 * Processing and handling commands from the PS/2 host:
 *   When a command is received from the PS/2 host, it first processed
 *   by 
 * 
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE file for license details.
 * ------------------------------------------------------------------------ */

#include <stddef.h>
#include "common.h"
#include "ps2d_kbd_config.h"
#include "byte_sequence.h"
#include "circular_buffer.h"
#include "circular_buffer_util.h"

#include "ps2_sc_conv.h"

#include "ps2d_xcvr.h"
#include "ps2d_kbd.h"
#include "console.h"
#include "con_msg_ps2d_kbd.h"


/* === Type Definitions =============================================== */
typedef enum
{
    PS2D_KBD_IDLE,
    PS2D_KBD_POR_WAIT,
    PS2D_KBD_RESET_WAIT_ACK,
    PS2D_KBD_BAT_WAIT,
    PS2D_KBD_BAT_XMIT,
    PS2D_KBD_XMIT,
} KbdState;


#ifdef USE_TYPEMATIC

typedef enum _TypematicState
{
    TM_INACTIVE,
    TM_DELAY,
    TM_ACTIVE
} TypematicState;

#endif


/* === Constant Defintions ============================================ */
#define PS2D_KBD_MAX_ID_LENGTH 2

#define PS2D_KBD_MIN_POR_DURATION 150UL /* 150 - 2000 milliseconds - from IBM docs */
#define PS2D_KBD_MAX_POR_DURATION 2000UL 
#define PS2D_KBD_MIN_BAT_DURATION 300UL /* 300 - 500 milliseconds - from IBM docs */
#define PS2D_KBD_MAX_BAT_DURATION 500UL 


#ifdef PS2D_KBD_POR_DURATION
    #if ((PS2D_KBD_POR_DURATION < PS2D_KBD_MIN_POR_DURATION ) || (PS2D_KBD_POR_DURATION > PS2D_KBD_MAX_POR_DURATION))
        #error "PS2D_KBD_POR_DURATION has invalid value."
    #endif
#else
    #define PS2D_KBD_POR_DURATION PS2D_KBD_MIN_POR_DURATION
#endif

#define POR_CLOCK_COUNT (uint16_t)PS2D_XCVR_INTERVAL_MS_TO_CLK_COUNT(PS2D_KBD_POR_DURATION)

#ifdef PS2D_KBD_BAT_DURATION
    #if ((PS2D_KBD_BAT_DURATION < PS2D_KBD_MIN_BAT_DURATION ) || (PS2D_KBD_BAT_DURATION > PS2D_KBD_MAX_BAT_DURATION))
        #error "PS2D_KBD_BAT_DURATION has invalid value."
    #endif
#else
    #define PS2D_KBD_BAT_DURATION PS2D_KBD_MIN_BAT_DURATION
#endif

#define BAT_CLOCK_COUNT (uint16_t)PS2D_XCVR_INTERVAL_MS_TO_CLK_COUNT(PS2D_KBD_BAT_DURATION)


#define PS2D_KBD_DEFAULT_SCAN_CODE_SET PS2_SCAN_CODE_SET2

/* From IBM docs: typematic interval = (8 + A) * 2^B * 0.00417 
 *  where A = bits[2:0]; B = bits[4:3] */
#define PS2D_KBD_TYPEMATIC_INTERVAL(bits) \
    ( (( 8 + (bits & 0x7)) * (1 << ((bits & 0x3) >> 3)) * 417UL / 100000UL ) 

/* From IBM docs: typematic delay = (1 + A) * 250
 *  where A = bits[6:5] */
#define PS2D_KBD_TYPEMATIC_DELAY(bits) \
    ( (1 + ((bits  & b01100000) >> 5)) * 250UL )

/* Default rate values 92ms and 500ms b01001011 */
#define PS2D_KBD_DEFAULT_TYPEMATIC_RATE 0x4B

/* Delay between consecutive bytes sent*/
#define INTER_BYTE_DELAY_CLOCKS (uint16_t)PS2D_XCVR_INTERVAL_US_TO_CLK_COUNT(PS2D_KBD_INTER_BYTE_DELAY)

/* === Exported Variables =============================================== */


/* === Local Variables =============================================== */
static uint8_t _sendBufferStorage[PS2D_SEND_STORAGE_SIZE]; //NOTE: Buffer must stay in RAM
static CircularBuffer _sendBuffer; /* Holds data ready to be sent to the host */

static uint8_t _ps2Id[PS2D_KBD_MAX_ID_LENGTH] = PS2D_KBD_DEVICE_ID;
static uint8_t _ps2IdLength = PS2D_KBD_MAX_ID_LENGTH;

static KbdState _state = PS2D_KBD_IDLE;
static uint16_t _clockCountStart;

static Ps2KeyCondition _keyConditions[KEY_CODE_COUNT];

#ifdef USE_TYPEMATIC

static TypematicState _typematicState = TM_INACTIVE;
static KeyCode _typematicActiveKey;
static uint16_t _typematicCount;

static uint16_t _typematicDelay;
static uint16_t _typematicInterval;

static ByteSequence* _typematicSequence;
static uint8_t _typematicSequenceStorage[PS2_MAX_SEQUENCE_LENGTH + 1];

#endif


static bool defaultBatHandler(void){ return true; }
static void defaultLedStatusUpdateHandler(Ps2LedStatus status) { }
static void defaultResetHandler(void) { }

static bool _enabled = false;

static Ps2ScanCodeSet _scanCodeSet = PS2_SCAN_CODE_SET2;

/* Funcion callack invoked when BAT is exectued. */
static Ps2dKbd_BatHandler _batHandler = &defaultBatHandler;
static bool _batSuccess;
/* Function callback invoked when a Set LED command is received from the host */
static Ps2dKbd_LedStatusUpdate _ledStatusUpdateHandler = &defaultLedStatusUpdateHandler;
/* Function callback invoked when a Reset command is received from the host */
static Ps2dKbd_ResetReceived _resetHandler = &defaultResetHandler;


/* === Forward declarations =========================================== */
static void BatCheckComplete(void);
static void BatXmit(void);
static void PorCheckComplete(void);
static void ResetAckCheckComplete(void);
static void BatInitiate(void);
static void PorInitiate(void);
static void ResetInitiate(void);
static void ProcessReceivedData(uint8_t data);
static void SendPs2Id(void);
static void SendResponse(uint8_t response);
static Ps2KeyCondition KeyCondition(KeyCode keycode);


static void TypematicInit(void);
static void TypematicReset(void);
static void TypematicCheck(void);
static void TypematicOnKeyEvent(KeyEvent* keyEvent, ByteSequence* sequence);
static void TypematicUpdateRate(uint8_t bits);

/* TODO for Scan code set 3 support
static void TypematicSetAllKeyTypes(TypematicKeyType type);
static void TypematicSetKeyType(uint8_t scanCode, TypematicKeyType type);
*/

void Ps2dKbd_SetId(uint8_t* id, uint8_t idLength)
{
    if (idLength > PS2D_KBD_MAX_ID_LENGTH)
        idLength = PS2D_KBD_MAX_ID_LENGTH;

    for (int i = 0; i < idLength; i++)
    {
        _ps2Id[i] = id[i];
    }
}


void Ps2Kbd_SetScanCodeSet(Ps2ScanCodeSet scanCodeSet)
{
    _scanCodeSet = scanCodeSet;

    switch (_scanCodeSet)
    {
        case PS2_SCAN_CODE_SET1:
            break;

        /* Scan code set 2: All keys make/break/typematic except PAUSE which is make only */
        case PS2_SCAN_CODE_SET2:
            for (int i = 0; i < sizeof(_keyConditions); i++)
            {
                _keyConditions[i] = PS2_KEY_COND_ALL;
            }
            _keyConditions[KC_PAUSE] = PS2_KEY_COND_MAKE;
   
            break;

        case PS2_SCAN_CODE_SET3:
            break;
    }

    return;
}


/* ------------------------------------------------------------------------
 *  Initialize the PS/2 Device subsystem
 *    - Initialize the internal send queue
 *    - Set default scan code set
 *    - Initialize the PS2 transceiver
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dKbd_Init(Ps2dKbd_BatHandler batHandler,
                  Ps2dKbd_LedStatusUpdate ledStatusUpdateHandler,
                  Ps2dKbd_ResetReceived resetHandler)
{
	_enabled = false;

    _batHandler = batHandler;
    _ledStatusUpdateHandler = ledStatusUpdateHandler;
    _resetHandler = resetHandler;

    CircularBuffer_Init(&_sendBuffer, _sendBufferStorage, PS2D_SEND_STORAGE_SIZE);

    CONSOLE_SEND8(CON_SRC_PS2D_KBD, CON_SEV_TRACE_INFO, CON_MSG_PS2D_KBD_CLKS_MS, PS2D_XCVR_CLOCKS_PER_MS);
    
    TypematicInit();

    Ps2Kbd_SetScanCodeSet(PS2D_KBD_DEFAULT_SCAN_CODE_SET);

    Ps2dXcvr_Init();

}

/* ------------------------------------------------------------------------
 * Transmits a PS/2 byte sequence across the PS/2 bus to the host.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dKbd_SendSequence(const ByteSequence* sequence)
{
	if (_enabled)
	{
        for (int i = 0; i < ByteSequence_Length(sequence); i++)
        {
            /* Insert an overrun indicator if the buffer is full */
            if (CircularBuffer_Count(&_sendBuffer) > CircularBuffer_Size(&_sendBuffer) - PS2D_KBD_MAX_ID_LENGTH) 
            {
                CircularBuffer_ReplaceIndex(&_sendBuffer, CircularBuffer_Count(&_sendBuffer) - 1, 0xFF);
            }
            else
            {
		        CircularBuffer_Insert(&_sendBuffer, ByteSequence_DataAt(sequence, i));
            }
        }
	}
}

/* ------------------------------------------------------------------------
 *  Start the PS/2 device keyboard subsystem
 *   Reset the device.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dKbd_Start(void)
{
    PorInitiate();
}
/* ------------------------------------------------------------------------
 *  Periodic update task for the PS/2 Keyboard subsystem
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dKbd_Task(void)
{
    uint8_t data;

    switch(_state)
    {
        case PS2D_KBD_IDLE:
            TypematicCheck();
            break;

        case PS2D_KBD_BAT_WAIT:
            BatCheckComplete();
            break;

        case PS2D_KBD_BAT_XMIT:
            BatXmit();
            break;

        case PS2D_KBD_POR_WAIT:
            PorCheckComplete();
            break;


        case PS2D_KBD_XMIT:
            {
                Ps2dXcvrStatus status = Ps2dXcvr_GetStatus();
                if (status & PS2D_XCVR_XMIT_COMPLETE) {
                    if (status & PS2D_XCVR_XMIT_INTERRUPTED) {
                        CONSOLE_SEND0(CON_SRC_PS2D_KBD, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_KBD_XMIT_INT);
                    } else {
                        CircularBuffer_Remove(&_sendBuffer);
                    }
                    _state = PS2D_KBD_IDLE;
                }                
            }
            break;

        case PS2D_KBD_RESET_WAIT_ACK:
            ResetAckCheckComplete();
            break;
    }

    if (_state == PS2D_KBD_IDLE) {
        if (Ps2dXcvr_DataReceived()) {
            bool frameError = Ps2dXcvr_StatusIsSet(PS2D_XCVR_RECV_FRAME_ERROR);

            data = Ps2dXcvr_ReadReceivedData();

            if (frameError) {
                CircularBuffer_Insert(&_sendBuffer, PS2_CMD_RESEND);
            } else {
                ProcessReceivedData(data);
            }
        } else if (!CircularBuffer_IsEmpty(&_sendBuffer)) {
            if (Ps2dXcvr_BusIdle()) {
                if (Ps2dXcvr_GetIdleCount() > INTER_BYTE_DELAY_CLOCKS) {
                    data = CircularBuffer_Peek(&_sendBuffer);
                    if (Ps2dXcvr_TransmitDataAsync(data))
                        _state = PS2D_KBD_XMIT;
                }
            }
        }
    }

    return;
}


void Ps2dKbd_OnKeyEvent(KeyEvent* keyEvent)
{
    bool active = true;

    if (!_enabled)
        return;

    CONSOLE_SEND88(CON_SRC_PS2D_KBD, CON_SEV_TRACE_INFO, CON_MSG_PS2D_KBD_KEYEVENT, KeyEvent_Code(keyEvent), KeyEvent_Action(keyEvent));

    ByteSequence* sendSequence = Ps2ScanCodeConvert(keyEvent, _scanCodeSet);

    TypematicOnKeyEvent(keyEvent, sendSequence);

    KeyCode keyCode = KeyEvent_Code(keyEvent);

    if (KeyEvent_IsPress(keyEvent))
        active = ((KeyCondition(keyCode) & PS2_KEY_COND_MAKE) != 0);
    else 
        active = ((KeyCondition(keyCode) & PS2_KEY_COND_BREAK) != 0);

    if (active)
        Ps2dKbd_SendSequence(sendSequence);

}

void BatCheckComplete(void)
{
    uint16_t currentClockCount = Ps2dXcvr_GetClockCount();
    uint16_t countSinceStart = (currentClockCount - _clockCountStart);

    /* Waiting for ACK indication from server to complete*/
    if ( countSinceStart < BAT_CLOCK_COUNT ) {
        return;
    }

    CONSOLE_SEND0(CON_SRC_PS2D_KBD, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_KBD_BAT_END);

    _ledStatusUpdateHandler(PS2_LED_ALL_OFF);

	Ps2dXcvr_Enable();

    _state = PS2D_KBD_BAT_XMIT;

    return;
}

void BatXmit(void)
{
    uint8_t batResponse;

    /* Wait for host to release buss */
    if (Ps2dXcvr_BusIdle()) {
        _enabled = _batSuccess;

        if (_batSuccess) {
            batResponse = PS2_RESP_SELF_TEST_OK;
        } else {
            batResponse = PS2_RESP_SELF_TEST_FAILED;
        }

        _state = PS2D_KBD_IDLE;
        CircularBuffer_Insert(&_sendBuffer, batResponse);
    }
}


void PorCheckComplete(void)
{
    uint16_t currentClockCount = Ps2dXcvr_GetClockCount();
    uint16_t countSinceStart = (currentClockCount - _clockCountStart);    

    /* Waiting for ACK indication from server to complete*/
    if (countSinceStart < POR_CLOCK_COUNT)
    {
        return;
    }

    CONSOLE_SEND0(CON_SRC_PS2D_KBD, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_KBD_POR_END);

    BatInitiate();
}

void BatInitiate(void)
{
    _state = PS2D_KBD_BAT_WAIT;

    CONSOLE_SEND16(CON_SRC_PS2D_KBD, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_KBD_BAT_START, BAT_CLOCK_COUNT);

    _clockCountStart = Ps2dXcvr_GetClockCount();

    _ledStatusUpdateHandler(PS2_LED_ALL_ON);

    _batSuccess = _batHandler();

    CircularBuffer_Clear(&_sendBuffer);

    TypematicReset();

    BatCheckComplete();

    return;
}

void PorInitiate(void)
{
    _clockCountStart = Ps2dXcvr_GetClockCount();    
    _state = PS2D_KBD_POR_WAIT;

    CONSOLE_SEND16(CON_SRC_PS2D_KBD, CON_SEV_TRACE_EVENT, CON_MSG_PS2D_KBD_POR_START, POR_CLOCK_COUNT);


    return;
}

void ResetInitiate(void)
{
    _enabled = false;

    /* Try to send RESET ACK, if fails, go to RESET_WAIT_ACK
     * else start BAT */
    if (!Ps2dXcvr_TransmitDataAsync(PS2_RESP_ACK))
        _state = PS2D_KBD_RESET_WAIT_ACK;
    else
        BatInitiate();

    return;
}

/* Host may inhibit bus after reset ack. 
   Wait for bus to be released */
void ResetAckCheckComplete(void)
{
    /* Waiting for ACK indication from server to complete*/
    if (!Ps2dXcvr_TransmitDataAsync(PS2_RESP_ACK))
        return;

    /* ACK indication from server complete */
    BatInitiate();
}


static void ProcessReceivedData(uint8_t data)
{
    static Ps2KeyboardCommand command;
    static bool waitingForParameter = false;

    uint8_t parameter = 0;
    Ps2Response response = PS2_RESP_ACK;

    if (waitingForParameter) {
        parameter = data;
    } else {
        command = (Ps2KeyboardCommand)data;
        CONSOLE_SEND8(CON_SRC_PS2D_KBD, CON_SEV_TRACE_INFO, CON_MSG_PS2D_KBD_CMD_RECV, data);        
    }

    if (waitingForParameter)
    {
        switch(command)
        {
            case PS2_CMD_KB_LEDS:
            {
                Ps2LedStatus ledStatus = (Ps2LedStatus)parameter;
                _ledStatusUpdateHandler(ledStatus);
                break;
            }

            case PS2_CMD_KB_REPEAT_RATE:
                TypematicUpdateRate(parameter);
                break;

            case PS2_CMD_KB_SCANCODESET:
                Ps2Kbd_SetScanCodeSet((Ps2ScanCodeSet)parameter);
                break;

            default:
                break;
        }
        waitingForParameter = false;

    }
    else
    {
        switch(command)
        {
            case PS2_CMD_KB_ID:
                SendPs2Id();
                _state = PS2D_KBD_IDLE;
                break;

            case PS2_CMD_KB_ECHO:
                response = PS2_RESP_ECHO;
                break;

            case PS2_CMD_KB_DISABLE:
                CircularBuffer_Clear(&_sendBuffer);
                _enabled = false;
                break;
            
            case PS2_CMD_KB_ENABLE:
                _enabled = true;
                break;

            case PS2_CMD_KB_RESET:
                ResetInitiate();
                break;

            case PS2_CMD_KB_REPEAT_RATE:
            case PS2_CMD_KB_LEDS:
            case PS2_CMD_KB_SCANCODESET:            
                waitingForParameter = true;
                break;

            default:
                break;
        }
    }

    SendResponse(response);
    
}


void SendResponse(uint8_t response)
{
    CircularBuffer_Push(&_sendBuffer, response);
    CONSOLE_SEND8(CON_SRC_PS2D_KBD, CON_SEV_TRACE_INFO, CON_MSG_PS2D_KBD_RSP_SENT, response);     
}

void SendPs2Id(void)
{
    if (_ps2IdLength == 0)
        return;

    uint8_t count = CircularBuffer_Count(&_sendBuffer);

    /* Insert id data in send buffer  and then cycle all existing bytes */
    for (int i = 0; i < _ps2IdLength; i++)
    {
        CircularBuffer_Insert(&_sendBuffer, _ps2Id[i]);
    }

    for (int i = 0; i < count; i++)
    {
        uint8_t code = CircularBuffer_RemoveN(&_sendBuffer, _ps2IdLength);
        CircularBuffer_Insert(&_sendBuffer, code);
    }

}


static Ps2KeyCondition KeyCondition(KeyCode keyCode)
{
    return _keyConditions[keyCode];
}


/* Typematic key support functions 
 * -------------------------------------------------------------------------------- */
#ifdef USE_TYPEMATIC        

void TypematicInit(void)
{
    _typematicState = TM_INACTIVE;
    TypematicUpdateRate(PS2D_KBD_DEFAULT_TYPEMATIC_RATE);
    
    _typematicSequence = ByteSequence_Create(_typematicSequenceStorage, sizeof(_typematicSequenceStorage));
}

void TypematicReset(void)
{
    _typematicState = TM_INACTIVE;
}

void TypematicCheck(void)
{
    bool sendKeyPress = false;

    switch (_typematicState)
    {
        case TM_INACTIVE:
            break;

        case TM_DELAY: 
            {
                uint16_t currentClockCount = Ps2dXcvr_GetClockCount();
                uint16_t interval = currentClockCount - _typematicCount;
                if (interval > _typematicDelay)
                {
                    _typematicState = TM_ACTIVE;
                    _typematicCount = currentClockCount;
                    sendKeyPress = true;
                    CONSOLE_SEND8(CON_SRC_PS2D_KBD, CON_SEV_TRACE_INFO, CON_MSG_PS2D_KBD_TM_ACTIVE, _typematicActiveKey);                    
                }
            }
            break;

        case TM_ACTIVE:
            {
                uint16_t currentClockCount = Ps2dXcvr_GetClockCount();
                uint16_t interval = currentClockCount - _typematicCount;
                if (interval > _typematicInterval)
                {
                    _typematicCount = currentClockCount;
                    sendKeyPress = true;                
                } 
            }       
            break;
    }

    if (sendKeyPress)
    {
        Ps2dKbd_SendSequence(_typematicSequence);        
    }

    return;
}

void TypematicOnKeyEvent(KeyEvent* keyEvent, ByteSequence* sequence)
{
    KeyCode code = KeyEvent_Code(keyEvent);

    if (KeyEvent_IsPress(keyEvent))
    {
        if (KeyCondition(code) & PS2_KEY_COND_TYPEMATIC)
        {
            _typematicActiveKey = code;
            ByteSequence_Duplicate(_typematicSequence, sequence);
            _typematicState = TM_DELAY;
            _typematicCount = Ps2dXcvr_GetClockCount();
        }
    }
    else if (code == _typematicActiveKey)
    {
#ifdef USE_CONSOLE
        if(_typematicState == TM_ACTIVE)
        {
            CONSOLE_SEND0(CON_SRC_PS2D_KBD, CON_SEV_TRACE_INFO, CON_MSG_PS2D_KBD_TM_INACTIVE);
        }
#endif
        _typematicState = TM_INACTIVE;
    }
}


void TypematicUpdateRate(uint8_t bits)
{
    /* From IBM docs: typematic interval = (8 + A) * 2^B * 0.00417 
     *  where A = bits[2:0]; B = bits[4:3] 
     * Note: approximate 4.17ms with 4 using a 2 bit left shift at runtime 
     *       This equates to 4.25% error which is acceptable */
//    _typematicInterval =  (uint16_t) (( 8 + (bits & 0x7)) * (1 << ((bits & 0x24) >> 3)) * 17UL / 4096UL ) ;
    uint16_t A = bits & 0x07;
    uint16_t B = (bits & 0x18) >> 3;

    _typematicInterval =  (uint16_t) ((8 + A) * (1 << B) << 2 ) * PS2D_XCVR_CLOCKS_PER_MS;

    CONSOLE_SEND16(CON_SRC_PS2D_KBD, CON_SEV_TRACE_INFO, CON_MSG_PS2D_KBD_TM_RATE, _typematicInterval);

    /* From IBM docs: typematic delay = (1 + A) * 250
     *  where A = bits[6:5] 
     * Note: approximate 250 with 256 with an 8 bit left shift at runtime 
     *       This results in a 2.4% error which is acceptable */
    A = (bits & 0x60) >> 5;
    _typematicDelay = (uint16_t)( (1 + A) << 8 ) * PS2D_XCVR_CLOCKS_PER_MS;

    CONSOLE_SEND16(CON_SRC_PS2D_KBD, CON_SEV_TRACE_INFO, CON_MSG_PS2D_KBD_TM_DELAY, _typematicDelay);
}


#else

void TypematicInit(void){}
void TypematicReset(void){}
void TypematicCheck(void){}
void TypematicOnKeyEvent(KeyEvent* keyEvent, ByteSequence* sequence){ }
void TypematicUpdateRate( uint8_t bits){}

#endif

