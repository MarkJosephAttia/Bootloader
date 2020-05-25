/**
 * @file Nvic.c
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This file is an implementation for the NVIC driver for the arm cortex m3
 * @version 0.1
 * @date 2020-02-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Std_Types.h"
#include "Nvic.h"
#include "Nvic_Cfg.h"

#define NVIC_BASE_ADDRESS              0xE000E100 

#define AIRC                           *((volatile uint32_t*)0xE000ED0C)               /* Application interrupt and reset control register */
#define AIRC_LOCK                      0x05FA0000                                      /* Application interrupt and reset control register Lock */
#define AIRC_LOCK_CLR                  0x0000FFFF                                      /* Application interrupt and reset control register Lock Clear Mask */

#define AIRC_SYS_RST                   0x00000004
#define AIRC_GROUP_CLR                 0x0000F8FF
#define NVIC_GROUP_CHECK               0xFFFFFFF8

#define NVIC_NON_IMPLEMENTED_PRI           (8 - NVIC_GROUP_SIZE - NVIC_SUBGROUP_SIZE)


#define NVIC_0_BIT_MASK                0b0
#define NVIC_1_BIT_MASK                0b1
#define NVIC_2_BIT_MASK                0b11
#define NVIC_3_BIT_MASK                0b111
#define NVIC_4_BIT_MASK                0b1111
#define NVIC_5_BIT_MASK                0b11111
#define NVIC_6_BIT_MASK                0b111111
#define NVIC_7_BIT_MASK                0b1111111

#define NVIC_CONCAT_MASK(x)            NVIC_CONCAT_MASK_HELP(x)
#define NVIC_CONCAT_MASK_HELP(x)       NVIC_##x##_BIT_MASK

/**
 * @brief The NVIC Registers
 * 
 */
typedef struct
{
  uint32_t SETEN[8];               /* Interrupt Set Enable Register */
  uint32_t _RESERVED0[24];
  uint32_t CLREN[8];               /* Interrupt Clear Enable Register */
  uint32_t _RSERVED1[24];
  uint32_t SETPND[8];               /* Interrupt Set Pending Register */
  uint32_t _RESERVED2[24];
  uint32_t CLRPND[8];               /* Interrupt Clear Pending Register */
  uint32_t _RESERVED3[24];
  uint32_t AB[8];               /* Interrupt Active bit Register */
  uint32_t _RESERVED4[56];
  uint8_t  PRI[240];               /* Interrupt Priority Register */
  uint32_t _RESERVED5[644];
  uint32_t STIR;                   /* Software Trigger Interrupt Register */
}  nvic_t;

#define NVIC            ((volatile nvic_t*)(NVIC_BASE_ADDRESS))

/**
 * @brief Enables a specific Interrupt
 * 
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_EnableInterrupt(uint8_t intNumber)
{
    NVIC->SETEN[intNumber>>5] = (uint32_t)1<<((uint32_t)intNumber&31);
    return E_OK;
}

/**
 * @brief Disables a specific Interrupt
 * 
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_DisableInterrupt(uint8_t intNumber)
{
    NVIC->CLREN[intNumber>>5] = (uint32_t)1<<((uint32_t)intNumber&31);
    return E_OK;
}

/**
 * @brief Sets the pending flag for a specific interrupt
 * 
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_SetPending(uint8_t intNumber)
{
    NVIC->SETPND[intNumber>>5] = (uint32_t)1<<((uint32_t)intNumber&31);
    return E_OK;
}

/**
 * @brief Clears the pending flag for a specific interrupt
 * 
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_ClearPending(uint8_t intNumber)
{
    NVIC->CLRPND[intNumber>>5] = (uint32_t)1<<((uint32_t)intNumber&31);
    return E_OK;
}

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
Std_ReturnType Nvic_IsInterruptActive(uint8_t* activeState, uint8_t intNumber)
{
    *activeState = !(NVIC->AB[intNumber>>5] & (uint32_t)1<<((uint32_t)intNumber&31));
    return E_OK;
}

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
Std_ReturnType Nvic_SetSubpriority(uint8_t priority, uint8_t intNumber)
{
    NVIC->PRI[intNumber] &= ~(NVIC_CONCAT_MASK(NVIC_SUBGROUP_SIZE) << NVIC_NON_IMPLEMENTED_PRI);
    NVIC->PRI[intNumber] |= priority<<NVIC_NON_IMPLEMENTED_PRI;
    return E_OK;
}

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
Std_ReturnType Nvic_GetSubpriority(uint8_t* priority, uint8_t intNumber)
{
    *priority = (NVIC->PRI[intNumber]>>NVIC_NON_IMPLEMENTED_PRI) & NVIC_CONCAT_MASK(NVIC_SUBGROUP_SIZE);
    return E_OK;
}

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
Std_ReturnType Nvic_SetGroupPriority(uint8_t priority, uint8_t intNumber)
{
    NVIC->PRI[intNumber] &= ~(NVIC_CONCAT_MASK(NVIC_GROUP_SIZE)<<(NVIC_NON_IMPLEMENTED_PRI+NVIC_SUBGROUP_SIZE));
    NVIC->PRI[intNumber] |= priority<<(NVIC_NON_IMPLEMENTED_PRI+NVIC_SUBGROUP_SIZE);
    return E_OK;
}

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
Std_ReturnType Nvic_GetGroupPriority(uint8_t* priority, uint8_t intNumber)
{
    *priority = ((uint8_t)NVIC->PRI[intNumber])>>(NVIC_NON_IMPLEMENTED_PRI+NVIC_SUBGROUP_SIZE);
    return E_OK;
}

/**
 * @brief Configure the group size
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_ConfigGroupSize(void)
{
    Std_ReturnType err = E_NOT_OK;
    uint32_t tmp = AIRC & AIRC_LOCK_CLR;
    if(!((NVIC_SUBGROUP_SIZE + NVIC_NON_IMPLEMENTED_PRI - 1) | NVIC_GROUP_CHECK))
    {
        tmp &= AIRC_GROUP_CLR;
        tmp |= ((NVIC_SUBGROUP_SIZE + NVIC_NON_IMPLEMENTED_PRI - 1)<<8) | AIRC_LOCK;
        AIRC = tmp;
        err = E_OK;
    }
    return err;
}

/**
 * @brief Resets the microcontroller
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_ResetSystem(void)
{
    uint32_t tmp = AIRC & AIRC_LOCK_CLR;
    tmp |= AIRC_LOCK | AIRC_SYS_RST;
    AIRC = tmp;
    return E_OK;
}

/**
 * @brief Enables the prepherals interrupts
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_EnablePeripheral(void)
{
    asm("CPSIE I");
    return E_OK;
}

/**
 * @brief Disable the prepherals interrupts
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_DisablePeripheral(void)
{
    asm("CPSID I");
    return E_OK;
}

/**
 * @brief Blocks all interrupts including hard fault
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_SetFault(void)
{
    asm("CPSIE F");
    return E_OK;
}

/**
 * @brief Returns from fault mode
 * 
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_ClearFault(void)
{
    asm("CPSID F");
    return E_OK;
}

/**
 * @brief Only allow interrupts over a certain priority
 * 
 * @param per the minimum priority allowed
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_FilterPriority(uint8_t pri)
{
    asm("MSR basepri, %0" : : "r" ((uint32_t)pri<<4) : "memory");
    return E_OK;
}

/**
 * @brief Generates a software interrupt (Atomic function to generate interrupt immediately)
 * 
 * @param intNumber the number of the interrupt in the vector table
 *                  @arg NVIC_IRQNUM_X
 * @return Std_ReturnType 
 *                    E_OK: If the function executed successfully
 *                    E_NOT_OK: If the function failed to execute 
 */
Std_ReturnType Nvic_GenerateSoftwareInterrupt(uint8_t intNumber)
{
    NVIC->STIR = intNumber;
    return E_OK;
}
