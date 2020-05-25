/**
 * @file Protocol.h
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This is the user interface for the self designed software protocol
 * @version 0.1
 * @date 2020-05-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define PROTOCOL_DATA 1
#define PROTOCOL_ADDRESS 3
#define PROTOCOL_EOT 5

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
extern Std_ReturnType Protocol_Send(uint16_t msgType, uint16_t length, uint8_t* data);

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
extern Std_ReturnType Protocol_Receive(uint16_t* msgType, uint16_t* length, uint8_t* data);

/**
 * @brief Sends Acknowledgement
 *
 * @return Std_ReturnType A Status
 *                E_OK     : If the function was executed successfully
 *                E_NOT_OK : If the function didn't execute successfully
 */
extern Std_ReturnType Protocol_SendAck(void);

#endif
