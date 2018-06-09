#include "UrtLib.h"
#include "flash.h"
#include "app.h"
#include "protocol.h"
#include "serial.h"

extern uint32_t g_prg_data[];

#define GETC_TIMEOUT(c) \
	if (!receive(&c, proto_timeout)) { \
		cont = 0; \
		break; \
	}

static unsigned long CRC24_D32(const unsigned long old_CRC, const unsigned long Data);
long int GenerateChecksumCRC24_D32(unsigned long ulNumValues,unsigned long *pulData);
uint32_t g_last_word = 0;

#define NAK 0x07
#define ACK 0x06

//
// Process command outputs -> 3 possible results
//
enum e_process_command
{
	eNone = 0,
	eDontContinue = 1,
	eContinue = 2,
	eCommandFailed = 0xFF
};

void start()
{
	int cont = 1;
	const int proto_timeout = 0x500000;
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
			  uint32_t bytes = (unsigned char)ch;
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
				  if ((0xFF&(cs+received_cs)) == 0) {
					  // Received valid
					  enum e_process_command cmd_out = process_command(bytes, command, data);
					  if (eCommandFailed == cmd_out) {
						  SendChar(NAK);
						  cont = 0;
					  } else  {
						  if (eDontContinue == cmd_out) {
							  cont = 0;
						  }
						  SendChar(ACK);
					  }
				  } else {
					  SendChar(NAK);
				  }
			  }
		  }
	  } else {
		  cont = 0;
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

enum e_process_command process_command(int bytes, int command, uint8_t* data)
{
	enum e_process_command cmd_out = eContinue;
	uint32_t address = 0;
	int pages = 0;
	switch (command){
	case 0x45:
		address = data[3] + (data[2]<<8) + (data[1]<<16);
		pages = data[4];
		erase_pages(address, pages);
		break;
	case 0x57:
		address = data[3] + (data[2]<<8) + (data[1]<<16);
		data[bytes-1] = data[bytes+0] = data[bytes+1] = 0xFF; // make possibly remaining bytes in word un-programmed
		write_to_flash(address, (bytes-5));
		break;
	case 0x56:
		address = data[3] + (data[2]<<8) + (data[1]<<16) +  (data[0]<<24);
		if (address == 0x80000000UL)
		{
			g_last_word = data[4] + (data[5]<<8) + (data[6]<<16) +  (data[7]<<24);
		}
		else if (address < 0x1000)
		{
			// pretend verify OK
			cmd_out = eContinue;
		}
		else
		{
			uint32_t received_crc = data[4] + (data[5]<<8) + (data[6]<<16);
			uint32_t* pulData = (uint32_t*)address;
			uint32_t crc = GenerateChecksumCRC24_D32(127, pulData);
			if ((received_crc != crc)||(g_last_word != *((uint32_t*)(address + 0x1fc)) )) {
				cmd_out = eCommandFailed; // failed CRC
			} else {
				cmd_out = eContinue;
			}

		}
		break;
	case 0x52:
		start_application();
		cmd_out = eDontContinue;
		break;
	}

	return cmd_out;
}

long int GenerateChecksumCRC24_D32(unsigned long ulNumValues,unsigned long *pulData)
{
	unsigned long i,ulData,lfsr = 0xFFFFFF;
	for (i= 0x0; i < ulNumValues;i++)	{
		ulData = pulData[i];
		lfsr = CRC24_D32(lfsr,ulData);
	}
	return lfsr;
}

static unsigned long CRC24_D32(const unsigned long old_CRC, const unsigned long Data)
{
	unsigned long D[32];
	unsigned long C[24];
	unsigned long NewCRC [24];
	unsigned long ulCRC24_D32;
	unsigned long int f;
	unsigned long int bit_mask = 0x000001;
	// Convert previous CRC value to binary.
	bit_mask = 0x000001;
	for (f = 0; f <= 23; f++) {
		C[f] = (old_CRC & bit_mask) >> f;
		bit_mask = bit_mask << 1;
	}
	// Convert data to binary.
	bit_mask = 0x000001;
	for (f = 0; f <= 31; f++) {
		D[f]= (Data & bit_mask) >> f;
		bit_mask = bit_mask << 1;
	}
	// Calculate new LFSR value.
	NewCRC[0] = D[31] ^ D[30] ^ D[29] ^ D[28] ^ D[27] ^ D[26] ^ D[25] ^
		D[24] ^ D[23] ^ D[17] ^ D[16] ^ D[15] ^ D[14] ^ D[13] ^
		D[12] ^ D[11] ^ D[10] ^ D[9] ^ D[8] ^ D[7] ^ D[6] ^
		D[5] ^ D[4] ^ D[3] ^ D[2] ^ D[1] ^ D[0] ^ C[0] ^ C[1] ^
		C[2] ^ C[3] ^ C[4] ^ C[5] ^ C[6] ^ C[7] ^ C[8] ^ C[9] ^
		C[15] ^ C[16] ^ C[17] ^ C[18] ^ C[19] ^ C[20] ^ C[21] ^
		C[22] ^ C[23];
	NewCRC[1] = D[23] ^ D[18] ^ D[0] ^ C[10] ^ C[15];
	NewCRC[2] = D[24] ^ D[19] ^ D[1] ^ C[11] ^ C[16];
	NewCRC[3] = D[25] ^ D[20] ^ D[2] ^ C[12] ^ C[17];
	NewCRC[4] = D[26] ^ D[21] ^ D[3] ^ C[13] ^ C[18];
	NewCRC[5] = D[31] ^ D[30] ^ D[29] ^ D[28] ^ D[26] ^ D[25] ^ D[24] ^
		D[23] ^ D[22] ^ D[17] ^ D[16] ^ D[15] ^ D[14] ^ D[13] ^
		D[12] ^ D[11] ^ D[10] ^ D[9] ^ D[8] ^ D[7] ^ D[6] ^
		D[5] ^ D[3] ^ D[2] ^ D[1] ^ D[0] ^ C[0] ^ C[1] ^ C[2] ^
		C[3] ^ C[4] ^ C[5] ^ C[6] ^ C[7] ^ C[8] ^ C[9] ^ C[14] ^
		C[15] ^ C[16] ^ C[17] ^ C[18] ^ C[20] ^ C[21] ^ C[22] ^
		C[23];
	NewCRC[6] = D[28] ^ D[18] ^ D[5] ^ D[0] ^ C[10] ^ C[20];
	NewCRC[7] = D[29] ^ D[19] ^ D[6] ^ D[1] ^ C[11] ^ C[21];
	NewCRC[8] = D[30] ^ D[20] ^ D[7] ^ D[2] ^ C[12] ^ C[22];
	NewCRC[9] = D[31] ^ D[21] ^ D[8] ^ D[3] ^ C[0] ^ C[13] ^ C[23];
	NewCRC[10] = D[22] ^ D[9] ^ D[4] ^ C[1] ^ C[14];
	NewCRC[11] = D[23] ^ D[10] ^ D[5] ^ C[2] ^ C[15];
	NewCRC[12] = D[24] ^ D[11] ^ D[6] ^ C[3] ^ C[16];
	NewCRC[13] = D[25] ^ D[12] ^ D[7] ^ C[4] ^ C[17];
	NewCRC[14] = D[26] ^ D[13] ^ D[8] ^ C[0] ^ C[5] ^ C[18];
	NewCRC[15] = D[27] ^ D[14] ^ D[9] ^ C[1] ^ C[6] ^ C[19];
	NewCRC[16] = D[28] ^ D[15] ^ D[10] ^ C[2] ^ C[7] ^ C[20];
	NewCRC[17] = D[29] ^ D[16] ^ D[11] ^ C[3] ^ C[8] ^ C[21];
	NewCRC[18] = D[30] ^ D[17] ^ D[12] ^ C[4] ^ C[9] ^ C[22];
	NewCRC[19] = D[31] ^ D[18] ^ D[13] ^ C[5] ^ C[10] ^ C[23];
	NewCRC[20] = D[19] ^ D[14] ^ C[6] ^ C[11];
	NewCRC[21] = D[20] ^ D[15] ^ C[7] ^ C[12];
	NewCRC[22] = D[21] ^ D[16] ^ C[8] ^ C[13];
	NewCRC[23] = D[31] ^ D[30] ^ D[29] ^ D[28] ^ D[27] ^ D[26] ^ D[25] ^
		D[24] ^ D[23] ^ D[22] ^ D[16] ^ D[15] ^ D[14] ^ D[13] ^
		D[12] ^ D[11] ^ D[10] ^ D[9] ^ D[8] ^ D[7] ^ D[6] ^
		D[5] ^ D[4] ^ D[3] ^ D[2] ^ D[1] ^ D[0] ^ C[0] ^ C[1] ^
		C[2] ^ C[3] ^ C[4] ^ C[5] ^ C[6] ^ C[7] ^ C[8] ^ C[14] ^
		C[15] ^ C[16] ^ C[17] ^ C[18] ^ C[19] ^ C[20] ^ C[21] ^
		C[22] ^ C[23];
	ulCRC24_D32 = 0;
	// LFSR value from binary to hex.
	bit_mask = 0x000001;
	for (f = 0; f <= 23; f++) {
		ulCRC24_D32 = ulCRC24_D32 + NewCRC[f] * bit_mask;
		bit_mask = bit_mask << 1;
	}
	return(ulCRC24_D32 & 0x00FFFFFF);
}
