/**
 * @file Fpec.h
 * @author Mark Attia (markjosephattia@gmail.com)
 * @brief This is the user interface for the FPEC Driver
 * @version 0.1
 * @date 2020-05-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef FPEC_H_
#define FPEC_H_

/**
 * @brief Locks The FPEC
 * 
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
extern Std_ReturnType Fpec_Lock(void);

/**
 * @brief Unlocks The FPEC
 * 
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
extern Std_ReturnType Fpec_Unlock(void);

/**
 * @brief Writes A Half Word To The Flash
 * 
 * @param address The Address In Flash
 * @param data The Half Word To Write
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
extern Std_ReturnType Fpec_WriteHalfWord(uint16_t* address, uint16_t data);

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
extern Std_ReturnType Fpec_WriteBlock(uint16_t* flashAddress, uint16_t* srcAddress, uint16_t blockSize);

/**
 * @brief Erases A Page In The Flash
 * 
 * @param pageAddress The Address Of The Page
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
extern Std_ReturnType Fpec_ErasePage(uint32_t* pageAddress);

/**
 * @brief Erases The Flash Completely !!
 * 
 * @return Std_ReturnType A Status
 *                      E_OK : If The Function Executed Successfully
 *                      E_NOT_OK : If The Function Didn't Execute Successfully
 */
extern Std_ReturnType Fpec_MassErase(void);

#endif