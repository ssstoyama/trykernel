TARGETS := src/boot/boot2.c \
					 src/boot/reset_hdr.c \
					 src/boot/vector_tbl.c \
					 src/kernel/syslib.c \
					 src/application/main.c 
KERNEL := build/kernel.elf

.PHONY: all
all:
	mkdir -p build
	arm-none-eabi-gcc -I src/include -T src/linker/pico_memmap.ld -mcpu=cortex-m0plus -O0 -nostartfiles -o $(KERNEL) $(TARGETS)
	./modules/elf2uf2/elf2uf2 $(KERNEL) build/kernel.uf2

.PHONY: clean
clean:
	rm -rf build/*
