#include <stdio.h>

#include "console_expansion.h"
#include "con_exp_keymap.h"
#include "con_msg_keymap.h"

static char* keymapString[] =
{
    "Stock",
    "User",
};

static void Handler(char* out, ConsoleMessage* message);

char keymapSourceText[11] = "KEYMAP";

ConsoleMessageHandler keymapConsoleHandler = &Handler;

void Handler(char* out, ConsoleMessage* message)
{
    out[0] = 0;
    switch (message->messageId)
    {
        case CON_MSG_KEYMAP_LAYER_CHANGE:
            {
                uint8_t layer = message->data.type8.data1;
                sprintf(out, "Layer changed to: %d", layer);
            }
            break;

        case CON_MSG_KEYMAP_COMBO:
            {
                uint8_t data1 = message->data.type888.data1;
                uint8_t data2 = message->data.type888.data2;
                uint8_t data3 = message->data.type888.data3;

                sprintf(out, "Combo: %02X + %02X -> %02X", data1, data2, data3);
            }
            break;
        case CON_MSG_KEYMAP_SWAP:
            {
                uint8_t data = message->data.type8.data1;
                sprintf(out, "Keymap swap to: %s", keymapString[data]);
            }
            break;

        default:
            sprintf(out, "Unknown Message: %02X", message->messageId);
            break;
    }
    return;
}
