#include "cortexm/ExceptionHandlers.h"

extern unsigned int _estack;

typedef void
(* const pHandler)(void);

struct jumpTable
{
	unsigned int branch_instr_0;
	unsigned int branch_instr_1;
	pHandler           handler_0;
	pHandler           handler_1;
};


// Define common jump table locations (vectors shared for all applications)

#define COMMON_JUMP_TABLE  0x001001
// The vector table.
#define COMMON_Reset_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x00)                            // The reset handler
#define COMMON_NMI_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x04)                              // The NMI handler
#define COMMON_HardFault_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x10)                        // The hard fault handler
#define COMMON_MemManage_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x14)                        // The MPU fault handler
#define COMMON_BusFault_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x20)// The bus fault handler
#define COMMON_UsageFault_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x24)// The usage fault handler
#define COMMON_SVC_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x30)                              // SVCall handler
#define COMMON_DebugMon_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x34)                         // Debug monitor handler
#define COMMON_PendSV_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x40)                           // The PendSV handler
#define COMMON_SysTick_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x44)                          // The SysTick handler
#define COMMON_WakeUp_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x50)        //[ 0]
#define COMMON_Ext_Int0_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x54)          //[ 1]
#define COMMON_Ext_Int1_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x60)          //[ 2]
#define COMMON_Ext_Int2_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x64)          //[ 3]
#define COMMON_Ext_Int3_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x70)          //[ 4]
#define COMMON_Ext_Int4_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x74)          //[ 5]
#define COMMON_Ext_Int5_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x80)          //[ 6]
#define COMMON_Ext_Int6_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x84)          //[ 7]
#define COMMON_Ext_Int7_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x90)          //[ 8]
#define COMMON_WDog_Tmr_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x94)      //[ 9]
#define COMMON_GP_Tmr0_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xa0)       //[11]
#define COMMON_GP_Tmr1_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xa4)       //[12]
#define COMMON_ADC0_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xb0)          //[13]
#define COMMON_ADC1_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xb4)          //[14]
#define COMMON_SINC2_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xc0)         //[15]
#define COMMON_Flsh_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xc4)          //[16]
#define COMMON_UART_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xd0)          //[17]
#define COMMON_SPI0_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xd4)          //[18]
#define COMMON_SPI1_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xe0)          //[19]
#define COMMON_I2C0_Slave_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xe4)    //[20]
#define COMMON_I2C0_Master_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xf0)   //[21]
#define COMMON_DMA_Err_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0xf4)       //[22]
#define COMMON_DMA_SPI1_TX_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x100)   //[23]
#define COMMON_DMA_SPI1_RX_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x104)   //[24]
#define COMMON_DMA_UART_TX_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x110)   //[25]
#define COMMON_DMA_UART_RX_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x114)   //[26]
#define COMMON_DMA_I2C0_STX_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x120)  //[27]
#define COMMON_DMA_I2C0_SRX_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x124)  //[28]
#define COMMON_DMA_I2C0_MTX_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x130)  //[29]
#define COMMON_DMA_I2C0_MRX_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x134)  //[30]
#define COMMON_DMA_DAC_Out_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x140)   //[31]
#define COMMON_DMA_ADC0_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x144)      //[32]
#define COMMON_DMA_ADC1_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x150)      //[33]
#define COMMON_DMA_SINC2_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x154)     //[34]
#define COMMON_PWMTRIP_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x160)       //[35]
#define COMMON_PWM0_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x164)          //[36]
#define COMMON_PWM1_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x170)          //[37]
#define COMMON_PWM2_Int_Handler  (pHandler)(COMMON_JUMP_TABLE + 0x174)          //[38]

__attribute__ ((section(".isr_vector")))
pHandler __isr_vectors[] = {
   //
   (pHandler) &_estack,                          // The initial stack pointer
   Reset_Handler,
   COMMON_NMI_Handler,                              // The NMI handler
   COMMON_HardFault_Handler,                        // The hard fault handler
   COMMON_MemManage_Handler,                        // The MPU fault handler
   COMMON_BusFault_Handler,					 // The bus fault handler
   COMMON_UsageFault_Handler,                // The usage fault handler
   0,                                        // Reserved
   0,                                        // Reserved
   0,                                        // Reserved
   0,                                        // Reserved
   COMMON_SVC_Handler,                              // SVCall handler
   COMMON_DebugMon_Handler,                         // Debug monitor handler
   0,                                        // Reserved
   COMMON_PendSV_Handler,                           // The PendSV handler
   COMMON_SysTick_Handler,                          // The SysTick handler

   // ----------------------------------------------------------------------
   // ADuCM360 vectors
   COMMON_WakeUp_Int_Handler,        //[ 0]
   COMMON_Ext_Int0_Handler,          //[ 1]
   COMMON_Ext_Int1_Handler,          //[ 2]
   COMMON_Ext_Int2_Handler,          //[ 3]
   COMMON_Ext_Int3_Handler,          //[ 4]
   COMMON_Ext_Int4_Handler,          //[ 5]
   COMMON_Ext_Int5_Handler,          //[ 6]
   COMMON_Ext_Int6_Handler,          //[ 7]
   COMMON_Ext_Int7_Handler,          //[ 8]
   COMMON_WDog_Tmr_Int_Handler,      //[ 9]
   0,                                //[10]
   COMMON_GP_Tmr0_Int_Handler,       //[11]
   COMMON_GP_Tmr1_Int_Handler,       //[12]
   COMMON_ADC0_Int_Handler,          //[13]
   COMMON_ADC1_Int_Handler,          //[14]
   COMMON_SINC2_Int_Handler,         //[15]
   COMMON_Flsh_Int_Handler,          //[16]
   COMMON_UART_Int_Handler,          //[17]
   COMMON_SPI0_Int_Handler,          //[18]
   COMMON_SPI1_Int_Handler,          //[19]
   COMMON_I2C0_Slave_Int_Handler,    //[20]
   COMMON_I2C0_Master_Int_Handler,   //[21]
   COMMON_DMA_Err_Int_Handler,       //[22]
   COMMON_DMA_SPI1_TX_Int_Handler,   //[23]
   COMMON_DMA_SPI1_RX_Int_Handler,   //[24]
   COMMON_DMA_UART_TX_Int_Handler,   //[25]
   COMMON_DMA_UART_RX_Int_Handler,   //[26]
   COMMON_DMA_I2C0_STX_Int_Handler,  //[27]
   COMMON_DMA_I2C0_SRX_Int_Handler,  //[28]
   COMMON_DMA_I2C0_MTX_Int_Handler,  //[29]
   COMMON_DMA_I2C0_MRX_Int_Handler,  //[30]
   COMMON_DMA_DAC_Out_Int_Handler,   //[31]
   COMMON_DMA_ADC0_Int_Handler,      //[32]
   COMMON_DMA_ADC1_Int_Handler,      //[33]
   COMMON_DMA_SINC2_Int_Handler,     //[34]
   COMMON_PWMTRIP_Int_Handler,       //[35]
   COMMON_PWM0_Int_Handler,          //[36]
   COMMON_PWM1_Int_Handler,          //[37]
   COMMON_PWM2_Int_Handler,          //[38]
   0,                                //[39]
};

void __attribute__ ((section(".after_vectors")))
Default_Handler(void)
{
   while (1) {
   }
}

#define LOAD_PC 0x47004801

__attribute__ ((section(".jmp_table")))
struct jumpTable __jmp_table[] = {
		{ 0x47004801, 0x47004801, Reset_Handler, NMI_Handler},
		{ 0x47004801, 0x47004801, HardFault_Handler, MemManage_Handler},
		{ 0x47004801, 0x47004801, BusFault_Handler, UsageFault_Handler},
		{ 0x47004801, 0x47004801, SVC_Handler, DebugMon_Handler},
		{ 0x47004801, 0x47004801, PendSV_Handler, SysTick_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
		{ 0x47004801, 0x47004801, Reset_Handler, Reset_Handler},
};

#define MAGIC_ID   0xBEA70000 /* Make this record invalid  */
#define APP_START  0x1000
#ifndef APP_END
#define APP_END 0x1000
#warning Please recompile again with valid APP_END
#endif


__attribute__ ((section(".app_desc")))
uint32_t application_descriptors[] = {
 MAGIC_ID,
 APP_START,
 APP_END,
};
