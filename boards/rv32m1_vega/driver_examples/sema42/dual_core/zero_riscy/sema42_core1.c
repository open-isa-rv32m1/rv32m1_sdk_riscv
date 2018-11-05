/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_sema42.h"
#include "board.h"
#include "fsl_mu.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_MU MUB
#define APP_SEMA42 SEMA420
#define LED_OFF() LED1_OFF()
/* Flag indicates Core Boot Up*/
#define BOOT_FLAG 0x01U
/* The SEMA42 gate */
#define SEMA42_GATE 0U
/* Core 1 lock semaphore */
#define LOCK_CORE 1U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* MUB init */
    MU_Init(APP_MU);
    /* Send flag to Core 0 to indicate Core 1 has startup */
    MU_SetFlags(APP_MU, BOOT_FLAG);

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);

    /* Lock the sema42 gate. */
    SEMA42_Lock(APP_SEMA42, SEMA42_GATE, LOCK_CORE);

    /* Turn off led */
    LED_OFF();

    while (1)
    {
    }
}
