C_DEFS += \
	-DSTM32F103xB \

C_INCLUDE += \
	-I${DEVICE_DIR}

ASM_SOURCES += \
	$(wildcard $(DEVICE_DIR)/*.s)

LD_SCRIPT += \
	$(wildcard $(DEVICE_DIR)/*.ld)
