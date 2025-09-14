# @file: boot/Makefile
# @author: LinhengXilan
# @data: 2025-9-14
# @version: build14

OutputPath = $(TOutputPath)boot/
bin = $(OutputPath)boot.bin $(OutputPath)loader.bin

ASM = nasm

ASMFLAGS = -w-zeroing

.PHONY: all clean img

all: mkdir build image

img: mkdir build floppy

mkdir:
ifeq ($(wildcard $(OutputPath)),)
	mkdir -p $(OutputPath)
endif

build: $(bin)

image:
	dd if=$(OutputPath)boot.bin of=$(UsbStorage) bs=512 count=1 conv=notrunc
	mount $(UsbStorage) /mnt/floppy
	cp -fv $(OutputPath)loader.bin /mnt/floppy/
	sync
	umount /mnt/floppy

floppy:
	dd if=$(OutputPath)boot.bin of=$(ImgPath)a.img bs=512 count=1 conv=notrunc
	mount $(ImgPath)a.img /mnt/floppy
	cp -fv $(OutputPath)loader.bin /mnt/floppy/
	sync
	umount /mnt/floppy

clean:
	rm -rf $(OutputPath)

$(OutputPath)%.bin: %.asm
	$(ASM) $< $(ASMFLAGS) -o $@
