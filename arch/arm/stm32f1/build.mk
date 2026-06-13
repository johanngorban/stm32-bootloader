C_DEFS += \
	-DSTM32F1

CPU = cortex-m3

BOARD ?= stm32f103xb

BOARD_DIR := $(MACHINE_DIR)/$(BOARD)

include ${BOARD_DIR}/build.mk

HAL_SRC_DIR := \
	$(SHARED_LIBS_DIR)/stm32f1xx_hal_driver/Src

C_INCLUDE += \
	-I${MACHINE_DIR}/common \
	-I${SHARED_LIBS_DIR}/cmsis_device_f1/Include \
	-I${SHARED_LIBS_DIR}/stm32f1xx_hal_driver/Inc

C_SOURCE += \
	$(wildcard ${MACHINE_DIR}/common/*.c) \
	$(wildcard ${SHARED_LIBS_DIR}/cmsis_device_f1/Source/*.c) \
	$(filter-out %_template.c, $(wildcard $(HAL_SRC_DIR)/*.c))
