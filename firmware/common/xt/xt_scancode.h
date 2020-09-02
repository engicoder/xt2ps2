/* =======================================================================
 * xt_scancode.h
 * 
 * Purpose:
 *  Mnemonics for the base scan code values received from the PC/XT 
 *  keyboard.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef XT_SCANCODE_H_
#define XT_SCANCODE_H_

/* -----------------------------------------------------------------------
 * XT Scan code values.
 *  
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
typedef enum
{
    XT_SC_NONE          = 0x00,
    XT_SC_ESCAPE        = 0x01,
    XT_SC_1             = 0x02,
    XT_SC_2             = 0x03,
    XT_SC_3             = 0x04,
    XT_SC_4             = 0x05,
    XT_SC_5             = 0x06,
    XT_SC_6             = 0x07,
    XT_SC_7             = 0x08,
    XT_SC_8             = 0x09,
    XT_SC_9             = 0x0A,
    XT_SC_0             = 0x0B,
    XT_SC_MINUS         = 0x0C,
    XT_SC_EQUAL         = 0x0D,
    XT_SC_BACKSPACE     = 0x0E,
    XT_SC_TAB           = 0x0F,
    XT_SC_Q             = 0x10,
    XT_SC_W             = 0x11,
    XT_SC_E             = 0x12,
    XT_SC_R             = 0x13,
    XT_SC_T             = 0x14,
    XT_SC_Y             = 0x15,
    XT_SC_U             = 0x16,
    XT_SC_I             = 0x17,
    XT_SC_O             = 0x18,
    XT_SC_P             = 0x19,
    XT_SC_LBRACKET      = 0x1A,
    XT_SC_RBRACKET      = 0x1B,
    XT_SC_ENTER         = 0x1C,
    XT_SC_CONTROL       = 0x1D,
    XT_SC_A             = 0x1E,
    XT_SC_S             = 0x1F,
    XT_SC_D             = 0x20,
    XT_SC_F             = 0x21,
    XT_SC_G             = 0x22,
    XT_SC_H             = 0x23,
    XT_SC_J             = 0x24,
    XT_SC_K             = 0x25,
    XT_SC_L             = 0x26,
    XT_SC_SEMI_COLON    = 0x27,
    XT_SC_QUOTE         = 0x28,
    XT_SC_GRAVE         = 0x29,
    XT_SC_LSHIFT        = 0x2A,
    XT_SC_BACKSLASH     = 0x2B,
    XT_SC_Z             = 0x2C,
    XT_SC_X             = 0x2D,
    XT_SC_C             = 0x2E,
    XT_SC_V             = 0x2F,
    XT_SC_B             = 0x30,
    XT_SC_N             = 0x31,
    XT_SC_M             = 0x32,
    XT_SC_COMMA         = 0x33,
    XT_SC_PERIOD        = 0x34,
    XT_SC_FWD_SLASH     = 0x35,
    XT_SC_RSHIFT        = 0x36,
    XT_SC_KP_ASTERISK   = 0x37,
    XT_SC_ALT           = 0x38,
    XT_SC_SPACE         = 0x39,
    XT_SC_CAPSLOCK      = 0x3A,
    XT_SC_F1            = 0x3B,
    XT_SC_F2            = 0x3C,
    XT_SC_F3            = 0x3D,
    XT_SC_F4            = 0x3E,
    XT_SC_F5            = 0x3F,
    XT_SC_F6            = 0x40,
    XT_SC_F7            = 0x41,
    XT_SC_F8            = 0x42,
    XT_SC_F9            = 0x43,
    XT_SC_F10           = 0x44,
    XT_SC_NUM_LOCK      = 0x45,
    XT_SC_SCROLL_LOCK   = 0x46,
    XT_SC_KP_7          = 0x47,
    XT_SC_KP_8          = 0x48,
    XT_SC_KP_9          = 0x49,
    XT_SC_KP_MINUS      = 0x4A,
    XT_SC_KP_4          = 0x4B,
    XT_SC_KP_5          = 0x4C,
    XT_SC_KP_6          = 0x4D,
    XT_SC_KP_PLUS       = 0x4E,
    XT_SC_KP_1          = 0x4F,
    XT_SC_KP_2          = 0x50,
    XT_SC_KP_3          = 0x51,
    XT_SC_KP_0          = 0x52,
    XT_SC_KP_DOT        = 0x53,
    XT_SC_MAX_CODE      = 0x53,
    XT_SC_BAT_COMPLETE  = 0xAA,   
} XtScanCode;

#endif /* XT_SCANCODE_H_ */