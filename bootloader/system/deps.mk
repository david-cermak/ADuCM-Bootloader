SRCS += system/src/drivers/DioLib.c  \
		system/src/drivers/FeeLib.c \
		system/src/drivers/UrtLib.c

ifeq ($(DEVICE),ADUCM360)
SRCS += system/src/cmsis/boot_vectors.c \
		system/src/system_ADuCM360.c
else
SRCS += system/src/cmsis/boot_vectors_363.c \
		system/src/system_ADuCM362.c
endif

SRCS += system/src/cortexm/_initialize_hardware.c \
		system/src/cortexm/_reset_hardware.c \
		system/src/cortexm/exception_handlers.c \
		system/src/newlib/_exit.c  \
		system/src/newlib/_sbrk.c  \
		system/src/newlib/_startup.c \
		system/src/newlib/_syscalls.c \
		system/src/newlib/assert.c



INCS += -Isystem/include/drivers -Isystem/include/cmsis -Isystem/include -Isystem/include -Isystem/include/arm