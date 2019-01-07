/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CORE_RISCV32_H__
#define __CORE_RISCV32_H__

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define RISCV32

#if defined ( __GNUC__ )
  #define __ASM            __asm                                      /*!< asm keyword for GNU Compiler */
  #define __INLINE         inline                                     /*!< inline keyword for GNU Compiler */
  #define __STATIC_INLINE  static inline

#else
  #error Unknown compiler
#endif

#if defined ( __GNUC__ )

#define __BKPT(x) __ASM("ebreak")

__attribute__((always_inline)) __STATIC_INLINE void __NOP(void)
{
  __ASM volatile ("nop");
}

__attribute__((always_inline)) __STATIC_INLINE void __DSB(void)
{
  __ASM volatile ("nop");
}

__attribute__((always_inline)) __STATIC_INLINE void __ISB(void)
{
  __ASM volatile ("nop");
}

__attribute__((always_inline)) __STATIC_INLINE void __WFI(void)
{
  __ASM volatile ("wfi");
}

__attribute__((always_inline)) __STATIC_INLINE void __WFE(void)
{
}

__attribute__( ( always_inline ) ) __STATIC_INLINE void __enable_irq(void)
{
  __ASM volatile ("csrsi mstatus, 8");
}

__attribute__( ( always_inline ) ) __STATIC_INLINE void __disable_irq(void)
{
  __ASM volatile ("csrci mstatus, 8");
}

__attribute__((always_inline)) __STATIC_INLINE uint32_t __REV(uint32_t value)
{
  return __builtin_bswap32(value);
}

__attribute__((always_inline)) __STATIC_INLINE uint32_t __REV16(uint32_t value)
{
  return __builtin_bswap16(value);
}

#else
  #error Unknown compiler
#endif

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

#ifdef __cplusplus
}
#endif

#endif /* __CORE_RISCV32_H__ */
