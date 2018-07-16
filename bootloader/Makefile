TARGET_NAME = bootloader
RM := rm -rf

ifeq ($(DEVICE),ADUCM360)
$(info "Configured to device ADUCM360")
LD_MEM=mem_360.ld
DEFINE_DEVICE=__ADUCM360__
else
$(info "Configured to device ADUCM362")
LD_MEM=mem_362.ld
DEFINE_DEVICE=__ADUCM362__
endif

# Needed libs
include system/deps.mk

SECONDARY_FLASH += $(TARGET_NAME).hex

FLASH_INFO = system/src/cmsis/boot_vectors
SRCS += src/main.c src/flash.c src/app.c src/protocol.c src/serial.c
OBJECTS += $(SRCS:.c=.o)
$(info $(OBJECTS))

# All Target
all: clean_map $(TARGET_NAME).map $(TARGET_NAME).elf $(TARGET_NAME).hex $(TARGET_NAME).bin


.PHONY: clean_map
clean_map: 
	rm -f $(TARGET_NAME).map

.PHONY: recompile_$(FLASH_INFO).o
recompile_$(FLASH_INFO).o:
	@echo 'CC $(FLASH_INFO).c'
	@arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -D$(DEFINE_DEVICE)  -DDEBUG -DAPP_END=`grep -Po '(0x[0-9a-fA-F]+)(?=\s+__flash_end)' bootloader.map` $(INCS) -std=gnu11 -Wno-missing-prototypes -Wno-padded -Wno-missing-declarations -MMD -MP -c -o "$(FLASH_INFO).o" "$(FLASH_INFO).c"
	# rm -f $(FLASH_INFO).o

%.o: %.c
	@echo 'CC $<'
	@arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -D$(DEFINE_DEVICE) -DDEBUG $(INCS) -std=gnu11 -Wno-missing-prototypes -Wno-padded -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -c -o "$@" "$<"

# Tool invocations
$(TARGET_NAME).map: $(OBJECTS)
	@echo 'LD $@ -- first time'
	@arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T ld/$(LD_MEM) -T ld/sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"$(TARGET_NAME).map" --specs=nano.specs -o "$(TARGET_NAME).elf" $(OBJECTS) $(LIBS)

$(TARGET_NAME).elf: recompile_$(FLASH_INFO).o $(OBJECTS)
	@echo 'LD $@ -- with flash info'
	@arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T ld/$(LD_MEM) -T ld/sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"$(TARGET_NAME).map" --specs=nano.specs -o "$(TARGET_NAME).elf" $(OBJECTS) $(LIBS)

$(TARGET_NAME).hex: $(TARGET_NAME).elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "$(TARGET_NAME).elf"  "$(TARGET_NAME).hex"
	@echo 'Finished building: $@'
	@echo ' '

$(TARGET_NAME).bin: $(TARGET_NAME).elf
	arm-none-eabi-objcopy -S -O binary "$(TARGET_NAME).elf"  "$(TARGET_NAME).bin"

clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) $(TARGET_NAME).elf
	rm -rf $(OBJECTS).o $(TARGET_NAME).map $(TARGET_NAME).elf $(TARGET_NAME).hex
	-@echo ' '


.PHONY: all clean dependents

