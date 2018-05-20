/*
 * Bootloader main -- serial protocol impl
 */

#include "UrtLib.h"
#include "DioLib.h"
#include "flash.h"
#include "app.h"
#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//
// Global data for programming
//
uint32_t g_prg_data[256/4];

//
//
int main()
{
	//
	// Init IO & Uart -- needed peripherals, but no interrupts
	DioCfg(pADI_GP0, 0x003C);
	UrtCfg(pADI_UART, B9600, 3, 0);

	char *id_packet = "ADuCM360   128 XXX \n\r"; // Info packet
	int i = 0;
	int default_timeout = 0x500000;

	while (1) {
	  char ch;

	  // initial check with timeout to start app
	  if (!receive(&ch, default_timeout)) {
		  start_application();
	  } else if (ch == 0x40) { // character to stay in bootloader (a bit longer)
		  default_timeout = 0x5000000;
	  }

	  // waiting for backspace
	  if (ch == 0x08) {
		  // bootstrapping activated -- sending identifier
		  for (i = 0; i< 24; i++) {
			  UrtTx(pADI_UART, id_packet[i]);
			  while (0 == (pADI_UART->COMLSR&0x20)) {}
		  }
		  start();
	  }
	}
}
