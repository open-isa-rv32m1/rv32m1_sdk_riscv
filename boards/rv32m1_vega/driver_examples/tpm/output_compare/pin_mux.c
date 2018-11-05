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

#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */
#define PIN26_IDX                       26u   /*!< Pin number for pin 26 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm4, enableClock: 'true'}
- pin_list:
  - {pin_num: N2, peripheral: LPUART0, signal: RX, pin_signal: LPCMP0_IN0/PTC7/LLWU_P15/LPSPI0_PCS3/LPUART0_RX/LPI2C1_HREQ/TPM0_CH0/LPTMR1_ALT1}
  - {pin_num: P3, peripheral: LPUART0, signal: TX, pin_signal: LPCMP0_IN1/PTC8/LPSPI0_SCK/LPUART0_TX/LPI2C0_HREQ/TPM0_CH1}
  - {pin_num: B5, peripheral: LPUART1, signal: RX, pin_signal: PTA25/LPUART1_RX/LPSPI3_SOUT/LPI2C2_SDAS/FB_AD31}
  - {pin_num: A5, peripheral: LPUART1, signal: TX, pin_signal: PTA26/LPUART1_TX/LPSPI3_PCS2/LPI2C2_SCLS/FB_AD30}
  - {pin_num: B1, peripheral: TPM0, signal: 'CH, 0', pin_signal: PTB2/LLWU_P5/RF0_RFOSC_EN/LPSPI0_PCS1/LPUART1_RX/SAI0_TX_D0/FB_AD11/TPM0_CH0}
  - {pin_num: G3, peripheral: TPM3, signal: 'CH, 0', pin_signal: PTB13/LPUART2_CTS_b/LPI2C1_SDA/LPI2C0_SDAS/FB_AD25/TPM3_CH0/FXIO0_D3}
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
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Clock Gate Control: 0x01u */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTA, PIN25_IDX, kPORT_MuxAlt2);           /* PORTA25 (pin B5) is configured as LPUART1_RX */
  PORT_SetPinMux(PORTA, PIN26_IDX, kPORT_MuxAlt2);           /* PORTA26 (pin A5) is configured as LPUART1_TX */
  PORT_SetPinMux(PORTB, PIN13_IDX, kPORT_MuxAlt6);           /* PORTB13 (pin G3) is configured as TPM3_CH0 */
  PORT_SetPinMux(PORTB, PIN2_IDX, kPORT_MuxAlt6);            /* PORTB2 (pin B1) is configured as TPM0_CH0 */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt3);            /* PORTC7 (pin N2) is configured as LPUART0_RX */
  PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAlt3);            /* PORTC8 (pin P3) is configured as LPUART0_TX */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
