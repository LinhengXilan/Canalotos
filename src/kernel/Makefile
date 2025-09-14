# @file: kernel/Makefile
# @author: LinhengXilan
# @data: 2025-9-14
# @version: build14

OutputPath = $(TOutputPath)kernel/
ObjectPath = $(TObjectPath)kernel/

obj = $(ObjectPath)head.o\
	$(ObjectPath)main.o\
	$(ObjectPath)global.o\
	$(ObjectPath)printk.o\
	$(ObjectPath)memory.o\
	$(ObjectPath)entry.o\
	$(ObjectPath)interrupt.o\
	$(ObjectPath)keyboard.o\
	$(ObjectPath)lib/lib.o\
	$(ObjectPath)lib/string.o\
	$(ObjectPath)syscall/8259A.o \
	$(ObjectPath)syscall/irq.o\
	$(ObjectPath)syscall/syscall.o\
	$(ObjectPath)process/process.o \
	$(ObjectPath)process/switch.o

.PHONY: all disasm clean img

all: mkdir build_sub build image

img: mkdir build_sub build floppy

mkdir:
ifeq ($(wildcard $(ObjectPath)),)
	mkdir -p $(ObjectPath)
endif
ifeq ($(wildcard $(OutputPath)),)
	mkdir -p $(OutputPath)
endif

build_sub:
	cd lib && $(MAKE)
	cd syscall && $(MAKE)
	cd process && $(MAKE)

build: $(OutputPath)kernel.bin

image:
	mount $(UsbStorage) /mnt/floppy
	cp -fv $(OutputPath)kernel.bin /mnt/floppy/
	sync
	umount /mnt/floppy

floppy:
	mount $(ImgPath)a.img /mnt/floppy
	cp -fv $(OutputPath)kernel.bin /mnt/floppy/
	sync
	umount /mnt/floppy

clean:
	cd lib && $(MAKE) clean
	cd syscall && $(MAKE) clean
	cd process && $(MAKE) clean
	rm -rf $(OutputPath)
	rm -rf $(ObjectPath)

$(ObjectPath)%.o: %.asm
	$(ASM) $< $(ASMFLAGS) -o $@

$(ObjectPath)%.o: %.c
	$(CC) $< $(CCFLAGS) -o $@

$(OutputPath)kernel.bin : $(obj)
	$(LD) $(obj) $(LDFLAGS) -o $(ObjectPath)system
	objcopy $(ObjectPath)system -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary $(OutputPath)kernel.bin