/*
 * Bootloader main -- serial protocol impl
 */

#include "flash.h"
#include "app.h"
#include "protocol.h"
#include "serial.h"

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
	char *id_packet = "ADuCM360   128 XXX \n\r"; // Info packet
	int default_timeout = 0x500000;

	while (1) {
	  char ch;

	  // initial check with timeout to start app
	  if (!receive(&ch, default_timeout)) {
		  ch = '\0';
		  start_application();
	  } else if (ch == 0x40) { // character to stay in bootloader (a bit longer)
		  default_timeout = 0x5000000;
	  }

	  // waiting for backspace
	  if (ch == 0x08) {
		  // bootstrapping activated -- sending identifier
		  SendString(id_packet);
		  start();
	  }
	}
}
