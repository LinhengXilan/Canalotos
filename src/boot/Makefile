# @file: boot/Makefile
# @author: LinhengXilan
# @data: 2025-7-29
# @version: build11

OutputFile = $(TOutputFile)boot/
bin = $(OutputFile)boot.bin $(OutputFile)loader.bin

ASM = nasm

ASMFLAGS = -w-zeroing

.PHONY: all clear

all: mkdir build image

mkdir:
ifeq ($(wildcard $(OutputFile)),)
	mkdir $(OutputFile)
endif

build: $(bin)

image:
	dd if=$(OutputFile)boot.bin of=$(ImgFile)boot.img bs=512 count=1 conv=notrunc
	mount $(ImgFile)boot.img /mnt/floppy -o loop
	cp -fv $(OutputFile)loader.bin /mnt/floppy/
	sync
	umount /mnt/floppy

clear:
	rm -rf $(OutputFile)

$(OutputFile)%.bin: %.asm
	$(ASM) $< $(ASMFLAGS) -o $@
