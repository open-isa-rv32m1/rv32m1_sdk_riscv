/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_mu.h"
#include "fsl_msmc.h"
#include "fsl_lptmr.h"
#include "fsl_llwu.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_intmux.h"
#include "board.h"

#include "pin_mux.h"
#include "fsl_spm.h"
#include "fsl_lpuart.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_MU MUA
#define APP_MU_XFER_CMD_CHANNEL_INDEX 0U
#define APP_MU_SHAKE_HAND_VALUE 0xAAAAAAAA
#define APP_CORE1_BOOT_MODE kMU_CoreBootFromDflashBase
#define APP_MU_IRQHandler MUA_IRQHandler

#define APP_SMC SMC0

#define APP_LLWU LLWU0
#define APP_LLWU_LPTMR_IDX 0U
/* According to Table 4-11. Wakeup Sources for LLWU0 inputs, LLWU_M4DR - LPTMR0 \ \
    Trigger*/
#define APP_LLWU_IRQHandler LLWU0_IRQHandler

#define APP_LPTMR LPTMR0
#define APP_LPTMR_IRQn LPTMR0_IRQn
#define APP_LPTMR_IRQHandler LPTMR0_IRQHandler

/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 'A' - 1,
    kAPP_PowerModeRun,   /* Normal RUN mode */
    kAPP_PowerModeWait,  /* WAIT mode. */
    kAPP_PowerModeStop,  /* STOP mode. */
    kAPP_PowerModeVlpr,  /* VLPR mode. */
    kAPP_PowerModeVlpw,  /* VLPW mode. */
    kAPP_PowerModeVlps,  /* VLPS mode. */
    kAPP_PowerModeLls,   /* LLS mode. */
    kAPP_PowerModeVlls2, /* VLLS2 mode. */
    kAPP_PowerModeVlls0, /* VLLS0 mode. */
    kAPP_PowerModeHsrun, /* HSRUN mode. */
    kAPP_PowerModeMax
} app_power_mode_t;

/* Flag indicates Core Boot Up*/
#define BOOT_FLAG 0x01U

/*******************************************************************************
 * Variables
 ******************************************************************************/
app_power_mode_t targetPowerMode;
smc_power_state_t curPowerState;
static uint8_t g_WakeupTimeoutSecond;/* Wakeup timeout. (Unit:Second) */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_WaitDebugConsoleOutputComplete(void);
void prepareForLowPower(void);
void restoreFromLowPower(void);
void turn_off_spi_flash(void);
static bool APP_CheckPowerModeCanTransfer(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode);
static bool APP_CheckPowerModeNeedWakeup(app_power_mode_t targetPowerMode);
static void APP_ShowPowerMode(smc_power_state_t powerMode);
static void LPTMR_Configuration(void);
static uint8_t APP_GetWakeupTimeout(void);
void APP_PowerModeSwitch(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode);

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_InitDebugConsoleWithSirc(void)
{
    scg_sirc_config_t scgSircConfigStruct = {
        .enableMode =
            kSCG_SircEnable | kSCG_SircEnableInLowPower, /* Enable SIRC clock, Enable SIRC in low power mode */
        .div1 = kSCG_AsyncClkDisable,                    /* Slow IRC Clock Divider 1: Clock output is disabled */
        .div2 = kSCG_AsyncClkDivBy1,                     /* Slow IRC Clock Divider 2: Clock output is disabled */
        .div3 = kSCG_AsyncClkDivBy1,                     /* Slow IRC Clock Divider 3: divided by 1 */
        .range = kSCG_SircRangeHigh,                     /* Slow IRC high range clock (8 MHz) */
    };
    uint32_t uartClkSrcFreq;

    /* Init SIRC with DIV2 enabled for LPUART in low power mode. */
    CLOCK_InitSirc(&scgSircConfigStruct);

    /* Set PCC LPUART0 selection */
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);
    uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;
    DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void BOARD_WaitDebugConsoleOutputComplete(void)
{
    while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
    }
}

/*!
 * @brief Application entry.
 */
int main()
{
    uint32_t power_mode_cmd, i;

    /* Init board hardware.*/
    /* Clear the IO lock when reset from VLLSx low power mode */
    if(SPM_GetPeriphIOIsolationFlag(SPM))
    {
        SPM_ClearPeriphIOIsolationFlag(SPM);    
    }

    /* Diasble CAU3, otherwise when boot from ROM, SACKERR reset will occur (CAU3 is enabled enable in ROM code) in
       stop mode. */
    CLOCK_DisableClock(kCLOCK_Cau3);

    BOARD_InitPins_Core0();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsoleWithSirc();
    INTMUX_Init(INTMUX0);
    LPTMR_Configuration();

    /* Must disable the SPI Flash and leave the SPI pins configured as SPI
       pins to achieve the lowest power currents! */
    turn_off_spi_flash();

    RSIM->SW_CONFIG |= RSIM_SW_CONFIG_RADIO_POR_BIT_MASK;
    for(i=0;i<0xffff;i++);
    RSIM->SW_CONFIG &= ~RSIM_SW_CONFIG_RADIO_POR_BIT_MASK;

    /* Unlock the Very Low Power protection. */
    SMC_SetPowerModeProtection(APP_SMC, kSMC_AllowPowerModeAll);

    PRINTF("\r\nPower Mode Switch demo - dual core, core 0.\r\n");

    /* For core 1.
    * The whole system's lowest power mode depends on the highest power mode of both core.
    * To meet the lowest system power mode, here we would set the other core into the lowest power mode.
    */
    MU_Init(APP_MU);
    PRINTF("Boot up another core, core 1 ...\r\n");
    /* Wake up another core, core 1. */
    MU_BootCoreB(APP_MU, APP_CORE1_BOOT_MODE);

    /* Wait Core 1 is Boot Up */
    while (BOOT_FLAG != MU_GetFlags(APP_MU))
    {
    }

    PRINTF("Core 1 is booted up now.\r\n");

    /* Shake hands with the other core.
    *  The other core would fall into the lowest power mode after send out the ACK.
    */
    MU_SendMsg(APP_MU, APP_MU_XFER_CMD_CHANNEL_INDEX, APP_MU_SHAKE_HAND_VALUE);
    if (APP_MU_SHAKE_HAND_VALUE == MU_ReceiveMsg(APP_MU, APP_MU_XFER_CMD_CHANNEL_INDEX))
    {
        PRINTF("Shake hands done.\r\n");
    }
    else
    {
        PRINTF("Bad response from the other core.\r\n");
    }

    while (1)
    {
        curPowerState = SMC_GetPowerModeState(APP_SMC);
        APP_ShowPowerMode(curPowerState);
        do
        {
            PRINTF("--------------------------------\r\n");

            PRINTF(" - %c: kAPP_PowerModeRun\r\n", kAPP_PowerModeRun);
            PRINTF(" - %c: kAPP_PowerModeWait\r\n", kAPP_PowerModeWait);
            PRINTF(" - %c: kAPP_PowerModeStop\r\n", kAPP_PowerModeStop);
            PRINTF(" - %c: kAPP_PowerModeVlpr\r\n", kAPP_PowerModeVlpr);
            PRINTF(" - %c: kAPP_PowerModeVlpw\r\n", kAPP_PowerModeVlpw);
            PRINTF(" - %c: kAPP_PowerModeVlps\r\n", kAPP_PowerModeVlps);
            PRINTF(" - %c: kAPP_PowerModeLls\r\n", kAPP_PowerModeLls);
            PRINTF(" - %c: kAPP_PowerModeVlls2\r\n", kAPP_PowerModeVlls2);
            PRINTF(" - %c: kAPP_PowerModeVlls0\r\n", kAPP_PowerModeVlls0);
            PRINTF(" - %c: kAPP_PowerModeHsrun\r\n", kAPP_PowerModeHsrun);

            PRINTF("Input %c to %c: \r\n", kAPP_PowerModeMin + 1U, kAPP_PowerModeMax - 1U);

            power_mode_cmd = GETCHAR();
            PRINTF("%c\r\n", power_mode_cmd);
        } while ((power_mode_cmd <= kAPP_PowerModeMin) || (power_mode_cmd >= kAPP_PowerModeMax));
        targetPowerMode = (app_power_mode_t)power_mode_cmd;

        /* Check if the power mode switch is not available or not necessary. */
        if (!APP_CheckPowerModeCanTransfer(curPowerState, targetPowerMode))
        {
            continue;
        }

        PRINTF("Available transfer.\r\n");

        /* Check if a wakeup source is needed to return the run mode. */
        if (APP_CheckPowerModeNeedWakeup(targetPowerMode))
        {
            PRINTF("Prepare the wakeup source...\r\n");

            /* Setup LLWU if necessary. */
            if ((kAPP_PowerModeWait != targetPowerMode) && (kAPP_PowerModeVlpw != targetPowerMode) &&
                (kAPP_PowerModeVlps != targetPowerMode) && (kAPP_PowerModeStop != targetPowerMode))
            {
                PRINTF("LLWU_EnableInternalModuleInterruptWakup()\r\n");
                LLWU_EnableInternalModuleInterruptWakup(APP_LLWU, APP_LLWU_LPTMR_IDX, true);
            } /* Else with NVIC or AWIC as default. */

            /* Setup the LPTMR. */
            g_WakeupTimeoutSecond = APP_GetWakeupTimeout();
            LPTMR_SetTimerPeriod(APP_LPTMR, (1000U * g_WakeupTimeoutSecond) - 1U);
            LPTMR_StartTimer(APP_LPTMR);
        }

        if (kAPP_PowerModeVlls0 == targetPowerMode)
        {
            PRINTF("Reset to wakeup the chip.\r\n");
        }

        /* Switch the power mode. */
        APP_PowerModeSwitch(curPowerState, targetPowerMode);
    }
}

/*
 * Check whether could switch to target power mode from current mode.
 * Return true if could switch, return false if could not switch.
 */
static bool APP_CheckPowerModeCanTransfer(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode)
{
    bool modeValid = true;

    /*
     * Check wether the mode change is allowed.
     *
     * 1. If current mode is HSRUN mode, the target mode must be RUN mode.
     * 2. If current mode is RUN mode, the target mode must not be VLPW mode.
     * 3. If current mode is VLPR mode, the target mode must not be HSRUN/WAIT/STOP mode.
     * 4. If already in the target mode.
     */
    switch (curPowerState)
    {
        case kSMC_PowerStateHsrun:
            if (kAPP_PowerModeRun != targetPowerMode)
            {
                PRINTF("Current mode is HSRUN, please choose RUN mode as target mode.\r\n");
                modeValid = false;
            }
            break;

        case kSMC_PowerStateRun:
            if (kAPP_PowerModeVlpw == targetPowerMode)
            {
                PRINTF("Could not enter VLPW mode from RUN mode.\r\n");
                modeValid = false;
            }
            break;

        case kSMC_PowerStateVlpr:
            if ((kAPP_PowerModeWait == targetPowerMode) || (kAPP_PowerModeHsrun == targetPowerMode) ||
                (kAPP_PowerModeStop == targetPowerMode))
            {
                PRINTF("Could not enter HSRUN/STOP/WAIT modes from VLPR mode.\r\n");
                modeValid = false;
            }
            break;
        default:
            PRINTF("Wrong power state.\r\n");
            modeValid = false;
            break;
    }

    if (!modeValid)
    {
        return false;
    }

    /* Don't need to change power mode if current mode is already the target mode. */
    if (((kAPP_PowerModeRun == targetPowerMode) && (kSMC_PowerStateRun == curPowerState)) ||
        ((kAPP_PowerModeHsrun == targetPowerMode) && (kSMC_PowerStateHsrun == curPowerState)) ||
        ((kAPP_PowerModeVlpr == targetPowerMode) && (kSMC_PowerStateVlpr == curPowerState)))
    {
        PRINTF("Already in the target power mode.\r\n");
        return false;
    }

    return true;
}

static bool APP_CheckPowerModeNeedWakeup(app_power_mode_t targetPowerMode)
{
    bool needSetWakeup;

    /* If target mode is RUN/VLPR/HSRUN, don't need to set wakeup source. */
    if ((kAPP_PowerModeRun == targetPowerMode) || (kAPP_PowerModeHsrun == targetPowerMode) ||
        (kAPP_PowerModeVlpr == targetPowerMode))
    {
        needSetWakeup = false;
    }
    /* If target mode is the lowest power mode (kAPP_PowerModeVlls0), LPTMR could not wakeup it. */
    else if (kAPP_PowerModeVlls0 == targetPowerMode)
    {
        needSetWakeup = false;
    }
    else
    {
        needSetWakeup = true;
    }

    return needSetWakeup;
}

static void APP_ShowPowerMode(smc_power_state_t powerMode)
{
    PRINTF("\r\n**************************************************************************\r\n");
    switch (powerMode)
    {
        case kSMC_PowerStateRun:
            PRINTF("    Power mode: RUN\r\n");
            break;
        case kSMC_PowerStateVlpr:
            PRINTF("    Power mode: VLPR\r\n");
            break;
        case kSMC_PowerStateHsrun:
            PRINTF("    Power mode: HSRUN\r\n");
            break;
        default:
            PRINTF("    Power mode wrong\r\n");
            break;
    }
}

static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds...\r\n");
        PRINTF("The allowed range is 1s ~ 9s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = GETCHAR();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        PRINTF("Wrong value!\r\n");
    }
}

static void LPTMR_Configuration(void)
{
    lptmr_config_t lptmrConfig;

    /* Setup LPTMR. */
    LPTMR_GetDefaultConfig(&lptmrConfig); /* LPO as clock source. */
    LPTMR_Init(APP_LPTMR, &lptmrConfig);

    /* Enable the interrupt. */
    LPTMR_EnableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable);
    INTMUX_EnableInterrupt(INTMUX0, 0, APP_LPTMR_IRQn);

    /* For RISCV, the event enable bit in event unit should be set for wakeup. */
    EVENT_UNIT->EVENTEN |= (1 << INTMUX0_0_IRQn);
}

void APP_LPTMR_IRQHandler(void)
{
    LPTMR_ClearStatusFlags(APP_LPTMR, kLPTMR_TimerCompareFlag);
    /* Clear the event pending flag */
    EVENT_UNIT->EVTPENDCLEAR = (1 << INTMUX0_0_IRQn);
    LPTMR_StopTimer(APP_LPTMR);
}

/* Even with the defination, this ISR would be executed only when wakup from LLS/VLLSx mode. */
void APP_LLWU_IRQHandler(void)
{
    /* LPTMR_DisableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable); */
    LPTMR_ClearStatusFlags(APP_LPTMR, kLPTMR_TimerCompareFlag);
    LPTMR_StopTimer(APP_LPTMR);
}

/*
 * Power mode switch.
 */
void APP_PowerModeSwitch(smc_power_state_t curPowerState, app_power_mode_t targetPowerMode)
{
    switch (targetPowerMode)
    {
        case kAPP_PowerModeRun:

            /* Power mode change. */
            SMC_SetPowerModeRun(APP_SMC);
            while (kSMC_PowerStateRun != SMC_GetPowerModeState(APP_SMC))
            {
            }
            break;
        case kAPP_PowerModeWait:
            SMC_SetPowerModeWait(APP_SMC);
            break;

        case kAPP_PowerModeStop:
            prepareForLowPower();
            SMC_SetPowerModeStop(APP_SMC, kSMC_PartialStop);
            restoreFromLowPower();
            break;
        case kAPP_PowerModeVlpr:
            SMC_SetPowerModeVlpr(APP_SMC);
            while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(APP_SMC))
            {
            }
            break;
        case kAPP_PowerModeVlpw:
            SMC_SetPowerModeVlpw(APP_SMC);
            break;
        case kAPP_PowerModeVlps:
            prepareForLowPower();
            SMC_SetPowerModeVlps(APP_SMC);
            restoreFromLowPower();
            break;
        case kAPP_PowerModeLls:
            prepareForLowPower();
            SMC_SetPowerModeLls(APP_SMC);
            restoreFromLowPower();
            break;
        case kAPP_PowerModeVlls0:
            prepareForLowPower();
            SMC_SetPowerModeVlls0(APP_SMC);
            restoreFromLowPower();
            break;
        case kAPP_PowerModeVlls2:
            prepareForLowPower();
            SMC_SetPowerModeVlls2(APP_SMC);
            restoreFromLowPower();
            break;
        case kAPP_PowerModeHsrun:
            SMC_SetPowerModeHsrun(APP_SMC);
            while (kSMC_PowerStateHsrun != SMC_GetPowerModeState(APP_SMC))
            {
            }
            break;
        default:
            PRINTF("Wrong value");
            break;
    }
}

void prepareForLowPower(void)
{
    gpio_pin_config_t lpuartTxPinGpioConfig =
    {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1,
    };

    port_pin_config_t lpuartTxPinPortConfig =
    {
        .pullSelect = kPORT_PullUp,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = 0,
        .openDrainEnable = 0,
        .driveStrength = 0,
        .mux = kPORT_MuxAsGpio,
        .lockRegister = 0,
    };
  /* Need to disable the JTAG pins, NMI pin, and LPUART pins for lowest power.
     Enable clock gates to PORT A and PORT C first */
  CLOCK_EnableClock(kCLOCK_PortC);
  CLOCK_EnableClock(kCLOCK_PortA);

  /* Disable JTAG pins and NMI pin */
  PORT_SetPinMux(PORTA, 0, kPORT_PinDisabledOrAnalog);
  PORT_SetPinMux(PORTA, 1, kPORT_PinDisabledOrAnalog);
  PORT_SetPinMux(PORTA, 2, kPORT_PinDisabledOrAnalog);
  PORT_SetPinMux(PORTA, 3, kPORT_PinDisabledOrAnalog);
  PORT_SetPinMux(PORTA, 4, kPORT_PinDisabledOrAnalog);

  /* Disable LPUART pins
   * When entering stop modes, make sure the debug console output finished.
   * Pull the TX pin because floating state results to current leakage.
   */
  BOARD_WaitDebugConsoleOutputComplete();
  PORT_SetPinConfig(PORTC, 8, &lpuartTxPinPortConfig);
  GPIO_PinInit(GPIOC, 8, &lpuartTxPinGpioConfig);
}

void restoreFromLowPower(void)
{
  /* Need to re-enable the JTAG pins, NMI pin, and LPUART pins after waking up.
     Enable clock gates to PORT A and PORT C first */
  CLOCK_EnableClock(kCLOCK_PortC);
  CLOCK_EnableClock(kCLOCK_PortA);

  /* Re-enable JTAG pins and NMI pin */
  PORT_SetPinMux(PORTA, 0, kPORT_MuxAlt7);
  PORT_SetPinMux(PORTA, 1, kPORT_MuxAlt7);
  PORT_SetPinMux(PORTA, 2, kPORT_MuxAlt7);
  PORT_SetPinMux(PORTA, 3, kPORT_MuxAlt7);
  PORT_SetPinMux(PORTA, 4, kPORT_MuxAlt7);

  /* Re-enable LPUART pins */
  PORT_SetPinMux(PORTC, 7, kPORT_MuxAlt3);
  PORT_SetPinMux(PORTC, 8, kPORT_MuxAlt3);
}

void turn_off_spi_flash(void)
{
  uint32_t i;
  gpio_pin_config_t spiFlashCsPin = {
    .pinDirection = kGPIO_DigitalOutput,
    .outputLogic = 1
  };

  // Enable Port B PCC
  CLOCK_EnableClock(kCLOCK_PortB);

  // enable SPI pins first
  PORT_SetPinMux(BOARD_SPI_FLASH_SCLK_PORT, BOARD_SPI_FLASH_SCLK_GPIO_PIN, kPORT_MuxAlt2);
  PORT_SetPinMux(BOARD_SPI_FLASH_SI_PORT, BOARD_SPI_FLASH_SI_GPIO_PIN, kPORT_MuxAlt2);
  PORT_SetPinMux(BOARD_SPI_FLASH_CS_PORT, BOARD_SPI_FLASH_CS_GPIO_PIN, kPORT_MuxAlt2);
  PORT_SetPinMux(BOARD_SPI_FLASH_SO_PORT, BOARD_SPI_FLASH_SO_GPIO_PIN, kPORT_MuxAlt2);

  // Need to toggle CS low and then high to ensure the SPI is NOT in DP mode.
  // This is to prevent the SPI flash from being left in an unknown state before
  // trying to put it to sleep.

  GPIO_PinInit(BOARD_SPI_FLASH_CS_GPIO, BOARD_SPI_FLASH_CS_GPIO_PIN, &spiFlashCsPin);

  PORT_SetPinMux(BOARD_SPI_FLASH_CS_PORT, BOARD_SPI_FLASH_CS_GPIO_PIN, kPORT_MuxAsGpio);

  for(i=0; i<50; i++);
  GPIO_ClearPinsOutput(BOARD_SPI_FLASH_CS_GPIO, (1 << BOARD_SPI_FLASH_CS_GPIO_PIN));

  // Need to hold low for at least 20ns
  for(i=0; i< 1000; i++);

  GPIO_SetPinsOutput(BOARD_SPI_FLASH_CS_GPIO, (1 << BOARD_SPI_FLASH_CS_GPIO_PIN));
  //GPIOB->PSOR = (1 << 22);

  // Need to hold high for at least 35us
  for(i=0; i<10000; i++);

  // Return to SPI function
  PORT_SetPinMux(BOARD_SPI_FLASH_CS_PORT, BOARD_SPI_FLASH_CS_GPIO_PIN, kPORT_MuxAlt2);

  // Clock LPSPI1 from the FIRC and Enable the Clock gate
  CLOCK_SetIpSrc(kCLOCK_Lpspi1, kCLOCK_IpSrcFircAsync);
  CLOCK_EnableClock(kCLOCK_Lpspi1);

  // configure LPSP1
  LPSPI1->CR |= LPSPI_CR_MEN_MASK; // enable LPSPI1 as it is the SPI flash SPI port

  LPSPI1->CFGR1 |= LPSPI_CFGR1_MASTER_MASK;

  LPSPI1->TCR = LPSPI_TCR_PCS(0x2) | LPSPI_TCR_FRAMESZ(0x7); // 8 bit transfer

  LPSPI1->CCR = 0x5F5F5E5E; // taken from FAT test project

  LPSPI1->TDR = 0xB9; // command to enter low power mode for Macronix SPI flash

  for(i=0; i<15000; i++);

  LPSPI1->CR &= ~LPSPI_CR_MEN_MASK;

  PORTB->PCR[24] = 0;

  // Because of the SPI Flash, need to set some PortA pins.
  CLOCK_EnableClock(kCLOCK_PortA);

  LED1_INIT(1);
  LED2_INIT(1);
  LED3_INIT(1);

  for(i=0; i<100000; i++);

  LED1_OFF();
  LED2_OFF();
  LED3_OFF();
}
