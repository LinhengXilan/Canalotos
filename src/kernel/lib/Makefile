#	@file: lib/Makefile
#   @author: lhxl
#   @data: 2025-5-3
#	@version: build9

ObjectFile = ../../../Canalotos/obj/kernel/lib/

C_INCLUDE = ../../include

obj = $(ObjectFile)string.o\
	$(ObjectFile)lib.o

.PHONY: all

all: mkdir build

mkdir:
ifeq ($(wildcard $(ObjectFile)),)
	mkdir $(ObjectFile)
endif

build: $(obj)

$(ObjectFile)%.o: %.asm
	$(ASM) $< $(ASMFLAGS) -o $@