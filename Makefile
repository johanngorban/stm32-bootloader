TARGET = stm32-bootloader

ARCH ?= arm
MACHINE ?= stm32f1
BOARD ?= stm32f103xb
BUILD_DIR ?= build

ARCH_DIR := arch/$(ARCH)
MACHINE_DIR := $(ARCH_DIR)/$(MACHINE)
BOARD_DIR := $(MACHINE_DIR)/$(BOARD)

SHARED_LIBS_DIR := libs

# Optimization
OPT ?= -O0

C_INCLUDES += -IApplication/include \
			  -ICore/Inc \
              -I$(ARCH_DIR)/include \
              -I$(MACHINE_DIR)/include \
              -I$(BOARD_DIR)

C_SOURCES += \
	$(wildcard Application/source/*.c) \
	$(wildcard Core/Src/*.c) \
	$(wildcard $(BOARD_DIR)/*.c) \
	$(wildcard ${MACHINE_DIR}/*.c)

ASM_SOURCES := $(wildcard $(BOARD_DIR)/*.s)
LD_SCRIPT := $(wildcard $(BOARD_DIR)/*.ld)

# Enable debug flags
DEBUG ?= false
ifeq (${DEBUG}, true)
	C_FLAGS += -g -O0 -DDEBUG
endif

# Add arch specific makefile
include ${ARCH_DIR}/build.mk

# Add board specific makefiles
include ${MACHINE_DIR}/build.mk

# Set up C compiler
PREFIX = arm-none-eabi-
ifdef GCC_PATH
	CC = $(GCC_PATH)/$(PREFIX)gcc
	AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
	CP = $(GCC_PATH)/$(PREFIX)objcopy
	SZ = $(GCC_PATH)/$(PREFIX)size
else
	CC = $(PREFIX)gcc
	AS = $(PREFIX)gcc -x assembler-with-cpp
	CP = $(PREFIX)objcopy
	SZ = $(PREFIX)size
endif

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# Add lib flags
LIBS_FLAGS = -lc -lm -lnosys
LIBDIR =

# Add C flags
C_FLAGS += $(MCU_FLAGS) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

# Linker common flags
LD_FLAGS = $(MCU_FLAGS) -specs=nano.specs -T$(LD_SCRIPT) $(LIBDIR) $(LIBS_FLAGS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# Main rule
all: ${BUILD_DIR} $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin clean-objects

# Objects specification
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(C_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(C_FLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(C_FLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LD_FLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

# Clang linter
CLANG_FORMAT := $(shell which clang-format-20 2>/dev/null || which clang-format)
FORMAT_DIRS := -not -path './${SHARED_LIBS}/*' -not -path './build/*'

# Formatting rules
fmt:
	find . $(FORMAT_DIRS) \( -name '*.c' -o -name '*.h' \) | xargs $(CLANG_FORMAT) -i

fmt-check:
	find . $(FORMAT_DIRS) \( -name '*.c' -o -name '*.h' \) | xargs $(CLANG_FORMAT) --dry-run --Werror

# Cleanup
clean:
	rm -fR $(BUILD_DIR)

clean-objects:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.lst
