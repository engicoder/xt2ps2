/* =======================================================================
 * key_event.h
 * 
 * Purpose:
 *  Encapsulates the key code and press/release event type.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */

#ifndef KEYEVENT_H_
#define KEYEVENT_H_

#include <stdbool.h>
#include <stdint.h>

#include "keycode.h"

typedef enum _KeyAction
{
    KEY_ACTION_RELEASE,
    KEY_ACTION_PRESS,
    KEY_ACTION_NONE,
} KeyAction;

typedef struct _KeyEvent
{
    KeyAction action;
    KeyCode code;
    
} KeyEvent;

/* -----------------------------------------------------------------------
 * Description:
 *  Determines if the KeyEvent is a press event.
 *
 * Parameters:
 *  n/a
 *
 * Returns: bool
 *   true - if the KeyEvent is press
 *   false - if the KeyEvent is release
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline bool KeyEvent_IsPress(const KeyEvent* keyEvent)
{
    return keyEvent->action == KEY_ACTION_PRESS;
}


static inline bool KeyEvent_IsRelease(const KeyEvent* keyEvent)
{
    return keyEvent->action == KEY_ACTION_RELEASE;
}

/** -----------------------------------------------------------------------
 * @brief Copies the value of one KeyEvent to another.
 *
 * @param source the KeyEvent to copy from
 * @param target the KeyEvent to copy to
 *
 * @returns n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void KeyEvent_Copy(const KeyEvent* source, KeyEvent* target)
{
    target->action = source->action;
    target->code = source->code;
}

static inline KeyAction KeyEvent_Action(const KeyEvent* keyEvent)
{
    return keyEvent->action;
}

static inline KeyCode KeyEvent_Code(const KeyEvent* keyEvent)
{
    return keyEvent->code;
}

static inline void KeyEvent_Init(KeyEvent* keyEvent, KeyAction action, KeyCode code)
{
    keyEvent->action = action;
    keyEvent->code = code;
}


#define KEY_EVENT_SIZE 2


#endif /* KEYEVENT_H_ */