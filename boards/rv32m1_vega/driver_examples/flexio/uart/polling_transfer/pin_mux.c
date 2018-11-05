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

#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm4, enableClock: 'true'}
- pin_list:
  - {pin_num: N8, peripheral: FLEXIO0, signal: 'D, 24', pin_signal: PTD4/SDHC0_D5/LPSPI2_PCS1/EMVSIM0_SRST/FXIO0_D24}
  - {pin_num: N10, peripheral: FLEXIO0, signal: 'D, 25', pin_signal: ADC0_SE8/PTD5/SDHC0_D4/LPSPI2_PCS3/EMVSIM0_VCCEN/FXIO0_D25}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAlt7);            /* PORTD4 (pin N8) is configured as FXIO0_D24 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt7);            /* PORTD5 (pin N10) is configured as FXIO0_D25 */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
