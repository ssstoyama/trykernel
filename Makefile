SRC_DIR := src
TARGETS := $(SRC_DIR)/boot/boot2.o \
					 $(SRC_DIR)/boot/reset_hdr.o \
					 $(SRC_DIR)/boot/vector_tbl.o \
					 $(SRC_DIR)/kernel/context.o \
					 $(SRC_DIR)/kernel/dispatch.o \
					 $(SRC_DIR)/kernel/eventflag.o \
					 $(SRC_DIR)/kernel/inittsk.o \
					 $(SRC_DIR)/kernel/scheduler.o \
					 $(SRC_DIR)/kernel/syslib.o \
					 $(SRC_DIR)/kernel/systimer.o \
					 $(SRC_DIR)/kernel/task_manage.o \
					 $(SRC_DIR)/kernel/task_queue.o \
					 $(SRC_DIR)/kernel/task_sync.o \
					 $(SRC_DIR)/application/usermain.o 
KERNEL := build/kernel.elf

.PHONY: all
all: $(TARGETS)
	mkdir -p build
	arm-none-eabi-gcc -I $(SRC_DIR)/include -T $(SRC_DIR)/linker/pico_memmap.ld -mcpu=cortex-m0plus -O0 -nostartfiles -o $(KERNEL) $(TARGETS)
	./modules/elf2uf2/elf2uf2 $(KERNEL) build/kernel.uf2

%.o: %.S
	arm-none-eabi-gcc -I $(SRC_DIR)/include -mcpu=cortex-m0plus -O0 -o $@ -c $<

%.o: %.c
	arm-none-eabi-gcc -I $(SRC_DIR)/include -mcpu=cortex-m0plus -O0 -o $@ -c $<

.PHONY: clean
clean:
	rm -rf build/*
	rm -rf $(TARGETS)
