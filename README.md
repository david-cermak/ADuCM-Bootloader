# ADuCM-Bootloader
Simple bootcode for ADuCM360

## Interface
Bootloading protocol complies with Cortex-M3 Based ADuCxxx
Serial Download Protocol (based on AN-1160 by Analog Devices).
It could use UART0 on P0.1 & P0.2 as on ADUCINO board or RS485 on UART P0.6 & P0.7 with P0.5 for direction control
(#define ADUCINO_UART).
In case of RS485 it uses sequences to distinguish that a message started (feature could be disabled with #define DISABLE_RS485_SENTINELS)
Baud rate is fixed on 115200 (but can be easily changed in the code).

## Flash layout
Adresses 0 - 0x1800 are reserved for bootloader, the rest is for application.
Application descriptors are stored in page starting 0x1000, in 3 words:
* magic number (0xBEA70001)
* app start address (taken from init vector app locates at 0x0004)
* app flash end (taken from address 0x1008)

Application and bootloader shares a jump table located at fixed address 0x1000 with application section of flash.

## Application requirements
Application has to define the following sections
* cortex vectors from address 0x0
  - the only useful record is 0x0004 where bootloader finds app entry address to
place into 0x1000 app descriptor field
  - other records are useful for debugging without bootloader
  - interrupt handlers should point to the 0x1800 jump table, where jump instruction with real hanlder address are located
* application descriptors at 0x1000
  - bootloader finds here application end address -> so bootloader can update app descriptors after entire flash is correctly written
* jump table from 0x1200
  - jump intructions to real handlers -> so bootloader does not have to delete standard vectors at 0x0
  to avoid unpredictable behaviour if flash not correctly or entirely written
* normal application code after jump table (after 0x1000...)
