/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "UrtLib.h"
#include "DioLib.h"
#include "core_cm3.h"

#include "cmsis_device.h"

// ----------------------------------------------------------------------------

#define TIMER_FREQUENCY_HZ (1000u)

typedef uint32_t timer_ticks_t;

extern volatile timer_ticks_t timer_delayCount;
// ----------------------------------------------------------------------------
//
// Standalone $(shortChipFamily) empty sample (trace via NONE).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

void
timer_tick (void);

// ----------------------------------------------------------------------------

volatile timer_ticks_t timer_delayCount;

// ----------------------------------------------------------------------------

void
timer_start (void)
{
  // Use SysTick as reference for the delay loops.
  SysTick_Config (SystemCoreClock / TIMER_FREQUENCY_HZ);
}

void
timer_sleep (timer_ticks_t ticks)
{
  timer_delayCount = ticks;

  // Busy wait until the SysTick decrements the counter to zero.
  while (timer_delayCount != 0u)
    ;
}

void
timer_tick (void)
{
  // Decrement to zero the counter used by the delay routine.
	if (timer_delayCount != 0u)
	{
		--timer_delayCount;
	}
	else
	{
		UrtTx(pADI_UART, '\n');
		timer_delayCount = TIMER_FREQUENCY_HZ;
	}
}

// ----- SysTick_Handler() ----------------------------------------------------

void
SysTick_Handler (void)
{
  timer_tick ();
}

// ----- main() ---------------------------------------------------------------

int
main()
{
	  DioCfg(pADI_GP0, 0x003C);
	  UrtCfg(pADI_UART, B9600, 3, 0);
	  timer_start ();

  // Infinite loop
  while (1)
    {
	  while (0 ==(pADI_UART->COMLSR&1)) {}
	  char ch = UrtRx(pADI_UART);
	  if (ch == 0x40)
	  {
		  // Command switch to boot
		  NVIC_SystemReset();
	  }
	  UrtTx(pADI_UART, ch);
    }
}


// ----------------------------------------------------------------------------
