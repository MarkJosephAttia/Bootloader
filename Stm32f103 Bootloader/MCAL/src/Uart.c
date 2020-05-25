/**
 * @file Uart.c
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This is the implementation for the UART driver
 * @version 0.1
 * @date 2020-03-26
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "Std_Types.h"
#include "Uart_Cfg.h"
#include "Uart.h"
#if UART_MODE == UART_MODE_DMA
#include "Dma.h"
#endif

#define UART_NUMBER_OF_MODULES        3

/**
 * @brief The UART Registers
 * 
 */
typedef struct
{
  uint32_t SR;    /* The UART Status Register */
  uint32_t DR;    /* The Data Status Register */
  uint32_t BRR;   /* The UART BaudRate Register */
  uint32_t CR1;   /* The UART Configuration Register 1 */
  uint32_t CR2;   /* The UART Configuration Register 2 */
  uint32_t CR3;   /* The UART Configuration Register 3 */
  uint32_t GTPR;  /* The UART Guard Time Register */
} uart_t;

/**
 * @brief The Uart Data Buffer
 * 
 */
typedef struct
{
  uint8_t *ptr;   /* A pointer to the bytes of data */
  uint32_t pos;   /* The current position */
  uint32_t size;  /* The size of the data in the buffer */
  uint8_t state;  /* The state of the buffer (UART_BUFFER_IDLE/UART_BUFFER_BUSY) */
} dataBuffer_t;

#define UART_INT_NUMBER 37

#define UART_BUFFER_IDLE 0
#define UART_BUFFER_BUSY 1

#define UART_TXE_CLR 0xFFFFFF7F
#define UART_TC_CLR 0xFFFFFFBF
#define UART_RXNE_CLR 0xFFFFFFDF
#define UART_PE_CLR 0xFFFFFFFE
#define UART_DR_CLR 0xFFFFFE00
#define UART_STOP_CLR 0xFFFFCFFF
#define UART_TXEIE_CLR 0xFFFFFF7F
#define UART_PS_CLR 0xFFFFFDFF
#define UART_M_CLR 0xFFFFEFFF
#define UART_LBD_CLR 0xFFFFFEFF
#define UART_LBDIE_CLR 0xFFFFFFBF
#define UART_TXE_GET 0x00000080
#define UART_TC_GET 0x00000040
#define UART_RXNE_GET 0x00000020
#define UART_PE_GET 0x00000001
#define UART_UE_SET 0x00002000
#define UART_PCE_SET 0x00000400
#define UART_PEIE_SET 0x00000100
#define UART_TXEIE_SET 0x00000080
#define UART_TCIE_SET 0x00000040
#define UART_RXNEIE_SET 0x00000020
#define UART_IDLEIE_SET 0x00000010
#define UART_TE_SET 0x00000008
#define UART_RE_SET 0x00000004
#define UART_M_SET 0x00001000
#define UART_LBD_SET 0x00000100
#define UART_LBDIE_SET 0x00000040
#define UART_DMAT_SET 0x00000080
#define UART_DMAR_SET 0x00000040
#define UART_SBK_SET 0x00000001
#define UART_LINEN_CLR 0xFFFFBFFF
#define UART_RTSE_CLR 0xFFFFFEFF

#define UART_NO_PRESCALER 0x1

#define DMA_DID_NOT_RECEIVE             0
#define DMA_RECEIVED                    1

/**
 * @brief The Base Adresses of the UART module
 * 
 */
const uint32_t Uart_Address[UART_NUMBER_OF_MODULES] = {
  0x40013800,
  0x40004400,
  0x40004800
};


static volatile dataBuffer_t txBuffer[UART_NUMBER_OF_MODULES];
static volatile dataBuffer_t rxBuffer[UART_NUMBER_OF_MODULES];

static volatile txCb_t appTxNotify[UART_NUMBER_OF_MODULES];
static volatile rxCb_t appRxNotify[UART_NUMBER_OF_MODULES];
static volatile brCb_t appBreakNotify[UART_NUMBER_OF_MODULES];

static volatile uint8_t Uart_interrupt[UART_NUMBER_OF_MODULES];

#if UART_MODE == UART_MODE_DMA

static volatile uint8_t  Uart_dmaRec[UART_NUMBER_OF_MODULES];
const volatile uint8_t Uart_DmaTxChannelNumber[UART_NUMBER_OF_MODULES] =
{
  DMA_CH_4,
  DMA_CH_7,
  DMA_CH_2
};
const volatile uint8_t Uart_DmaRxChannelNumber[UART_NUMBER_OF_MODULES] =
{
  DMA_CH_5,
  DMA_CH_6,
  DMA_CH_3
};
#endif

#if UART_MODE == UART_MODE_DMA
static void USART1_DMA_IRQHandler(void);
static void USART2_DMA_IRQHandler(void);
static void USART3_DMA_IRQHandler(void);
#endif

/**
 * @brief The Interrupt Handler for the UART driver
 * 
 * @param uartModule the module number of the UART
 *                 @arg UART1
 *                 @arg UART2
 *                 @arg UART3
 */
static void UART_IRQHandler(uint8_t uartModule)
{
  volatile uart_t* Uart = (volatile uart_t*)Uart_Address[uartModule];
  /* If a Lin break is generated */
  if((UART_LBD_SET & Uart->SR) && (Uart_interrupt[uartModule] & UART_INTERRUPT_LBD))
  {
    /* Clear The Lin Break Flag */
    Uart->SR &= UART_LBD_CLR;
    if (appBreakNotify[uartModule])
    {
      appBreakNotify[uartModule](uartModule);
    }
  }
#if UART_MODE == UART_MODE_ASYNC
  /* If the TX is Empty */
  if ( (UART_TXE_GET & Uart->SR) && (Uart_interrupt[uartModule] & UART_INTERRUPT_TXE) )
  {
    /* If there is still data in the buffer */
    if (txBuffer[uartModule].size != txBuffer[uartModule].pos)
    {
      Uart->DR = txBuffer[uartModule].ptr[txBuffer[uartModule].pos++];
    }
    else
    {
      txBuffer[uartModule].ptr = NULL;
      txBuffer[uartModule].size = 0;
      txBuffer[uartModule].pos = 0;
      txBuffer[uartModule].state = UART_BUFFER_IDLE;
      Uart->CR1 &= UART_TXEIE_CLR;
      if (appTxNotify[uartModule])
      {
        appTxNotify[uartModule](uartModule);
      }
    }
  }

  /* If the TX is Complete */
  if ( (UART_TC_GET & Uart->SR) && (Uart_interrupt[uartModule] & UART_INTERRUPT_TC) )
  {
    /* Clear The Flag */
    Uart->SR &= UART_TC_CLR;
    /* If there is still data in the buffer */
    if (txBuffer[uartModule].size != txBuffer[uartModule].pos)
    {
      Uart->DR = txBuffer[uartModule].ptr[txBuffer[uartModule].pos++];
    }
    else
    {
      txBuffer[uartModule].ptr = NULL;
      txBuffer[uartModule].size = 0;
      txBuffer[uartModule].pos = 0;
      txBuffer[uartModule].state = UART_BUFFER_IDLE;
      if (appTxNotify[uartModule])
      {
        appTxNotify[uartModule](uartModule);
      }
    }
  }

  if ( (UART_RXNE_GET & Uart->SR) && (Uart_interrupt[uartModule] & UART_INTERRUPT_RXNE))
  {
    Uart->SR &= UART_RXNE_CLR;
    /* If there is still data to receive */
    if (UART_BUFFER_BUSY == rxBuffer[uartModule].state)
    {
      rxBuffer[uartModule].ptr[rxBuffer[uartModule].pos] = Uart->DR;
      rxBuffer[uartModule].pos++;
      /* If the data is received successfully */
      if (rxBuffer[uartModule].pos == rxBuffer[uartModule].size)
      {
        rxBuffer[uartModule].ptr = NULL;
        rxBuffer[uartModule].size = 0;
        rxBuffer[uartModule].pos = 0;
        rxBuffer[uartModule].state = UART_BUFFER_IDLE;
        if (appRxNotify[uartModule])
        {
          appRxNotify[uartModule](uartModule);
        }
      }
    }
  }
#endif
#if UART_MODE == UART_MODE_DMA
  /* If the Transmittion is Complete */
  if ( (UART_TC_GET & Uart->SR) && (Uart_interrupt[uartModule] & UART_INTERRUPT_TC) )
  {
    /* Clear The Flag */
    Uart->SR &= UART_TC_CLR;
    txBuffer[uartModule].state = UART_BUFFER_IDLE;
    if (appTxNotify[uartModule])
    {
      appTxNotify[uartModule](uartModule);
    }
  }
  if(Uart_dmaRec[uartModule] == DMA_RECEIVED)
  {
    Uart_dmaRec[UART2] = DMA_DID_NOT_RECEIVE;
    rxBuffer[uartModule].state = UART_BUFFER_IDLE;
    if (appRxNotify[uartModule])
    {
      appRxNotify[uartModule](uartModule);
    }
  }
#endif
}


/**
 * @brief Initializes the UART
 *
 * @param cfg The Uart Configurations
 * @return Std_ReturnType A Status
 *                  E_OK: If the function executed successfully
 *                  E_NOT_OK: If the did not execute successfully
 */
Std_ReturnType Uart_Init(Uart_cfg_t* cfgUart)
{
  volatile uart_t* Uart = (volatile uart_t*)Uart_Address[cfgUart->uartModule];
  f64 tmpBaudRate = ((f64)cfgUart->sysClk / ((f64)cfgUart->baudRate));
#if UART_MODE == UART_MODE_DMA
  /* Tx Configurations */
  dmaPrephCfg_t cfg = 
  {
    .channel = Uart_DmaTxChannelNumber[cfgUart->uartModule],
    .interrupt = DMA_INT_NO_INT,
    .direction = DMA_READ_FROM_MEM,
    .circular = DMA_CIRCULAR_MODE_OFF,
    .prephInc = DMA_PREPH_INC_OFF,
    .memInc = DMA_MEM_INC_ON,
    .prephSize = DMA_PREPH_8_BIT,
    .memSize = DMA_MEM_8_BIT,
    .priority = DMA_PRIORITY_HIGH
  };
#endif
  Uart_interrupt[cfgUart->uartModule] = cfgUart->interrupts;
  /* Set Baudrate */
  Uart->BRR = ((uint16_t)(tmpBaudRate));
  /* Setting the parity bit */
  if (UART_NO_PARITY == cfgUart->parity)
  {
    Uart->CR1 &= UART_M_CLR;
    Uart->CR1 &= UART_NO_PARITY;
  }
  else
  {
    Uart->CR1 &= UART_PS_CLR;
    Uart->CR1 |= cfgUart->parity | UART_M_SET | UART_PCE_SET;
  }
  /* Setting the stop bit and Lin Break Interrupt */
  Uart->CR2 &= UART_STOP_CLR & UART_LBDIE_CLR & UART_LINEN_CLR;
  Uart->CR2 |= cfgUart->stopBits | cfgUart->linEn;
  if(UART_INTERRUPT_LBD & cfgUart->interrupts)
  {
    Uart->CR2 |= UART_LBDIE_SET;
  }
  /* Set the hardware flowcontrol */
  Uart->CR3 &= UART_RTSE_CLR;
#if UART_MODE == UART_MODE_DMA
  /* DMA Configurations */
  Dma_ConfigurePrephChannel(&cfg);
  /* Rx Configure */
  cfg.channel = Uart_DmaRxChannelNumber[cfgUart->uartModule];
  cfg.interrupt = DMA_INT_TRANSFER_COMPLETE;
  cfg.direction = DMA_READ_FROM_PREPH;
  Dma_ConfigurePrephChannel(&cfg);
  switch(cfgUart->uartModule)
  {
    case UART1:
      Dma_SetCallBack(Uart_DmaRxChannelNumber[cfgUart->uartModule], USART1_DMA_IRQHandler);
      break;
    case UART2:
      Dma_SetCallBack(Uart_DmaRxChannelNumber[cfgUart->uartModule], USART2_DMA_IRQHandler);
      break;
    case UART3:
      Dma_SetCallBack(Uart_DmaRxChannelNumber[cfgUart->uartModule], USART3_DMA_IRQHandler);
      break;
  }
  Uart->CR3 |= UART_DMAT_SET | UART_DMAR_SET;
#endif
  Uart->CR3 |= cfgUart->flowControl;
  Uart->GTPR |= UART_NO_PRESCALER;
  /* Set the buffer states to idle */
  rxBuffer[cfgUart->uartModule].state = UART_BUFFER_IDLE;
  txBuffer[cfgUart->uartModule].state = UART_BUFFER_IDLE;
  Uart->SR &= UART_TC_CLR;
#if UART_MODE == UART_MODE_DMA
  /* Enable the UART, Receiver, Transmitter and the Receiver not empty interrupt */
  Uart->CR1 |= UART_UE_SET | UART_TE_SET | UART_RE_SET | (UART_TCIE_SET & cfgUart->interrupts) | (UART_LBDIE_SET & cfgUart->interrupts);
#else
  /* Enable the UART, Receiver, Transmitter and the Receiver not empty interrupt */
  Uart->CR1 |= UART_UE_SET | UART_TE_SET | UART_RE_SET | (UART_LBDIE_SET & cfgUart->interrupts);;
#endif
  return E_OK;
}

/**
 * @brief Sends data through the UART
 *
 * @param data The data to send
 * @param length the length of the data in bytes
 * @param uartModule the module number of the UART
 *                 @arg UART1
 *                 @arg UART2
 *                 @arg UART3
 * @return Std_ReturnType A Status
 *                  E_OK: If the driver is ready to send
 *                  E_NOT_OK: If the driver can't send data right now
 */
Std_ReturnType Uart_Send(uint8_t *data, uint16_t length, uint8_t uartModule)
{
  Std_ReturnType error = E_NOT_OK;
  volatile uart_t* Uart = (volatile uart_t*)Uart_Address[uartModule];
  /*If there is valid data and length and the TX buffer is idle*/
  if (data && (length > 0) && txBuffer[uartModule].state == UART_BUFFER_IDLE)
  {

#if UART_MODE == UART_MODE_ASYNC
    txBuffer[uartModule].state = UART_BUFFER_BUSY;
    txBuffer[uartModule].ptr = data;
    txBuffer[uartModule].pos = 0;
    txBuffer[uartModule].size = length;

    Uart->DR = txBuffer[uartModule].ptr[txBuffer[uartModule].pos++];
    if(Uart_interrupt[uartModule] & UART_INTERRUPT_TXE)
    {
        Uart->CR1 |= UART_TXEIE_SET;
    }
    if(Uart_interrupt[uartModule] & UART_INTERRUPT_TC)
    {   
      Uart->SR &= UART_TC_CLR;
      Uart->CR1 |= UART_TCIE_SET;;
    }
#endif

#if UART_MODE == UART_MODE_DMA
    txBuffer[uartModule].state = UART_BUFFER_BUSY;
    Dma_TransferPrephData(Uart_DmaTxChannelNumber[uartModule],(uint32_t)(&(Uart->DR)), (uint32_t)data, length);
#endif

    error = E_OK;
  }
  return error;
}
/**
 * @brief Receives data through the UART
 *
 * @param data The buffer to receive data in
 * @param length the length of the data in bytes
 * @param uartModule the module number of the UART
 *                 @arg UART1
 *                 @arg UART2
 *                 @arg UART3
 * @return Std_ReturnType A Status
 *                  E_OK: If the driver is ready to receive
 *                  E_NOT_OK: If the driver can't receive data right now
 */
Std_ReturnType Uart_Receive(uint8_t *data, uint16_t length, uint8_t uartModule)
{
  Std_ReturnType error = E_NOT_OK;
  volatile uart_t* Uart = (volatile uart_t*)Uart_Address[uartModule];
  /* If the RX buffer is idle */
  if (rxBuffer[uartModule].state == UART_BUFFER_IDLE)
  {

#if UART_MODE == UART_MODE_ASYNC
    rxBuffer[uartModule].ptr = data;
    rxBuffer[uartModule].size = length;
    rxBuffer[uartModule].pos = 0;
    rxBuffer[uartModule].state = UART_BUFFER_BUSY;
    if(Uart_interrupt[uartModule] & UART_INTERRUPT_RXNE)
    {   
      Uart->CR1 |= UART_RXNEIE_SET;
    }
#endif

#if UART_MODE == UART_MODE_DMA
    rxBuffer[uartModule].state = UART_BUFFER_BUSY;
    Dma_TransferPrephData(Uart_DmaRxChannelNumber[uartModule],(uint32_t)(&(Uart->DR)), (uint32_t)data, length);
#endif

    error = E_OK;
  }
  return error;
}
/**
 * @brief Sends data through the UART synchronously
 *
 * @param data The data to send
 * @param length the length of the data in bytes
 * @param uartModule the module number of the UART
 *                 @arg UART1
 *                 @arg UART2
 *                 @arg UART3
 * @return Std_ReturnType A Status
 *                  E_OK: If the driver is ready to send
 *                  E_NOT_OK: If the driver can't send data right now
 */
Std_ReturnType Uart_SendSync(uint8_t *data, uint16_t length, uint8_t uartModule)
{
  Std_ReturnType error = E_NOT_OK;
  uint16_t itr;
  volatile uart_t* Uart = (volatile uart_t*)Uart_Address[uartModule];
  /*If there is valid data and length and the TX buffer is idle*/
  if (data && (length > 0))
  {
    for(itr=0; itr<length; itr++)
    {
      Uart->DR = data[itr];
      while((UART_TXE_GET & Uart->SR) == 0);
    }
    while((UART_TC_GET & Uart->SR) == 0);
    if (appTxNotify[uartModule])
    {
      appTxNotify[uartModule](uartModule);
    }
    error = E_OK;
  }
  return error;
}
/**
 * @brief Receives data through the UART synchronously
 *
 * @param data The buffer to receive data in
 * @param length the length of the data in bytes
 * @param uartModule the module number of the UART
 *                 @arg UART1
 *                 @arg UART2
 *                 @arg UART3
 * @return Std_ReturnType A Status
 *                  E_OK: If the driver is ready to receive
 *                  E_NOT_OK: If the driver can't receive data right now
 */
Std_ReturnType Uart_ReceiveSync(uint8_t *data, uint16_t length, uint8_t uartModule)
{
  uint16_t itr;
  volatile uart_t* Uart = (volatile uart_t*)Uart_Address[uartModule];
  Uart->SR &= UART_RXNE_CLR;
  for(itr=0; itr<length; itr++)
  {
    while((UART_RXNE_GET & Uart->SR) == 0);
    data[itr] = Uart->DR;
  }
  if (appRxNotify[uartModule])
  {
    appRxNotify[uartModule](uartModule);
  }
  return E_OK;
}


/**
 * @brief Sets the callback function that will be called when transmission is
 * completed
 *
 * @param func the callback function
 * @param uartModule the module number of the UART
 *                 @arg UART1
 *                 @arg UART2
 *                 @arg UART3
 * @return Std_ReturnType A Status
 *                  E_OK: If the function executed successfully
 *                  E_NOT_OK: If the did not execute successfully
 */
Std_ReturnType Uart_SetTxCb(txCb_t func, uint8_t uartModule)
{
  appTxNotify[uartModule] = func;
  return E_OK;
}

/**
 * @brief Sets the callback function that will be called when receive is
 * completed
 *
 * @param func the callback function
 * @param uartModule the module number of the UART
 *                 @arg UART1
 *                 @arg UART2
 *                 @arg UART3
 * @return Std_ReturnType A Status
 *                  E_OK: If the function executed successfully
 *                  E_NOT_OK: If the did not execute successfully
 */
Std_ReturnType Uart_SetRxCb(rxCb_t func, uint8_t uartModule)
{
  appRxNotify[uartModule] = func;
  return E_OK;
}

/**
 * @brief Sets the callback function that will be called when break happens
 *
 * @param func the callback function
 * @param uartModule the module number of the UART
 *                 @arg UART1
 *                 @arg UART2
 *                 @arg UART3
 * @return Std_ReturnType A Status
 *                  E_OK: If the function executed successfully
 *                  E_NOT_OK: If the did not execute successfully
 */
Std_ReturnType Uart_SetBreakCb(brCb_t func, uint8_t uartModule)
{
  appBreakNotify[uartModule] = func;
  return E_OK;
}

/**
 * @brief Sends a Lin break of 13 bit length
 * 
 * @param uartModule the module number of the UART
 *                 @arg UART1
 *                 @arg UART2
 *                 @arg UART3
 * @return Std_ReturnType A Status
 *                  E_OK: If the function executed successfully
 *                  E_NOT_OK: If the did not execute successfully
 */
Std_ReturnType Uart_SendBreak(uint8_t uartModule)
{
  volatile uart_t* Uart = (volatile uart_t*)Uart_Address[uartModule];
  Uart->CR1 |= UART_SBK_SET;
  return E_OK;
}

/**
 * @brief The UART 1 Handler
 * 
 */
void USART1_IRQHandler(void)
{
  UART_IRQHandler(UART1);
}
/**
 * @brief The UART 2 Handler
 * 
 */
void USART2_IRQHandler(void)
{
  UART_IRQHandler(UART2);
}
/**
 * @brief The UART 3 Handler
 *
 */
void USART3_IRQHandler(void)
{
  UART_IRQHandler(UART3);
}

#if UART_MODE == UART_MODE_DMA

/**
 * @brief The UART 1 DMA Handler
 * 
 */
static void USART1_DMA_IRQHandler(void)
{
  Uart_dmaRec[UART1] = DMA_RECEIVED;
  UART_IRQHandler(UART1);
}
/**
 * @brief The UART 2 DMA Handler
 * 
 */
static void USART2_DMA_IRQHandler(void)
{
  Uart_dmaRec[UART2] = DMA_RECEIVED;
  UART_IRQHandler(UART2);
}
/**
 * @brief The UART 3 DMA Handler
 *
 */
static void USART3_DMA_IRQHandler(void)
{
  Uart_dmaRec[UART3] = DMA_RECEIVED;
  UART_IRQHandler(UART3);
}
#endif
