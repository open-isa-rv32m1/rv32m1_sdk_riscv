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

#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN30_IDX                       30u   /*!< Pin number for pin 30 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm4, enableClock: 'true'}
- pin_list:
  - {pin_num: N2, peripheral: LPUART0, signal: RX, pin_signal: LPCMP0_IN0/PTC7/LLWU_P15/LPSPI0_PCS3/LPUART0_RX/LPI2C1_HREQ/TPM0_CH0/LPTMR1_ALT1}
  - {pin_num: P3, peripheral: LPUART0, signal: TX, pin_signal: LPCMP0_IN1/PTC8/LPSPI0_SCK/LPUART0_TX/LPI2C0_HREQ/TPM0_CH1}
  - {pin_num: N17, peripheral: FLEXIO0, signal: 'D, 3', pin_signal: PTE13/SAI0_TX_BCLK/LPI2C3_SCLS/TPM3_CH0/FXIO0_D3}
  - {pin_num: R3, peripheral: LPSPI0, signal: PCS0, pin_signal: ADC0_SE7/PTC12/LLWU_P18/LPSPI0_PCS0/LPI2C1_SCL/LPI2C0_SCLS/TPM0_CH5/EWM_OUT_b}
  - {pin_num: T1, peripheral: LPSPI0, signal: IN, pin_signal: ADC0_SE6/PTC11/LLWU_P17/LPSPI0_SIN/LPI2C1_SDA/LPI2C0_SDAS/TPM0_CH4/EWM_IN}
  - {pin_num: R7, peripheral: LPSPI0, signal: SCK, pin_signal: PTC30/LPUART1_TX/LPSPI0_SCK/TPM0_CH1/FXIO0_D19}
  - {pin_num: J17, peripheral: FLEXIO0, signal: 'D, 10', pin_signal: PTE21/SAI0_TX_D1/USB0_SOF_OUT/TPM2_CH4/FXIO0_D10}
  - {pin_num: K16, peripheral: FLEXIO0, signal: 'D, 9', pin_signal: PTE19/SAI0_MCLK/TPM2_CH3/FXIO0_D9}
  - {pin_num: R1, peripheral: LPSPI0, signal: OUT, pin_signal: ADC0_SE4/LPCMP0_IN2/PTC9/LLWU_P16/LPSPI0_SOUT/LPUART0_CTS_b/LPI2C0_SDA/TPM0_CH2/LPTMR0_ALT2}
  - {pin_num: G1, peripheral: FLEXIO0, signal: 'D, 5', pin_signal: PTB15/LPI2C1_HREQ/LPI2C3_SCL/FB_CS5_b/FB_TSIZ1/FB_BE23_16_b/TPM0_CLKIN/FXIO0_D5}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Clock Gate Control: 0x01u */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Clock Gate Control: 0x01u */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTB, PIN15_IDX, kPORT_MuxAlt7);           /* PORTB15 (pin G1) is configured as FXIO0_D5 */
  PORT_SetPinMux(PORTC, PIN11_IDX, kPORT_MuxAlt2);           /* PORTC11 (pin T1) is configured as LPSPI0_SIN */
  PORT_SetPinMux(PORTC, PIN12_IDX, kPORT_MuxAlt2);           /* PORTC12 (pin R3) is configured as LPSPI0_PCS0 */
  PORT_SetPinMux(PORTC, PIN30_IDX, kPORT_MuxAlt3);           /* PORTC30 (pin R7) is configured as LPSPI0_SCK */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt3);            /* PORTC7 (pin N2) is configured as LPUART0_RX */
  PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAlt3);            /* PORTC8 (pin P3) is configured as LPUART0_TX */
  PORT_SetPinMux(PORTC, PIN9_IDX, kPORT_MuxAlt2);            /* PORTC9 (pin R1) is configured as LPSPI0_SOUT */
  PORT_SetPinMux(PORTE, PIN13_IDX, kPORT_MuxAlt7);           /* PORTE13 (pin N17) is configured as FXIO0_D3 */
  PORT_SetPinMux(PORTE, PIN19_IDX, kPORT_MuxAlt7);           /* PORTE19 (pin K16) is configured as FXIO0_D9 */
  PORT_SetPinMux(PORTE, PIN21_IDX, kPORT_MuxAlt7);           /* PORTE21 (pin J17) is configured as FXIO0_D10 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
