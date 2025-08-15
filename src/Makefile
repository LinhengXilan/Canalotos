# @file: Makefile
# @author: LinhengXilan
# @data: 2025-8-16
# @version: build12

RootDir = $(shell pwd)
TOutputFile = $(RootDir)/../bin/
TObjectFile = $(RootDir)/../obj/
ImgFile = $(RootDir)/../img/

MAKE = make
ASM = nasm
CC = gcc
LD = ld

ASMFLAGS = -f elf64 -w-zeroing
CCFLAGS = -mcmodel=large -fno-builtin -m64 -c -I $(RootDir)/include -fno-stack-protector -std=c23 -O -march=core2 #-mno-sse -Wdiscarded-qualifiers
LDFLAGS = -b elf64-x86-64 -T kernel.lds -z muldefs #-s --no-warn-rwx-segments

export MAKE ASM CC LD
export ASMFLAGS CCFLAGS LDFLAGS
export ImgFile TOutputFile TObjectFile

.PHONY: all clean bochs disasm

all: mkdir build

mkdir:
ifeq ($(wildcard $(TOutputFile)),)
	mkdir -p $(TOutputFile)
endif
ifeq ($(wildcard $(TObjectFile)),)
	mkdir -p $(TObjectFile)
endif
ifeq ($(wildcard $(ImgFile)),)
	mkdir -p $(ImgFile)
endif
	dd if=/dev/zero of=$(ImgFile)boot.img bs=512 count=2880

build:
	cd boot && $(MAKE)
	cd kernel && $(MAKE)

clean:
	cd boot && $(MAKE) clean
	cd kernel && $(MAKE) clean
	rm -rf $(TObjectFile)
	rm -rf $(TOutputFile)
	rm -rf $(TDisasmFile)
	rm -rf $(ImgFile)

bochs:
	bochs -debugger -f $(RootDir)/../bochsrc