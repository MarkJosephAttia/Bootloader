/**
 * @file Nvic.h
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This file is a user interface for the NVIC driver for the arm cortex m3
 * @version 0.1
 * @date 2020-02-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef NVIC_H
#define NVIC_H

#define     NVIC_ACTIVE              0
#define     NVIC_NOT_ACTIVE          !NVIC_ACTIVE

/** Interrupt Number **/
#define NVIC_IRQNUM_WWDG            0
#define NVIC_IRQNUM_PVD             1
#define NVIC_IRQNUM_TAMPER          2
#define NVIC_IRQNUM_RTC             3
#define NVIC_IRQNUM_FLASH           4
#define NVIC_IRQNUM_RCC             5
#define NVIC_IRQNUM_EXTI0           6
#define NVIC_IRQNUM_EXTI1           7
#define NVIC_IRQNUM_EXTI2           8
#define NVIC_IRQNUM_EXTI3           9
#define NVIC_IRQNUM_EXTI4           10
#define NVIC_IRQNUM_DMA1_CHANNEL1   11
#define NVIC_IRQNUM_DMA1_CHANNEL2   12
#define NVIC_IRQNUM_DMA1_CHANNEL3   13
#define NVIC_IRQNUM_DMA1_CHANNEL4   14
#define NVIC_IRQNUM_DMA1_CHANNEL5   15
#define NVIC_IRQNUM_DMA1_CHANNEL6   16
#define NVIC_IRQNUM_DMA1_CHANNEL7   17
#define NVIC_IRQNUM_ADC1_2          18
#define NVIC_IRQNUM_USB_HP_CAN_TX   19
#define NVIC_IRQNUM_USB_HP_CAN_RX0  20
#define NVIC_IRQNUM_CAN_RX1         21
#define NVIC_IRQNUM_CAN_SCE         22
#define NVIC_IRQNUM_EXTI9_5         23
#define NVIC_IRQNUM_TIM1_BRK        24
#define NVIC_IRQNUM_TIM1_UP         25
#define NVIC_IRQNUM_TIM1_TRG_COM    26
#define NVIC_IRQNUM_TIM1_CC         27
#define NVIC_IRQNUM_TIM2            28
#define NVIC_IRQNUM_TIM3            29
#define NVIC_IRQNUM_TIM4            30
#define NVIC_IRQNUM_I2C1_EV         31

#define NVIC_IRQNUM_I2C1_ER         32
#define NVIC_IRQNUM_I2C2_EV         33
#define NVIC_IRQNUM_I2C2_ER         34
#define NVIC_IRQNUM_SPI1            35
#define NVIC_IRQNUM_SPI2            36
#define NVIC_IRQNUM_USART1          37
#define NVIC_IRQNUM_USART2          38
#define NVIC_IRQNUM_USART3          39
#define NVIC_IRQNUM_EXTI15_10       40
#define NVIC_IRQNUM_RTC_ALARM       41
#define NVIC_IRQNUM_USB_WAKE_UP     42
#define NVIC_IRQNUM_TIM8_BRK        43
#define NVIC_IRQNUM_TIM8_UP         44
#define NVIC_IRQNUM_TIM8_TRG_COM    45
#define NVIC_IRQNUM_TIM8_CC         46
#define NVIC_IRQNUM_ADC3            47
#define NVIC_IRQNUM_FSMC            48
#define NVIC_IRQNUM_SDIO            49
#define NVIC_IRQNUM_TIM5            50
#define NVIC_IRQNUM_SPI3            51
#define NVIC_IRQNUM_UART4           52
#define NVIC_IRQNUM_UART5           53
#define NVIC_IRQNUM_TIM6            54
#define NVIC_IRQNUM_TIM7            55
#define NVIC_IRQNUM_DMA2_Channel1   56
#define NVIC_IRQNUM_DMA2_Channel2   57
#define NVIC_IRQNUM_DMA2_Channel3   58
#define NVIC_IRQNUM_DMA2_Channel4_5 59

/**
 * @brief Enables a specific Interrupt
 * 
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_EnableInterrupt(uint8_t intNumber);

/**
 * @brief Disables a specific Interrupt
 * 
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_DisableInterrupt(uint8_t intNumber);

/**
 * @brief Sets the pending flag for a specific interrupt
 * 
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_SetPending(uint8_t intNumber);

/**
 * @brief Clears the pending flag for a specific interrupt
 * 
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_ClearPending(uint8_t intNumber);

/**
 * @brief Checks if the interrupt is active
 * 
 * @param activeState the active state of the interrupt
 *                  @arg NVIC_ACTIVE
 *                  @arg NVIC_NOT_ACTIVE
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_IsInterruptActive(uint8_t* activeState, uint8_t intNumber);

/**
 * @brief Sets the subpriority for aspecific interrupt
 * 
 * @param priority the priority you want to set
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_SetSubpriority(uint8_t priority, uint8_t intNumber);

/**
 * @brief Gets the subpriority for aspecific interrupt
 * 
 * @param priority  the priority you want to get
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_GetSubpriority(uint8_t* priority, uint8_t intNumber);

/**
 * @brief Sets the group priority for aspecific interrupt
 * 
 * @param priority the priority you want to set
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_SetGroupPriority(uint8_t priority, uint8_t intNumber);

/**
 * @brief Gets the group priority for aspecific interrupt
 * 
 * @param priority  the priority you want to get
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_GetGroupPriority(uint8_t* priority, uint8_t intNumber);

/**
 * @brief Configure the group size
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_ConfigGroupSize(void);

/**
 * @brief Resets the microcontroller
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_ResetSystem(void);

/**
 * @brief Enables the prepherals interrupts
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_EnablePeripheral(void);

/**
 * @brief Disable the prepherals interrupts
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_DisablePeripheral(void);

/**
 * @brief Blocks all interrupts including hard fault
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_SetFault(void);

/**
 * @brief Returns from fault mode
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_ClearFault(void);

/**
 * @brief Only allow interrupts over a certain priority
 * 
 * @param per the minimum priority allowed
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
extern Std_ReturnType Nvic_FilterPriority(uint8_t pri);

/**
 * @brief Generates a software interrupt (Atomic function to generate interrupt immediately)
 * 
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_GenerateSoftwareInterrupt(uint8_t intNumber);

#endif