#include <stdio.h>

#include "console_expansion.h"
#include "con_exp_ps2d_xcvr.h"
#include "con_msg_ps2d_xcvr.h"




static void Handler(char* out, ConsoleMessage* message);

char ps2dXcvrSourceText[11] = "PS2 XCVR";

ConsoleMessageHandler ps2dXcvrConsoleHandler = &Handler;

static char ps2dXcvrStateStrings[5][13] =
{
    "DISABLED",
    "IDLE",
    "INHIBIT",
	"RECEIVING",
	"TRANSMITTING",
};


void Handler(char* out, ConsoleMessage* message)
{
    switch (message->messageId)
    {
        case CON_MSG_PS2D_XCVR_RECV:
            {
                uint8_t data = message->data.type8.data1;
                sprintf(out, "%02X <-", data);
            }
            break;

        case CON_MSG_PS2D_XCVR_RECV_ERROR:
            sprintf(out, "Frame error detected; frame ignored.");
            break;

        case CON_MSG_PS2D_XCVR_XMIT:
            {
                uint8_t data = message->data.type8.data1;
                sprintf(out, "%02X ->",data);
            }
            break;

        case CON_MSG_PS2D_XCVR_XMIT_BUFFER_FULL:
            {
                sprintf(out, "Transmit: Buffer full");
            }
            break;


        case CON_MSG_PS2D_XCVR_XMIT_BUSY:
            {
                uint8_t data = message->data.type8.data1;
                sprintf(out, "Transmit: Bus not ready. State: %s", ps2dXcvrStateStrings[data]);
            }
            break;

        case CON_MSG_PS2D_XCVR_REXMIT:
            {
                uint8_t data = message->data.type8.data1;
                sprintf(out, "%02X -> [retransmit]", data);
            }
            break;

        case CON_MSG_PS2D_XCVR_CLK_PERIOD:
            {
                uint8_t clockPeriod = message->data.type8.data1;
                float kHz = 1000.0/clockPeriod;
                sprintf(out, "PS/2 clock [T: %d us f: %.1f kHz]", clockPeriod, kHz);
            }
            break;


        default:
            sprintf(out, "Unknown Message: %02X", message->messageId);
            break;
    }
    return;
}
