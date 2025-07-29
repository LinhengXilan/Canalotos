# @file: Makefile
# @author: LinhengXilan
# @data: 2025-7-29
# @version: build11

RootDir = $(shell pwd)
TOutputFile = $(RootDir)/../Canalotos/bin/
TObjectFile = $(RootDir)/../Canalotos/obj/
ImgFile = $(RootDir)/../Canalotos/img/

MAKE = make
ASM = nasm
CC = gcc
LD = ld

ASMFLAGS = -f elf64 -w-zeroing
CCFLAGS = -mcmodel=large -fno-builtin -m64 -c -I $(RootDir)/include -fno-stack-protector -std=c23 -O -march=core2# -Wdiscarded-qualifiers
LDFLAGS = -b elf64-x86-64 -T kernel.lds -z muldefs #-s --no-warn-rwx-segments

export MAKE ASM CC LD
export ASMFLAGS CCFLAGS LDFLAGS
export ImgFile TOutputFile TObjectFile


.PHONY: all clear bochs

all: mkdir build

mkdir:
ifeq ($(wildcard $(TOutputFile)),)
	mkdir $(TOutputFile)
endif
ifeq ($(wildcard $(TObjectFile)),)
	mkdir $(TObjectFile)
endif
ifeq ($(wildcard $(ImgFile)),)
	mkdir $(ImgFile)
endif
	dd if=/dev/zero of=$(ImgFile)boot.img bs=512 count=2880

build:
	cd boot && $(MAKE)
	cd kernel && $(MAKE)

clear:
	cd boot && $(MAKE) clear
	cd kernel && $(MAKE) clear
	rm -rf $(TObjectFile)
	rm -rf $(TOutputFile)
	rm -rf $(ImgFile)

bochs:
	bochs -debugger -f $(RootDir)/../Canalotos/bochsrc