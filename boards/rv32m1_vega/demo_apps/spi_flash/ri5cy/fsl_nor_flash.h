/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_NOR_Flash_H_
#define _FSL_NOR_Flash_H_

#include "fsl_common.h"
#include "fsl_spi_adapter.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the Flash peripheral.
 *
 * @param handle Flash operation handle.
 * @param config Pointer to the user-defined configuration structure.
 * @return Returns @ref flash_ok if initialize success, otherwise returns
 * error code.
 */
flash_err_t NOR_Flash_Init(flash_handle_t *handle, flash_config_t *config);

/*!
 * @brief Reads the specified number of bytes from NOR_Flash memory.
 *
 * @param handle Flash operation handle.
 * @param address NOR_Flash address to start reading from.
 * @param data Pointer to a memory location where the data read out from
 * NOR_Flash will be stored.
 * @param dataSize Number of bytes to be read.
 * @return Returns @ref flash_ok if read success, otherwise returns
 * error code.
 */
flash_err_t NOR_Flash_ReadData(flash_handle_t *handle, uint32_t address, uint8_t *data, uint32_t dataSize);

/*!
 * @brief Writes the specified number of bytes to NOR_Flash memory.
 *
 * @param handle Flash operation handle.
 * @param address NOR_Flash address to start writing to.
 * @param data Pointer to a memory location where the data data to be
 * written to NOR_Flash is stored.
 * @param dataSize Number of bytes to be written.
 * @return Returns @ref flash_ok if write success, otherwise returns
 * error code.
 */
flash_err_t NOR_Flash_WriteData(flash_handle_t *handle, uint32_t address, uint8_t *data, uint32_t dataSize);

/*!
 * @brief Get the NOR_Flash busy status.
 *
 * @param handle Flash operation handle.
 * @return Returns @ref flash_busy if flash busy, otherwise returns
 * flash_ok. If error happens, return error code.
 */
flash_err_t NOR_Flash_isBusy(flash_handle_t *handle);

/*!
 * @brief Erase a block of block size memory
 *
 * @param handle Flash operation handle.
 * @param address NOR_Flash address to start erase.
 * @param blockSize Erase block size.
 * @return Returns @ref flash_ok if erase success, otherwise return error code.
 */
flash_err_t NOR_Flash_EraseBlock(flash_handle_t *handle, uint32_t address, uint32_t blockSize);

/*!
 * @brief Erase the entire NOR_Flash memory.
 *
 * @param handle Flash operation handle.
 * @return Returns @ref flash_ok if erase success, otherwise return error code.
 */
flash_err_t NOR_Flash_ChipErase(flash_handle_t *handle);

#if defined(__cplusplus)
}
#endif

#endif
