
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include <windows.h>
#include <windef.h>
#include <winnt.h>
#include <winbase.h>

#include "ftd2xx.h"

#include <string.h>

#include "getopt.h"

#include "console.h"
#include "console_expansion.h"
#include "con_exp_test.h"
#include "con_exp_xth_xcvr.h"
#include "con_exp_xth_kbd.h"
#include "con_exp_ps2d_kbd.h"
#include "con_exp_ps2d_xcvr.h"
#include "con_exp_xt2ps2.h"
#include "con_exp_keymap.h"

typedef enum _DataSource
{
    SOURCE_FTDI,
    SOURCE_FILE,
} DataSource;

typedef struct _Options
{
    long baudRate;
    char rawFilename[255];
    char textFilename[255];
    char sourceFilename[255];
    DataSource dataSource;
} Options;



Options options =
{
    38400,
    "out.bin",
    "out.txt",
    "",
    SOURCE_FTDI
};
FILE* source;
FT_HANDLE ft_handle;


BOOL WINAPI exitHandler(DWORD signal);

bool FtdiInit(unsigned long baudRate);
bool SourceFileOpen( char* sourceFilename);

bool DataSourceInit(Options* options);
bool DataSourceGetNext(uint8_t* data, Options* options);
void DataSourceExit(Options* options);

int main(int argc, char** argv)
{


    int option_index = 0;


    while (( option_index = getopt(argc, argv, "u:g")) != -1){

    switch (option_index)
    {
        case 'b':
            options.baudRate = atol(optarg);
            break;
        case 'r':
            strncpy(options.rawFilename, optarg, 255);
            break;
        case 't':
            strncpy(options.textFilename, optarg, 255);
            break;
        case 's':
            strncpy(options.sourceFilename, optarg, 255);
            options.dataSource = SOURCE_FILE;
            break;
        default:
            fprintf(stderr, "Invalid option '%c'\n", option_index);
            return 1;
        } //end block for switch
    }  //end block for while


    if (!SetConsoleCtrlHandler(exitHandler, TRUE)) {
        fprintf(stderr, "\nERROR: Could not set exit handler");
        return 1;
    }


    if (!ConsoleExpansion_Init(options.rawFilename, options.textFilename, "XT2PS2 Console Host"))
    {
        return 1;
    }


    bool initSuccess = DataSourceInit(&options);

    if (!initSuccess)
    {
        return 1;
    }

    ConsoleExpansion_RegisterExpander(CON_SRC_TEST, testConsoleHandler, testSourceText);
    ConsoleExpansion_RegisterExpander(CON_SRC_XTH_XCVR, xthXcvrConsoleHandler, xthXcvrSourceText);
    ConsoleExpansion_RegisterExpander(CON_SRC_XTH_KBD, xthKbdConsoleHandler, xthKbdSourceText);
    ConsoleExpansion_RegisterExpander(CON_SRC_PS2D_KBD, ps2dKbdConsoleHandler, ps2dKbdSourceText);
    ConsoleExpansion_RegisterExpander(CON_SRC_PS2D_XCVR, ps2dXcvrConsoleHandler, ps2dXcvrSourceText);
    ConsoleExpansion_RegisterExpander(CON_SRC_XT2PS2, xt2Ps2ConsoleHandler, xt2Ps2SourceText);
    ConsoleExpansion_RegisterExpander(CON_SRC_KEYMAP, keymapConsoleHandler, keymapSourceText);

    while (1)
    {
        uint8_t data;
        if (DataSourceGetNext(&data, &options))
        {
            ConsoleExpansion_ProcessData(data);
        }

    }

    ConsoleExpansion_Exit();

    return 0;
}



bool FtdiInit(unsigned long baudRate)
{
    LONG comPortNumber;
    FT_STATUS ft_status;

    ft_status = FT_Open(0,&ft_handle);

    if (ft_status != FT_OK)
    {
        fprintf(stderr, "No FTDI serial port detected");
        return false;
    }

    ft_status = FT_GetComPortNumber(ft_handle, &comPortNumber);

    FT_SetBaudRate(ft_handle, baudRate);

    FT_SetDataCharacteristics(ft_handle,       // Handle of the chip(FT232)
                              FT_BITS_8,       // No of Data bits = 8
                              FT_STOP_BITS_1,  // No of Stop Bits = 1
                              FT_PARITY_NONE   // Parity = NONE
                              );

    FT_SetFlowControl(ft_handle, FT_FLOW_NONE, 0, 0);

    FT_Purge(ft_handle, FT_PURGE_RX | FT_PURGE_TX);

    printf("Connected to FTDI serial converter on COM%ld\n", comPortNumber);
    printf("Ready..\n");
    printf("\n");

    return true;
}



bool SourceFileOpen( char* sourceFilename)
{
    source = fopen(sourceFilename, "r");
    if (source == NULL)
    {
        fprintf(stderr, "Unable to open file to read source data: %s", sourceFilename);
        return false;
    }

    return true;
}


bool FtdiRead(uint8_t* data)
{
    DWORD bytesToRead;
    unsigned char byteRead;
    DWORD bytesRead;

    FT_STATUS ft_status;

    ft_status = FT_GetQueueStatus(ft_handle, &bytesToRead);
    if (ft_status == FT_OK && bytesToRead > 0)
    {
        ft_status = FT_Read(ft_handle, &byteRead, 1, &bytesRead );
        if (ft_status != FT_OK)
        {
            fprintf(stderr, "Failure reading from FTDI");
            return false;
        }
        else
        {
            *data = byteRead;
        }
    }
    else
    {
        return false;
    }


    return true;
}

bool SourceFileRead(uint8_t* data)
{
    size_t bytesRead;

    bytesRead = fread(&data, 1, 1, source);

    if (bytesRead < 1)
    {
        fprintf(stderr, "Failure reading from source file");
        return false;
    }

    return data;
}


bool DataSourceInit(Options* options)
{
    bool result = false;

    switch (options->dataSource)
    {
        case SOURCE_FTDI:
            result = FtdiInit(options->baudRate);
            break;
        case SOURCE_FILE:
            result = SourceFileOpen(options->sourceFilename);
            break;
    }
    return result;
}

bool DataSourceGetNext(uint8_t* data, Options* options)
{
    bool result;
    switch (options->dataSource)
    {
        case SOURCE_FTDI:
            result = FtdiRead(data);
            break;
        case SOURCE_FILE:
            result = SourceFileRead(data);
            break;
    }

    return result;
}

void DataSourceExit(Options* options)
{
    switch (options->dataSource)
    {
        case SOURCE_FTDI:
            FT_Close(ft_handle);
            break;
        case SOURCE_FILE:
            fclose(source);
            break;
    }

}

BOOL WINAPI exitHandler(DWORD signal)
{
    ConsoleExpansion_Exit();
    DataSourceExit(&options);

    printf("\n");
    printf("Exiting...\n");

    return FALSE;
}
