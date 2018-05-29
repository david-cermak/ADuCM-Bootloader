#include "flash.h"
#include "FeeLib.h"

#define INIT_VECTOR 0x00000004

extern uint32_t g_prg_data[];

uint32_t g_app_start = 0;
uint32_t g_app_end = 0;


void write_to_flash(uint32_t page, uint32_t size)
{
	uint32_t *array = &g_prg_data[1];
	volatile uint32_t *address = (uint32_t*)page;
	uint32_t i = 0;

	// wait till previous operation finished
	while (pADI_FEE->FEESTA  == 1) {}

	// enable writing
	pADI_FEE->FEECON0 |= 0x4;


	for (i = 0; i < size; i+=4) {
		// write only after 0x1000 (not rewrite bootcode & app descriptors)
		if (address >= (uint32_t*)0x1000) {
			*address  = *array; // actual write
			// waiting till finished
			while ((pADI_FEE->FEESTA & 0x8) == 0x0) {}
			// check if it was the last word
			if (g_app_start && g_app_end && address == (uint32_t*)g_app_end) {
				// and then let's write app descriptors
				FeePErs(0xE00);
				while (pADI_FEE->FEESTA  == 1) {}
				*((uint32_t*)0xE00) = 0xBEA70001; // magic id
				while ((pADI_FEE->FEESTA & 0x8) == 0x0) {}
				*((uint32_t*)0xE04) = g_app_start;     // program start
				while ((pADI_FEE->FEESTA & 0x8) == 0x0) {}
				*((uint32_t*)0xE08) = g_app_end;       // program end
				while ((pADI_FEE->FEESTA & 0x8) == 0x0) {}
				// TODO: checksum
			}

		} else if (address == (uint32_t*)0x0004) {
			g_app_start = *array;  // Get start address from startup vector
		} else if (address == (uint32_t*)0xE08)  {
			g_app_end = (*array)&0xFFFFFFFC;  // End address from the actual app descriptor
		}
		address++;
		array++;
	}

	// disable writing
	pADI_FEE->FEECON0 &= 0xFB;
}

void erase_pages(int address, int pages)
{
	// not rewriting bootcode
	if (address < 0x1000) return;

	int i;
	for (i=0; i<pages; i++)
	{
		FeePErs(address);
		while (pADI_FEE->FEESTA  == 1) {}
		address += 512;
	}
}
