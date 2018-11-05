/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_lpit.h"
#include "usb_timer.h"
#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
#include "fsl_intmux.h"
#endif

static usb_timer_callback_t timerCallback;

void LPIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    LPIT_ClearStatusFlags(LPIT0, kLPIT_Channel0TimerFlag);
    timerCallback();
}

void LPIT1_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    LPIT_ClearStatusFlags(LPIT1, kLPIT_Channel0TimerFlag);
    timerCallback();
}

void USB_TimerInit(uint8_t instance, uint32_t interval, uint32_t clock, usb_timer_callback_t callback)
{
    LPIT_Type *instanceList[] = LPIT_BASE_PTRS;
    IRQn_Type instanceIrq[][FSL_FEATURE_LPIT_TIMER_COUNT] = LPIT_IRQS;
    /* Structure of initialize PIT */
    lpit_config_t lpitConfig;
    lpit_chnl_params_t lpitChannelConfig;

    /*
     * lpitConfig.enableRunInDebug = false;
     * lpitConfig.enableRunInDoze = false;
     */
    LPIT_GetDefaultConfig(&lpitConfig);

    /* Init lpit module */
    LPIT_Init(instanceList[instance], &lpitConfig);

    lpitChannelConfig.chainChannel = false;
    lpitChannelConfig.enableReloadOnTrigger = false;
    lpitChannelConfig.enableStartOnTrigger = false;
    lpitChannelConfig.enableStopOnTimeout = false;
    lpitChannelConfig.timerMode = kLPIT_PeriodicCounter;
    /* Set default values for the trigger source */
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
    lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;

    /* Init lpit channel 0 */
    LPIT_SetupChannel(instanceList[instance], kLPIT_Chnl_0, &lpitChannelConfig);

    /* Set timer period for channel 0 */
    LPIT_SetTimerPeriod(instanceList[instance], kLPIT_Chnl_0, USEC_TO_COUNT(interval, clock));

    /* Enable timer interrupts for channel 0 */
    LPIT_EnableInterrupts(instanceList[instance], kLPIT_Channel0TimerInterruptEnable);

    timerCallback = callback;

    /* Enable at the NVIC */
    if (kStatus_Success != EnableIRQ(instanceIrq[instance][0]))
    {
    }
}

void USB_TimerInt(uint8_t instance, uint8_t enable)
{
    LPIT_Type *instanceList[] = LPIT_BASE_PTRS;
    if (enable)
    {
        /* Start counting */
        LPIT_StartTimer(instanceList[instance], kLPIT_Chnl_0);
    }
    else
    {
        LPIT_StopTimer(instanceList[instance], kLPIT_Chnl_0);
    }
}
