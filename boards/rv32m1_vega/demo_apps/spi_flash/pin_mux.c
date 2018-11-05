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
#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm4, enableClock: 'true'}
- pin_list:
  - {pin_num: N2, peripheral: LPUART0, signal: RX, pin_signal: LPCMP0_IN0/PTC7/LLWU_P15/LPSPI0_PCS3/LPUART0_RX/LPI2C1_HREQ/TPM0_CH0/LPTMR1_ALT1}
  - {pin_num: P3, peripheral: LPUART0, signal: TX, pin_signal: LPCMP0_IN1/PTC8/LPSPI0_SCK/LPUART0_TX/LPI2C0_HREQ/TPM0_CH1}
  - {pin_num: L2, peripheral: LPSPI1, signal: IN, pin_signal: PTB24/LPSPI1_SIN/LPUART0_RTS_b/LPI2C2_SCL/FB_AD2/EWM_IN/FXIO0_D13}
  - {pin_num: J1, peripheral: LPSPI1, signal: SCK, pin_signal: PTB20/LLWU_P11/LPSPI1_SCK/LPUART2_CTS_b/FB_CS0_b/TPM1_CH0/FXIO0_D10}
  - {pin_num: J2, peripheral: LPSPI1, signal: OUT, pin_signal: PTB21/LPSPI1_SOUT/LPUART2_RTS_b/LPI2C2_HREQ/FB_AD4/TPM1_CH1/FXIO0_D11}
  - {pin_num: L1, peripheral: GPIOB, signal: 'GPIO, 22', pin_signal: PTB22/LLWU_P12/LPSPI1_PCS2/LPUART0_CTS_b/LPI2C2_SDA/FB_AD3/TPM2_CLKIN/FXIO0_D12}
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

  PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_MuxAlt2);           /* PORTB20 (pin J1) is configured as LPSPI1_SCK */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_MuxAlt2);           /* PORTB21 (pin J2) is configured as LPSPI1_SOUT */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_MuxAsGpio);         /* PORTB22 (pin L1) is configured as PTB22 */
  PORT_SetPinMux(PORTB, PIN24_IDX, kPORT_MuxAlt2);           /* PORTB24 (pin L2) is configured as LPSPI1_SIN */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt3);            /* PORTC7 (pin N2) is configured as LPUART0_RX */
  PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAlt3);            /* PORTC8 (pin P3) is configured as LPUART0_TX */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
