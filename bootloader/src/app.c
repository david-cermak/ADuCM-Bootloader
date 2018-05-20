#include <stdint.h>

void start_application()
{
	// small test if the app look correct

	// TODO: check the app flash integrity
	if ( *((uint32_t *)0x1F4) != 0xFFFFFFFFUL && *((uint32_t *)0x1F8) != 0xFFFFFFFFUL )
	{
		// TODO: remove hardcoded value
		asm("B 0x1F4");
	}
}
