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
#include "UrtLib.h"

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
	pADI_GP0->GPCON = (pADI_GP0->GPCON & ~GP0CON_CON1_MSK) | GP0CON_CON1_UARTRXD;
	pADI_GP0->GPCON = (pADI_GP0->GPCON & ~GP0CON_CON2_MSK) | GP0CON_CON2_UARTTXD;
//	pADI_CLKCTL->CLKCON1 = (pADI_CLKCTL->CLKCON1 & ~CLKCON1_UARTCD_MSK) | CLKCON1_UARTCD_DIV1;
//	pADI_UART->COMCON = COMCON_DISABLE_DIS;
//
//	pADI_UART->COMLCR = COMLCR_BRK_DIS | COMLCR_SP_DIS | COMLCR_EPS_DIS | COMLCR_PEN_DIS | COMLCR_STOP_EN
//	                    | COMLCR_WLS_8BITS;
//
//	pADI_UART->COMMCR = COMMCR_LOOPBACK_DIS | COMMCR_OUT1_DIS | COMMCR_OUT2_DIS | COMMCR_RTS_DIS | COMMCR_DTR_DIS;
//	pADI_UART->COMDIV = 0;
//
//	pADI_UART->COMFBR = COMFBR_ENABLE_EN | 55;
//
	GP0OEN_OEN2_BBA = 1;
	GP0OEN_OEN1_BBA = 0;
#if BOARD_TEST
	pADI_GP0->GPCON = (pADI_GP0->GPCON & ~GP0CON_CON5_MSK) | GP0CON_CON5_GPIOIRQ1;
	   GP0OCE_OCE5_BBA = 0;
	   GP0OEN_OEN5_BBA = 1;
	   GP0SET_SET5_BBA = 1;
int i = 0;
    while (i<100)
    {
    			i++;
    			if (i>10) i=0;
    		    if (i%2)
    		       {GP0SET_SET5_BBA = 1;}
    		    else
    		       {GP0CLR_CLR5_BBA = 1;}
    }
#endif
//    DISUARTCLK
	SerialInit();
	while (1) {
	  char ch;
#if 0
//	  pADI_UART->COMTX = 0x0;
	  UrtTx(pADI_UART1, 0x55);
		uint32_t cnt = 0x50000;
		while (0 ==(pADI_UART1->COMLSR&1) && cnt) {
			cnt--;
		}
#else
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
#endif
	}
}
