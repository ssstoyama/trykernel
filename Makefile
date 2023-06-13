SRC_DIR := src
TARGETS := $(SRC_DIR)/boot/boot2.c \
					 $(SRC_DIR)/boot/reset_hdr.c \
					 $(SRC_DIR)/boot/vector_tbl.c \
					 $(SRC_DIR)/kernel/syslib.c \
					 $(SRC_DIR)/kernel/context.c \
					 $(SRC_DIR)/kernel/dispatch.S \
					 $(SRC_DIR)/application/main.c 
KERNEL := build/kernel.elf

.PHONY: all
all:
	mkdir -p build
	arm-none-eabi-gcc -I $(SRC_DIR)/include -T $(SRC_DIR)/linker/pico_memmap.ld -mcpu=cortex-m0plus -O0 -nostartfiles -o $(KERNEL) $(TARGETS)
	./modules/elf2uf2/elf2uf2 $(KERNEL) build/kernel.uf2

.PHONY: clean
clean:
	rm -rf build/*
