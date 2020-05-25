/**
 * @file  Rcc.c 
 * @brief This file is to be used as an implementation of the RCC driver.
 *
 * @author Mark Attia
 * @date January 22, 2020
 *
 */
#include "Std_Types.h"
#include "RCC.h"

#define     RCC_BASE_ADDRESS            0x40021000
#define     RCC_CR                      *((volatile u32*)(RCC_BASE_ADDRESS + 0x00))
#define     RCC_CFGR                    *((volatile u32*)(RCC_BASE_ADDRESS + 0x04))
#define     RCC_CIR                     *((volatile u32*)(RCC_BASE_ADDRESS + 0x08))
#define     RCC_APB2RSTR                *((volatile u32*)(RCC_BASE_ADDRESS + 0x0C))
#define     RCC_APB1RSTR                *((volatile u32*)(RCC_BASE_ADDRESS + 0x10))
#define     RCC_AHBENR                  *((volatile u32*)(RCC_BASE_ADDRESS + 0x14))
#define     RCC_APB2ENR                 *((volatile u32*)(RCC_BASE_ADDRESS + 0x18))
#define     RCC_APB1ENR                 *((volatile u32*)(RCC_BASE_ADDRESS + 0x1C))
#define     RCC_BDCR                    *((volatile u32*)(RCC_BASE_ADDRESS + 0x20))
#define     RCC_CRS                     *((volatile u32*)(RCC_BASE_ADDRESS + 0x24))
#define     RCC_AHBRSTR                 *((volatile u32*)(RCC_BASE_ADDRESS + 0x28))
#define     RCC_CFGR2                   *((volatile u32*)(RCC_BASE_ADDRESS + 0x2C))

#define     RCC_MCO_CLR                 0xF8FFFFFF
#define     RCC_SYS_CLK_SELECT_CLR      0xFFFFFFFC
#define     RCC_PLL_MUL_CLR             0xFFC3FFFF
#define     RCC_PLL_SRC_CLR             0xFFFEFFFF
#define     RCC_SYS_CLK_STATUS          0x0000000C

/**
 * Function:  Rcc_SetClockState 
 * --------------------
 *  @brief Choose a specific clock and changes its state (On / Off) 
 *
 *  @param clock: The clock you want to configure
 *                  @arg RCC_HSI_SET: for the high speed internal clock
 *                  @arg RCC_HSE_SET: for the high speed external clock
 *                  @arg RCC_PLL_SET: for the PLL clock
 *  
 * @param state : The state of the clock (On / Off)
 *                 @arg  RCC_CLK_ON : To set the clock on
 *                 @arg  RCC_CLK_OFF : To set the clock off
 *
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_SetClockState(uint32_t clock, uint8_t state)
{
    Std_ReturnType errorRet = E_NOT_OK;
    if(state == RCC_CLK_ON)
    {
        RCC_CR |= clock;
        errorRet = E_OK;
    }
    if(state == RCC_CLK_OFF)
    {
        RCC_CR &= ~clock;
        errorRet = E_OK;
    }
    return errorRet;
}

/**
 * Function:  Rcc_IsReady 
 * --------------------
 *  @brief Checks if a specific clock is ready or not 
 *
 *  @param clock: The clock you want to check for
 *                  @arg RCC_HSI_RDY: for the high speed internal clock
 *                  @arg RCC_HSE_RDY: for the high speed external clock
 *                  @arg RCC_PLL_RDY: for the PLL clock
 *
 *  @param ready: Saves the ready state in
 *                 @arg RCC_IS_RDY : if the clock is ready
 *                 @arg RCC_NOT_RDY : if the clock is not ready
 * 
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_IsClockReady(uint32_t clock, uint8_t* ready)
{
    *ready = !(RCC_CR & clock);
    return E_OK;
}

/**
 * Function:  Rcc_SelectMcoClock 
 * --------------------
 *  @brief Selects the clock on the mco pin 
 *
 *  @param clock: The clock you want to configure
 *                  @arg RCC_MCO_NO_CLK  : No clock will be on MCO 
 *                  @arg RCC_MCO_SYS_CLK : Select system clock on the MCO
 *                  @arg RCC_MCO_HSI_CLK : Select high speed internal clock on the MCO
 *                  @arg RCC_MCO_HSE_CLK : Select high speed external clock on the MCO
 *                  @arg RCC_MCO_PLL_CLK : Select PLL clock on the MCO
 *
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_SelectMcoClock(uint32_t clock)
{
    RCC_CFGR &= RCC_MCO_CLR;
    RCC_CFGR |= clock;
    return E_OK;
}

/**
 * Function:  Rcc_SetPrescaler 
 * --------------------
 *  @brief Sets the prescaler value for a specific clock 
 *
 *  @param clock: The clock you want to configure
 *                  @arg RCC_USB_PRE     : For the USB prescaler
 *                  @arg RCC_PLL_HSE_PRE : For the Pll prescaler
 *                  @arg RCC_ADC_PRE     : For the ADC prescaler
 *                  @arg RCC_APB2_PRE    : For the APB2 prescaler
 *                  @arg RCC_APB1_PRE    : For the APB1 prescaler
 *                  @arg RCC_AHB_PRE     : For the AHB  prescaler
 *
 *  @param value : The state of the clock (On / Off)
 *                  @arg RCC_USB_PRE_1_5   : No USB prescaler value
 *                  @arg RCC_USB_PRE_1_5   : USB prescaler 1.5
 *                  @arg RCC_PLL_HSE_PRE_X : PLL Prescaler value using high speed external clock 
 *                  @arg RCC_ADC_PRE_X     : ADC Prescaler value
 *                  @arg RCC_APB2_PRE_XX   : APB2 prescater value
 *                  @arg RCC_APB1_PRE_XX   : APB1 prescater value
 *                  @arg RCC_AHB_PRE_XXX   : AHB prescater value
 *  
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_SetPrescaler(uint32_t clock, uint32_t value)
{
    RCC_CFGR &= clock;
    RCC_CFGR |= value;
    return E_OK;
}

/**
 * Function:  Rcc_SetPllMultiplier 
 * --------------------
 *  @brief Sets the PLL Multiplication factor
 * 
 *  @param pll : The PLL multiplication factor
 *                  @arg RCC_PLL_MUL_XX   : Set PLL multiplication factor
 *
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_SetPllMultiplier(uint32_t pll)
{
    RCC_CFGR &= RCC_PLL_MUL_CLR;
    RCC_CFGR |= pll;
    return E_OK;
}

/**
 * Function:  Rcc_SetPllSource 
 * --------------------
 *  @brief Chooses the PLL clock source
 * 
 *  @param source : The PLL clock source
 *                  @arg RCC_PLL_SRC_HSI : Choose high speed internal clock / 2 as a PLL clock source
 *                  @arg RCC_PLL_SRC_HSE : Choose high speed external clock as a PLL clock source
 *
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_SetPllSource(uint32_t source)
{
    RCC_CFGR &= RCC_PLL_SRC_CLR;
    RCC_CFGR |= source;
    return E_OK;
}

/**
 * Function:  Rcc_GetSystemClockStatus 
 * --------------------
 *  @brief Which clock is working as system clock
 * 
 *  @param sysClk: Saves the clock that is working as system clock in
 *                @arg RCC_HSI_SYS : High speed internal clock is used as system clock
 *                @arg RCC_HSE_SYS : High speed external clock is used as system clock
 *                @arg RCC_PLL_SYS : PLL clock is used as system clock
 *
 *  @param returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 * 
 */
Std_ReturnType Rcc_GetSystemClockStatus(uint8_t* sysClk)
{
    *sysClk = (RCC_SYS_CLK_STATUS & RCC_CFGR);
    return E_OK;
}

/**
 * Function:  Rcc_SwitchSystemClock
 * --------------------
 *  @brief Switches the system clock (HSI / HSE / PLL)
 * 
 *  @param clock : The PLL clock source
 *                  @arg RCC_SYS_CLK_SELECT_HSI : Select high speed internal clock as system clock
 *                  @arg RCC_SYS_CLK_SELECT_HSE : Select high speed external clock as system clock
 *                  @arg RCC_SYS_CLK_SELECT_PLL : Select PLL clock as system clock
 * 
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_SwitchSystemClock(uint32_t clock)
{
    uint32_t tmp = RCC_CFGR;
    tmp &= RCC_SYS_CLK_SELECT_CLR;
    tmp |= clock;
    RCC_CFGR = tmp;
    return E_OK;
}

/**
 * Function:  Rcc_SetApb2PeriphClockState
 * --------------------
 *  @brief Choose a specific peripheral on the APB2 bus and changes its state (On / Off) 
 *
 *  @param periph: The peripheral clock you want to configure
 *                  @arg RCC_AFIO_CLK_EN: Alternate function input output clock enable
 *                  @arg RCC_IOPA_CLK_EN: Port A input output clock enable
 *                  @arg RCC_IOPB_CLK_EN: Port B input output clock enable
 *                  @arg RCC_IOPC_CLK_EN: Port C input output clock enable
 *                  @arg RCC_IOPD_CLK_EN: Port D input output clock enable
 *                  @arg RCC_IOPE_CLK_EN: Port E input output clock enable
 *                  @arg RCC_IOPF_CLK_EN: Port F input output clock enable                 
 *                  @arg RCC_IOPG_CLK_EN: Port G input output clock enable
 *                  @arg RCC_ADC1_CLK_EN: ADC 1 clock enable
 *                  @arg RCC_ADC2_CLK_EN: ADC 2 clock enable
 *                  @arg RCC_TIM1_CLK_EN: Timer 1 clock enable
 *                  @arg RCC_SPI1_CLK_EN: SPI 1 clock enable
 *                  @arg RCC_TIM8_CLK_EN: Timer 8 clock enable
 *                  @arg RCC_USART1_CLK_EN: USART 1 clock enable
 *                  @arg RCC_ADC3_CLK_EN: ADC 3 clock enable
 *                  @arg RCC_TIM9_CLK_EN: Timer 9 clock enable
 *                  @arg RCC_TIM10_CLK_EN: Timer 10 clock enable
 *                  @arg RCC_TIM11_CLK_EN: Timer 11 clock enable
 *                  
 *  
 *  @param state: The state of the clock (On / Off)
 *                  @arg RCC_PERIPH_CLK_ON : To set the clock on
 *                  @arg RCC_PERIPH_CLK_OFF : To set the clock off
 *
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_SetApb2PeriphClockState(uint32_t periph, uint8_t state)
{
    uint8_t retError = E_NOT_OK;
    if(state == RCC_PERIPH_CLK_ON)
    {
        RCC_APB2ENR |= periph;
        retError = E_OK;
    }
    if(state == RCC_PERIPH_CLK_OFF)
    {
        RCC_APB2ENR &= ~periph;
        retError = E_OK;
    }
    return retError;
}

/**
 * Function:  Rcc_ResetApb2Periph
 * --------------------
 *  @brief Resets a specific peripheral on the APB2 bus 
 *
 *  @param periph: The peripheral you want to reset
 *                  @arg RCC_AFIO_RST: Alternate function input output reset
 *                  @arg RCC_IOPA_RST: Port A input output reset
 *                  @arg RCC_IOPB_RST: Port B input output reset
 *                  @arg RCC_IOPC_RST: Port C input output reset
 *                  @arg RCC_IOPD_RST: Port D input output reset
 *                  @arg RCC_IOPE_RST: Port E input output reset
 *                  @arg RCC_IOPF_RST: Port F input output reset                 
 *                  @arg RCC_IOPG_RST: Port G input output reset
 *                  @arg RCC_ADC1_RST: ADC 1 reset
 *                  @arg RCC_ADC2_RST: ADC 2 reset
 *                  @arg RCC_TIM1_RST: Timer 1 reset
 *                  @arg RCC_SPI1_RST: SPI 1 reset
 *                  @arg RCC_TIM8_RST: Timer 8 reset
 *                  @arg RCC_USART1_RST: USART 1 reset
 *                  @arg RCC_ADC3_RST: ADC 3 reset
 *                  @arg RCC_TIM9_RST: Timer 9 reset
 *                  @arg RCC_TIM10_RSTN: Timer 10 reset
 *                  @arg RCC_TIM11_RSTN: Timer 11 reset
 *                  
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_ResetApb2Periph(uint32_t periph)
{
    RCC_APB2RSTR |= periph;
    return E_OK;
}

/**
 * Function:  Rcc_SetApb1PeriphClockState
 * --------------------
 *  @brief Choose a specific peripheral on the APB1 bus and changes its state (On / Off) 
 *
 *  @param periph: The peripheral clock you want to configure
 *                  @arg RCC_TIM2_CLK_EN: Timer 2 clock enable
 *                  @arg RCC_TIM3_CLK_EN: Timer 3 clock enable
 *                  @arg RCC_TIM4_CLK_EN: Timer 4 clock enable
 *                  @arg RCC_TIM5_CLK_EN: Timer 5 clock enable
 *                  @arg RCC_TIM6_CLK_EN: Timer 6 clock enable
 *                  @arg RCC_TIM7_CLK_EN: Timer 7 clock enable
 *                  @arg RCC_TIM12_CLK_EN: Timer 12 clock enable
 *                  @arg RCC_TIM13_CLK_EN: Timer 13 clock enable
 *                  @arg RCC_TIM14_CLK_EN: Timer 14 clock enable
 *                  @arg RCC_WWD_GEN_CLK_EN: Window watchdog clock enable
 *                  @arg RCC_SPI2_CLK_EN: SPI 2 clock enable
 *                  @arg RCC_SPI3_CLK_EN: SPI 3 clock enable
 *                  @arg RCC_USART2_CLK_EN: USART 2 clock enable
 *                  @arg RCC_USART3_CLK_EN: USART 3 clock enable
 *                  @arg RCC_USART4_CLK_EN: USART 4 clock enable
 *                  @arg RCC_USART5_CLK_EN: USART 5 clock enable
 *                  @arg RCC_I2C1_CLK_EN: I2C 1 clock enable
 *                  @arg RCC_I2C2_CLK_EN: I2C 2 clock enable
 *                  @arg RCC_USB_CLK_EN: USB clock enable
 *                  @arg RCC_CAN_CLK_EN: CAN clock enable
 *                  @arg RCC_BKP_CLK_EN: Backup interface clock enable
 *                  @arg RCC_PWR_CLK_EN: Power interface clock enable
 *                  @arg RCC_DAC_CLK_EN: DAC interface clock enable
 *  
 * @param state: The state of the clock (On / Off)
 *                  @arg RCC_PERIPH_CLK_ON : To set the clock on
 *                  @arg RCC_PERIPH_CLK_OFF : To set the clock off
 *
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_SetApb1PeriphClockState(uint32_t periph, uint8_t state)
{
    uint8_t retError = E_NOT_OK;
    if(state == RCC_PERIPH_CLK_ON)
    {
        RCC_APB1ENR |= periph;
        retError = E_OK;
    }
    if(state == RCC_PERIPH_CLK_OFF)
    {
        RCC_APB1ENR &= ~periph;
        retError = E_OK;
    }
    return retError;
}

/**
 * Function:  Rcc_ResetApb1Periph
 * --------------------
 *  @brief Resets a specific peripheral on the APB1 bus 
 *
 *  @param periph: The peripheral you want to reset
 *                  @arg RCC_TIM2_RST: Timer 2 reset
 *                  @arg RCC_TIM3_RST: Timer 3 reset
 *                  @arg RCC_TIM4_RST: Timer 4 reset
 *                  @arg RCC_TIM5_RST: Timer 5 reset
 *                  @arg RCC_TIM6_RST: Timer 6 reset
 *                  @arg RCC_TIM7_RST: Timer 7 reset
 *                  @arg RCC_TIM12_RST: Timer 12 reset
 *                  @arg RCC_TIM13_RST: Timer 13 reset
 *                  @arg RCC_TIM14_RST: Timer 14 reset
 *                  @arg RCC_WWD_GEN_RST: Window watchdog reset
 *                  @arg RCC_SPI2_RST: SPI 2 reset
 *                  @arg RCC_SPI3_RST: SPI 3 reset
 *                  @arg RCC_USART2_RST: USART 2 reset
 *                  @arg RCC_USART3_RST: USART 3 reset
 *                  @arg RCC_USART4_RST: USART 4 reset
 *                  @arg RCC_USART5_RST: USART 5 reset
 *                  @arg RCC_I2C1_RST: I2C 1 reset
 *                  @arg RCC_I2C2_RST: I2C 2 reset
 *                  @arg RCC_USB_RST: USB reset
 *                  @arg RCC_CAN_RST: CAN reset
 *                  @arg RCC_BKP_RST: Backup interface reset
 *                  @arg RCC_PWR_RST: Power interface reset
 *                  @arg RCC_DAC_RST: DAC interface reset
 *  
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_ResetApb1Periph(uint32_t periph)
{
    RCC_APB1RSTR |= periph;
    return E_OK;
}

/**
 * Function:  Rcc_SetAhbPeriphClockState
 * --------------------
 *  @brief Choose a specific peripheral on the AHB bus and changes its state (On / Off) 
 *
 *  @param periph: The peripheral clock you want to configure
 *                  @arg RCC_DMA1_CLK_EN: DMA 1 clock enable
 *                  @arg RCC_DMA2_CLK_EN: DMA 2 clock enable                
 *                  @arg RCC_SRAM_CLK_EN: SRAM interface clock enable
 *                  @arg RCC_FLITF_CLK_EN: FLITF clock enable
 *                  @arg RCC_CRC_CLK_EN: CRC clock enable
 *                  @arg RCC_OTGFS_CLK_EN: OTGFS clock enable
 *                  @arg RCC_ETHMAC_CLK_EN: Ethernet MAC clock enable
 *                  @arg RCC_ETHMACTX_CLK_EN: Ethernet MAC TX clock enable
 *                  @arg RCC_ETHMACRX_CLK_EN: Ethernet MAC RX clock enable
 * 
 * @param state : The state of the clock (On / Off)
 *                  @arg RCC_PERIPH_CLK_ON: To set the clock on
 *                  @arg RCC_PERIPH_CLK_OFF : To set the clock off
 *
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_SetAhbPeriphClockState(uint32_t periph, uint8_t state)
{
    uint8_t retError = E_NOT_OK;
    if(state == RCC_PERIPH_CLK_ON)
    {
        RCC_AHBENR |= periph;
        retError = E_OK;
    }
    if(state == RCC_PERIPH_CLK_OFF)
    {
        RCC_AHBENR &= ~periph;
        retError = E_OK;
    }
    return retError;
}

/**
 * Function:  Rcc_ResetAhbPeriph
 * --------------------
 *  @brief Resets a specific peripheral on the AHB bus 
 *
 *  @param periph: The peripheral you want to reset
 *                 @arg RCC_OTGFS_RST: OTGFS reset                  
 *                 @arg RCC_ETHMAC_RST: Ethernet MAC reset
 * 
 *  @returns: A status
 *              E_OK : if the function is executed correctly
 *              E_NOT_OK : if the function is not executed correctly
 */
Std_ReturnType Rcc_ResetAhbPeriph(uint32_t periph)
{
    RCC_AHBRSTR |= periph;
    return E_OK;
}
