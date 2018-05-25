#include <stdint.h>

//
// Address of application descriptors provided by bootloader when flashing
#define BASE_ADDR           0xE00
#define MAGIC_ID            *((uint32_t*)BASE_ADDR+0)
#define START_APP           *((uint32_t*)(BASE_ADDR+4))
#define END_APP             *((uint32_t*)(BASE_ADDR+8))
#define CHECKSUM_APP        *((uint32_t*)(BASE_ADDR+C))

void start_application()
{
	// small test if the app look correct

	if ( MAGIC_ID == 0xBEA70001)
	{
//		int app_start = *((uint32_t*)BASE_ADDR+4);
		((void(*)())(START_APP))();
//		asm("ldr     r0, [0xE04]");
//		asm("bx       r0");
	}
}
