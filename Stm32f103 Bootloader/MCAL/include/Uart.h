/**
 * @file Uart.h
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This is the user interface for the UART driver
 * @version 0.1
 * @date 2020-03-26
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef UART_H
#define UART_H

#define UART1       0
#define UART2       1
#define UART3       2

#define UART_ODD_PARITY 0x00000200
#define UART_EVEN_PARITY 0x00000000
#define UART_NO_PARITY 0xFFFFFBFF
#define UART_STOP_ONE_BIT 0x00000000
#define UART_STOP_TWO_BITS 0x00003000

#define UART_FLOW_CONTROL_EN 0x00000100
#define UART_FLOW_CONTROL_DIS 0x00000000

#define UART_LIN_EN             0x00004000
#define UART_LIN_DIS            0x00000000

#define UART_INTERRUPT_DIS              0
#define UART_INTERRUPT_TXE              1
#define UART_INTERRUPT_TC               2
#define UART_INTERRUPT_RXNE             4
#define UART_INTERRUPT_LBD              8

#define UART_MODE_ASYNC                 0
#define UART_MODE_DMA                   1

typedef void (*txCb_t)(uint8_t);
typedef void (*rxCb_t)(uint8_t);
typedef void (*brCb_t)(uint8_t);

typedef struct
{
    uint32_t baudRate;          /* The Baudrate To Use */
    uint32_t stopBits;          /* UART_STOP_x_BIT */
    uint32_t parity;            /* UART_x_PARITY */
    uint32_t flowControl;       /* UART_FLOW_CONTROL_x */
    uint32_t sysClk;            /* The System Clock */
    uint32_t linEn;             /* UART_LIN_x */
    uint8_t  interrupts;        /* UART_INTERRUPT_x */
    uint8_t  uartModule;        /* UARTx */
}Uart_cfg_t;


/**
 * @brief Initializes the UART
 *
 * @param cfgUart The Uart Configurations
 * @return Std_ReturnType A Status
 *                  E_OK: If the function executed successfully
 *                  E_NOT_OK: If the did not execute successfully
 */
extern Std_ReturnType Uart_Init(Uart_cfg_t* cfgUart);

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
extern Std_ReturnType Uart_Send(uint8_t *data, uint16_t length, uint8_t uartModule);
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
extern Std_ReturnType Uart_SendBreak(uint8_t uartModule);
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
extern Std_ReturnType Uart_Receive(uint8_t *data, uint16_t length, uint8_t uartModule);
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
extern Std_ReturnType Uart_SendSync(uint8_t *data, uint16_t length, uint8_t uartModule);
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
extern Std_ReturnType Uart_ReceiveSync(uint8_t *data, uint16_t length, uint8_t uartModule);
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
extern Std_ReturnType Uart_SetTxCb(txCb_t func, uint8_t uartModule);
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
extern Std_ReturnType Uart_SetRxCb(rxCb_t func, uint8_t uartModule);

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
extern Std_ReturnType Uart_SetBreakCb(brCb_t func, uint8_t uartModule);

#endif
