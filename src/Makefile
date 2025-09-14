# @file: Makefile
# @author: LinhengXilan
# @data: 2025-9-14
# @version: build14

RootDir = $(shell pwd)
TOutputPath = $(RootDir)/../bin/
TObjectPath = $(RootDir)/../obj/
ImgPath = $(RootDir)/../img/
UsbStorage = /dev/sdg1	# u盘分区的路径

MAKE = make
ASM = nasm
CC = gcc
LD = ld

ASMFLAGS = -f elf64 -w-zeroing
CCFLAGS = -mcmodel=large -fno-builtin -m64 -c -I $(RootDir)/include -fno-stack-protector -std=c23 -O -march=core2 #-mno-sse #-Wdiscarded-qualifiers
LDFLAGS = -b elf64-x86-64 -T kernel.lds -z muldefs #-s --no-warn-rwx-segments

export MAKE ASM CC LD
export ASMFLAGS CCFLAGS LDFLAGS
export TOutputPath TObjectPath ImgPath UsbStorage

.PHONY: all clean bochs disasm img

all: mkdir build

img: mkdir mkdir_img build_img

mkdir:
ifeq ($(wildcard $(TOutputPath)),)
	mkdir -p $(TOutputPath)
endif
ifeq ($(wildcard $(TObjectPath)),)
	mkdir -p $(TObjectPath)
endif
	dd if=/dev/zero of=$(UsbStorage) bs=512 count=100

mkdir_img:
ifeq ($(wildcard $(ImgPath)),)
	mkdir -p $(ImgPath)
endif
	dd if=/dev/zero of=$(ImgPath)a.img bs=512 count=2880

build:
	cd boot && $(MAKE)
	cd kernel && $(MAKE)

build_img:
	cd boot && $(MAKE) img
	cd kernel && $(MAKE) img

clean:
	cd boot && $(MAKE) clean
	cd kernel && $(MAKE) clean
	rm -rf $(TObjectPath)
	rm -rf $(TOutputPath)
	rm -rf $(TImgPath)

bochs:
	bochs -debugger -f $(RootDir)/../bochsrc