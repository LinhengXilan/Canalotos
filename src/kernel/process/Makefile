#	@file: kernel/process/Makefile
#   @author: LinhengXilan
#   @data: 2025-7-29
#	@version: build11

ObjectFile = $(TObjectFile)kernel/process/

obj = $(ObjectFile)process.o\
	$(ObjectFile)switch.o

.PHONY: all clear

all: mkdir build

mkdir:
ifeq ($(wildcard $(ObjectFile)),)
	mkdir $(ObjectFile)
endif

build: $(obj)

clear:
	rm -rf $(ObjectFile)

$(ObjectFile)%.o: %.asm
	$(ASM) $< $(ASMFLAGS) -o $@

$(ObjectFile)%.o: %.c
	$(CC) $< $(CCFLAGS) -o $@