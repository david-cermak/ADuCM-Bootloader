#include "flash.h"
#include "FeeLib.h"

#define INIT_VECTOR 0x00000004

extern uint32_t g_prg_data[];

void write_to_flash(uint32_t page, uint32_t size)
{
	uint32_t *array = &g_prg_data[1];
	volatile uint32_t *address = (uint32_t*)page;
	uint32_t i = 0;
	uint32_t  value_to_write;

	// wait till previous operation finished
	while (pADI_FEE->FEESTA  == 1) {}

	// enable writing
	pADI_FEE->FEECON0 |= 0x4;


	for (i = 0; i < size; i+=4) {
		// not writing to INIT_VECTOR
		if (address == (uint32_t*)INIT_VECTOR) {
			// _start of bootloader
			value_to_write = 0x0001e005;  // TODO: replace hardcoded value
		} else {
			value_to_write = *array;
		}

		*address  = value_to_write; // actual write
		// waiting till finished
		while ((pADI_FEE->FEESTA & 0x8) == 0x0) {}
		address++;
		array++;
	}

	// disable writing
	pADI_FEE->FEECON0 &= 0xFB;
}

void erase_pages(int address, int pages)
{
	int i;
	for (i=0; i<pages; i++)
	{
		FeePErs(address);
		while (pADI_FEE->FEESTA  == 1) {}
		address += 512;
	}
}
