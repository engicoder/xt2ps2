#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>
#include <windef.h>
#include <winnt.h>
#include <winbase.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>


#include "console.h"
#include "console_expansion.h"

#define OUT_STRING_MAX 60

#define SOURCE_TEXT_MAX_LEN 10

uint8_t consoleMessageLength[] = { 2, 3, 4, 5, 6, 4, 6, 6, 5, 6, };

typedef struct _ConsoleMessageExpander
{
	ConsoleMessageHandler handler;
	char sourceText[SOURCE_TEXT_MAX_LEN + 1]; /* Max string length of 10 */
} ConsoleMessageExpander;

ConsoleMessageExpander expanders[CON_SRC_COUNT];


typedef enum _state
{
    IDLE,
    RECV,
    OUTPUT,
} State;


void DefaultMessageHandler(char* out, ConsoleMessage* message);
char defaultSourceText[] = "UNKOWN";

void ConsoleHandler(char* out, ConsoleMessage* message);
char consoleSourceText[] = "CONSOLE";

FILE* raw;
FILE* decode;

struct timeval timeVal;
time_t nowtime;
struct tm *nowtm;
char tmbuf[64];

bool ConsoleExpansion_Init(char* binaryFilename, char* textFilename, char* startupText)
{

    raw = fopen(binaryFilename, "w+");
    if (raw == NULL)
    {
        printf("Unable to open file to write raw data");
        return false;
    }

    decode = fopen(textFilename, "w+");
    if (decode == NULL)
    {
        fclose(raw);
        printf("Unable to open file to write decoded data");
        return false;
    }

    printf("*\n");
    printf("%s\n", startupText);
    printf("*\n");
    printf("\n");

    for (int i = 0; i < CON_SRC_COUNT; i++)
    {
        expanders[i].handler = DefaultMessageHandler;
        strncpy(expanders[i].sourceText, defaultSourceText, SOURCE_TEXT_MAX_LEN);
    }

    ConsoleExpansion_RegisterExpander(CON_SRC_CONSOLE, ConsoleHandler, consoleSourceText);

    return true;
}

void ConsoleExpansion_RegisterExpander(ConsoleSource source, ConsoleMessageHandler handler, char* sourceText)
{
    expanders[(int)source].handler = handler;
    if (sourceText != 0)
        strncpy(expanders[(int)source].sourceText, sourceText, SOURCE_TEXT_MAX_LEN);
}

void ConsoleExpansion_Exit(void)
{
    fclose(raw);
    fclose(decode);
}

void ConsoleExpansion_ProcessData(uint8_t data)
{
    fputc(data, raw);

    static State state = IDLE;
    static ConsoleMessage message;
    static uint8_t byteCount;
    ConsoleMessageExpander* expander;

    char outString[OUT_STRING_MAX];

    switch (state)
    {
        case IDLE:
            message.sourceType = data;
            gettimeofday(&timeVal, NULL);
            nowtime = timeVal.tv_sec;
            nowtm = localtime(&nowtime);
            strftime(tmbuf, sizeof tmbuf, "%H:%M:%S", nowtm);
            state = RECV;
            byteCount = 1;
            break;

        case RECV:
            message.messageBytes[byteCount] = data;
            byteCount++;
            ConsoleMessageType type = ConsoleMessage_Type(&message);
            int length = consoleMessageLength[type];
            if (byteCount == length)
            {
                ConsoleSource source = ConsoleMessage_Source(&message);
                if (source < CON_SRC_COUNT) {
                    expander = &expanders[ConsoleMessage_Source(&message)];
                    expander->handler(outString, &message);
                    state = OUTPUT;
                } else {
                    printf("Message discarded, unknown source: %d\n", source);
                    fprintf(decode, "Message discarded, unknown source: %d\n", source);

                }
            }

            break;

        case OUTPUT:
            state = IDLE;
            break;
    }

    if (state == OUTPUT)
    {
        printf("%s.%03ld ", tmbuf, (timeVal.tv_usec/1000));
        printf("%10s: %s\n", expander->sourceText, outString);
        fprintf(decode, "%s.%03ld ", tmbuf, (timeVal.tv_usec/1000));
        fprintf(decode, "%10s: %s\n", expander->sourceText, outString);

        state = IDLE;
    }

    return;
}


void ConsoleHandler(char* out, ConsoleMessage* message)
{
    switch (message->messageId)
    {
        case CON_MSG_CONSOLE_START:
            sprintf(out, "%s", "Console started.");
            break;

        default:
            out[0] = 0;
            break;
    }
}

void DefaultMessageHandler(char* out, ConsoleMessage* message)
{
    ConsoleMessageType type = ConsoleMessage_Type(message);

    char dataString[16];

    switch(type)
    {
        case CON_MSG_DATA0:
            dataString[0] = 0;
            break;
        case CON_MSG_DATA8:
            sprintf(dataString, "%02X", message->data.type8.data1);
            break;
        case CON_MSG_DATA88:
            sprintf(dataString, "%02X %02X", message->data.type88.data1, message->data.type88.data2);
            break;
        case CON_MSG_DATA888:
            sprintf(dataString, "%02X %02X %04X", message->data.type888.data1, message->data.type888.data2, message->data.type888.data3);
            break;
        case CON_MSG_DATA8888:
            sprintf(dataString, "%02X %02X %02X %02X", message->data.type8888.data1, message->data.type8888.data2, message->data.type8888.data3, message->data.type8888.data3);
            break;
        case CON_MSG_DATA16:
            sprintf(dataString, "%04X", message->data.type16.data1);
            break;
        case CON_MSG_DATA1616:
            sprintf(dataString, "%04X %04X", message->data.type1616.data1, message->data.type1616.data2);
            break;
        case CON_MSG_DATA32:
            sprintf(dataString, "%08X", message->data.type32.data1);
            break;
        case CON_MSG_DATA816:
            sprintf(dataString, "%02X %04X", message->data.type816.data1, message->data.type816.data2);
            break;
        case CON_MSG_DATA8816:
            sprintf(dataString, "%02X %02X %04X", message->data.type8816.data1, message->data.type8816.data2, message->data.type8816.data3);
            break;
        default:

            break;
    }

    sprintf(out, "S: %02X ID: %02X [ %s ]", ConsoleMessage_Source(message), message->messageId, dataString);
}
