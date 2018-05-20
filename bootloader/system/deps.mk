SRCS += system/src/aducm360/DioLib.c  \
		system/src/aducm360/FeeLib.c \
		system/src/aducm360/UrtLib.c \
		system/src/cmsis/system_ADuCM360.c \
		system/src/cmsis/vectors_ADuCM360.c \
#		system/src/cortexm/exception_handlers.c
		
INCS += -Isystem/include/aducm360 -Isystem/include/cmsis -Isystem/include