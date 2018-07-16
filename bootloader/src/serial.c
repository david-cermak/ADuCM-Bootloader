#include "serial.h"
#include "UrtLib.h"
#include "DioLib.h"
#include "string.h"

//#define ADUCINO_UART
//#define DISABLE_RS485_SENTINELS

#ifdef __ADUCM360__
# define MY_UART    pADI_UART
#else
# define MY_UART    pADI_UART1
#endif /* __ADUCM360__ */

#ifdef ADUCINO_UART
#define SET_DIRECTION_TX()
#define SET_DIRECTION_RX()
#else
#define SET_DIRECTION_TX() DioWr(pADI_GP0, 1<<PIN5)
#define SET_DIRECTION_RX() DioWr(pADI_GP0, 0)
#endif



void SerialInit()
{
	pADI_CLKCTL->CLKDIS &= ~0x08;

#ifndef __ADUCM360__
	pADI_CLKCTL->CLKCON2 &= ~ ( 0x01 | CLKCON2_UART1CD_MSK);
#endif

	//
	// Init IO & Uart -- needed peripherals, but no interrupts
#ifdef ADUCINO_UART
	DioCfg(pADI_GP0, 0x003C); // P0.1 & P0.2
#else
	DioCfg(pADI_GP0, 0x8000 | 0x1000); // P0.6 & P0.7
	DioOen(pADI_GP0, 1<<PIN5);
	SET_DIRECTION_RX();
#endif
//	UrtCfg(MY_UART, B9600, 3, 0);
	UrtCfg(MY_UART, B115200, 3, 0);

}

char* sentinels = "bl_reps";

void SendSentinels()
{
#ifndef DISABLE_RS485_SENTINELS
	uint32_t i;
	for (i = 0; i< strlen(sentinels); i++) {
		UrtTx(MY_UART, sentinels[i]);
		while (0 == (MY_UART->COMLSR&0x40)) {}
	}
#endif
}

void TinyDelayBeforeDirectionSwitch()
{
	volatile int cnt = 0x500;
	while (cnt) {
		cnt--;
	}
}


void SendChar(char ch)
{
#ifndef ADUCINO_UART
	TinyDelayBeforeDirectionSwitch();
	SET_DIRECTION_TX();
#endif
	SendSentinels();
	UrtTx(MY_UART, ch);
	while (0 == (MY_UART->COMLSR&0x40)) {}
	SET_DIRECTION_RX();
}

void SendString(char* string)
{
	uint32_t i;
	SET_DIRECTION_TX();
	SendSentinels();

	for (i = 0; i< strlen(string); i++) {
		UrtTx(MY_UART, string[i]);
		while (0 == (MY_UART->COMLSR&0x20)) {}
	}
	SET_DIRECTION_RX();
}
