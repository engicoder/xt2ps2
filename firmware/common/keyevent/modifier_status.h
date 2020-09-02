/*
 * ModifierStatus.h
 *
 * Created: 9/11/2015 4:31:32 PM
 *  Author: Ben
 *  Modifier status tracks the state of all modifier keys. This information is used for keymap swapping
 *  and supporting key combinations.
 */ 


#ifndef MODIFIERSTATUS_H_
#define MODIFIERSTATUS_H_

#include <stdbool.h>
#include <stdint.h>

#include "keyevent.h"

typedef enum _Modifiers
{
    MODS_NONE    = 0,
    MODS_LCTRL   = (1 << 0),
    MODS_LSHIFT  = (1 << 1),
    MODS_LALT    = (1 << 2),
    MODS_LGUI    = (1 << 3),
    MODS_RCTRL   = (1 << 4),
    MODS_RSHIFT  = (1 << 5),
    MODS_RALT    = (1 << 6),
    MODS_RGUI    = (1 << 7),
} Modifiers;

typedef struct _ModifierStatus
{
    Modifiers Mods;
} ModifierStatus;

static inline bool ModifierStatus_None(ModifierStatus* status)
{
    return status->Mods == MODS_NONE;
}

static inline void ModifierStatus_Clear(ModifierStatus* status)
{
    status->Mods = MODS_NONE;
}

static inline bool ModifierStatus_IsOnly(ModifierStatus* status, Modifiers modStatus)
{
    return (status->Mods == modStatus);
}

static inline bool ModifierStatus_IsDown(ModifierStatus* status, Modifiers modStatus)
{
    return (modStatus == (status->Mods & modStatus));
}

static inline void ModifierStatus_Set(ModifierStatus* status, Modifiers modifier, bool isDown)
{
    if (isDown)
        status->Mods |= modifier;
    else
        status->Mods &= ~modifier;

    return;
}

void ModifierStatus_Update(ModifierStatus* status, KeyEvent* keyEvent);

#define MODIFIER_STATUS_NONE { MODS_NONE }

#endif /* MODIFIERSTATUS_H_ */