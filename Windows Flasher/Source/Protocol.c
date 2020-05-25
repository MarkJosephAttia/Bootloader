/**
 * @file Protocol.c
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This is the implementation for a self designed software protocol
 * @version 0.1
 * @date 2020-05-15
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <windows.h>
#include <inttypes.h>
#include "Std_Types.h"
#include "Uart.h"
#include "Protocol.h"

#define PROTOCOL_HEADER_SIZE 8

#define PROTOCOL_ACK 0x55
#define PROTOCOL_ACK_SIZE 1

#define PROTOCOL_DATA_KEY 2
#define PROTOCOL_ADDRESS_KEY 4
#define PROTOCOL_EOT_KEY 6

typedef struct
{
  uint32_t key;
  uint16_t type;
  uint16_t length;
}header_t;

typedef struct
{
  union
  {
    header_t header;
    uint8_t headerData[PROTOCOL_HEADER_SIZE];
  };
} packet_t;


/**
 * @brief Sends a packet
 *
 * @param msgType The type of the message
 *              @arg PROTOCOL_x
 * @param length the length of the data in the message
 * @param data the data to send
 * @return Std_ReturnType A Status
 *                E_OK     : If the function was executed successfully
 *                E_NOT_OK : If the function didn't execute successfully
 */
Std_ReturnType Protocol_Send(uint16_t msgType, uint16_t length, uint8_t* data)
{
  Std_ReturnType error = E_OK;
  uint8_t ack;
  packet_t packet;
  packet.header.type = msgType;
  packet.header.length = length;
  switch(packet.header.type)
  {
    case PROTOCOL_DATA:
      packet.header.key = PROTOCOL_DATA_KEY;
      break;
    case PROTOCOL_ADDRESS:
      packet.header.key = PROTOCOL_ADDRESS_KEY;
      break;
    case PROTOCOL_EOT:
      packet.header.key = PROTOCOL_EOT_KEY;
      break;
  }
  Uart_Send(packet.headerData, PROTOCOL_HEADER_SIZE);
  Uart_Receive(&ack, PROTOCOL_ACK_SIZE);
  if(ack == PROTOCOL_ACK)
  {
    Uart_Send(data, packet.header.length);
    Uart_Receive(&ack, PROTOCOL_ACK_SIZE);
    if(ack != PROTOCOL_ACK)
    {
      error = E_NOT_OK;
    }
  }
  else
  {
    error = E_NOT_OK;
  }
  return error;
}

/**
 * @brief Sends a packet
 *
 * @param msgType The type of the message
 *              @arg PROTOCOL_x
 * @param length the length of the data in the message
 * @param data the data to send
 * @return Std_ReturnType A Status
 *                E_OK     : If the function was executed successfully
 *                E_NOT_OK : If the function didn't execute successfully
 */
Std_ReturnType Protocol_Receive(uint16_t* msgType, uint16_t* length, uint8_t* data)
{
  Std_ReturnType error = E_NOT_OK;
  uint8_t ack = PROTOCOL_ACK;
  packet_t packet;
  Uart_Receive(packet.headerData, PROTOCOL_HEADER_SIZE);
  switch(packet.header.type)
  {
    case PROTOCOL_DATA:
      if(packet.header.key == PROTOCOL_DATA_KEY)
      {
        error = E_OK;
      }
      break;
    case PROTOCOL_ADDRESS:
      if(packet.header.key == PROTOCOL_ADDRESS_KEY)
      {
        error = E_OK;
      }
      break;
    case PROTOCOL_EOT:
      if(packet.header.key == PROTOCOL_EOT_KEY)
      {
        error = E_OK;
      }
      break;
  }
  if(error == E_OK)
  {
    Uart_Send(&ack, PROTOCOL_ACK_SIZE);
    Uart_Receive(data, packet.header.length);
    *msgType = packet.header.type;
    *length = packet.header.length;
  }
  return error;
}

/**
 * @brief Sends Acknowledgement
 *
 * @return Std_ReturnType A Status
 *                E_OK     : If the function was executed successfully
 *                E_NOT_OK : If the function didn't execute successfully
 */
Std_ReturnType Protocol_SendAck(void)
{
  uint8_t ack = PROTOCOL_ACK;
  Uart_Send(&ack, PROTOCOL_ACK_SIZE);
  return E_OK;
}
