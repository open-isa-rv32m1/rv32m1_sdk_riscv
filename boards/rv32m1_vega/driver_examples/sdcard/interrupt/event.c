/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "event.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get event instance.
 * @param eventType The event type
 * @return The event instance's pointer.
 */
static volatile uint32_t *EVENT_GetInstance(event_t eventType);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Card detect event. */
static volatile uint32_t g_eventCardDetect;

/*! @brief transfer complete event. */
static volatile uint32_t g_eventTransferComplete;

/*! @brief Time variable unites as milliseconds. */
volatile uint32_t g_timeMilliseconds;

/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
#ifdef __CA7_REV
    SystemClearSystickFlag();
#endif
#if defined(__riscv)
    SystemClearSystickFlag();
#endif
    g_timeMilliseconds++;
}

void EVENT_InitTimer(void)
{
#ifdef __CA7_REV
    /* special for i.mx6ul */
    SystemSetupSystick(1000U, (void *)SysTick_Handler, 32U);
    SystemClearSystickFlag();
#elif defined(__riscv)
    SystemSetupSystick (1000, 7);
    SystemClearSystickFlag();
#else
    /* Set systick reload value to generate 1ms interrupt */
    SysTick_Config(CLOCK_GetFreq(kCLOCK_CoreSysClk) / 1000U);
#endif
}

static volatile uint32_t *EVENT_GetInstance(event_t eventType)
{
    volatile uint32_t *event;

    switch (eventType)
    {
        case kEVENT_TransferComplete:
            event = &g_eventTransferComplete;
            break;
        case kEVENT_CardDetect:
            event = &g_eventCardDetect;
            break;
        default:
            event = NULL;
            break;
    }

    return event;
}

bool EVENT_Create(event_t eventType)
{
    volatile uint32_t *event = EVENT_GetInstance(eventType);

    if (event)
    {
        *event = 0;
        return true;
    }
    else
    {
        return false;
    }
}

bool EVENT_Wait(event_t eventType, uint32_t timeoutMilliseconds)
{
    uint32_t startTime;
    uint32_t elapsedTime;

    volatile uint32_t *event = EVENT_GetInstance(eventType);

    if (timeoutMilliseconds && event)
    {
        startTime = g_timeMilliseconds;
        do
        {
            elapsedTime = (g_timeMilliseconds - startTime);
        } while ((*event == 0U) && (elapsedTime < timeoutMilliseconds));
        *event = 0U;

        return ((elapsedTime < timeoutMilliseconds) ? true : false);
    }
    else
    {
        return false;
    }
}

bool EVENT_Notify(event_t eventType)
{
    volatile uint32_t *event = EVENT_GetInstance(eventType);

    if (event)
    {
        *event = 1U;
        return true;
    }
    else
    {
        return false;
    }
}

void EVENT_Delete(event_t eventType)
{
    volatile uint32_t *event = EVENT_GetInstance(eventType);

    if (event)
    {
        *event = 0U;
    }
}
