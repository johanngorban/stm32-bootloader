# $(FPU) $(FLOAT-ABI) might be needed
MCU_FLAGS = -mcpu=$(CPU) -mthumb

C_INCLUDE += \
	-I${ARCH_DIR}/common \
	-I${SHARED_LIBS_DIR}/cmsis_core/CMSIS/Core/Include

C_SOURCE += \
	$(wildcard ${ARCH_DIR}/common/*.c) \
	$(wildcard ${ARCH_DIR}/common/asm/*.c) \
	$(wildcard ${ARCH_DIR}/common/asm/*.s)
