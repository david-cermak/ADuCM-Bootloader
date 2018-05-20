TARGET_NAME = bootloader
RM := rm -rf

# Needed libs
include system/deps.mk

SECONDARY_FLASH += $(TARGET_NAME).hex

SRCS += src/main.c src/flash.c src/app.c src/protocol.c
OBJECTS += $(SRCS:.c=.o)

# All Target
all: $(TARGET_NAME).elf $(TARGET_NAME).hex

%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG $(INCS) -std=gnu11 -Wno-missing-prototypes -Wno-padded -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Tool invocations
$(TARGET_NAME).elf: $(OBJECTS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T ld/mem.ld -T ld/sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"adtest.map" --specs=nano.specs -o "$(TARGET_NAME).elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(TARGET_NAME).hex: $(TARGET_NAME).elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "$(TARGET_NAME).elf"  "$(TARGET_NAME).hex"
	@echo 'Finished building: $@'
	@echo ' '

clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) $(TARGET_NAME).elf
	-@echo ' '


.PHONY: all clean dependents

