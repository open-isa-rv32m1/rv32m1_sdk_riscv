/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "fsl_nor_flash.h"
#include "fsl_spi_adapter.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_LPSPI_MASTER_BASEADDR (LPSPI1)
#define EXAMPLE_LPSPI_MASTER_IRQN (LPSPI1_IRQn)
#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ (CLOCK_GetIpFreq(kCLOCK_Lpspi1))
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT (kLPSPI_Pcs1)
#define VERIFY_PATTERN 0x5A
#define ERASE_32K_SIZE 32 * 1024
#define ERASE_64K_SIZE 64 * 1024
/*******************************************************************************
* Prototypes
******************************************************************************/
/*!
 * @brief Show the menu for user.
 *
 */
static uint8_t show_menu(void);

/*******************************************************************************
 * variables
 ******************************************************************************/

static char s_StrInputAddress[] = "\r\nInput the %s address(HEX), 0x";
static char s_StrInvalid[] = "\r\nInvalid input!\r\n";
static char s_StrEraseFail[] = "\r\nErase failed!\r\n";
static char s_StrEraseOk[] = "\r\nErase OK!\r\n";

extern flash_handle_t handle;
extern flash_config_t flashConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/

flash_command_table_t flashCommand = {
    .writeEnable =
        {
            .command = {0x06u}, .commandSize = 1,
        },
    .writeDisable =
        {
            .command = {0x04u}, .commandSize = 1,
        },
    .pageProgram =
        {
            .command = {0x02u}, .commandSize = 1,
        },
    .readData =
        {
            .command = {0x03u}, .commandSize = 1,
        },
    .readDataFast =
        {
            .command = {0x0Bu}, .commandSize = 1,
        },
    .writeStatus =
        {
            .command = {0x01u}, .commandSize = 1,
        },
    .readStatus =
        {
            .command = {0x05u}, .commandSize = 1,
        },
    .blockErase4K =
        {
            .command = {0x20u}, .commandSize = 1,
        },
    .blockErase32K =
        {
            .command = {0x52u}, .commandSize = 1,
        },
    .blockErase64K =
        {
            .command = {0xd8u}, .commandSize = 1,
        },
    .chipErase =
        {
            .command = {0xc7u}, .commandSize = 1,
        },
};

flash_config_t flashConfig = {
    .totalSize = 0x400000u,
    .pageSize = 256,
    .sectorSize = 0x1000u,
    .needWriteEnable = true,
    .statusValueSize = 1,
    .statusBusyMask = 0x01u,
    .statusBusyValue = 1,
};

void BOARD_SPI_CSActivate(void)
{
    GPIO_WritePinOutput(BOARD_SPI_FLASH_CS_GPIO, BOARD_SPI_FLASH_CS_GPIO_PIN, 0);
}

void BOARD_SPI_Deactivate(void)
{
    GPIO_WritePinOutput(BOARD_SPI_FLASH_CS_GPIO, BOARD_SPI_FLASH_CS_GPIO_PIN, 1);
}

spi_resource_t spiResource = {
    .spi = LPSPI1,
    .whichPCSTransfer = kLPSPI_MasterPcs2,
    .assertCS = BOARD_SPI_CSActivate,
    .deassertCS = BOARD_SPI_Deactivate,
    .commandTable = &flashCommand,
};

flash_handle_t handle = {
    .executeCommand = SPI_Flash_ExecuteCommand, .resource = &spiResource,
};

void BOARD_InitSPIAdapter(void)
{
    lpspi_master_config_t masterConfig;

    /*Master config*/
    masterConfig.baudRate = 20000000;
    masterConfig.bitsPerFrame = 8;
    masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction = kLPSPI_MsbFirst;

    masterConfig.pcsToSckDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;

    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, EXAMPLE_LPSPI_MASTER_CLOCK_FREQ);

    /* Sets the FLASH PCS as low logic */
    gpio_pin_config_t csPinConfig = {
        kGPIO_DigitalOutput, 1,
    };
    GPIO_PinInit(BOARD_SPI_FLASH_CS_GPIO, BOARD_SPI_FLASH_CS_GPIO_PIN, &csPinConfig);
}


static uint8_t show_menu(void)
{
    char input;

    PRINTF("\r\n1 - Erase entire chip\r\n");
    PRINTF("2 - Erase sectors\r\n");
    PRINTF("3 - Erase block\r\n");
    PRINTF("4 - Program one page with pattern (0x%x) and verify\r\n", VERIFY_PATTERN);
    PRINTF("5 - Read byte\r\n");
    PRINTF("Please enter your choice (1-5): ");

    SCANF(" %c", &input);
    PRINTF("\r\n");
    return (input - '0');
}

/*Error trap function*/
void ErrorTrap(void)
{
    PRINTF("\n\rError Occured. Please check the configurations.\n\r");
    while (1)
    {
        ;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    char input, buf[1024];
    uint32_t ret;
    uint32_t blkSize;
    uint32_t blockSize;
    uint32_t offset = 0x00U;
    uint32_t i = 0U;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(kCLOCK_Lpspi1, kCLOCK_IpSrcFircAsync);

    BOARD_InitSPIAdapter();

    PRINTF("\r\n***SPI Flash Demo***\r\n");

    NOR_Flash_Init(&handle, &flashConfig);

    while (1)
    {
        /* showing the UI */
        input = show_menu();

        offset = 0x00U;

        switch (input)
        {
            case 1:
                /* erase all chips */
                ret = NOR_Flash_ChipErase(&handle);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                }
                else
                {
                    PRINTF(s_StrEraseOk);
                }
                break;

            case 2:
                PRINTF(s_StrInputAddress, "sector");
                SCANF("%x", &offset);

                if (offset & (handle.sectorSize - 1))
                {
                    PRINTF("Address should be 0x%x aligned !\r\n", handle.sectorSize);
                }

                if (offset >= handle.totalSize)
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                /* erase sector */
                ret = NOR_Flash_EraseBlock(&handle, offset, handle.sectorSize);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                }
                else
                {
                    PRINTF(s_StrEraseOk);
                }
                break;

            case 3:
                PRINTF(s_StrInputAddress, "block");
                SCANF("%x", &offset);
                PRINTF("\r\n1. 32K block size");
                PRINTF("\r\n2. 64K block size");
                PRINTF("\r\nSelect the block size:");
                SCANF("%d", &blkSize);
                PRINTF("\r\n");

                if ((offset >= handle.totalSize) || ((blkSize != 1) && (blkSize != 2)))
                {
                    PRINTF(s_StrInvalid);
                    break;
                }
                blockSize = (blkSize == 1 ? ERASE_32K_SIZE : ERASE_64K_SIZE);
                if ((offset % blockSize))
                {
                    PRINTF(s_StrInvalid);
                    PRINTF("Address should be block size aligned !\r\n");
                    break;
                }

                /* erase one block */
                ret = NOR_Flash_EraseBlock(&handle, offset, blockSize);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                }
                else
                {
                    PRINTF(s_StrEraseOk);
                }
                break;

            case 4:
                /* get the program address and length */
                PRINTF(s_StrInputAddress, "program");
                SCANF("%x", &offset);
                PRINTF("\r\n");

                if (offset > (handle.totalSize - handle.pageSize))
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                if ((offset % handle.pageSize))
                {
                    PRINTF("Address should be page size aligned !\r\n");
                    break;
                }
                /* erase sectors first */
                ret = NOR_Flash_EraseBlock(&handle, (offset & ~(handle.sectorSize - 1)), handle.sectorSize);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                    break;
                }

                /* fill the write buffer with pattern */
                memset(buf, VERIFY_PATTERN, handle.pageSize);
                ret = NOR_Flash_WriteData(&handle, offset, (void *)buf, handle.pageSize);
                if (ret)
                {
                    PRINTF("Program page failed!\r\n");
                    break;
                }

                /* read back and verify */
                memset(buf, 0, handle.pageSize);
                ret = NOR_Flash_ReadData(&handle, offset, (void *)buf, handle.pageSize);
                if (ret)
                {
                    PRINTF("Read page failed!\r\n");
                    break;
                }

                /* compare */
                for (i = 0; i < handle.pageSize; i++)
                {
                    if (buf[i] != VERIFY_PATTERN)
                    {
                        PRINTF("Program and verify failed!\r\n");
                        break;
                    }
                }

                PRINTF("Program and verify done!\r\n");
                break;

            case 5:
                /* get the read address */
                PRINTF(s_StrInputAddress, "read");
                SCANF("%x", &offset);
                PRINTF("\r\n");

                if (offset >= handle.totalSize)
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                ret = NOR_Flash_ReadData(&handle, offset, (void *)buf, 1);
                if (ret)
                {
                    PRINTF("Read byte failed!\r\n");
                    break;
                }
                PRINTF("0x%x = 0x%x\r\n", offset, buf[0]);
                break;

            default:
                PRINTF(s_StrInvalid);
                break;
        }
    }
}
