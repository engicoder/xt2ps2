/* =======================================================================
* ps2_command.h
*
* Purpose:
*  Declaration of the command and response values used in communication
*  between the device and host.
*
* License:
*  Copyright (c) 2015, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */

#ifndef PS2_COMMAND_H_
#define PS2_COMMAND_H_

/* Commands send from host to device */
typedef enum {
    PS2_CMD_NONE_RECEIVED = 0x00,
    PS2_CMD_ID = 0xF2,
    PS2_CMD_ENABLE = 0xF4,
    PS2_CMD_DISABLE = 0xF5,
    PS2_CMD_DEFAULTS = 0xF6,
    PS2_CMD_RESEND = 0xFE,
    PS2_CMD_RESET = 0xFF,
}Ps2Command;

typedef enum {
    PS2_CMD_KB_LEDS =          0xED,
    PS2_CMD_KB_ECHO,         /* 0xEE */
    PS2_CMD_KB_INVALID_0XEF, /* 0xEF */
    PS2_CMD_KB_SCANCODESET,  /* 0xF0 */
    PS2_CMD_KB_INVALID_0xF1, /* 0xF1 */
    PS2_CMD_KB_ID,           /* 0xF2 */
    PS2_CMD_KB_REPEAT_RATE,  /* 0xF3 */
    PS2_CMD_KB_ENABLE,       /* 0xF4 */
    PS2_CMD_KB_DISABLE,      /* 0xF5 */
    PS2_CMD_KB_DEFAULTS,     /* 0xF6 */
    PS2_CMD_KB_ALL_R,        /* 0xF7 */
    PS2_CMD_KB_ALL_MB,       /* 0xF8 */
    PS2_CMD_KB_ALL_M,        /* 0xF9 */
    PS2_CMD_KB_ALL_MBR,      /* 0xFA */
    PS2_CMD_KB_ONER,         /* 0xFB */
    PS2_CMD_KB_ONEMB,        /* 0xFC */
    PS2_CMD_KB_ONEM,         /* 0xFD */
    PS2_CMD_KB_RESEND,       /* 0xFE */
    PS2_CMD_KB_RESET,        /* 0xFF */    
}Ps2KeyboardCommand;

typedef enum {
    PS2_CMD_MS_REMOTE_MODE = 0xF0,
    PS2_CMD_MS_SAMPLE_RATE = 0xF3,
    PS2_CMD_MS_WRAP_MODE = 0xEE,
    PS2_CMD_MS_RESET_WRAP_MODE = 0xEC,
    PS2_CMD_MS_READ_DATA = 0xEB,
    PS2_CMD_MS_STREAM_MODE = 0xEA,
    PS2_CMD_MS_STATUS_REQUEST = 0xE9,
    PS2_CMD_MS_RESOLUTION = 0xE8,
    PS2_CMD_MS_SCALING_2 = 0xE7,
    PS2_CMD_MS_SCALING_1 = 0xE6,

}Ps2MouseCommand;



/* Bit field passed as the parameter to the PS2_CMD_LEDS command */
typedef enum {
    PS2_LED_ALL_OFF = 0,
    PS2_LED_SCROLL_LOCK = (1 << 0),
    PS2_LED_NUM_LOCK = (1 << 1),
    PS2_LED_CAPS_LOCK = (1 << 2),
    PS2_LED_ALL_ON = (PS2_LED_SCROLL_LOCK | PS2_LED_NUM_LOCK | PS2_LED_CAPS_LOCK),
} Ps2LedStatus;



/* Responses sent from the device to host */
typedef enum {
    PS2_RESP_23_ERR_OVERRUN = 0x00,
    PS2_RESP_SELF_TEST_OK = 0xAA,
    PS2_RESP_ECHO = 0xEE,
    PS2_RESP_ACK = 0xFA,
    PS2_RESP_SELF_TEST_FAILED = 0xFD,
    PS2_RESP_RESEND = 0xFE,
    PS2_RESP_1_ERR_OVERRUNN = 0xFF,
}Ps2Response;

typedef enum _Ps2KeyCondition
{
    PS2_KEY_COND_TYPEMATIC = (1 << 0),
    PS2_KEY_COND_MAKE = (1 << 1),
    PS2_KEY_COND_BREAK = (1 << 2),
    PS2_KEY_COND_ALL = (PS2_KEY_COND_TYPEMATIC | PS2_KEY_COND_MAKE | PS2_KEY_COND_BREAK),
} Ps2KeyCondition;

#endif /* PS2_COMMAND_H_ */