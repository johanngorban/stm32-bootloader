ARCH ?= arm
BOARD =  # e.g. stm32f1
BUILD_DIR ?= build

NON_BOARD_TARGETS := clean fmt fmt-check release print-version

ifeq ($(MAKECMDGOALS),)
  NEED_BOARD := 1
else
  ifeq ($(filter-out $(NON_BOARD_TARGETS), $(MAKECMDGOALS)),)
    NEED_BOARD := 0
  else
    NEED_BOARD := 1
  endif
endif

ifeq ($(NEED_BOARD),1)
  ifeq ($(strip $(BOARD)),)
    $(error Error: BOARD is not set. Please specify your MCU, e.g. BOARD=stm32f1)
  endif
endif

TARGET =  $(BOARD)-bootloader

ARCH_DIR := arch/$(ARCH)
BOARD_DIR := $(ARCH_DIR)/$(BOARD)

SHARED_LIBS_DIR := libs

# Optimization
OPT ?= -O0

ASM_SOURCES :=
LD_SCRIPT :=

# Enable debug flags
DEBUG ?= false
ifeq (${DEBUG}, true)
	C_FLAGS += -g -O0 -DDEBUG
endif

# Add bootloader kernel code
include bootloader/build.mk

# Add arch specific makefile
include ${ARCH_DIR}/build.mk

# Add board specific makefiles
include ${BOARD_DIR}/build.mk

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
C_FLAGS += $(MCU_FLAGS) $(C_DEFS) $(C_INCLUDE) $(OPT) -Wall -fdata-sections -ffunction-sections

# Linker common flags
LD_FLAGS = $(MCU_FLAGS) -specs=nano.specs -T$(LD_SCRIPT) $(LIBDIR) $(LIBS_FLAGS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# Main rule
all: ${BUILD_DIR} $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin clean-objects

# Objects specification
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCE:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCE)))

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


# Release
DEVICE_DIRS := $(patsubst %/,%,$(dir $(wildcard arch/$(ARCH)/*/build.mk)))

release-target = $(if $(wildcard $(1)/*/build.mk),\
  $(subst /,@,$(patsubst arch/$(ARCH)/%,%,$(patsubst %/,%,$(dir $(wildcard $(1)/*/build.mk))))),\
  $(patsubst arch/$(ARCH)/%,%,$(1)))

RELEASE_TARGETS := $(foreach d,$(DEVICE_DIRS),$(call release-target,$(d)))
VERSION := $(shell grep -oiE '[0-9]+\.[0-9]+\.[0-9]+([.-][0-9A-Za-z]+)*' VERSION | head -n1)

release: $(addprefix release-,$(RELEASE_TARGETS))

release-%:
	$(MAKE) BOARD=$(firstword $(subst @, ,$*)) \
	        $(if $(word 2,$(subst @, ,$*)),DEVICE=$(word 2,$(subst @, ,$*))) \
	        BUILD_DIR=build/$(subst @,/,$*)
	mkdir -p build/release
	cp build/$(subst @,/,$*)/$(firstword $(subst @, ,$*))-bootloader.bin \
	   build/release/$(lastword $(subst @, ,$*))-bootloader-$(VERSION).bin

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

.PHONY: all clean clean-objects fmt fmt-check release print-version
