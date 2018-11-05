/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_sema42.h"
#include "fsl_mu.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_xrdc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LED_INIT()              LED1_INIT(LOGIC_LED_ON)
#define APP_MU                  MUA
#define APP_SEMA42              SEMA420
#define APP_CORE1_BOOT_MODE     kMU_CoreBootFromDflashBase
/* Flag indicates Core Boot Up*/
#define BOOT_FLAG 0x01U
/* The SEMA42 gate */
#define SEMA42_GATE 0U
/* Core 0 lock semaphore */
#define LOCK_CORE 0U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void APP_InitDomain(void);
void APP_DeInitDomain(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_InitDomain(void)
{
    uint32_t i;
    xrdc_periph_access_config_t periConfig;
    xrdc_processor_domain_assignment_t domainConfig;
    xrdc_mem_access_config_t memConfig;

    const xrdc_periph_t periphAccessible[] = {
    kXRDC_PeriphLpuart0, kXRDC_PeriphWdog0,   kXRDC_PeriphXrdcMgr, kXRDC_PeriphXrdcMdac, kXRDC_PeriphXrdcPac,
    kXRDC_PeriphXrdcMrc, kXRDC_PeriphSema420, kXRDC_PeriphSema421, kXRDC_PeriphWdog1,    kXRDC_PeriphPcc0,
    kXRDC_PeriphPcc1,    kXRDC_PeriphMua,     kXRDC_PeriphMub}; 

    XRDC_Init(XRDC);
    XRDC_SetGlobalValid(XRDC, false);

    /* Assign ri5cy to domain 0. */
    XRDC_GetDefaultProcessorDomainAssignment(&domainConfig);
    domainConfig.domainId = 0;
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterRI5CYCodeBus, 0, &domainConfig);
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterRI5CYSystemBus, 0, &domainConfig);

    /* Assign zero_riscy to domain 1. */
    domainConfig.domainId = 1;
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterZERORISCYCodeBus, 0, &domainConfig);
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterZERORISCYSystemBus, 0, &domainConfig);

    /*
     * Configure the peripheral policy.
     */
    XRDC_GetPeriphAccessDefaultConfig(&periConfig);
    /* Access permission for domain 0. */
    periConfig.policy[0] = kXRDC_AccessPolicyAll;
    /* Access permission for domain 1. */
    periConfig.policy[1] = kXRDC_AccessPolicyAll;

    for (i = 0; i < ARRAY_SIZE(periphAccessible); i++)
    {
        periConfig.periph = periphAccessible[i];
        XRDC_SetPeriphAccessConfig(XRDC, &periConfig);
    }

    /* Configure the memory policy. */
    XRDC_GetMemAccessDefaultConfig(&memConfig);

    memConfig.mem = kXRDC_MemMrc0_0;
    memConfig.baseAddress = 0x00000000U;
    memConfig.endAddress = 0x000FFFFFU;
    memConfig.codeRegion = kXRDC_MemCodeRegion1;
    memConfig.policy[0] = kXRDC_AccessFlagsAlt4;
    memConfig.policy[1] = kXRDC_AccessFlagsNone;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    memConfig.mem = kXRDC_MemMrc1_0;
    memConfig.baseAddress = 0x01000000U;
    memConfig.endAddress = 0x0103FFFFU;
    memConfig.codeRegion = kXRDC_MemCodeRegion1;
    memConfig.policy[0] = kXRDC_AccessFlagsNone;
    memConfig.policy[1] = kXRDC_AccessFlagsAlt4;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    memConfig.mem = kXRDC_MemMrc0_1;
    memConfig.baseAddress = 0x08000000U;
    memConfig.endAddress = 0x0800FFFFU;
    memConfig.codeRegion = kXRDC_MemCodeRegion1;
    memConfig.policy[0] = kXRDC_AccessFlagsAlt4;
    memConfig.policy[1] = kXRDC_AccessFlagsNone;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    memConfig.mem = kXRDC_MemMrc0_2;
    memConfig.baseAddress = 0x08800000U;
    memConfig.endAddress = 0x0880BFFFU;
    memConfig.codeRegion = kXRDC_MemCodeRegion1;
    memConfig.policy[0] = kXRDC_AccessFlagsAlt4;
    memConfig.policy[1] = kXRDC_AccessFlagsAlt4;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    memConfig.mem = kXRDC_MemMrc1_1;
    memConfig.baseAddress = 0x09000000U;
    memConfig.endAddress = 0x0901FFFFU;
    memConfig.codeRegion = kXRDC_MemCodeRegion1;
    memConfig.policy[0] = kXRDC_AccessFlagsNone;
    memConfig.policy[1] = kXRDC_AccessFlagsAlt4;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    memConfig.mem = kXRDC_MemMrc1_2;
    memConfig.baseAddress = 0xF0006000U;
    memConfig.endAddress = 0xF0006FFFU;
    memConfig.codeRegion = kXRDC_MemCodeRegion0;
    memConfig.policy[0] = kXRDC_AccessFlagsNone;
    memConfig.policy[1] = kXRDC_AccessFlagsAlt4;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    memConfig.mem = kXRDC_MemMrc0_3;
    memConfig.baseAddress = 0x20000000U;
    memConfig.endAddress = 0x2001FFFFU;
    memConfig.codeRegion = kXRDC_MemCodeRegion0;
    memConfig.policy[0] = kXRDC_AccessFlagsAlt4;
    memConfig.policy[1] = kXRDC_AccessFlagsNone;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    memConfig.mem = kXRDC_MemMrc0_4;
    memConfig.baseAddress = 0x20020000U;
    memConfig.endAddress = 0x2003FFFFU;
    memConfig.codeRegion = kXRDC_MemCodeRegion0;
    memConfig.policy[0] = kXRDC_AccessFlagsAlt4;
    memConfig.policy[1] = kXRDC_AccessFlagsNone;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    XRDC_SetGlobalValid(XRDC, true);
}

void APP_DeinitDomain(void)
{
    XRDC_SetGlobalValid(XRDC, false);
}

static void BOARD_InitLedPin(void)
{
    const gpio_pin_config_t config = {
        .pinDirection = kGPIO_DigitalOutput, .outputLogic = 1,
    };

    GPIO_PinInit(BOARD_LED1_GPIO, BOARD_LED1_GPIO_PIN, &config);
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware.*/
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitLedPin();
    /* Initialize LED */
    LED_INIT();

    /* MUA init */
    MU_Init(APP_MU);

    /* Print the initial banner */
    PRINTF("\r\nSema42 example!\r\n");

    APP_InitDomain();

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);
    /* Reset the sema42 gate */
    SEMA42_ResetAllGates(APP_SEMA42);
    /* Lock the sema42 gate. */
    SEMA42_Lock(APP_SEMA42, SEMA42_GATE, LOCK_CORE);

    /* Boot Core 1. */
    MU_BootCoreB(APP_MU, APP_CORE1_BOOT_MODE);
    /* Wait Core 1 is Boot Up */
    while (BOOT_FLAG != MU_GetFlags(APP_MU))
    {
    }

    /* Wait until user press any key */
    PRINTF("Press any key to unlock semaphore and Core 1 will turn off the LED\r\n");
    GETCHAR();

    /* Unlock the sema42 gate. */
    SEMA42_Unlock(APP_SEMA42, SEMA42_GATE);

    PRINTF("Now the LED should be turned off\r\n");

    APP_DeinitDomain();

    PRINTF("\r\nSema42 example succeed!\r\n");

    while (1)
    {
    }
}
