/*
 * Copyright 2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_common.h"

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

#define PIN7_IDX        7u      /*!< Pin number for pin 7 in a port*/
#define PIN8_IDX        8u      /*!< Pin number for pin 8 in a port*/
#define PIN24_IDX       24u     /*!< Pin number for pin 24 in a port*/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: N2, peripheral: LPUART0, signal: RX, pin_signal:
LPCMP0_IN0/PTC7/LLWU_P15/LPSPI0_PCS3/LPUART0_RX/LPI2C1_HREQ/TPM0_CH0/LPTMR1_ALT1}
  - {pin_num: P3, peripheral: LPUART0, signal: TX, pin_signal:
LPCMP0_IN1/PTC8/LPSPI0_SCK/LPUART0_TX/LPI2C0_HREQ/TPM0_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_PortC); /* Clock Gate Control: 0x01u */

    PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt3); /* PORTC7 (pin N2) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAlt3); /* PORTC8 (pin P3) is configured as LPUART0_TX */

    CLOCK_EnableClock(kCLOCK_PortA);
    PORT_SetPinMux(PORTA, PIN24_IDX, kPORT_MuxAsGpio);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/


