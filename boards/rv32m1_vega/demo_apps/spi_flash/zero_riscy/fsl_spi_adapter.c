/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spi_adapter.h"

static flash_err_t SPI_Flash_Transfer(
    spi_resource_t *resource, uint8_t *txData, uint32_t txDataSize, uint8_t *rxData, uint32_t rxDataSize)
{
    spi_type_t spi = resource->spi;

    status_t result = kStatus_Success;
#if defined(FSL_FEATURE_SOC_SPI_COUNT) && FSL_FEATURE_SOC_SPI_COUNT
    spi_transfer_t xfer;
    xfer.txData = txData;
    xfer.rxData = NULL;
    xfer.dataSize = txDataSize;
    result = SPI_MasterTransferBlocking(spi, &xfer);
    if (result != kStatus_Success)
    {
        return flash_error;
    }

    if (rxData != NULL)
    {
        xfer.txData = NULL;
        xfer.rxData = rxData;
        xfer.dataSize = rxDataSize;
        result = SPI_MasterTransferBlocking(spi, &xfer);
        if (result != kStatus_Success)
        {
            return flash_error;
        }
    }
#elif defined(FSL_FEATURE_SOC_DSPI_COUNT) && FSL_FEATURE_SOC_DSPI_COUNT
    dspi_transfer_t xfer;
    xfer.txData = txData;
    xfer.rxData = NULL;
    xfer.dataSize = txDataSize;
    xfer.configFlags =
        kDSPI_MasterCtar0 | resource->whichPCSTransfer | kDSPI_MasterPcsContinuous | kDSPI_MasterActiveAfterTransfer;
    result = DSPI_MasterTransferBlocking(spi, &xfer);
    if (result != kStatus_Success)
    {
        return flash_error;
    }

    if (rxData != NULL)
    {
        xfer.txData = NULL;
        xfer.rxData = rxData;
        xfer.dataSize = rxDataSize;
        xfer.configFlags = kDSPI_MasterCtar0 | resource->whichPCSTransfer | kDSPI_MasterPcsContinuous;
        result = DSPI_MasterTransferBlocking(spi, &xfer);
        if (result != kStatus_Success)
        {
            return flash_error;
        }
    }
#elif defined(FSL_FEATURE_SOC_LPSPI_COUNT) && FSL_FEATURE_SOC_LPSPI_COUNT
    lpspi_transfer_t xfer;
    xfer.txData = txData;
    xfer.rxData = NULL;
    xfer.dataSize = txDataSize;
    xfer.configFlags = resource->whichPCSTransfer | kLPSPI_MasterPcsContinuous;
    result = LPSPI_MasterTransferBlocking(spi, &xfer);
    if (result != kStatus_Success)
    {
        return flash_error;
    }
    if (rxData != NULL)
    {
        xfer.txData = NULL;
        xfer.rxData = rxData;
        xfer.dataSize = rxDataSize;
        xfer.configFlags = resource->whichPCSTransfer | kLPSPI_MasterPcsContinuous;
        result = LPSPI_MasterTransferBlocking(spi, &xfer);
        if (result != kStatus_Success)
        {
            return flash_error;
        }
    }
#endif

    return flash_ok;
}

flash_err_t SPI_Flash_ExecuteCommand(void *spiResource, flash_transfer_t *xfer)
{
    spi_resource_t *resource = (spi_resource_t *)spiResource;
    flash_err_t result;
    flash_command_table_t *commandTable = resource->commandTable;
    uint8_t cmd[NOR_FLASH_MAX_COMMAND_SZIE + 3];
    uint8_t i;
    uint8_t commandSize;

    if (resource->assertCS != NULL)
    {
        resource->assertCS();
    }
    switch (xfer->opCode)
    {
        case kNorFlash_WriteEnable:
            for (i = 0; i < commandTable->writeEnable.commandSize; i++)
            {
                cmd[i] = commandTable->writeEnable.command[i];
            }

            result = SPI_Flash_Transfer(resource, cmd, commandTable->writeEnable.commandSize, NULL, 0);
            if (result != flash_ok)
            {
                return result;
            }
            break;

        case kNorFlash_EraseChip:
            for (i = 0; i < commandTable->chipErase.commandSize; i++)
            {
                cmd[i] = commandTable->chipErase.command[i];
            }

            result = SPI_Flash_Transfer(resource, cmd, commandTable->chipErase.commandSize, NULL, 0);
            if (result != flash_ok)
            {
                return result;
            }
            break;

        case kNorFlash_EraseSector:
            switch (xfer->dataSize)
            {
                case (4 * 1024):
                    for (i = 0; i < commandTable->blockErase4K.commandSize; i++)
                    {
                        cmd[i] = commandTable->blockErase4K.command[i];
                    }
                    commandSize = commandTable->blockErase4K.commandSize;
                    break;

                case (32 * 1024):
                    for (i = 0; i < commandTable->blockErase32K.commandSize; i++)
                    {
                        cmd[i] = commandTable->blockErase32K.command[i];
                    }
                    commandSize = commandTable->blockErase32K.commandSize;
                    break;

                case (64 * 1024):
                    for (i = 0; i < commandTable->blockErase64K.commandSize; i++)
                    {
                        cmd[i] = commandTable->blockErase64K.command[i];
                    }
                    commandSize = commandTable->blockErase64K.commandSize;
                    break;

                default:
                    return flash_error;
            }

            cmd[commandSize] = (xfer->address & 0xFF0000U) >> 16U;
            cmd[commandSize + 1] = (xfer->address & 0x00FF00U) >> 8U;
            cmd[commandSize + 2] = (xfer->address & 0x0000FFU);
            result = SPI_Flash_Transfer(resource, cmd, commandSize + 3, NULL, 0);
            if (result != flash_ok)
            {
                return result;
            }
            break;

        case kNorFlash_Write:
            for (i = 0; i < commandTable->pageProgram.commandSize; i++)
            {
                cmd[i] = commandTable->pageProgram.command[i];
            }

            cmd[commandTable->pageProgram.commandSize] = (xfer->address & 0xFF0000U) >> 16U;
            cmd[commandTable->pageProgram.commandSize + 1] = (xfer->address & 0x00FF00U) >> 8U;
            cmd[commandTable->pageProgram.commandSize + 2] = (xfer->address & 0x0000FFU);
            result = SPI_Flash_Transfer(resource, cmd, commandTable->pageProgram.commandSize + 3, NULL, 0);
            if (result != flash_ok)
            {
                return result;
            }
            result = SPI_Flash_Transfer(resource, xfer->data, xfer->dataSize, NULL, 0);
            if (result != flash_ok)
            {
                return result;
            }
            break;

        case kNorFlash_Read:
            for (i = 0; i < commandTable->readData.commandSize; i++)
            {
                cmd[i] = commandTable->readData.command[i];
            }

            cmd[commandTable->readData.commandSize] = (xfer->address & 0xFF0000U) >> 16U;
            cmd[commandTable->readData.commandSize + 1] = (xfer->address & 0x00FF00U) >> 8U;
            cmd[commandTable->readData.commandSize + 2] = (xfer->address & 0x0000FFU);

            result =
                SPI_Flash_Transfer(resource, cmd, commandTable->readData.commandSize + 3, xfer->data, xfer->dataSize);
            if (result != flash_ok)
            {
                return result;
            }
            break;

        case kNorFlash_ReadStatus:
            for (i = 0; i < commandTable->readStatus.commandSize; i++)
            {
                cmd[i] = commandTable->readStatus.command[i];
            }

            result =
                SPI_Flash_Transfer(resource, cmd, commandTable->readStatus.commandSize, xfer->data, xfer->dataSize);
            if (result != flash_ok)
            {
                return result;
            }
            break;

        default:
            break;
    }

    if (resource->deassertCS != NULL)
    {
        resource->deassertCS();
    }

    return flash_ok;
}
