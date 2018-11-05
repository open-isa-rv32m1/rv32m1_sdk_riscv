/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_lpadc.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPADC_BASE ADC0
#define DEMO_LPADC_IRQn ADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC ADC0_IRQHandler
#define DEMO_LPADC_USER_CHANNEL 0U
#define DEMO_LPADC_USER_CMDID 1U /* CMD1 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_LpadcConversionCompletedFlag = false;
volatile uint32_t g_LpadcInterruptCounter = 0U;
lpadc_conv_result_t g_LpadcResultConfigStruct;
/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_LPADC_IRQ_HANDLER_FUNC(void)
{
    g_LpadcInterruptCounter++;
    if (LPADC_GetConvResult(DEMO_LPADC_BASE, &g_LpadcResultConfigStruct))
    {
        g_LpadcConversionCompletedFlag = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrcDiv(kCLOCK_Lpadc0, kCLOCK_IpSrcFircAsync, 2U, 0U);

    PRINTF("LPADC Interrupt Example\r\n");

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.FIFOWatermark = 0U; /* Set watermark as 0U. */
    LPADC_Init(DEMO_LPADC_BASE, &mLpadcConfigStruct);

#if defined(FSL_FEATURE_LPADC_HAS_CFG_CALOFS) && FSL_FEATURE_LPADC_HAS_CFG_CALOFS
    LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
#endif /* FSL_FEATURE_LPADC_HAS_CFG_CALOFS */

    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = DEMO_LPADC_USER_CHANNEL;
    LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, DEMO_LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = DEMO_LPADC_USER_CMDID; /* CMD15 is executed. */
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(DEMO_LPADC_BASE, 0U, &mLpadcTriggerConfigStruct); /* Configurate the trigger0. */

    /* Enable the watermark interrupt. */
    LPADC_EnableInterrupts(DEMO_LPADC_BASE, kLPADC_FIFOWatermarkInterruptEnable);
    EnableIRQ(DEMO_LPADC_IRQn);

    /* When the number of datawords stored in the ADC Result FIFO is greater
    * than watermark value(0U), LPADC watermark interrupt would be triggered.
    */
    PRINTF("Please press any key to get user channel's ADC value.\r\n");
    while (1)
    {
        GETCHAR();
        LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U); /* 1U is trigger0 mask. */
        while (!g_LpadcConversionCompletedFlag)
        {
        }
        PRINTF("ADC value: %d\r\nADC interrupt count: %d\r\n", ((g_LpadcResultConfigStruct.convValue) >> 3U),
               g_LpadcInterruptCounter);
        g_LpadcConversionCompletedFlag = false;
    }
}
