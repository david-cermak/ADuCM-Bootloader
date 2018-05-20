#include <stdint.h>

void write_to_flash(uint32_t page, uint32_t size);

void erase_pages(int address, int pages);
