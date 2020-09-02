#include <stdio.h>

#include "console_expansion.h"
#include "con_exp_ps2d_kbd.h"
#include "con_msg_ps2d_kbd.h"

char * ps2CommandStrings[] =
{
   "SET_LEDS",
   "ECHO",
   "Invalid",
   "GET/SET SCAN CODE SET",
   "Invalid",
   "ID KEYBOARD",
   "SET_REPEAT_RATE",
   "ENABLE",
   "DISABLE",
   "SET DEFAULTS",
   "SET ALL TYPEMATIC",
   "SET ALL MAKE BREAK",
   "SET ALL MAKE ONLY",
   "SET ALL MAKE BREAK TYPEMATIC",
   "SET ONE TYPEMATIC",
   "SET_ONE MAKE BREAK",
   "SET_ONE MAKE ONLY",
   "RESEND",
   "RESET",
};

char* ps2ResponseStrings[] =
{
    " ",
    "BAT COMPLETE",
    "BAT FAILURE",
    "ECHO",
    "ACK",
    "RESEND",
};

static char* actionString[] =
{
    "RELEASE",
    "PRESS",
};

typedef enum
{
    PS2_RESP_EMPTY,
    PS2_RESP_BAT_COMPLETE,
    PS2_RESP_BAT_FAILURE,
    PS2_RESP_ECHO,
    PS2_RESP_ACK,
    PS2_RESP_RESEND,
} Ps2Response;


static void Handler(char* out, ConsoleMessage* message);

char ps2dKbdSourceText[11] = "PS2 KBD";

ConsoleMessageHandler ps2dKbdConsoleHandler = &Handler;

void Handler(char* out, ConsoleMessage* message)
{
    out[0] = 0;
    static uint8_t clocksPerMs = 50;
    switch (message->messageId)
    {
        case CON_MSG_PS2D_KBD_XMIT_INT:
            sprintf(out, "Transmission interrupted by host.");
            break;

        case CON_MSG_PS2D_KBD_CLKS_MS:
            {
                clocksPerMs = message->data.type8.data1;
                sprintf(out, "%d clocks/ms", clocksPerMs);
            }
            break;

        case CON_MSG_PS2D_KBD_TM_DELAY:
            {
                uint16_t delay = message->data.type16.data1;
                sprintf(out, "Typematic delay: %d ms", delay/clocksPerMs);
            }
            break;

        case CON_MSG_PS2D_KBD_TM_RATE:
            {
                uint16_t rate = message->data.type16.data1;
                sprintf(out, "Typematic rate: %d cps", 1000 * clocksPerMs / rate);
            }
            break;

        case CON_MSG_PS2D_KBD_CMD_RECV:
            {
                uint8_t data = message->data.type8.data1;
                if (data >= 0xED)
                    sprintf(out, "CMD: %s", ps2CommandStrings[data - 0xED]);
                else
                    sprintf(out, "CMD: %02X - Unexepected", data);
            }
            break;
        case CON_MSG_PS2D_KBD_RSP_SENT:
            {
                uint8_t data = message->data.type8.data1;
                Ps2Response response = PS2_RESP_EMPTY;
                switch (data)
                {
                    case 0xAA:
                        response = PS2_RESP_BAT_COMPLETE;
                        break;
                    case 0xFC:
                        response = PS2_RESP_BAT_FAILURE;
                        break;
                    case 0xFA:
                        response = PS2_RESP_ACK;
                        break;
                    case 0xEE:
                        response = PS2_RESP_ECHO;
                        break;
                    case 0xFE:
                        response = PS2_RESP_RESEND;
                        break;
                    default:
                        response = PS2_RESP_EMPTY;
                }
                sprintf(out, "RSP: %s", ps2ResponseStrings[response]);
            }
            break;

        case CON_MSG_PS2D_KBD_TM_ACTIVE:
            {
                uint8_t data = message->data.type8.data1;
                sprintf(out, "%02X: typematic active", data);
            }
            break;

        case CON_MSG_PS2D_KBD_TM_INACTIVE:
            {
                sprintf(out, "typematic inactive");
            }
            break;


         case CON_MSG_PS2D_KBD_KEYEVENT:
            {
                uint8_t code = message->data.type88.data1;
                uint8_t action = message->data.type88.data2;
                sprintf(out, "KeyEvent: %02X %s", code, actionString[action]);
            }
            break;

         case CON_MSG_PS2D_KBD_BUFFER_COUNT:
            {
                uint8_t count = message->data.type8.data1;
                sprintf(out, "Send buffer: %02X", count);
            }
            break;

         case CON_MSG_PS2D_KBD_POR_START:
            {
                uint16_t length = message->data.type16.data1;
                sprintf(out, "POR started: %d", length);
            }
            break;

         case CON_MSG_PS2D_KBD_POR_END:
            {
                sprintf(out, "POR complete");
            }
            break;


         case CON_MSG_PS2D_KBD_BAT_START:
            {
                uint16_t length = message->data.type16.data1;
                sprintf(out, "BAT started: %d", length);
            }
            break;

         case CON_MSG_PS2D_KBD_BAT_END:
            {
                sprintf(out, "BAT complete");
            }
            break;

        default:
            sprintf(out, "Unknown Message: %02X", message->messageId);
            break;
    }
    return;
}
