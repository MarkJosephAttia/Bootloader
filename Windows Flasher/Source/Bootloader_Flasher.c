/**
 * @file elfParser.c
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This is the elf parser source code
 * @version 0.1
 * @date 2020-05-13
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <stdio.h>
#include <inttypes.h>
#include <windows.h>
#include "Std_Types.h"
#include "Uart.h"
#include "Protocol.h"

#define MAX_BUFFER_SIZE         1000000

typedef struct
{
    unsigned char ident[16];
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t phoff;
    uint32_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
} elfHeader_t;

typedef struct
{
    uint32_t type;
    uint32_t offset;
    uint32_t vaddr;
    uint32_t paddr;
    uint32_t filesz;
    uint32_t memsz;
    uint32_t flags;
    uint32_t align;
} elfProgHeader_t;

typedef struct
{
    uint32_t   name;
    uint32_t   type;
    uint32_t   flags;
    uint32_t   addr;
    uint32_t   offset;
    uint32_t   size;
    uint32_t   link;
    uint32_t   info;
    uint32_t   addralign;
    uint32_t   entsize;
} secHeader_t;


typedef struct
{
    uint8_t* header;
    uint32_t size;
    uint32_t address;
}sendPack_t;

int main(int argc, const char* argv[])
{
    uint32_t elfSize;
    uint32_t hdr, byt;
    uint8_t* elfBuffer;
    uint16_t nHeaders;
    uint32_t addItr, modifiedAddress;
    char* filename;
    char* comPort;
    int argItr;
    elfHeader_t* elfHeader;
    elfProgHeader_t* elfProgHeader;
    secHeader_t* secHeader;
    sendPack_t* sendPack;
    FILE* elf;

    /* Checking For The User Inputs */
    if(argc == 3)
    {
        comPort = argv[1];
        if(Uart_Init(comPort) == E_NOT_OK)
        {
            printf("Please Check The Com Port You Have Entered\n");
            return 0;
        }
        filename = argv[2];
        elf = fopen(filename, "r");
        if(!elf)
        {
            printf("Please Enter A Valid File\n");
            return 0;
        }
        else
        {
            fseek(elf,0,SEEK_END);
            elfSize = ftell(elf);
            fseek(elf,0,SEEK_SET);
            elfBuffer = (uint8_t*)malloc(elfSize);
            fread(elfBuffer,elfSize,1,elf);
            elfHeader = (elfHeader_t*)elfBuffer;
            if(elfHeader->ident[0] != 0x7F || elfHeader->ident[1] != 'E' || elfHeader->ident[2] != 'L' || elfHeader->ident[3] != 'F')
            {
                printf("The File You Have Entered In Not An Elf File\n");
                return 0;
            }
        }

    }
    else
    {
        printf("Please Enter The Correct Arguments:\n\t\tBootloader_Flasher COMx example.elf\n");
        return 0;
    }

    sendPack = malloc(elfHeader->phnum * sizeof(sendPack_t));
    nHeaders = elfHeader->phnum;
    elfProgHeader = (elfProgHeader_t*) &elfBuffer[elfHeader->phoff];

    secHeader = (secHeader_t*) &elfBuffer[elfHeader->shoff];

    /* Buffering The Required Data From Program Header */
    for(hdr=0; hdr<elfHeader->phnum; hdr++)
    {
        sendPack[hdr].header = malloc(elfProgHeader->filesz);
        sendPack[hdr].size = elfProgHeader->filesz;
        sendPack[hdr].address = elfProgHeader->paddr;
        for(byt=0; byt<elfProgHeader->filesz; byt++)
        {
            sendPack[hdr].header[byt] = elfBuffer[elfProgHeader->offset + byt];
        }
        printf("0x%08x\n",elfProgHeader->paddr);
        elfProgHeader++;
    }
    fclose(elf);

    /* Sending The Buffered Data */
    for(hdr=0; hdr<nHeaders; hdr++)
    {
        if(sendPack[hdr].size != 0)
        {
            addItr=0;
            modifiedAddress = sendPack[hdr].address;
            while(sendPack[hdr].size > 1024)
            {
                Protocol_Send(PROTOCOL_ADDRESS,4,(uint8_t*)&modifiedAddress);
                Protocol_Send(PROTOCOL_DATA,1024,&sendPack[hdr].header[addItr]);
                addItr+=1024;
                sendPack[hdr].size -= 1024;
                modifiedAddress += 1024;
            }
            Protocol_Send(PROTOCOL_ADDRESS,4,(uint8_t*)&modifiedAddress);
            Protocol_Send(PROTOCOL_DATA,sendPack[hdr].size,&sendPack[hdr].header[addItr]);
        }
    }
    Protocol_Send(PROTOCOL_EOT,4,&sendPack[0].address);
    Uart_End();
    return 0;
}
