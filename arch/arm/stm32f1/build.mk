C_DEFS += \
	-DSTM32F103xB \
	-DSTM32F1

CPU = cortex-m3

include ${BOARD_DIR}/build.mk

C_INCLUDES += \
	-Icommon \
	-I${SHARED_LIBS_DIR}/cmsis_device_f1/Include \
	-I${SHARED_LIBS_DIR}/stm32f1xx_hal_driver/Inc

C_SOURCES += \
	$(wildcard common/*.c) \
	${SHARED_LIBS_DIR}/cmsis_device_f1/Source \
	${SHARED_LIBS_DIR}/stm32f1xx_hal_driver/Src
