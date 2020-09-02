/* ========================================================================
 * keymap.c
 * 
 * Purpose:
 *  Maps XT scan codes to KeyEvent objects
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

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

#include "keymap.h"
#include "keymap_stock.h"
#include "keymap_user.h"
#include "circular_buffer.h"
#include "circular_buffer_util.h"
#include "eeprom_util.h"
#include "xt_scancode.h"
#include "keycode.h"
#include "modifier_status.h"

#include "console.h"
#include "con_msg_keymap.h"


#define SELECTED_KEYMAP_EEPROM_SIZE 128
uint8_t _eepromSelectedKeymap[SELECTED_KEYMAP_EEPROM_SIZE] EEMEM =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


/* === Preprocessor Definitions ======================================= */
#define MAX_KEY_COMBOS ((STOCK_KEY_COMBO_SIZE > USER_KEY_COMBO_SIZE)?(STOCK_KEY_COMBO_SIZE):(USER_KEY_COMBO_SIZE))

/* === Type Definitions ============================================== */
typedef enum
{
    STOCK,
    USER,
} KeymapSelection;
#define NUM_KEYMAPS ((uint8_t)(USER + 1))


/* === Static Global Variable Declarations ============================ */
static KeymapSelection _selectedKeymap = STOCK;
static const Keymap* _keymaps[2] = {&stockKeyMap, &userKeyMap};
static ModifierStatus _mappedModStatus = { .Mods = MODS_NONE };
static uint8_t _currentKeymapLayer = 0;


/* === Forward Declarations =========================================== */
bool Keymap_CheckForKeyCombo( KeyEvent* keyEvent);
bool Keymap_CheckForKeymapSwap ( XtScanCode scanCode, KeyEvent* keyEvent );
static inline bool Keymap_CheckForLayerChange(KeyEvent* keyEvent);
KeyCode Keymap_CheckForSupressedKey(KeyEvent* keyEvent);
static inline KeyCode Keymap_MapScanCodeToKeyCode(const Keymap* keymap, XtScanCode xtScanCode, uint8_t layer);

/* -----------------------------------------------------------------------
 *  The keymap allows two key combinations to be defined that will be mapped
 *  to a single key stroke. The keys are a modifier + a non-modifier.
 *  This function records the modifier state and for each key stroke,
 *  checks each key combination to determine if the current key satisfies it
 *  requirements. If so, the KeyEvent is modified with the mapped keycode.
 *  Returns: 
 *    true  - if key combination was completed and a keycode mapped.
 *    false - otherwise.
 *  Note: It is also possible for this function to modify the value of
 *        the KeyEvent passed as an argument.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool Keymap_CheckForKeyCombo( KeyEvent* keyEvent)
{
    /* Records the KeyCombinations that have been triggered (i.e. Active) */
    static bool _active[MAX_KEY_COMBOS];

    /* If no modifiers are down, no KeyCombination can be triggered */
    if (ModifierStatus_None(&_mappedModStatus))
        return false;

    /* Iterate through all defined KeyCombinations and check to see if the 
     * current key completes one */
    for(uint8_t n = 0; n < _keymaps[_selectedKeymap]->NumKeyCombinations; n++)
    {
        const KeyCombination* combo = &(_keymaps[_selectedKeymap]->KeyCombinations[n]);

        /* If the KeyCode matches the non-modifier part of the combo */
        if (KeyEvent_Code(keyEvent) == combo->Original)
        {
            if (KeyEvent_IsPress(keyEvent))
            {
                /* If the required mods are down, map the KeyCode and mark as active */
                if(ModifierStatus_IsDown(&_mappedModStatus, combo->RequiredModifiers))
                {
                    CONSOLE_SEND888(CON_SRC_KEYMAP, CON_SEV_TRACE_EVENT, CON_MSG_KEYMAP_COMBO, combo->Original, combo->RequiredModifiers, combo->MappedTo);
                    keyEvent->code = combo->MappedTo;
                    _active[n] = true;
                    return true;
                }
                /* If the required mods are no longer down, we will suppress it */
                else if (_active[n])
                {
                    keyEvent->code = KC_NONE;
                    return true;
                }
            }
            /* If this is a key release and the combo is active, map the code */
            else if (_active[n])
            {
                keyEvent->code = combo->MappedTo;
                _active[n] = false;
                return true;                
            }
        }        
    }

    return false;
}



/* -----------------------------------------------------------------------
 *  A "magic" key combination is defined to trigger a swap between the
 *  stock keymap and a user defined keymap. The keymap swap combination is:
 *   LSHIFT + RSHIFT + SWAPKEY
 *  Every time a scan code is received from the the keyboard it is examined.
 *  If the scan code is the swap key and the left and right shift keys are
 *  in the pressed/down state, the keymap is swapped.
 *  The shift key state is tracked as a state variable across calls.
 *  The press status of the swap key is also tracked as the swap key press
 *  and release events as part of the keymap swap combination should not be 
 *  passed on to the host.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool Keymap_CheckForKeymapSwap ( XtScanCode scanCode, KeyEvent* keyEvent )
{
    static bool _swapKeyDown = false;
    static ModifierStatus _shiftStatus =  { .Mods = MODS_NONE }; 
    bool isBreakCode = !KeyEvent_IsPress(keyEvent);

    /* If the scan code represents a:
     *  - left or right shift key, modify shift status and return false.
     *  - the defined SWAPKEY, continue.
     *  - any other key, return false. */
    switch (scanCode)
    {
        case XT_SC_RSHIFT: 
            ModifierStatus_Set(&_shiftStatus, MODS_RSHIFT, !isBreakCode);
            return false;
        case XT_SC_LSHIFT:
            ModifierStatus_Set(&_shiftStatus, MODS_LSHIFT, !isBreakCode);
            return false;
        case USER_KEYMAP_SWAP_KEY:
            break;
        default:
            return false;
    }

    /* SWAPKEY PRESS */
    if (!isBreakCode ) 
    {
        /* If both shift keys are down and the SWAPKEY is not already down,
         * this is the first time we have detected the swap combination; 
         *  => Swap keymaps */
        if (ModifierStatus_IsDown(&_shiftStatus, (MODS_LSHIFT | MODS_RSHIFT)) &&
            !_swapKeyDown)
        {
            /* Mark the SWAPKEY as down */
            _swapKeyDown = true;

            /* Swap the selected keymap */
            _selectedKeymap = (_selectedKeymap == STOCK) ? USER : STOCK;

            /* Update the KeyCode to indicate the new keymap */
            keyEvent->code = (_selectedKeymap == STOCK) ? KC_S : KC_U;

            CONSOLE_SEND8(CON_SRC_KEYMAP, CON_SEV_TRACE_EVENT, CON_MSG_KEYMAP_SWAP, _selectedKeymap);

            /* Write the new keymap selection to EEPROM */
            EepromWld_WriteByte((uint8_t)_selectedKeymap);

            return true;

        }
        /* If the SWAPKEY is already down, suppress typematic key presses. */
        else if (_swapKeyDown)
        {
            return true;
        }
    }
    /* SWAPKEY RELEASE */
    else if (_swapKeyDown) 
    {
        /* Update KeyCode to send the release event for the keymap indication */
        keyEvent->code = (_selectedKeymap == USER) ? KC_U : KC_S;

        _swapKeyDown = false;

        return true;
    }

    return false;
}
/* -----------------------------------------------------------------------
 *  Checks to see if the KeyEvent requires a change in the current keymap
 *  layer, and if so, changes the layer appropriately.
 *  Returns:
 *   true  - if the KeyEvent was a layer changed occured
 *   false - otherwise.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline bool Keymap_CheckForLayerChange(KeyEvent* keyEvent)
{

    bool layerChanged = false;

    /* If TOGGLE, switch the non-active layer on PRESS only */
    /* If MOMENTARY, switch layers on both PRESS and RELEASE */
    if ((keyEvent->code == KC_TOGGLE_LAYER && KeyEvent_IsPress(keyEvent)) ||
        (keyEvent->code == KC_MOMENTARY_LAYER))
    {
        _currentKeymapLayer = (_currentKeymapLayer + 1) % 2;
        CONSOLE_SEND8(CON_SRC_KEYMAP, CON_SEV_TRACE_EVENT, CON_MSG_KEYMAP_LAYER_CHANGE, _currentKeymapLayer);
        layerChanged = true;
    }

    /* Absorb the keycode */
    if (keyEvent->code == KC_TOGGLE_LAYER || 
        keyEvent->code == KC_MOMENTARY_LAYER) {
        keyEvent->code = KC_NONE;
    }

    return layerChanged;
}

/* -----------------------------------------------------------------------
 *  Checks to see if the KeyEvent requires a modifier that is currently
 *  in the DOWN state to be suppressed. This is a special case for the 
 *  XT keyboard which requires certain key to be suppressed when they
 *  are converted to PS/2 or USB.
 *  Returns:
 *   KeyCode of modifier to be suppressed
 *   KC_NONE if no modifier should be suppressed.
 *  Note: It is also possible for this function to modify the value of 
 *        the KeyEvent passed as an argument.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
KeyCode Keymap_CheckForSupressedKey(KeyEvent* keyEvent)
{
    
    switch(keyEvent->code)
    {
        /* On the XT keyboard, L_CTRL + NUM_LOCK is mapped to PAUSE. This 
         * causes a problem as CTRL + PAUSE is mapped to Break by both
         * PS/2 and USB. Therefore, L_CTRL has to be suppressed on PRESS. 
         * On RELEASE, PS/2 does not send any code, so we need to change 
         * the KeyCode to KC_NONE to suppress the PAUSE itself.*/
        case KC_PAUSE:
            if (ModifierStatus_IsDown(&_mappedModStatus, MODS_LCTRL))
            {
                if (KeyEvent_IsPress(keyEvent))
                {
                    keyEvent->code = KC_PAUSE;
                    return KC_LCTRL;
                }
                else
                {
                    keyEvent->code = KC_NONE;
                }
            }
            break;
        default:
            break;
    }
    
    return KC_NONE;    
}

/* -----------------------------------------------------------------------
 *  Initialize the keymap.
 *    - Read the last selected keymap index from EEPROM
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Keymap_Init(void)
{
    EepromWld_Init(_eepromSelectedKeymap, SELECTED_KEYMAP_EEPROM_SIZE);
    uint8_t eepromValue = EepromWld_ReadByte();

    if (eepromValue == (uint8_t)USER)
    {
        _selectedKeymap = USER;
    }
    else
    {
        _selectedKeymap = STOCK;
    }
}

/* -----------------------------------------------------------------------
 *  Maps scan codes received from the XT device to the appropriate 
 *  KeyEvents and adds the resulting KeyEvents to the KeyEvent queue.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool Keymap_MapToKeyEvent (KeyEvent* xtEvent, KeyEvent* mappedEvent)
{
    KeyEvent mapped;
    KeyEvent suppressed;

    mappedEvent->code = KC_NONE;

    XtScanCode baseCode = xtEvent->code;
    mapped.action = xtEvent->action;

    suppressed.code = KC_NONE;

    /* Map the XT base scan code to the appropriate KeyCode 
     * using the current layer of the current keymap.*/
    mapped.code = Keymap_MapScanCodeToKeyCode(_keymaps[_selectedKeymap], baseCode, _currentKeymapLayer);

    /* Update the modifier status */
    ModifierStatus_Update(&_mappedModStatus, &mapped);

    /* Check to see if the current key completes the keymap swap combination */
    if (!Keymap_CheckForKeymapSwap(baseCode, &mapped))
    {
        /* Check to see if the current keycode completes a key combination */
        Keymap_CheckForKeyCombo(&mapped);

        /* Check to see if the current keycode is a layer change. */
        Keymap_CheckForLayerChange(&mapped);

        if (mapped.code == KC_NONE)
            return false;

        /* Check to see if the current keycode requires 
         * other keys to be suppressed */
        suppressed.code = Keymap_CheckForSupressedKey(&mapped);

        if (mapped.code == KC_NONE)
            return false;
    }

    /* If a key must be suppressed, add a release before and press 
     * following the current KeyEvent */

    if (suppressed.code != KC_NONE)
    {
        suppressed.action = KEY_ACTION_RELEASE;
        KeyEvent_Copy(&suppressed, mappedEvent);
    }

    KeyEvent_Copy(&mapped, mappedEvent);

    if (suppressed.code != KC_NONE)
    {
        suppressed.action = KEY_ACTION_PRESS;
        KeyEvent_Copy(&suppressed, mappedEvent);
    }


    return true;
}

/* -----------------------------------------------------------------------
 * Description:
 *  Maps a XT scan code to a KeyCode using the specified layer of the 
 *  specified keymap. 
 *
 * Parameters:
 *  keymap     - the keymap to use.
 *  xtScanCode - the XT scan code to be mapped.
 *  layer      - the layer of the keymap to used.
 *
 * Returns: KeyCode
 *   The mapped KeyCode
 *------------------------------------------------------------------------*/
static inline KeyCode Keymap_MapScanCodeToKeyCode(const Keymap* keymap, XtScanCode xtScanCode, uint8_t layer)
{
    return pgm_read_byte(&(keymap->ScanCodeMap[(uint8_t)xtScanCode - 1][layer]));
}

