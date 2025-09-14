# @file: kernel/syscall/Makefile
# @author: LinhengXilan
# @data: 2025-8-18
# @version: build13

ObjectPath = $(TObjectPath)kernel/syscall/

obj = $(ObjectPath)8259A.o\
	$(ObjectPath)irq.o \
	$(ObjectPath)syscall.o \

.PHONY: all clean

all: mkdir build

mkdir:
ifeq ($(wildcard $(ObjectPath)),)
	mkdir -p $(ObjectPath)
endif

build: $(obj)

clean:
	rm -rf $(ObjectPath)

$(ObjectPath)%.o: %.asm
	$(ASM) $< $(ASMFLAGS) -o $@

$(ObjectPath)%.o: %.c
	$(CC) $< $(CCFLAGS) -o $@