/**
 * @file HRcc.h
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This is the user interface for the RCC Handler
 * @version 0.1
 * @date 2020-03-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef HRCC_H
#define HRCC_H

/**
 * @brief This function initializes the system clock
 * 
 * @return Std_ReturnType 
 *                 E_OK : if the function is executed correctly
 *                 E_NOT_OK : if the function is not executed correctly
 */
extern Std_ReturnType HRcc_SystemClockInit(void);

/**
 * @brief This function initializes the clock for a specific GPIO port
 * 
 * @param port The GPIO port
 *                  @arg GPIO_PORTX : The pin number you want to configure
 * @return Std_ReturnType  
 *                 E_OK : if the function is executed correctly
 *                 E_NOT_OK : if the function is not executed correctly
 */
extern Std_ReturnType HRcc_EnPortClock(uint32_t port);

#endif