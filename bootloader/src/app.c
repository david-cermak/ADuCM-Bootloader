#include <stdint.h>
#include <adi_processor.h>
//
// Address of application descriptors provided by bootloader when flashing
#define BASE_ADDR           0x1000
#define MAGIC_ID            *((uint32_t*)BASE_ADDR+0)
#define START_APP           *((uint32_t*)(BASE_ADDR+4))
#define END_APP             *((uint32_t*)(BASE_ADDR+8))
#define CHECKSUM_APP        *((uint32_t*)(BASE_ADDR+C))

void start_application()
{
	// small test if the app looks correct
	if ( MAGIC_ID == 0xBEA70001)
	{
//		NVIC_SystemReset();
	    asm volatile
	    (
	        " ldr     r1,=0x1004 \n"
	    	" ldr     r0, [r1] \n"
	        " bx      r0"
	        :
	        :
	        :
	    );

//	    ((void(*)())(START_APP))();
	}
}
