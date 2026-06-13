C_DEFS += \
	-DSTM32F103xB \

C_INCLUDE += \
	-I${BOARD_DIR}

ASM_SOURCES += \
	$(wildcard $(BOARD_DIR)/*.s)

LD_SCRIPT += \
	$(wildcard $(BOARD_DIR)/*.ld)
