/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_gpio.h"
#include "fsl_ewm.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_intmux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SW_GPIO BOARD_SW2_GPIO
#define SW_GPIO_PIN BOARD_SW2_GPIO_PIN
#define SW_NAME BOARD_SW2_NAME
#define WDOG_EWM_IRQHandler EWM_IRQHandler
#define WDOG_EWM_IRQn EWM_IRQn

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/
volatile bool ewmIsrFlag = false;
static EWM_Type *base = EWM;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Configure gpio as input for button
 *
 */
void gpio_configure(void)
{
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput, 0,
    };
    GPIO_PinInit(SW_GPIO, SW_GPIO_PIN, &sw_config);
}

/*!
 * @brief Check if button is pressed.
 *
 * This function gets the state of button.
 *
 * @return 0 if button is not pressed.
 *         1 if button is pressed
 */
static uint32_t is_key_pressed(void)
{
    return !(GPIO_ReadPinInput(SW_GPIO, SW_GPIO_PIN));
}

/*!
 * @brief EWM interrupt handler
 *
 */
void WDOG_EWM_IRQHandler(void)
{
    EWM_DisableInterrupts(base, kEWM_InterruptEnable); /*!< de-assert interrupt request */
    ewmIsrFlag = true;
}

void delay(void)
{
    for (uint32_t i = 0; i < 0x7fffffU; i++)
    {
        __NOP();
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    ewm_config_t config;

    /* Init hardware*/
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    INTMUX_Init(INTMUX1);
    /* Enable EWM intmux source on INTMUX channel 0 */
    INTMUX_EnableInterrupt(INTMUX1, 0, EWM_IRQn);
    gpio_configure();

    /* EWM peripheral driver test */
    /*
     * config.enableEwm = true;
     * config.enableEwmInput = false;
     * config.setInputAssertLogic = false;
     * config.enableInterrupt = false;
     * config.clockSource = kEWM_LpoClockSource0;
     * config.prescaler = 0U;
     * config.compareLowValue = 0U;
     * config.compareHighValue = 0xFEU;
     */
    EWM_GetDefaultConfig(&config);
    config.enableInterrupt = true;
#if !defined(__riscv)   
    NVIC_EnableIRQ(WDOG_EWM_IRQn);
#else 
    EnableIRQ(WDOG_EWM_IRQn);
#endif
    EWM_Init(base, &config);

    PRINTF("\r\n EWM example ");
    PRINTF("\r\n Press %s to expire EWM ", SW_NAME);
    while (1)
    {
        /* Restart counter*/
        EWM_Refresh(base);
        /* Check for SW button push*/
        if (is_key_pressed())
        {
            /*Check ewm interrupt occur*/
            while (!ewmIsrFlag)
            {
            }
            PRINTF("\r\n EWM interrupt is occurred");

            /* Clear interrupt flag*/
            ewmIsrFlag = false;
            PRINTF("\r\n Press %s to expire EWM again", SW_NAME);
            /*Restart counter and enable interrupt for next run*/
            EWM_Refresh(base);
            /* Delay for a while in order to press button, interrrupt occurred only once*/
            delay();
            /*Enable EWM interrupt*/
            EWM_EnableInterrupts(base, kEWM_InterruptEnable);
        }
    }
}
