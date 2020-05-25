#include <inttypes.h>
#include <windows.h>
#include <string.h>
#include "Std_Types.h"
#include "Uart.h"

#define COM                 "COM11"

static HANDLE hComm;
static DCB dcbSerialParams;

Std_ReturnType Uart_Init(char* com)
{
    char comInitializer[10] = "\\\\.\\";
    hComm = CreateFile(strcat(comInitializer, com), GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);

    if(hComm == INVALID_HANDLE_VALUE)
    {
        return E_NOT_OK;
    }
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;  // Setting BaudRate = 9600
    dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
    dcbSerialParams.Parity   = NOPARITY;  // Setting Parity = None
    SetCommState(hComm, &dcbSerialParams);
    return E_OK;
}

uint8_t Uart_Send(char* data, DWORD size)
{
    DWORD bytesRead;
    WriteFile(hComm, data, size, &bytesRead, NULL);
    if(bytesRead == size)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Uart_Receive(char* data, DWORD size)
{
    DWORD bytesRead, dwEventMask;
    uint16_t i=0;
    char TempChar;
    while(i < size)
    {
        SetCommMask(hComm, EV_RXCHAR);
        WaitCommEvent(hComm, &dwEventMask, NULL);
        ReadFile(hComm, &TempChar, sizeof(TempChar), &bytesRead, NULL);
        data[i] = TempChar;
        i++;
    }
}

void Uart_End(void)
{
    CloseHandle(hComm);
}
