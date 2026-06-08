# $(FPU) $(FLOAT-ABI) might be needed
MCU_FLAGS = -mcpu=$(CPU) -mthumb

C_INCLUDES += \
	-I${SHARED_LIBS_DIR}/cmsis_core/CMSIS/Core/Include
