# @file: kernel/syscall/Makefile
# @author: LinhengXilan
# @data: 2025-8-18
# @version: build13

ObjectFile = $(TObjectFile)kernel/syscall/

obj = $(ObjectFile)8259A.o\
	$(ObjectFile)irq.o \
	$(ObjectFile)syscall.o \

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