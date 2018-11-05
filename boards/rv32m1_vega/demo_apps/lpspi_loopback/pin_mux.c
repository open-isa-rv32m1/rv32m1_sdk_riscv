/*
 * Copyright 2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: RV32M1
package_id: RV32M1
mcu_data: ksdk2_0
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/*FUNCTION**********************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 *END**************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN27_IDX                       27u   /*!< Pin number for pin 27 in a port */
#define PIN30_IDX                       30u   /*!< Pin number for pin 30 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm4, enableClock: 'true'}
- pin_list:
  - {pin_num: R1, peripheral: LPSPI0, signal: OUT, pin_signal: ADC0_SE4/LPCMP0_IN2/PTC9/LLWU_P16/LPSPI0_SOUT/LPUART0_CTS_b/LPI2C0_SDA/TPM0_CH2/LPTMR0_ALT2}
  - {pin_num: T1, peripheral: LPSPI0, signal: IN, pin_signal: ADC0_SE6/PTC11/LLWU_P17/LPSPI0_SIN/LPI2C1_SDA/LPI2C0_SDAS/TPM0_CH4/EWM_IN}
  - {pin_num: R3, peripheral: LPSPI0, signal: PCS0, pin_signal: ADC0_SE7/PTC12/LLWU_P18/LPSPI0_PCS0/LPI2C1_SCL/LPI2C0_SCLS/TPM0_CH5/EWM_OUT_b}
  - {pin_num: R7, peripheral: LPSPI0, signal: SCK, pin_signal: PTC30/LPUART1_TX/LPSPI0_SCK/TPM0_CH1/FXIO0_D19}
  - {pin_num: N2, peripheral: LPUART0, signal: RX, pin_signal: LPCMP0_IN0/PTC7/LLWU_P15/LPSPI0_PCS3/LPUART0_RX/LPI2C1_HREQ/TPM0_CH0/LPTMR1_ALT1}
  - {pin_num: P3, peripheral: LPUART0, signal: TX, pin_signal: LPCMP0_IN1/PTC8/LPSPI0_SCK/LPUART0_TX/LPI2C0_HREQ/TPM0_CH1}
  - {pin_num: A3, peripheral: LPSPI3, signal: IN, pin_signal: PTA27/LPUART1_CTS_b/LPSPI3_SIN/FB_AD29}
  - {pin_num: M11, peripheral: LPSPI3, signal: OUT, pin_signal: ADC0_SE21/LPCMP1_IN1/PTE4/SDHC0_D6/LPI2C0_SCL/LPSPI3_SOUT/CLKOUT/TPM1_CLKIN/RF0_DTM_RX}
  - {pin_num: N16, peripheral: LPSPI3, signal: PCS0, pin_signal: ADC0_SE23/PTE9/LLWU_P24/SDHC0_CMD/LPUART3_TX/LPSPI3_PCS0/TPM1_CH1/FXIO0_D0}
  - {pin_num: N12, peripheral: LPSPI3, signal: SCK, pin_signal: ADC0_SE20/LPCMP1_IN0/PTE3/LLWU_P22/SDHC0_D7/LPI2C0_SDA/LPSPI3_SCK/TPM0_CLKIN/LPTMR0_ALT3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Clock Gate Control: 0x01u */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Clock Gate Control: 0x01u */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTA, PIN27_IDX, kPORT_MuxAlt3);           /* PORTA27 (pin A3) is configured as LPSPI3_SIN */
  PORT_SetPinMux(PORTC, PIN11_IDX, kPORT_MuxAlt2);           /* PORTC11 (pin T1) is configured as LPSPI0_SIN */
  PORT_SetPinMux(PORTC, PIN12_IDX, kPORT_MuxAlt2);           /* PORTC12 (pin R3) is configured as LPSPI0_PCS0 */
  PORT_SetPinMux(PORTC, PIN30_IDX, kPORT_MuxAlt3);           /* PORTC30 (pin R7) is configured as LPSPI0_SCK */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt3);            /* PORTC7 (pin N2) is configured as LPUART0_RX */
  PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAlt3);            /* PORTC8 (pin P3) is configured as LPUART0_TX */
  PORT_SetPinMux(PORTC, PIN9_IDX, kPORT_MuxAlt2);            /* PORTC9 (pin R1) is configured as LPSPI0_SOUT */
  PORT_SetPinMux(PORTE, PIN3_IDX, kPORT_MuxAlt4);            /* PORTE3 (pin N12) is configured as LPSPI3_SCK */
  PORT_SetPinMux(PORTE, PIN4_IDX, kPORT_MuxAlt4);            /* PORTE4 (pin M11) is configured as LPSPI3_SOUT */
  PORT_SetPinMux(PORTE, PIN9_IDX, kPORT_MuxAlt4);            /* PORTE9 (pin N16) is configured as LPSPI3_PCS0 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
