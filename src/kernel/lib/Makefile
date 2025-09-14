# @file: kernel/lib/Makefile
# @author: LinhengXilan
# @data: 2025-8-16
# @version: build12

ObjectPath = $(TObjectPath)kernel/lib/

obj = $(ObjectPath)string.o\
	$(ObjectPath)lib.o

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