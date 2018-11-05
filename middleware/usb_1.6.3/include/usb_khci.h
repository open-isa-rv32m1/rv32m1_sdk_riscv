/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_KHCI_H__
#define __USB_KHCI_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define USB_KHCI_BDT_DEVICE_OUT_TOKEN (0x01U)
#define USB_KHCI_BDT_DEVICE_IN_TOKEN (0x09U)
#define USB_KHCI_BDT_DEVICE_SETUP_TOKEN (0x0DU)

#define USB_KHCI_BDT_OWN (0x80U)
#define USB_KHCI_BDT_DATA01(x) ((((uint32_t)(x)) & 0x01U) << 0x06U)
#define USB_KHCI_BDT_BC(x) ((((uint32_t)(x)) & 0x3FFU) << 0x10U)
#define UBS_KHCI_BDT_KEEP (0x20U)
#define UBS_KHCI_BDT_NINC (0x10U)
#define USB_KHCI_BDT_DTS (0x08U)
#define USB_KHCI_BDT_STALL (0x04U)

typedef enum _usb_khci_interrupt_type
{
    kUSB_KhciInterruptReset = 0x01U,
    kUSB_KhciInterruptError = 0x02U,
    kUSB_KhciInterruptSofToken = 0x04U,
    kUSB_KhciInterruptTokenDone = 0x08U,
    kUSB_KhciInterruptSleep = 0x10U,
    kUSB_KhciInterruptResume = 0x20U,
    kUSB_KhciInterruptAttach = 0x40U,
    kUSB_KhciInterruptStall = 0x80U,
} usb_khci_interrupt_type_t;

#endif /* __USB_KHCI_H__ */
