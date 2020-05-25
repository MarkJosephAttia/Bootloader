/**
 * @file  Switch.h 
 * @brief This file is to be used as an interface for the user of the Switch Handler.
 *
 * @author Mark Attia
 * @date January 22, 2020
 *
 */
#ifndef SWITCH_H
#define SWITCH_H
#include "Switch_Cfg.h"

/**
 * @brief The Switch pin layout
 * 
 */
typedef struct
{
    uint32_t pin;           /* The pin of the switch */
    uint32_t port;          /* The port of the switch */
    uint8_t activeState;    /* The active state of the switch (GPIO_PIN_SET/GPIO_PIN_RESET) */
} switch_t;

#define SWITCH_PRESSED                  0
#define SWITCH_NOT_PRESSED              !SWITCH_PRESSED

/**
 * Function:  Switch_Init 
 * --------------------
 *  @brief Initializes GPIOs for the Switches
 * 
 *  @returns: A status
 *                 E_OK : if the function is executed correctly
 *                 E_NOT_OK : if the function is not executed correctly
 */
extern Std_ReturnType Switch_Init(void);

/**
 * Function:  Switch_GetSwitchStatus 
 * --------------------
 *  @brief Gets the status of the switch
 * 
 *  @param switchName: The name of the Switch
 *                   
 *  @param state: Save the status of the switch in
 *                 @arg SWITCH_PRESSED : if the switch is pressed
 *                 @arg SWITCH_NOT_PRESSED : if the switch is not pressed
 *  @returns: A status
 *                 E_OK : if the function is executed correctly
 *                 E_NOT_OK : if the function is not executed correctly
 */
extern Std_ReturnType Switch_GetSwitchStatus(uint8_t switchName, uint8_t* state);

#endif
