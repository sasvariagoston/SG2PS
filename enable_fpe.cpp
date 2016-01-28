// Copyright (C) 2012-2016, Ali Baharev
// All rights reserved.
// This code is published under the GNU Lesser General Public License.

#ifdef ENABLE_FPE

#if __linux__

#include <fenv.h>
void enable_fpe() {
    feenableexcept(FE_ALL_EXCEPT & ~FE_INEXACT);
}

#elif _WIN32

#include <fenv.h>
static void feenableexcept(uint16_t fpflags){
   uint16_t mask(FE_ALL_EXCEPT & ~fpflags);
   asm(
   "fldcw %0"
   :
   : "m" (mask)
   : "cc");
}

void enable_fpe() {
    feenableexcept(FE_ALL_EXCEPT & ~FE_INEXACT);
}

/*
#undef __STRICT_ANSI__
#include <stdio.h>
#include <float.h>
#pragma fenv_access (on)
static void enable_fpe() {
    unsigned int cw = _control87(0,0) & MCW_EM;
    cw &= ~(_EM_INVALID|_EM_ZERODIVIDE|_EM_OVERFLOW|_EM_UNDERFLOW);
    _control87(cw,MCW_EM);
}*/

#else

#error "Your platform is not supported when floating-point exceptions are enabled"

#endif
#else

void enable_fpe() { }

#endif
