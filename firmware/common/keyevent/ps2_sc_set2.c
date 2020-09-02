/* ========================================================================
 * keyevt_conv_ps2_set2.c
 * 
 * Purpose:
 *  Converts KeyEvent objects to PS/2 Set 2 scan code sequences.
 *
 * Operational Summary:
 *  Uses a static mapping table for most codes. Special key combinations
 *  are handled separately
 * 
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE file for license details.
 * ------------------------------------------------------------------------ */
#include <stddef.h>
#include <stdint.h>

#include <avr/pgmspace.h>
#include "progmem_util.h"

#include "keyevent.h"
#include "byte_sequence.h"
#include "ps2_sc_set2.h"
#include "modifier_status.h"



const uint8_t PROGMEM PGM_SET2_NO_EVENT       [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_ROLL_OVER      [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_POST_FAIL      [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_UNDEFINED      [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_A              [] = { 0x01, 0x1C, };
const uint8_t PROGMEM PGM_SET2_B              [] = { 0x01, 0x32, };
const uint8_t PROGMEM PGM_SET2_C              [] = { 0x01, 0x21, };
const uint8_t PROGMEM PGM_SET2_D              [] = { 0x01, 0x23, };
const uint8_t PROGMEM PGM_SET2_E              [] = { 0x01, 0x24, };
const uint8_t PROGMEM PGM_SET2_F              [] = { 0x01, 0x2B, };
const uint8_t PROGMEM PGM_SET2_G              [] = { 0x01, 0x34, };
const uint8_t PROGMEM PGM_SET2_H              [] = { 0x01, 0x33, };
const uint8_t PROGMEM PGM_SET2_I              [] = { 0x01, 0x43, };
const uint8_t PROGMEM PGM_SET2_J              [] = { 0x01, 0x3B, };
const uint8_t PROGMEM PGM_SET2_K              [] = { 0x01, 0x42, };
const uint8_t PROGMEM PGM_SET2_L              [] = { 0x01, 0x4B, };
const uint8_t PROGMEM PGM_SET2_M              [] = { 0x01, 0x3A, };
const uint8_t PROGMEM PGM_SET2_N              [] = { 0x01, 0x31, };
const uint8_t PROGMEM PGM_SET2_O              [] = { 0x01, 0x44, };
const uint8_t PROGMEM PGM_SET2_P              [] = { 0x01, 0x4D, };
const uint8_t PROGMEM PGM_SET2_Q              [] = { 0x01, 0x15, };
const uint8_t PROGMEM PGM_SET2_R              [] = { 0x01, 0x2D, };
const uint8_t PROGMEM PGM_SET2_S              [] = { 0x01, 0x1B, };
const uint8_t PROGMEM PGM_SET2_T              [] = { 0x01, 0x2C, };
const uint8_t PROGMEM PGM_SET2_U              [] = { 0x01, 0x3C, };
const uint8_t PROGMEM PGM_SET2_V              [] = { 0x01, 0x2A, };
const uint8_t PROGMEM PGM_SET2_W              [] = { 0x01, 0x1D, };
const uint8_t PROGMEM PGM_SET2_X              [] = { 0x01, 0x22, };
const uint8_t PROGMEM PGM_SET2_Y              [] = { 0x01, 0x35, };
const uint8_t PROGMEM PGM_SET2_Z              [] = { 0x01, 0x1A, };
const uint8_t PROGMEM PGM_SET2_1              [] = { 0x01, 0x16, };
const uint8_t PROGMEM PGM_SET2_2              [] = { 0x01, 0x1E, };
const uint8_t PROGMEM PGM_SET2_3              [] = { 0x01, 0x26, };
const uint8_t PROGMEM PGM_SET2_4              [] = { 0x01, 0x25, };
const uint8_t PROGMEM PGM_SET2_5              [] = { 0x01, 0x2E, };
const uint8_t PROGMEM PGM_SET2_6              [] = { 0x01, 0x36, };
const uint8_t PROGMEM PGM_SET2_7              [] = { 0x01, 0x3D, };
const uint8_t PROGMEM PGM_SET2_8              [] = { 0x01, 0x3E, };
const uint8_t PROGMEM PGM_SET2_9              [] = { 0x01, 0x46, };
const uint8_t PROGMEM PGM_SET2_0              [] = { 0x01, 0x45, };
const uint8_t PROGMEM PGM_SET2_ENTER          [] = { 0x01, 0x5A, };
const uint8_t PROGMEM PGM_SET2_ESCAPE         [] = { 0x01, 0x76, };
const uint8_t PROGMEM PGM_SET2_BACKSPACE      [] = { 0x01, 0x66, };
const uint8_t PROGMEM PGM_SET2_TAB            [] = { 0x01, 0x0D, };
const uint8_t PROGMEM PGM_SET2_SPACE          [] = { 0x01, 0x29, };
const uint8_t PROGMEM PGM_SET2_MINUS          [] = { 0x01, 0x4E, };
const uint8_t PROGMEM PGM_SET2_EQUAL          [] = { 0x01, 0x55, };
const uint8_t PROGMEM PGM_SET2_LBRACKET       [] = { 0x01, 0x54, };
const uint8_t PROGMEM PGM_SET2_RBRACKET       [] = { 0x01, 0x5B, };
const uint8_t PROGMEM PGM_SET2_BACKSLASH      [] = { 0x01, 0x5D, };
const uint8_t PROGMEM PGM_SET2_NONUS_HASH     [] = { 0x01, 0x5D, };
const uint8_t PROGMEM PGM_SET2_SEMI_COLON     [] = { 0x01, 0x4C, };
const uint8_t PROGMEM PGM_SET2_QUOTE          [] = { 0x01, 0x52, };
const uint8_t PROGMEM PGM_SET2_GRAVE          [] = { 0x01, 0x0E, };
const uint8_t PROGMEM PGM_SET2_COMMA          [] = { 0x01, 0x41, };
const uint8_t PROGMEM PGM_SET2_PERIOD         [] = { 0x01, 0x49, };
const uint8_t PROGMEM PGM_SET2_FWD_SLASH      [] = { 0x01, 0x4A, };
const uint8_t PROGMEM PGM_SET2_CAPSLOCK       [] = { 0x01, 0x58, };
const uint8_t PROGMEM PGM_SET2_F1             [] = { 0x01, 0x05, };
const uint8_t PROGMEM PGM_SET2_F2             [] = { 0x01, 0x06, };
const uint8_t PROGMEM PGM_SET2_F3             [] = { 0x01, 0x04, };
const uint8_t PROGMEM PGM_SET2_F4             [] = { 0x01, 0x0C, };
const uint8_t PROGMEM PGM_SET2_F5             [] = { 0x01, 0x03, };
const uint8_t PROGMEM PGM_SET2_F6             [] = { 0x01, 0x0B, };
const uint8_t PROGMEM PGM_SET2_F7             [] = { 0x01, 0x83, };
const uint8_t PROGMEM PGM_SET2_F8             [] = { 0x01, 0x0A, };
const uint8_t PROGMEM PGM_SET2_F9             [] = { 0x01, 0x01, };
const uint8_t PROGMEM PGM_SET2_F10            [] = { 0x01, 0x09, };
const uint8_t PROGMEM PGM_SET2_F11            [] = { 0x01, 0x78, };
const uint8_t PROGMEM PGM_SET2_F12            [] = { 0x01, 0x07, };
const uint8_t PROGMEM PGM_SET2_PRINT_SCREEN   [] = { 0x02, 0xE0, 0x7C };
const uint8_t PROGMEM PGM_SET2_SCROLL_LOCK    [] = { 0x01, 0x7E, };
const uint8_t PROGMEM PGM_SET2_PAUSE          [] = { 0x08, 0xE1, 0x14, 0x77, 0xE1, 0xF0, 0x14, 0xF0, 0x77 };
const uint8_t PROGMEM PGM_SET2_INSERT         [] = { 0x02, 0xE0, 0x70, };
const uint8_t PROGMEM PGM_SET2_HOME           [] = { 0x02, 0xE0, 0x6C, };
const uint8_t PROGMEM PGM_SET2_PAGE_UP        [] = { 0x02, 0xE0, 0x7D, };
const uint8_t PROGMEM PGM_SET2_DELETE         [] = { 0x02, 0xE0, 0x71, };
const uint8_t PROGMEM PGM_SET2_END            [] = { 0x02, 0xE0, 0x69, };
const uint8_t PROGMEM PGM_SET2_PAGE_DOWN      [] = { 0x02, 0xE0, 0x7A, };
const uint8_t PROGMEM PGM_SET2_RIGHT          [] = { 0x02, 0xE0, 0x74, };
const uint8_t PROGMEM PGM_SET2_LEFT           [] = { 0x02, 0xE0, 0x6B, };
const uint8_t PROGMEM PGM_SET2_DOWN           [] = { 0x02, 0xE0, 0x72, };
const uint8_t PROGMEM PGM_SET2_UP             [] = { 0x02, 0xE0, 0x75, };
const uint8_t PROGMEM PGM_SET2_NUM_LOCK       [] = { 0x01, 0x77, };
const uint8_t PROGMEM PGM_SET2_KP_SLASH       [] = { 0x02, 0xE0, 0x4A, };
const uint8_t PROGMEM PGM_SET2_KP_ASTERISK    [] = { 0x01, 0x7C, };
const uint8_t PROGMEM PGM_SET2_KP_MINUS       [] = { 0x01, 0x7B, };
const uint8_t PROGMEM PGM_SET2_KP_PLUS        [] = { 0x01, 0x79, };
const uint8_t PROGMEM PGM_SET2_KP_ENTER       [] = { 0x02, 0xE0, 0x5A, };
const uint8_t PROGMEM PGM_SET2_KP_1           [] = { 0x01, 0x69, };
const uint8_t PROGMEM PGM_SET2_KP_2           [] = { 0x01, 0x72, };
const uint8_t PROGMEM PGM_SET2_KP_3           [] = { 0x01, 0x7A, };
const uint8_t PROGMEM PGM_SET2_KP_4           [] = { 0x01, 0x6B, };
const uint8_t PROGMEM PGM_SET2_KP_5           [] = { 0x01, 0x73, };
const uint8_t PROGMEM PGM_SET2_KP_6           [] = { 0x01, 0x74, };
const uint8_t PROGMEM PGM_SET2_KP_7           [] = { 0x01, 0x6C, };
const uint8_t PROGMEM PGM_SET2_KP_8           [] = { 0x01, 0x75, };
const uint8_t PROGMEM PGM_SET2_KP_9           [] = { 0x01, 0x7D, };
const uint8_t PROGMEM PGM_SET2_KP_0           [] = { 0x01, 0x70, };
const uint8_t PROGMEM PGM_SET2_KP_DOT         [] = { 0x01, 0x71, };
const uint8_t PROGMEM PGM_SET2_NONUS_BSLASH   [] = { 0x01, 0x61, };
const uint8_t PROGMEM PGM_SET2_APPLICATION    [] = { 0x02, 0xE0, 0x2F, };
const uint8_t PROGMEM PGM_SET2_POWER          [] = { 0x02, 0xE0, 0x37, };
const uint8_t PROGMEM PGM_SET2_KP_EQUAL       [] = { 0x01, 0x0F, };
const uint8_t PROGMEM PGM_SET2_F13            [] = { 0x01, 0x08, };
const uint8_t PROGMEM PGM_SET2_F14            [] = { 0x01, 0x10, };
const uint8_t PROGMEM PGM_SET2_F15            [] = { 0x01, 0x18, };
const uint8_t PROGMEM PGM_SET2_F16            [] = { 0x01, 0x20, };
const uint8_t PROGMEM PGM_SET2_F17            [] = { 0x01, 0x28, };
const uint8_t PROGMEM PGM_SET2_F18            [] = { 0x01, 0x30, };
const uint8_t PROGMEM PGM_SET2_F19            [] = { 0x01, 0x38, };
const uint8_t PROGMEM PGM_SET2_F20            [] = { 0x01, 0x40, };
const uint8_t PROGMEM PGM_SET2_F21            [] = { 0x01, 0x48, };
const uint8_t PROGMEM PGM_SET2_F22            [] = { 0x01, 0x50, };
const uint8_t PROGMEM PGM_SET2_F23            [] = { 0x01, 0x57, };
const uint8_t PROGMEM PGM_SET2_F24            [] = { 0x01, 0x5F, };
const uint8_t PROGMEM PGM_SET2_EXECUTE        [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_HELP           [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_MENU           [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_SELECT         [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_STOP           [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_AGAIN          [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_UNDO           [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_CUT            [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_COPY           [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_PASTE          [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_FIND           [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_MUTE           [] = { 0x02, 0xE0, 0x23,};
const uint8_t PROGMEM PGM_SET2_VOLUP          [] = { 0x02, 0xE0, 0x32, }; /* From consumer device */
const uint8_t PROGMEM PGM_SET2_VOLDOWN        [] = { 0x02, 0xE0, 0x31, };
const uint8_t PROGMEM PGM_SET2_LOCKING_CAPS   [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_LOCKING_NUM    [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_LOCKING_SCROLL [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_KP_COMMA       [] = { 0x01, 0x6D, };
const uint8_t PROGMEM PGM_SET2_KP_EQUAL_AS400 [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_INT1           [] = { 0x01, 0x51, };
const uint8_t PROGMEM PGM_SET2_INT2           [] = { 0x01, 0x13, };
const uint8_t PROGMEM PGM_SET2_INT3           [] = { 0x01, 0x6A, };
const uint8_t PROGMEM PGM_SET2_INT4           [] = { 0x01, 0x64, };
const uint8_t PROGMEM PGM_SET2_INT5           [] = { 0x01, 0x67, };
const uint8_t PROGMEM PGM_SET2_INT6           [] = { 0x01, 0x27, };
const uint8_t PROGMEM PGM_SET2_INT7           [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_INT8           [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_INT9           [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_LANG1          [] = { 0x01, 0xF2, };
const uint8_t PROGMEM PGM_SET2_LANG2          [] = { 0x01, 0xF1, };
const uint8_t PROGMEM PGM_SET2_LANG3          [] = { 0x01, 0x63, };
const uint8_t PROGMEM PGM_SET2_LANG4          [] = { 0x01, 0x62, };
const uint8_t PROGMEM PGM_SET2_LANG5          [] = { 0x01, 0x5F, };
const uint8_t PROGMEM PGM_SET2_LANG6          [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_LANG7          [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_LANG8          [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_LANG9          [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_ALT_ERASE      [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_SYSREQ         [] = { 0x01, 0x84, };
const uint8_t PROGMEM PGM_SET2_CANCEL         [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_CLEAR          [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_PRIOR          [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_RETURN         [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_SEPARATOR      [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_OUT            [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_OPER           [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_CLEAR_AGAIN    [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_CRSEL          [] = { 0x01, 0x00, };
const uint8_t PROGMEM PGM_SET2_EXSEL          [] = { 0x01, 0x00, };


const uint8_t* const _baseMap[] PROGMEM =
{
    PGM_SET2_NO_EVENT       ,
    PGM_SET2_ROLL_OVER      ,
    PGM_SET2_POST_FAIL      ,
    PGM_SET2_UNDEFINED      ,
    PGM_SET2_A              ,
    PGM_SET2_B              ,
    PGM_SET2_C              ,
    PGM_SET2_D              ,
    PGM_SET2_E              ,
    PGM_SET2_F              ,
    PGM_SET2_G              ,
    PGM_SET2_H              ,
    PGM_SET2_I              ,
    PGM_SET2_J              ,
    PGM_SET2_K              ,
    PGM_SET2_L              ,
    PGM_SET2_M              ,
    PGM_SET2_N              ,
    PGM_SET2_O              ,
    PGM_SET2_P              ,
    PGM_SET2_Q              ,
    PGM_SET2_R              ,
    PGM_SET2_S              ,
    PGM_SET2_T              ,
    PGM_SET2_U              ,
    PGM_SET2_V              ,
    PGM_SET2_W              ,
    PGM_SET2_X              ,
    PGM_SET2_Y              ,
    PGM_SET2_Z              ,
    PGM_SET2_1              ,
    PGM_SET2_2              ,
    PGM_SET2_3              ,
    PGM_SET2_4              ,
    PGM_SET2_5              ,
    PGM_SET2_6              ,
    PGM_SET2_7              ,
    PGM_SET2_8              ,
    PGM_SET2_9              ,
    PGM_SET2_0              ,
    PGM_SET2_ENTER          ,
    PGM_SET2_ESCAPE         ,
    PGM_SET2_BACKSPACE      ,
    PGM_SET2_TAB            ,
    PGM_SET2_SPACE          ,
    PGM_SET2_MINUS          ,
    PGM_SET2_EQUAL          ,
    PGM_SET2_LBRACKET       ,
    PGM_SET2_RBRACKET       ,
    PGM_SET2_BACKSLASH      ,
    PGM_SET2_NONUS_HASH     ,
    PGM_SET2_SEMI_COLON     ,
    PGM_SET2_QUOTE          ,
    PGM_SET2_GRAVE          ,
    PGM_SET2_COMMA          ,
    PGM_SET2_PERIOD         ,
    PGM_SET2_FWD_SLASH      ,
    PGM_SET2_CAPSLOCK       ,
    PGM_SET2_F1             ,
    PGM_SET2_F2             ,
    PGM_SET2_F3             ,
    PGM_SET2_F4             ,
    PGM_SET2_F5             ,
    PGM_SET2_F6             ,
    PGM_SET2_F7             ,
    PGM_SET2_F8             ,
    PGM_SET2_F9             ,
    PGM_SET2_F10            ,
    PGM_SET2_F11            ,
    PGM_SET2_F12            ,
    PGM_SET2_PRINT_SCREEN   ,
    PGM_SET2_SCROLL_LOCK    ,
    PGM_SET2_PAUSE          ,
    PGM_SET2_INSERT         ,
    PGM_SET2_HOME           ,
    PGM_SET2_PAGE_UP        ,
    PGM_SET2_DELETE         ,
    PGM_SET2_END            ,
    PGM_SET2_PAGE_DOWN      ,
    PGM_SET2_RIGHT          ,
    PGM_SET2_LEFT           ,
    PGM_SET2_DOWN           ,
    PGM_SET2_UP             ,
    PGM_SET2_NUM_LOCK       ,
    PGM_SET2_KP_SLASH       ,
    PGM_SET2_KP_ASTERISK    ,
    PGM_SET2_KP_MINUS       ,
    PGM_SET2_KP_PLUS        ,
    PGM_SET2_KP_ENTER       ,
    PGM_SET2_KP_1           ,
    PGM_SET2_KP_2           ,
    PGM_SET2_KP_3           ,
    PGM_SET2_KP_4           ,
    PGM_SET2_KP_5           ,
    PGM_SET2_KP_6           ,
    PGM_SET2_KP_7           ,
    PGM_SET2_KP_8           ,
    PGM_SET2_KP_9           ,
    PGM_SET2_KP_0           ,
    PGM_SET2_KP_DOT         ,
    PGM_SET2_NONUS_BSLASH   ,
    PGM_SET2_APPLICATION    ,
    PGM_SET2_POWER          ,
    PGM_SET2_KP_EQUAL       ,
    PGM_SET2_F13            ,
    PGM_SET2_F14            ,
    PGM_SET2_F15            ,
    PGM_SET2_F16            ,
    PGM_SET2_F17            ,
    PGM_SET2_F18            ,
    PGM_SET2_F19            ,
    PGM_SET2_F20            ,
    PGM_SET2_F21            ,
    PGM_SET2_F22            ,
    PGM_SET2_F23            ,
    PGM_SET2_F24            ,
    PGM_SET2_EXECUTE        ,
    PGM_SET2_HELP           ,
    PGM_SET2_MENU           ,
    PGM_SET2_SELECT         ,
    PGM_SET2_STOP           ,
    PGM_SET2_AGAIN          ,
    PGM_SET2_UNDO           ,
    PGM_SET2_CUT            ,
    PGM_SET2_COPY           ,
    PGM_SET2_PASTE          ,
    PGM_SET2_FIND           ,
    PGM_SET2_MUTE           ,
    PGM_SET2_VOLUP          ,
    PGM_SET2_VOLDOWN        ,
    PGM_SET2_LOCKING_CAPS   ,
    PGM_SET2_LOCKING_NUM    ,
    PGM_SET2_LOCKING_SCROLL ,
    PGM_SET2_KP_COMMA       ,
    PGM_SET2_KP_EQUAL_AS400 ,
    PGM_SET2_INT1           ,
    PGM_SET2_INT2           ,
    PGM_SET2_INT3           ,
    PGM_SET2_INT4           ,
    PGM_SET2_INT5           ,
    PGM_SET2_INT6           ,
    PGM_SET2_INT7           ,
    PGM_SET2_INT8           ,
    PGM_SET2_INT9           ,
    PGM_SET2_LANG1          ,
    PGM_SET2_LANG2          ,
    PGM_SET2_LANG3          ,
    PGM_SET2_LANG4          ,
    PGM_SET2_LANG5          ,
    PGM_SET2_LANG6          ,
    PGM_SET2_LANG7          ,
    PGM_SET2_LANG8          ,
    PGM_SET2_LANG9          ,
    PGM_SET2_ALT_ERASE      ,
    PGM_SET2_SYSREQ         ,
    PGM_SET2_CANCEL         ,
    PGM_SET2_CLEAR          ,
    PGM_SET2_PRIOR          ,
    PGM_SET2_RETURN         ,
    PGM_SET2_SEPARATOR      ,
    PGM_SET2_OUT            ,
    PGM_SET2_OPER           ,
    PGM_SET2_CLEAR_AGAIN    ,
    PGM_SET2_CRSEL          ,
    PGM_SET2_EXSEL          ,
};

const uint8_t PROGMEM PGM_SET2_LCTRL [] = { 0x01, 0x14, };
const uint8_t PROGMEM PGM_SET2_LSHIFT[] = { 0x01, 0x12, };
const uint8_t PROGMEM PGM_SET2_LALT  [] = { 0x01, 0x11, };
const uint8_t PROGMEM PGM_SET2_LGUI  [] = { 0x02, 0xE0, 0x1F, };
const uint8_t PROGMEM PGM_SET2_RCTRL [] = { 0x02, 0xE0, 0x14, };
const uint8_t PROGMEM PGM_SET2_RSHIFT[] = { 0x01, 0x59, };
const uint8_t PROGMEM PGM_SET2_RALT  [] = { 0x02, 0xE0, 0x11, };
const uint8_t PROGMEM PGM_SET2_RGUI  [] = { 0x02, 0xE0, 0x27, };

const uint8_t* const _modifierMap[] PROGMEM =
{
    PGM_SET2_LCTRL  ,
    PGM_SET2_LSHIFT ,
    PGM_SET2_LALT   ,
    PGM_SET2_LGUI   ,
    PGM_SET2_RCTRL  ,
    PGM_SET2_RSHIFT ,
    PGM_SET2_RALT   ,
    PGM_SET2_RGUI   ,
};

const uint8_t PROGMEM PGM_SET2_BREAK        [] = { 0x02, 0xE0, 0x7E };
const uint8_t PROGMEM PGM_SET2_CD_VOLUP     [] = { 0x02, 0xE0, 0x32 };
const uint8_t PROGMEM PGM_SET2_CD_VOLDOWN   [] = { 0x02, 0xE0, 0x21,};
const uint8_t PROGMEM PGM_SET2_CD_MUTE      [] = { 0x02, 0xE0, 0x23, };
const uint8_t PROGMEM PGM_SET2_CD_STOP      [] = { 0x02, 0xE0, 0x3B, };
const uint8_t PROGMEM PGM_SET2_CD_PLAYPAUSE [] = { 0x02, 0xE0, 0x34, };
const uint8_t PROGMEM PGM_SET2_CD_NEXT_TRK  [] = { 0x02, 0xE0, 0x4D, };
const uint8_t PROGMEM PGM_SET2_CD_PREV_TRK  [] = { 0x02, 0xE0, 0x15, };


const uint8_t* const _miscMap[] PROGMEM =
{
    PGM_SET2_BREAK       ,
    PGM_SET2_CD_VOLUP    ,
    PGM_SET2_CD_VOLDOWN  ,
    PGM_SET2_CD_MUTE     ,
    PGM_SET2_CD_STOP     ,
    PGM_SET2_CD_PLAYPAUSE,
    PGM_SET2_CD_NEXT_TRK ,
    PGM_SET2_CD_PREV_TRK ,
};

const uint8_t PROGMEM PGM_SET2_PRTSC_SHIFT  [] = { 0x04, 0xE0, 0x12, 0xE0, 0x7C,};

/* Cheeky but risky way to statically define a ByteSequence to 
 * use as a temporary workspace. Resist the urge to change
 * and of the values in its initializer. */
static uint8_t _tempBuffer[] = 
{
    0x00, 
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 
};
ByteSequence* _temp = (ByteSequence*)_tempBuffer;

static uint8_t _convertedBuffer[] =
{
    0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
ByteSequence* _converted = (ByteSequence*)_convertedBuffer;


static bool CheckForPrintScreen(KeyEvent* keyEvent, ByteSequence* converted);
static void UpdateSequenceToBreak(ByteSequence* sequence);
static bool ConvertToSequence(KeyCode keyCode, ByteSequence* sequence);

ByteSequence* Ps2ScanCodeSet2_KeyEventToSequence(KeyEvent* keyEvent)
{
    KeyCode keyCode = KeyEvent_Code(keyEvent);

    if (keyCode == KC_NONE)
    {
        return NULL;
    }

    ByteSequence_Clear(_converted);

    if (!ConvertToSequence(keyCode, _converted))
    {
        return NULL;
    }
   
    if (!CheckForPrintScreen(keyEvent, _converted))
    {

    }

    if(KeyEvent_IsRelease(keyEvent))
    {
        UpdateSequenceToBreak(_converted);
    }

    return _converted;
}


bool ConvertToSequence(KeyCode keyCode, ByteSequence* sequence)
{
    const uint8_t* mapEntry;

    if (keyCode < 0xA4)
        mapEntry = (const uint8_t*)pgm_read_word(&(_baseMap[keyCode]));
    else if (keyCode >= KC_LCTRL && keyCode <= KC_RGUI)
        mapEntry = (const uint8_t*)pgm_read_word(&(_modifierMap[keyCode - KC_LCTRL]));
    else if (keyCode >= KC_BREAK && keyCode <= KC_CD_PREV_TRK)
        mapEntry = (const uint8_t*)pgm_read_word(&(_miscMap[keyCode - KC_BREAK]));
    else
        return false;

    ProgMem_ReadByteSequence((uint16_t)mapEntry, sequence);

    /* Note: watch for copy overrun if the sequence passed does not have enough capacity */

    return true;
}

/* Convert the make sequence to a break sequence by stuffing 0xF0
 * Before all primary codes (i.e. not E0, E1 or F0)
 *  eg {1C} => {F0 1C} and {E0 12} => {E0 F0 12} 
 * ------------------------------------------------------------------ */
void UpdateSequenceToBreak(ByteSequence* sequence)
{
    ByteSequence_Duplicate(_temp, sequence);
    ByteSequence_Clear(sequence);

    int length = ByteSequence_Length(_temp);

    for (int n = 0; n < length; n++)
    {
        uint8_t code = ByteSequence_DataAt(_temp, n);
        if (code != 0xE0)
        {
            ByteSequence_Add(sequence, 0xF0);
        }                

        ByteSequence_Add(sequence, code);
    }
}

bool CheckForPrintScreen(KeyEvent* keyEvent, ByteSequence* converted)
{
    static ModifierStatus modStatus = MODIFIER_STATUS_NONE;

    KeyCode keyCode = KeyEvent_Code(keyEvent);

    ModifierStatus_Update(&modStatus, keyEvent);

    if (keyCode != KC_PRINT_SCREEN)
    {
        return false;
    }

    else if (!ModifierStatus_IsDown(&modStatus, MODS_LSHIFT | MODS_RSHIFT))
    {
        ProgMem_ReadByteSequence((uint16_t)PGM_SET2_PRTSC_SHIFT, converted);
        return true;
    }

   return false;
}



