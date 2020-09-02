/*
 * modifier_status->c
 *
 * Created: 9/11/2015 4:33:35 PM
 *  Author: Ben
 */ 

#include "modifier_status.h"
#include "keyevent.h"
#include "keycode.h"

/* -----------------------------------------------------------------------
 *  Update the current modifier status
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
 void ModifierStatus_Update(ModifierStatus* modStatus, KeyEvent* keyEvent)
 {
     uint8_t isDown = KeyEvent_IsPress(keyEvent);

     switch(KeyEvent_Code(keyEvent))
     {
         case KC_LCTRL:  ModifierStatus_Set(modStatus, MODS_LCTRL, isDown ); break;
         case KC_RCTRL:  ModifierStatus_Set(modStatus, MODS_RCTRL, isDown ); break;
         case KC_LALT:   ModifierStatus_Set(modStatus, MODS_LALT, isDown ); break;
         case KC_RALT:   ModifierStatus_Set(modStatus, MODS_RALT, isDown ); break;
         case KC_LGUI:   ModifierStatus_Set(modStatus, MODS_LGUI, isDown ); break;
         case KC_RGUI:   ModifierStatus_Set(modStatus, MODS_RGUI, isDown ); break;
         case KC_LSHIFT: ModifierStatus_Set(modStatus, MODS_LSHIFT, isDown ); break;
         case KC_RSHIFT: ModifierStatus_Set(modStatus, MODS_RSHIFT, isDown ); break;
         default:
            break;
     }

     return;
 }