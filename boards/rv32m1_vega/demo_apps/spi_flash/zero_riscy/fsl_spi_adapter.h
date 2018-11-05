/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_SPI_ADAPTER_H_
#define _FSL_SPI_ADAPTER_H_

#include "fsl_flash.h"
#if defined(FSL_FEATURE_SOC_SPI_COUNT) && FSL_FEATURE_SOC_SPI_COUNT
#include "fsl_spi.h"
#elif defined(FSL_FEATURE_SOC_DSPI_COUNT) && FSL_FEATURE_SOC_DSPI_COUNT
#include "fsl_dspi.h"
#elif defined(FSL_FEATURE_SOC_LPSPI_COUNT) && FSL_FEATURE_SOC_LPSPI_COUNT
#include "fsl_lpspi.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(FSL_FEATURE_SOC_SPI_COUNT) && FSL_FEATURE_SOC_SPI_COUNT
typedef SPI_Type *spi_type_t;
#elif defined(FSL_FEATURE_SOC_DSPI_COUNT) && FSL_FEATURE_SOC_DSPI_COUNT
typedef DSPI_Type *spi_type_t;
#elif defined(FSL_FEATURE_SOC_LPSPI_COUNT) && FSL_FEATURE_SOC_LPSPI_COUNT
typedef LPSPI_Type *spi_type_t;
#endif

/*! @brief Flash command structure. */
typedef struct _flash_command
{
    uint8_t command[NOR_FLASH_MAX_COMMAND_SZIE]; /*!< Command array. */
    uint8_t commandSize;                         /*!< Command array size. */
} flash_command_t;

/*! @brief Flash command table structure. */
typedef struct _flash_command_table
{
    flash_command_t writeEnable;   /*!< WriteEnable Command. */
    flash_command_t writeDisable;  /*!< WriteDisable Command. */
    flash_command_t pageProgram;   /*!< PageProgram Command. */
    flash_command_t readData;      /*!< Read Data Command. */
    flash_command_t readDataFast;  /*!< Read Data fast Command. */
    flash_command_t writeStatus;   /*!< Write status Command. */
    flash_command_t readStatus;    /*!< Read status Command. */
    flash_command_t blockErase4K;  /*!< Block erase 4K Command. */
    flash_command_t blockErase8K;  /*!< Block erase 8K Command. */
    flash_command_t blockErase16K; /*!< Block erase 16K Command. */
    flash_command_t blockErase32K; /*!< Block erase 32K Command. */
    flash_command_t blockErase64K; /*!< Block erase 64K Command. */
    flash_command_t chipErase;     /*!< Chip erase Command. */
} flash_command_table_t;

typedef struct _spi_resource spi_resource_t;
struct _spi_resource
{
    spi_type_t spi;                      /*!< SPI type. */
    flash_command_table_t *commandTable; /*!< command table. */
    void (*assertCS)(void);              /*!< Assert CS pin. */
    void (*deassertCS)(void);            /*!< Deassert CS pin. */
    uint8_t whichPCSTransfer;            /*!< Use which PCS for transfer. */
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus. */

/*!
 * @name spi adpater command execution API
 * @{
 */

/*!
 * @brief spi adpater command execution API.
 *
 * @param spiResource pointer to #spi_resource_t
 * @param xfer pointer to #flash_transfer_t structure which stores the transfer message
 */
flash_err_t SPI_Flash_ExecuteCommand(void *spiResource, flash_transfer_t *xfer);

/* @} */
#if defined(__cplusplus)
}
#endif /*_cplusplus. */
/*@}*/

#endif
