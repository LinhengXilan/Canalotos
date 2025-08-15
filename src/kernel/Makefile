# @file: kernel/Makefile
# @author: LinhengXilan
# @data: 2025-8-16
# @version: build12

OutputFile = $(TOutputFile)kernel/
ObjectFile = $(TObjectFile)kernel/

obj = $(ObjectFile)head.o\
	$(ObjectFile)main.o\
	$(ObjectFile)global.o\
	$(ObjectFile)printk.o\
	$(ObjectFile)memory.o\
	$(ObjectFile)entry.o\
	$(ObjectFile)interrupt.o\
	$(ObjectFile)keyboard.o\
	$(ObjectFile)lib/lib.o\
	$(ObjectFile)lib/string.o\
	$(ObjectFile)irq/8259A.o \
	$(ObjectFile)irq/irq.o\
	$(ObjectFile)process/process.o \
	$(ObjectFile)process/switch.o

.PHONY: all disasm clean

all: mkdir build_sub build image

mkdir:
ifeq ($(wildcard $(ObjectFile)),)
	mkdir -p $(ObjectFile)
endif
ifeq ($(wildcard $(OutputFile)),)
	mkdir -p $(OutputFile)
endif

build_sub:
	cd lib && $(MAKE)
	cd irq && $(MAKE)
	cd process && $(MAKE)

build: $(OutputFile)kernel.bin

image:
	mount $(ImgFile)boot.img /mnt/floppy -o loop
	cp -fv $(OutputFile)kernel.bin /mnt/floppy/
	sync
	umount /mnt/floppy

clean:
	cd lib && $(MAKE) clean
	cd irq && $(MAKE) clean
	cd process && $(MAKE) clean
	rm -rf $(OutputFile)
	rm -rf $(ObjectFile)

$(ObjectFile)%.o: %.asm
	$(ASM) $< $(ASMFLAGS) -o $@

$(ObjectFile)%.o: %.c
	$(CC) $< $(CCFLAGS) -o $@

$(OutputFile)kernel.bin : $(obj)
	$(LD) $(obj) $(LDFLAGS) -o $(ObjectFile)system
	objcopy $(ObjectFile)system -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary $(OutputFile)kernel.bin