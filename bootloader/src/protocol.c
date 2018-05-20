#include "UrtLib.h"
#include "flash.h"
#include "app.h"
#include "protocol.h"

extern uint32_t g_prg_data[];

void start()
{
	int cont = 1;
	int i = 0;
	uint8_t *data = (uint8_t*)g_prg_data;
	while (cont) {
	  // Start the protocol
	  // TODO: Add timeout to restart protocol (if communication interrupted)
	  while (0 ==(pADI_UART->COMLSR&1)) {}
	  if (0x07 == UrtRx(pADI_UART)) {
		  while (0 ==(pADI_UART->COMLSR&1)) {}
		  if (0x0E == UrtRx(pADI_UART)) {
			  while (0 ==(pADI_UART->COMLSR&1)) {}
			  int bytes = UrtRx(pADI_UART);
			  if (bytes >= 5) {
				  int cs = bytes;
				  while (0 ==(pADI_UART->COMLSR&1)) {}
				  int command = UrtRx(pADI_UART);
				  cs += command;
				  for (i=0; i < bytes-1; i++) {
					  while (0 ==(pADI_UART->COMLSR&1)) {}
					  data[i] = UrtRx(pADI_UART);
					  cs += data[i];
				  }
				  while (0 ==(pADI_UART->COMLSR&1)) {}
				  int received_cs = UrtRx(pADI_UART);
				  if ((0xFF&(cs+received_cs)) == 0)
				  {
					  // Received valid
					  cont = process_command(bytes, command, data);
					  UrtTx(pADI_UART, 0x06);
				  }
				  else
				  {
					  // NAK
					  UrtTx(pADI_UART, 0x07);
				  }
			  }
		  }
	  }
	}

}

int receive(char* ch, int timeout)
{
	uint32_t cnt = timeout;
	while (0 ==(pADI_UART->COMLSR&1) && cnt) {
		cnt--;
	}
	if (0 == cnt) {
		return 0;
	}
	*ch = UrtRx(pADI_UART);
	return 1;
}

int process_command(int bytes, int command, uint8_t* data)
{
	int cont = 0;
	int address = 0;
	int pages = 0;
	switch (command){
	case 0x45:
		address = data[3] + (data[2]<<8) + (data[1]<<16);
		pages = data[4];
		erase_pages(address, pages);
		cont = 1; // page erase will continue
		break;
	case 0x57:
		address = data[3] + (data[2]<<8) + (data[1]<<16);
		write_to_flash(address, (bytes-5));
		cont = 1; // page erase will continue
		break;
	case 0x52:
		start_application();
		break;
	}

	return cont;

}
