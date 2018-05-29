#include "serial.h"
#include "UrtLib.h"
#include "DioLib.h"
#include "string.h"

#define ADUCINO_UART

#ifdef ADUCINO_UART
#define SET_DIRECTION_TX()
#define SET_DIRECTION_RX()
#else
#define SET_DIRECTION_TX() DioWr(pADI_GP0, PIN5)
#define SET_DIRECTION_RX() DioWr(pADI_GP0, 0)
#endif



void SerialInit()
{
	//
	// Init IO & Uart -- needed peripherals, but no interrupts
#ifdef ADUCINO_UART
	DioCfg(pADI_GP0, 0x003C); // P0.1 & P0.2
#else
	DioCfg(pADI_GP0, 0x8000 | 0x1000); // P0.6 & P0.7
	DioOen(pADI_GP0, PIN5);
#endif
	UrtCfg(pADI_UART, B9600, 3, 0);

}

void SendChar(char ch)
{
	SET_DIRECTION_TX();
	UrtTx(pADI_UART, ch);
	while (0 == (pADI_UART->COMLSR&0x20)) {}
	SET_DIRECTION_RX();
}

void SendString(char* string)
{
	uint32_t i;
	SET_DIRECTION_TX();
	for (i = 0; i< strlen(string); i++) {
		UrtTx(pADI_UART, string[i]);
		while (0 == (pADI_UART->COMLSR&0x20)) {}
	}
	SET_DIRECTION_RX();
}
