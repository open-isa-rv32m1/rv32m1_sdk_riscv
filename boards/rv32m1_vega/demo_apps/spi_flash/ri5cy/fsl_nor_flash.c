/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static flash_err_t NOR_Flash_WriteEnable(flash_handle_t *handle)
{
    flash_transfer_t xfer;

    xfer.opCode = kNorFlash_WriteEnable;
    xfer.data = NULL;
    xfer.dataSize = 0;

    return handle->executeCommand(handle->resource, &xfer);
}

flash_err_t NOR_Flash_Init(flash_handle_t *handle, flash_config_t *config)
{
    assert(handle);

    handle->totalSize = config->totalSize;
    handle->pageSize = config->pageSize;
    handle->sectorSize = config->sectorSize;
    handle->needWriteEnable = config->needWriteEnable;
    handle->statusValueSize = config->statusValueSize;
    handle->statusBusyMask = config->statusBusyMask;
    handle->statusBusyValue = config->statusBusyValue;

    return flash_ok;
}

flash_err_t NOR_Flash_isBusy(flash_handle_t *handle)
{
    flash_transfer_t xfer;
    uint32_t statusValue;
    flash_err_t result = flash_ok;

    xfer.opCode = kNorFlash_ReadStatus;
    xfer.data = (uint8_t *)&statusValue;
    xfer.dataSize = handle->statusValueSize;

    result = handle->executeCommand(handle->resource, &xfer);
    if (result != flash_ok)
    {
        return result;
    }

    if (statusValue & handle->statusBusyMask)
    {
        if (handle->statusBusyValue)
        {
            return flash_busy;
        }
    }
    else
    {
        if (!(handle->statusBusyValue))
        {
            return flash_busy;
        }
    }

    return flash_ok;
}

flash_err_t NOR_Flash_ChipErase(flash_handle_t *handle)
{
    flash_transfer_t xfer;
    flash_err_t result = flash_ok;

    while (NOR_Flash_isBusy(handle) != flash_ok)
        ;

    if (handle->needWriteEnable)
    {
        result = NOR_Flash_WriteEnable(handle);
        if (result != flash_ok)
        {
            return result;
        }
    }

    xfer.opCode = kNorFlash_EraseChip;
    xfer.data = NULL;
    xfer.dataSize = 0;

    result = handle->executeCommand(handle->resource, &xfer);
    if (result != flash_ok)
    {
        return result;
    }

    while (NOR_Flash_isBusy(handle) != flash_ok)
        ;

    return result;
}

flash_err_t NOR_Flash_EraseBlock(flash_handle_t *handle, uint32_t address, uint32_t blockSize)
{
    flash_transfer_t xfer;
    flash_err_t result = flash_ok;

    assert(!(address % blockSize));

    while (NOR_Flash_isBusy(handle) != flash_ok)
        ;

    if (handle->needWriteEnable)
    {
        result = NOR_Flash_WriteEnable(handle);
        if (result != flash_ok)
        {
            return result;
        }
    }

    xfer.opCode = kNorFlash_EraseSector;
    xfer.address = address;
    xfer.data = NULL;
    xfer.dataSize = blockSize;

    result = handle->executeCommand(handle->resource, &xfer);
    if (result != flash_ok)
    {
        return result;
    }

    while (NOR_Flash_isBusy(handle) != flash_ok)
        ;

    return result;
}

flash_err_t NOR_Flash_ReadData(flash_handle_t *handle, uint32_t address, uint8_t *data, uint32_t dataSize)
{
    flash_transfer_t xfer;

    while (NOR_Flash_isBusy(handle) != flash_ok)
        ;

    xfer.opCode = kNorFlash_Read;
    xfer.address = address;
    xfer.data = data;
    xfer.dataSize = dataSize;

    return handle->executeCommand(handle->resource, &xfer);
}

flash_err_t NOR_Flash_WriteData(flash_handle_t *handle, uint32_t address, uint8_t *data, uint32_t dataSize)
{
    flash_transfer_t xfer;
    flash_err_t result = flash_ok;

    while (NOR_Flash_isBusy(handle) != flash_ok)
        ;

    if (handle->needWriteEnable)
    {
        result = NOR_Flash_WriteEnable(handle);
        if (result != flash_ok)
        {
            return result;
        }
    }

    xfer.opCode = kNorFlash_Write;
    xfer.address = address;
    xfer.data = data;
    xfer.dataSize = dataSize;

    return handle->executeCommand(handle->resource, &xfer);
}
