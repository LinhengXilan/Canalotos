# @file: kernel/lib/Makefile
# @author: LinhengXilan
# @data: 2025-8-16
# @version: build12

ObjectFile = $(TObjectFile)kernel/lib/

obj = $(ObjectFile)string.o\
	$(ObjectFile)lib.o

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