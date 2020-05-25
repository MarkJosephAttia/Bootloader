/**
 * @file Bootloader.c
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief A Simple Bootloader Application Over Uart Using A Designed Software Protocol
 * @version 0.1
 * @date 2020-05-25
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Std_Types.h"
#include "Uart.h"
#include "Fpec.h"
#include "Rcc.h"
#include "HRcc.h"
#include "Nvic.h"
#include "Gpio.h"
#include "Switch.h"
#include "Protocol.h"

#define MAX_DATA_TO_BUFFER  1024

/* Application Exist Marker Value */
#define APP_EXIST                            0xAAAA
#define APP_NOT_EXIST                   0xBBBB

/* Flash Markers Addresses */
const uint16_t* appExistMarker = (uint16_t*)0x0800FFE0;
const uint16_t* addressMarker  = (uint16_t*)0x0800FFD0;

/* Default Application Start Address */
static uint32_t appAddress = 0x08008000;

typedef void (*app_t)(void);

int main(void)
{
  uint16_t msgType, length;
  uint8_t data[MAX_DATA_TO_BUFFER];
  gpio_t gpio;
  uint32_t* tmpAddress;
  uint8_t switchState;
  app_t app;
  /* The Uart Configurations */
  Uart_cfg_t cfg = {
      .baudRate = 9600,
      .stopBits = UART_STOP_ONE_BIT,
      .parity = UART_NO_PARITY,
      .flowControl = UART_FLOW_CONTROL_DIS,
      .sysClk = 8000000,
      .linEn = UART_LIN_DIS,
      .interrupts = UART_INTERRUPT_DIS,
      .uartModule = UART1
  };
  Switch_Init();
  Switch_GetSwitchStatus(BOOTLOADER_SWITCH, &switchState);
  if(APP_EXIST == *appExistMarker && switchState == SWITCH_NOT_PRESSED)
  {
    tmpAddress = (uint32_t*)*(uint32_t*)addressMarker;
    app = (app_t)*(tmpAddress+1);
    asm("MSR msp, %0\n" : : "r" (*tmpAddress) : "sp");
    app();
  }
  else
  {
    /* Uart Hardware Configurations */
    Rcc_SetApb2PeriphClockState(RCC_IOPA_CLK_EN, RCC_PERIPH_CLK_ON);
    Rcc_SetApb2PeriphClockState(RCC_AFIO_CLK_EN, RCC_PERIPH_CLK_ON);
    gpio.pins = GPIO_PIN_9;
    gpio.port = GPIO_PORTA;
    gpio.mode = GPIO_MODE_AF_OUTPUT_PP;
    gpio.speed = GPIO_SPEED_50_MHZ;
    Gpio_InitPins(&gpio);
    gpio.pins = GPIO_PIN_10;
    gpio.mode = GPIO_MODE_INPUT_PULL_UP;
    Gpio_InitPins(&gpio);
    Rcc_SetApb2PeriphClockState(RCC_USART1_CLK_EN, RCC_PERIPH_CLK_ON);
    Nvic_EnableInterrupt(NVIC_IRQNUM_USART1);
    HRcc_SystemClockInit();
    Uart_Init(&cfg);
    Fpec_Unlock();
    Fpec_ErasePage((uint32_t*)addressMarker);
    Fpec_WriteHalfWord((uint16_t*)appExistMarker, APP_NOT_EXIST);
    while(1)
    {
      Protocol_Receive(&msgType, &length, data);
      switch(msgType)
      {
      case PROTOCOL_ADDRESS:
        /* Address Received */
        appAddress = *(uint32_t*)data;
        Protocol_SendAck();
        break;
      case PROTOCOL_EOT:
        /* End Of Transmission Received */
        Fpec_ErasePage((uint32_t*)addressMarker);
        Fpec_WriteBlock((uint16_t*)addressMarker, (uint16_t*)data, 2);
        Fpec_WriteHalfWord((uint16_t*)appExistMarker, APP_EXIST);
        Protocol_SendAck();
        Nvic_ResetSystem();
        break;
      case PROTOCOL_DATA:
        /* Data Received */
        if(Fpec_WriteBlock((uint16_t*)appAddress, (uint16_t*)data, length/2) == E_NOT_OK)
        {
          Fpec_ErasePage((uint32_t*)appAddress);
          Fpec_WriteBlock((uint16_t*)appAddress, (uint16_t*)data, length/2);
        }
        Protocol_SendAck();
        break;
      }
    }
  }
  while(1);
  return 0;
}
