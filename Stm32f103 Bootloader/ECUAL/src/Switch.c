/**
 * @file  Switch.c
 * @brief This file is to be used as an implementation for the Switch Handler.
 *
 * @author Mark Attia
 * @date January 22, 2020
 *
 */
#include "Std_Types.h"
#include "Gpio.h"
#include "HRcc.h"
#include "Switch.h"
#ifdef SWITCH_USE_RTOS
#include "Sched.h"
#endif

extern const switch_t Switch_switches[SWITCH_NUMBER_OF_SWITCHES];
static volatile uint8_t Switch_state[SWITCH_NUMBER_OF_SWITCHES]; /* The state of the switches */
#ifdef SWITCH_USE_RTOS
static void Switch_Task(void);
#endif

/**
 * Function:  Switch_Init 
 * --------------------
 *  @brief Initializes GPIOs for the Switches
 * 
 *  @returns: A status
 *                 E_OK : if the function is executed correctly
 *                 E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Switch_Init(void)
{
    uint8_t i;
    gpio_t gpio;
    /* Initialize the GPIO pins and their clock */
    gpio.speed = GPIO_SPEED_50_MHZ;
    for(i=0; i<SWITCH_NUMBER_OF_SWITCHES; i++)
    {
        switch(Switch_switches[i].activeState)
        {
            case GPIO_PIN_RESET:
                gpio.mode = GPIO_MODE_INPUT_PULL_UP;
                break;
            case GPIO_PIN_SET:
                gpio.mode = GPIO_MODE_INPUT_PULL_DOWN;
                break;
        }
        gpio.pins = Switch_switches[i].pin;
        gpio.port = Switch_switches[i].port;
        HRcc_EnPortClock(Switch_switches[i].port);
        Gpio_InitPins(&gpio);
        Switch_state[i] = SWITCH_NOT_PRESSED;
    }
    return E_OK;
}


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
Std_ReturnType Switch_GetSwitchStatus(uint8_t switchName, uint8_t* state)
{
    #ifndef SWITCH_USE_RTOS
        uint8_t readVal;
        Gpio_ReadPin(Switch_switches[switchName].port, Switch_switches[switchName].pin, &readVal);
        *state = (Switch_switches[switchName].activeState ^ readVal);
        return E_OK;
    #else
        *state = Switch_state[switchName];
        return E_OK;
    #endif
}

#ifdef SWITCH_USE_RTOS
/**
 * @brief The running task of the switch driver to get the state of all of the switches
 * 
 */
static void Switch_Task(void)
{
    uint8_t i,readVal;
    static uint8_t prevState[SWITCH_NUMBER_OF_SWITCHES];
    static uint8_t counter[SWITCH_NUMBER_OF_SWITCHES];
    uint8_t currentState;
    /* Loop on the list of switches */
    for(i=0; i<SWITCH_NUMBER_OF_SWITCHES; i++)
    {
        Gpio_ReadPin(Switch_switches[i].port, Switch_switches[i].pin, &readVal);
        currentState = (Switch_switches[i].activeState ^ readVal);
        if(currentState == prevState[i])
        {
            counter[i]++;
        }
        else
        {
            counter[i] = 0;
        }
        /* Check for the stable state */
        if(counter[i] == 5)
        {
            Switch_state[i] = currentState;
            counter[i] = 0;
        }
        prevState[i] = currentState;
    }

}

const task_t Switch_task = {Switch_Task, 5};
#endif