C_DEFS += \
	-DSTM32G0

CPU = cortex-m0plus

DEVICE ?= stm32g031x8

DEVICE_DIR := $(BOARD_DIR)/$(DEVICE)

include ${DEVICE_DIR}/build.mk

HAL_SRC_DIR := \
	$(SHARED_LIBS_DIR)/stm32g0xx_hal_driver/Src

C_INCLUDE += \
	-I${BOARD_DIR}/common \
	-I${SHARED_LIBS_DIR}/cmsis_device_g0/Include \
	-I${SHARED_LIBS_DIR}/stm32g0xx_hal_driver/Inc

C_SOURCE += \
	$(wildcard ${BOARD_DIR}/common/*.c) \
	$(wildcard ${SHARED_LIBS_DIR}/cmsis_device_g0/Source/*.c) \
	$(filter-out %_template.c, $(wildcard $(HAL_SRC_DIR)/*.c))
