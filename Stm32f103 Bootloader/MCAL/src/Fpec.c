/**
 * @file Fpec.h
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This is the implementation for the FPEC Driver
 * @version 0.1
 * @date 2020-05-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "Std_Types.h"

#define FPEC_BASE_ADDRESS           0x40022000

#define FPEC_LOCK_SET               0x00000080
#define FPEC_MER_SET                0x00000004
#define FPEC_PER_SET                0x00000002
#define FPEC_PER_CLR                0xFFFFFFFD
#define FPEC_START_SET              0x00000040
#define FPEC_LOCK_GET               0x00000080
#define FPEC_BUSY_GET               0x00000001
#define FPEC_PG_SET                 0x00000001
#define FPEC_PG_CLR                 0xFFFFFFFE
#define FPEC_KEY1                   0x45670123
#define FPEC_KEY2                   0xCDEF89AB

#define FPEC_ERASED_VALUE           0xFFFFFFFF
#define FPEC_PAGE_SIZE_IN_WORDS     256


typedef struct
{
    uint32_t ACR;
    uint32_t KEYR;
    uint32_t OPTKEYR;
    uint32_t SR;
    uint32_t CR;
    uint32_t AR;
    uint32_t DUMMY;
    uint32_t OBR;
    uint32_t WRPR;
}Fpec_t;

#define FPEC            ((volatile Fpec_t*)(FPEC_BASE_ADDRESS))

/**
 * @brief Locks The FPEC
 * 
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
Std_ReturnType Fpec_Lock(void)
{
    FPEC->CR |= FPEC_LOCK_SET;
    return E_OK;
}

/**
 * @brief Unlocks The FPEC
 * 
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
Std_ReturnType Fpec_Unlock(void)
{
    Std_ReturnType error = E_OK;
    FPEC->KEYR = FPEC_KEY1;
    FPEC->KEYR = FPEC_KEY2;
    if(FPEC->CR & FPEC_LOCK_GET)
    {
        error = E_NOT_OK;
    }
    return error;
}

/**
 * @brief Writes A Half Word To The Flash
 * 
 * @param address The Address In Flash
 * @param data The Half Word To Write
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
Std_ReturnType Fpec_WriteHalfWord(uint16_t* address, uint16_t data)
{
    Std_ReturnType error = E_OK;
    if(FPEC->CR & FPEC_LOCK_GET)
    {
        error = E_NOT_OK;
    }
    else
    {
        FPEC->CR |= FPEC_PG_SET;
        *address = data;
        while(FPEC_BUSY_GET & FPEC->SR);
        if(*address != data)
        {
            error = E_NOT_OK;
        }
        FPEC->CR &= FPEC_PG_CLR;
    }
    return error;
}

/**
 * @brief Writes A Block To The Flash
 * 
 * @param flashAddress The Address In Flash
 * @param srcAddress The Source Address To Fetch Data From
 * @param blockSize The Size Of The Block In Half Words
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
Std_ReturnType Fpec_WriteBlock(uint16_t* flashAddress, uint16_t* srcAddress, uint16_t blockSize)
{
    uint16_t itr;
    Std_ReturnType error = E_OK;
    if(FPEC->CR & FPEC_LOCK_GET)
    {
        error = E_NOT_OK;
    }
    else
    {
        FPEC->CR |= FPEC_PG_SET;
        for(itr=0; itr<blockSize; itr++)
        {
            flashAddress[itr] = srcAddress[itr];
            while(FPEC_BUSY_GET & FPEC->SR);
            if(flashAddress[itr] != srcAddress[itr])
            {
                error = E_NOT_OK;
                break;
            }
        }
        FPEC->CR &= FPEC_PG_CLR;
    }
    return error;
}

/**
 * @brief Erases A Page In The Flash
 * 
 * @param pageAddress The Address Of The Page
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
Std_ReturnType Fpec_ErasePage(uint32_t* pageAddress)
{
    uint16_t itr;
    Std_ReturnType error = E_OK;
    if(FPEC->CR & FPEC_LOCK_GET)
    {
        error = E_NOT_OK;
    }
    else
    {
        FPEC->CR |= FPEC_PER_SET;
        FPEC->AR = (uint32_t)pageAddress;
        FPEC->CR |= FPEC_START_SET;
        while(FPEC_BUSY_GET & FPEC->SR);
        for(itr=0; itr<FPEC_PAGE_SIZE_IN_WORDS; itr++)
        {
            if(pageAddress[itr] != FPEC_ERASED_VALUE)
            {
                error = E_NOT_OK;
                break;
            }
        }
        FPEC->CR &= FPEC_PER_CLR;
    }
    return error;
}

/**
 * @brief Erases The Flash Completely !!
 * 
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
extern Std_ReturnType Fpec_MassErase(void)
{
    Std_ReturnType error = E_OK;
    if(FPEC->CR & FPEC_LOCK_GET)
    {
        error = E_NOT_OK;
    }
    else
    {
        FPEC->CR |= FPEC_MER_SET;
        FPEC->CR |= FPEC_START_SET;
    }
    return error;
}