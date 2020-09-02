/* =======================================================================
 * xt_stock_keymap.h
 * 
 * Purpose:
 *  Stock PC/XT keymap; mimics default behavior.
 *  Maps PX/XT scan codes to KeyCode values based on a lookup table. The
 *  table has two layers. As set of key combinations is also defined that
 *  allow a key combination to be mapped to a single KeyCode value.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef KEYMAP_STOCK_H_
#define KEYMAP_STOCK_H_

#include <avr/pgmspace.h>

#include "keymap_common.h"


static const KeyCode stockScanCodeMap[][2] PROGMEM = {
    /* XT_ESCAPE      Escape     */ {KC_ESCAPE,            KC_ESCAPE,         },
    /* XT_1           ! 1        */ {KC_1,                 KC_1,              },
    /* XT_2           @ 2        */ {KC_2,                 KC_2,              },
    /* XT_3           # 3        */ {KC_3,                 KC_3,              },
    /* XT_4           $ 4        */ {KC_4,                 KC_4,              },
    /* XT_5           % 5        */ {KC_5,                 KC_5,              },
    /* XT_6           ^ 6        */ {KC_6,                 KC_6,              },
    /* XT_7           & 7        */ {KC_7,                 KC_7,              },
    /* XT_8           * 8        */ {KC_8,                 KC_8,              },
    /* XT_9           ( 9        */ {KC_9,                 KC_9,              },
    /* XT_0           ) 0        */ {KC_0,                 KC_0,              },
    /* XT_MINUS       _ -        */ {KC_MINUS,             KC_MINUS,          },
    /* XT_EQUAL       + =        */ {KC_EQUAL,             KC_EQUAL,          },
    /* XT_BACKSPACE   Back Space */ {KC_BACKSPACE,         KC_BACKSPACE,      },
    /* XT_TAB         Tab        */ {KC_TAB,               KC_TAB,            },
    /* XT_Q           Q          */ {KC_Q,                 KC_Q,              },
    /* XT_W           W          */ {KC_W,                 KC_W,              },
    /* XT_E           E          */ {KC_E,                 KC_E,              },
    /* XT_R           R          */ {KC_R,                 KC_R,              },
    /* XT_T           T          */ {KC_T,                 KC_T,              },
    /* XT_Y           Y          */ {KC_Y,                 KC_Y,              },
    /* XT_U           U          */ {KC_U,                 KC_U,              },
    /* XT_I           I          */ {KC_I,                 KC_I,              },
    /* XT_O           O          */ {KC_O,                 KC_O,              },
    /* XT_P           P          */ {KC_P,                 KC_P,              },
    /* XT_LBRACKET    { [        */ {KC_LBRACKET,          KC_LBRACKET,       },
    /* XT_RBRACKET    } ]        */ {KC_RBRACKET,          KC_RBRACKET,       },
    /* XT_ENTER       Enter      */ {KC_ENTER,             KC_ENTER,          },
    /* XT_CONTROL     Ctrl L     */ {KC_LCTRL,             KC_LCTRL,          },
    /* XT_A           A          */ {KC_A,                 KC_A,              },
    /* XT_S           S          */ {KC_S,                 KC_S,              },
    /* XT_D           D          */ {KC_D,                 KC_D,              },
    /* XT_F           F          */ {KC_F,                 KC_F,              },
    /* XT_G           G          */ {KC_G,                 KC_G,              },
    /* XT_H           H          */ {KC_H,                 KC_H,              },
    /* XT_J           J          */ {KC_J,                 KC_J,              },
    /* XT_K           K          */ {KC_K,                 KC_K,              },
    /* XT_L           L          */ {KC_L,                 KC_L,              },
    /* XT_SEMICOLON   : ;        */ {KC_SEMI_COLON,        KC_SEMI_COLON,     },
    /* XT_QUOTE       " '        */ {KC_QUOTE,             KC_QUOTE,          },
    /* XT_GRAVE       ~ `        */ {KC_GRAVE,             KC_GRAVE,          },
    /* XT_LSHIFT      Shift L    */ {KC_LSHIFT,            KC_LSHIFT,         },
    /* XT_BACKSLASH   | \        */ {KC_BACKSLASH,         KC_BACKSLASH,      },
    /* XT_Z           Z          */ {KC_Z,                 KC_Z,              },
    /* XT_X           X          */ {KC_X,                 KC_X,              },
    /* XT_C           C          */ {KC_C,                 KC_C,              },
    /* XT_V           V          */ {KC_V,                 KC_V,              },
    /* XT_B           B          */ {KC_B,                 KC_B,              },
    /* XT_N           N          */ {KC_N,                 KC_N,              },
    /* XT_M           M          */ {KC_M,                 KC_M,              },
    /* XT_COMMA       < ,        */ {KC_COMMA,             KC_COMMA,          },
    /* XT_PERIOD      > .        */ {KC_PERIOD,            KC_PERIOD,         },
    /* XT_FWD_SLASH   ? /        */ {KC_FWD_SLASH,         KC_FWD_SLASH,      },
    /* XT_RSHIFT      Shift R    */ {KC_RSHIFT,            KC_RSHIFT,         },
    /* XT_KP_ASTERISK Keypad *   */ {KC_KP_ASTERISK,       KC_KP_ASTERISK,    },
    /* XT_ALT         Alt L      */ {KC_LALT,              KC_LALT,           },
    /* XT_SPACE       Space      */ {KC_SPACE,             KC_SPACE,          },
    /* XT_CAPSLOCK    Caps Lock  */ {KC_CAPSLOCK,          KC_CAPSLOCK,       },
    /* XT_F1          F1         */ {KC_F1,                KC_F1,             },
    /* XT_F2          F2         */ {KC_F2,                KC_F2,             },
    /* XT_F3          F3         */ {KC_F3,                KC_F3,             },
    /* XT_F4          F4         */ {KC_F4,                KC_F4,             },
    /* XT_F5          F5         */ {KC_F5,                KC_F5,             },
    /* XT_F6          F6         */ {KC_F6,                KC_F6,             },
    /* XT_F7          F7         */ {KC_F7,                KC_F7,             },
    /* XT_F8          F8         */ {KC_F8,                KC_F8,             },
    /* XT_F9          F9         */ {KC_F9,                KC_F9,             },    
    /* XT_F10         F10        */ {KC_F10,               KC_F10,            },
    /* XT_NUM_LOCK    Num Lock   */ {KC_NUM_LOCK,          KC_NUM_LOCK,       },
    /* XT_SCROLL_LOCK ScrLk/Bk   */ {KC_SCROLL_LOCK,       KC_SCROLL_LOCK,    },
    /* XT_KP_7        Keypad 7   */ {KC_KP_7,              KC_KP_7,           },
    /* XT_KP_8        Keypad 8   */ {KC_KP_8,              KC_KP_8,           },
    /* XT_KP_9        Keypad 9   */ {KC_KP_9,              KC_KP_9,           },
    /* XT_KP_MINUS    Keypad -   */ {KC_KP_MINUS,          KC_KP_MINUS,       },
    /* XT_KP_4        Keypad 4   */ {KC_KP_4,              KC_KP_4,           },
    /* XT_KP_5        Keypad 5   */ {KC_KP_5,              KC_KP_5,           },
    /* XT_KP_6        Keypad 6   */ {KC_KP_6,              KC_KP_6,           },
    /* XT_KP_PLUS     Keypad +   */ {KC_KP_PLUS,           KC_KP_PLUS,        },
    /* XT_KP_1        Keypad 1   */ {KC_KP_1,              KC_KP_1,           },
    /* XT_KP_2        Keypad 2   */ {KC_KP_2,              KC_KP_2,           },
    /* XT_KP_3        Keypad 3   */ {KC_KP_3,              KC_KP_3,           },
    /* XT_KP_0        Keypad 0   */ {KC_KP_0,              KC_KP_0,           },
    /* XT_KP_DOT      Keypad .   */ {KC_KP_DOT,            KC_KP_DOT,         }, 
}; 

/* Note, this is SRAM */
static const KeyCombination stockKeyCombinations[] = {
    {MODS_LSHIFT, KC_KP_ASTERISK, KC_PRINT_SCREEN},
    {MODS_LALT, KC_KP_ASTERISK, KC_SYSREQ },
    {MODS_LCTRL, KC_NUM_LOCK, KC_PAUSE},
    {MODS_LCTRL, KC_SCROLL_LOCK, KC_BREAK},
};

#define STOCK_KEY_COMBO_SIZE (sizeof(stockKeyCombinations) / sizeof(stockKeyCombinations[0]))

static const Keymap stockKeyMap =
{
    stockScanCodeMap,
    stockKeyCombinations,
    STOCK_KEY_COMBO_SIZE,
};


#endif /* KEYMAP_STOCK_H_*/