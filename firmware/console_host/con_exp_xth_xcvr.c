#include <stdio.h>

#include "console_expansion.h"
#include "con_msg_xth_xcvr.h"

static void Handler(char* out, ConsoleMessage* message);


char xthXcvrSourceText[11] = "XT XCVR";

ConsoleMessageHandler xthXcvrConsoleHandler = &Handler;



char* xtScanCodeStrings[] =
{
    "NO_CODE     ",
    "ESCAPE      ",
    "1           ",
    "2           ",
    "3           ",
    "4           ",
    "5           ",
    "6           ",
    "7           ",
    "8           ",
    "9           ",
    "0           ",
    "MINUS       ",
    "EQUAL       ",
    "BACKSPACE   ",
    "TAB         ",
    "Q           ",
    "W           ",
    "E           ",
    "R           ",
    "T           ",
    "Y           ",
    "U           ",
    "I           ",
    "O           ",
    "P           ",
    "LBRACKET    ",
    "RBRACKET    ",
    "ENTER       ",
    "CONTROL     ",
    "A           ",
    "S           ",
    "D           ",
    "F           ",
    "G           ",
    "H           ",
    "J           ",
    "K           ",
    "L           ",
    "SEMI_COLON  ",
    "QUOTE       ",
    "GRAVE       ",
    "LSHIFT      ",
    "BACKSLASH   ",
    "Z           ",
    "X           ",
    "C           ",
    "V           ",
    "B           ",
    "N           ",
    "M           ",
    "COMMA       ",
    "PERIOD      ",
    "FWD_SLASH   ",
    "RSHIFT      ",
    "KP_ASTERISK ",
    "ALT         ",
    "SPACE       ",
    "CAPSLOCK    ",
    "F1          ",
    "F2          ",
    "F3          ",
    "F4          ",
    "F5          ",
    "F6          ",
    "F7          ",
    "F8          ",
    "F9          ",
    "F10         ",
    "NUM_LOCK    ",
    "SCROLL_LOCK ",
    "KP_7        ",
    "KP_8        ",
    "KP_9        ",
    "KP_MINUS    ",
    "KP_4        ",
    "KP_5        ",
    "KP_6        ",
    "KP_PLUS     ",
    "KP_1        ",
    "KP_2        ",
    "KP_3        ",
    "KP_0        ",
    "KP_DOT      ",
    "MAX_CODE    "
};

char* actionString[] =
{
    "BREAK",
    "MAKE ",
};

void Handler(char* out, ConsoleMessage* message)
{
    switch (message->messageId)
    {
        case CON_MSG_XTH_XCVR_BAD_START_BIT:
            sprintf(out, "Bad start bit");
            break;

        case CON_MSG_XTH_XCVR_SOFT_RESET:
            sprintf(out, "Soft reset");
            break;

        case CON_MSG_XTH_XCVR_HARD_RESET:
            sprintf(out, "Hard reset");
            break;

        case CON_MSG_XTH_XCVR_RECV_SCODE:
            {
                uint8_t data = message->data.type8.data1;
                int action = ((data & (1 << 7))!= 0) ? 0 : 1;
                sprintf(out, "%2X -> %s, %s", data, actionString[action], xtScanCodeStrings[(data & 0x7F)]);
            }
            break;

        default:
            out[0] = 0;
            break;
    }
    return;
}

