/**
 * @file HRcc.h
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This is implementation for the RCC Handler
 * @version 0.1
 * @date 2020-03-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Std_Types.h"
#include "Rcc.h"
#include "Gpio.h"

/**
 * @brief This function initializes the system clock
 * 
 * @return Std_ReturnType 
 *                 E_OK : if the function is executed correctly
 *                 E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType HRcc_SystemClockInit(void)
{
    Rcc_SwitchSystemClock(RCC_HSE_SYS);
    return E_OK;
}

/**
 * @brief This function initializes the clock for a specific GPIO port
 * 
 * @param port The GPIO port
 *                  @arg GPIO_PORTX : The pin number you want to configure
 * @return Std_ReturnType  
 *                 E_OK : if the function is executed correctly
 *                 E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType HRcc_EnPortClock(uint32_t port)
{
    Std_ReturnType error = E_OK;
    switch(port)
    {
      case GPIO_PORTA:
        Rcc_SetApb2PeriphClockState(RCC_IOPA_CLK_EN, RCC_PERIPH_CLK_ON);
        break;
      case GPIO_PORTB:
        Rcc_SetApb2PeriphClockState(RCC_IOPB_CLK_EN, RCC_PERIPH_CLK_ON);
        break;
      case GPIO_PORTC:
        Rcc_SetApb2PeriphClockState(RCC_IOPC_CLK_EN, RCC_PERIPH_CLK_ON);
        break;
      case GPIO_PORTD:
        Rcc_SetApb2PeriphClockState(RCC_IOPD_CLK_EN, RCC_PERIPH_CLK_ON);
        break;
      case GPIO_PORTE:
        Rcc_SetApb2PeriphClockState(RCC_IOPE_CLK_EN, RCC_PERIPH_CLK_ON);
        break;
      case GPIO_PORTF:
        Rcc_SetApb2PeriphClockState(RCC_IOPF_CLK_EN, RCC_PERIPH_CLK_ON);
        break;
      case GPIO_PORTG:
        Rcc_SetApb2PeriphClockState(RCC_IOPG_CLK_EN, RCC_PERIPH_CLK_ON);
        break;
      default:
        error = E_NOT_OK;
        break;
    }
    return error;
}
