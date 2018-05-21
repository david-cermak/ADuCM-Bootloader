SRCS += system/src/aducm360/DioLib.c  \
		system/src/aducm360/FeeLib.c \
		system/src/aducm360/UrtLib.c \
		system/src/cmsis/system_ADuCM360.c \
		system/src/cmsis/vectors_ADuCM360.c \
		system/src/cortexm/_initialize_hardware.c \
		system/src/cortexm/_reset_hardware.c \
		system/src/cortexm/exception_handlers.c \
		system/src/newlib/_exit.c  \
		system/src/newlib/_sbrk.c  \
		system/src/newlib/_startup.c \
		system/src/newlib/_syscalls.c \
		system/src/newlib/assert.c

#		system/src/newlib/_cxx.cpp  \


INCS += -Isystem/include/aducm360 -Isystem/include/cmsis -Isystem/include -Isystem/include -Isystem/include/arm