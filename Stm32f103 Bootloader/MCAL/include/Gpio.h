/**
 * @file  Gpio.h 
 * @brief This file is to be used as an interface for the user of GPIO driver.
 *
 * @author Mark Attia
 * @date February 6, 2020
 *
 */

#ifndef GPIO_H
#define GPIO_H

/**
 * @brief The GPIO Type contains GPIO configurations
 * 
 */
typedef struct 
{
    uint32_t pins;      /* The GPIO pins GPIO_PIN_x */
    uint32_t speed;     /* The GPIO Speed GPIO_SPEED_x */
    uint32_t mode;      /* The GPIO mode GPIO_MODE_x */
    uint32_t port;      /* The GPIO port GPIO_PORTx */
} gpio_t;

#define GPIO_PIN_SET                    0
#define GPIO_PIN_RESET                  !GPIO_PIN_SET


#define GPIO_PIN_0                      0x0001
#define GPIO_PIN_1                      0x0002
#define GPIO_PIN_2                      0x0004
#define GPIO_PIN_3                      0x0008
#define GPIO_PIN_4                      0x0010
#define GPIO_PIN_5                      0x0020
#define GPIO_PIN_6                      0x0040
#define GPIO_PIN_7                      0x0080
#define GPIO_PIN_8                      0x0100
#define GPIO_PIN_9                      0x0200
#define GPIO_PIN_10                     0x0400
#define GPIO_PIN_11                     0x0800
#define GPIO_PIN_12                     0x1000
#define GPIO_PIN_13                     0x2000
#define GPIO_PIN_14                     0x4000
#define GPIO_PIN_15                     0x8000
#define GPIO_PIN_ALL                    0xFFFF

#define GPIO_SPEED_10_MHZ               0x01
#define GPIO_SPEED_02_MHZ               0x02
#define GPIO_SPEED_50_MHZ               0x03

#define GPIO_MODE_GP_OUTPUT_PP          0x00
#define GPIO_MODE_GP_OUTPUT_OD          0x04
#define GPIO_MODE_AF_OUTPUT_PP          0x08
#define GPIO_MODE_AF_OUTPUT_OD          0x0C

#define GPIO_MODE_INPUT_ANALOG          0x10
#define GPIO_MODE_INPUT_FLOATING        0x14
#define GPIO_MODE_INPUT_PULL_DOWN       0x18
#define GPIO_MODE_INPUT_PULL_UP         0x28

#define GPIO_PORTA                      (uint32_t)0x40010800
#define GPIO_PORTB                      (uint32_t)0x40010C00
#define GPIO_PORTC                      (uint32_t)0x40011000
#define GPIO_PORTD                      (uint32_t)0x40011400
#define GPIO_PORTE                      (uint32_t)0x40011800
#define GPIO_PORTF                      (uint32_t)0x40011C00
#define GPIO_PORTG                      (uint32_t)0x40012000


/**
 * Function:  Gpio_InitPins 
 * --------------------
 *
 *  @brief Initializes pins mode and speed for a specific port
 *
 *  @param gpio: An object of type gpio_t to set pins for
 *  
 *  
 *  @returns: A status
 *                 E_OK : if the function is executed correctly
 *                 E_NOT_OK : if the function is not executed correctly
 */
extern Std_ReturnType Gpio_InitPins(gpio_t* gpio);

/**
 * Function:  Gpio_WritePin 
 * --------------------
 *  @brief Write a value to a pin(0/1)
 *
 *  @param port: The port you want to configure
 *                 @arg GPIO_PORTX : The pin number you want to configure  
 * 
 *  @param pin: The pin you want to configure
 *                 @arg GPIO_PIN_X : The pin number you want to configure
 *                 //You can OR more than one pin\\
 *
 *  @param pinStatus: The status of the pins (GPIO_PIN_SET/GPIO_PIN_RESET)
 *                 @arg GPIO_PIN_SET : Sets the pin value to 1
 *                 @arg GPIO_PIN_RESET : Resets the pin value to 0
 *  
 *  @returns: A status
 *                 E_OK : if the function is executed correctly
 *                 E_NOT_OK : if the function is not executed correctly
 */
extern Std_ReturnType Gpio_WritePin(uint32_t port, uint32_t pin, uint32_t pinStatus);

/**
 * Function:  Gpio_ReadPin 
 * --------------------
 *  @brief Reads a value to a pin(0/1)
 *
 *  @param port: The port you want to read from
 *                 @arg GPIO_PORTX : The pin number you want to read from  
 * 
 *  @param pin: The pin you want to read
 *                 @arg GPIO_PIN_X : The pin number you want to read
 *                 //You can OR more than one pin\\
 *
 *  @param state: To return a status in
 *                  @arg GPIO_PIN_SET : The pin is set to 1
 *                  @arg GPIO_PIN_RESET : The pin is set to 0
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
extern Std_ReturnType Gpio_ReadPin(uint32_t port, uint32_t pin, uint8_t* state);

#endif