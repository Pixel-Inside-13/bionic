/*-
 * Copyright (c) 2004 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/lib/msun/arm/fenv.c,v 1.1 2004/06/06 10:03:59 das Exp $
 */

#ifndef ANDROID_LEGACY_FENV_INLINES_ARM_H
#define ANDROID_LEGACY_FENV_INLINES_ARM_H

#include <fenv.h>

#if __ANDROID_API__ < __ANDROID_API_L__ && defined(__arm__)

__BEGIN_DECLS

#define FPSCR_RMODE_SHIFT 22

static __inline int fegetenv(fenv_t* __envp) {
  fenv_t _fpscr;
  __asm__ __volatile__("vmrs %0,fpscr" : "=r" (_fpscr));
  *__envp = _fpscr;
  return 0;
}

static __inline int fesetenv(const fenv_t* __envp) {
  fenv_t _fpscr = *__envp;
  __asm__ __volatile__("vmsr fpscr,%0" : :"ri" (_fpscr));
  return 0;
}

static __inline int feclearexcept(int __excepts) {
  fexcept_t __fpscr;
  fegetenv(&__fpscr);
  __fpscr &= ~__excepts;
  fesetenv(&__fpscr);
  return 0;
}

static __inline int fegetexceptflag(fexcept_t* __flagp, int __excepts) {
  fexcept_t __fpscr;
  fegetenv(&__fpscr);
  *__flagp = __fpscr & __excepts;
  return 0;
}

static __inline int fesetexceptflag(const fexcept_t* __flagp, int __excepts) {
  fexcept_t __fpscr;
  fegetenv(&__fpscr);
  __fpscr &= ~__excepts;
  __fpscr |= *__flagp & __excepts;
  fesetenv(&__fpscr);
  return 0;
}

static __inline int feraiseexcept(int __excepts) {
  fexcept_t __ex = __excepts;
  fesetexceptflag(&__ex, __excepts);
  return 0;
}

static __inline int fetestexcept(int __excepts) {
  fexcept_t __fpscr;
  fegetenv(&__fpscr);
  return (__fpscr & __excepts);
}

static __inline int fegetround(void) {
  fenv_t _fpscr;
  fegetenv(&_fpscr);
  return ((_fpscr >> FPSCR_RMODE_SHIFT) & 0x3);
}

static __inline int fesetround(int __round) {
  fenv_t _fpscr;
  fegetenv(&_fpscr);
  _fpscr &= ~(0x3 << FPSCR_RMODE_SHIFT);
  _fpscr |= (__round << FPSCR_RMODE_SHIFT);
  fesetenv(&_fpscr);
  return 0;
}

static __inline int feholdexcept(fenv_t* __envp) {
  fenv_t __env;
  fegetenv(&__env);
  *__envp = __env;
  __env &= ~FE_ALL_EXCEPT;
  fesetenv(&__env);
  return 0;
}

static __inline int feupdateenv(const fenv_t* __envp) {
  fexcept_t __fpscr;
  fegetenv(&__fpscr);
  fesetenv(__envp);
  feraiseexcept(__fpscr & FE_ALL_EXCEPT);
  return 0;
}

static __inline int feenableexcept(int __mask __unused) {
  return -1;
}

static __inline int fedisableexcept(int __mask __unused) {
  return 0;
}

static __inline int fegetexcept(void) {
  return 0;
}

#undef FPSCR_RMODE_SHIFT

__END_DECLS

#endif /* __ANDROID_API__ < __ANDROID_API_L__ && defined(__arm__) */

#endif /* ANDROID_LEGACY_FENV_INLINES_ARM_H */
