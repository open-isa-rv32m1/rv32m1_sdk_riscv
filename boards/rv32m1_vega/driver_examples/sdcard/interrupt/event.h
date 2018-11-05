/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _EVENT_H_
#define _EVENT_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Event type  */
typedef enum _event
{
    kEVENT_TransferComplete = 0U, /*!< Transfer complete event */
    kEVENT_CardDetect = 1U,       /*!< Card detect event */
} event_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Event Function
 * @{
 */

/*!
 * @brief Initialize timer to implement wait event timeout.
 */
void EVENT_InitTimer(void);

/* Callback function for SDHC */

/*!
 * @brief Create event.
 * @param eventType The event type
 * @retval true Create event successfully.
 * @retval false Create event failed.
 */
bool EVENT_Create(event_t eventType);

/*!
 * @brief Wait event.
 *
 * @param eventType The event type
 * @param timeoutMilliseconds Timeout time in milliseconds.
 * @retval true Wait event successfully.
 * @retval false Wait event failed.
 */
bool EVENT_Wait(event_t eventType, uint32_t timeoutMilliseconds);

/*!
 * @brief Notify event.
 * @param eventType The event type
 * @retval true Notify event successfully.
 * @retval false Notify event failed.
 */
bool EVENT_Notify(event_t eventType);

/*!
 * @brief Delete event.
 * @param eventType The event type
 */
void EVENT_Delete(event_t eventType);

/* @} */

#if defined(__cplusplus)
}
#endif

#endif /* _EVENT_H_*/
