#include "UrtLib.h"
#include "flash.h"
#include "app.h"
#include "protocol.h"

extern uint32_t g_prg_data[];

#define GETC_TIMEOUT(c) if (!receive(&c, proto_timeout)) { \
	break; \
}

void start()
{
	int cont = 1;
	const int proto_timeout = 0x50000;
	int i = 0;
	uint8_t *data = (uint8_t*)g_prg_data;
	while (cont) {
	  char ch;
	  // Start the protocol
	  GETC_TIMEOUT(ch);
	  if (0x07 == ch) {
		  GETC_TIMEOUT(ch);
		  if (0x0E == ch) {
			  GETC_TIMEOUT(ch);
			  int bytes = ch;
			  if (bytes >= 5) {
				  int cs = bytes;
				  GETC_TIMEOUT(ch);
				  int command = ch;
				  cs += command;
				  for (i=0; i < bytes-1; i++) {
					  GETC_TIMEOUT(ch);
					  data[i] = ch;
					  cs += data[i];
				  }
				  GETC_TIMEOUT(ch);
				  int received_cs = ch;
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
