# Makefile for ATSAMD21 Project

# Toolchain definitions (ARM GCC)
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Project name
TARGET = firmware

# Directories
SRC_DIR = src
INC_DIR = inc
LINKER_DIR = linker
BUILD_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Linker script
LDSCRIPT = $(LINKER_DIR)/samd21.ld

# Compiler flags
CFLAGS  = -mcpu=cortex-m0plus -mthumb
CFLAGS += -g -Wall -Wextra -O0
CFLAGS += -I$(INC_DIR)
# CFLAGS += -D__SAMD21G18A__ # Define specific part if needed for CMSIS

# Linker flags
LDFLAGS = -mcpu=cortex-m0plus -mthumb
LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -specs=nano.specs -specs=nosys.specs

# Phony targets
.PHONY: all clean flash

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex print_size

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

print_size: $(BUILD_DIR)/$(TARGET).elf
	@echo "Size:"
	$(SIZE) $<

clean:
	rm -rf $(BUILD_DIR)

# Flash using OpenOCD
flash: $(BUILD_DIR)/$(TARGET).bin
	openocd -f openocd.cfg -c "program $(BUILD_DIR)/$(TARGET).bin verify reset exit 0x00000000"
