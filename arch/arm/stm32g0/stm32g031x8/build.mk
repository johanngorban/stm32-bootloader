C_DEFS += \
	-DSTM32G031xx \

C_INCLUDE += \
	-I${DEVICE_DIR}

ASM_SOURCES += \
	$(wildcard $(DEVICE_DIR)/*.s)

LD_SCRIPT += \
	$(wildcard $(DEVICE_DIR)/*.ld)
