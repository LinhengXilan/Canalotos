# @file: kernel/process/Makefile
# @author: LinhengXilan
# @data: 2025-8-16
# @version: build12

ObjectFile = $(TObjectFile)kernel/process/

obj = $(ObjectFile)process.o\
	$(ObjectFile)switch.o

.PHONY: all clean

all: mkdir build

mkdir:
ifeq ($(wildcard $(ObjectFile)),)
	mkdir -p $(ObjectFile)
endif

build: $(obj)

clean:
	rm -rf $(ObjectFile)

$(ObjectFile)%.o: %.asm
	$(ASM) $< $(ASMFLAGS) -o $@

$(ObjectFile)%.o: %.c
	$(CC) $< $(CCFLAGS) -o $@