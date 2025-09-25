# @file: kernel/firmware/Makefile
# @author: LinhengXilan
# @data: 2025-9-26
# @version: build15

ObjectPath = $(TObjectPath)kernel/firmware/

obj = $(ObjectPath)cpu.o

.PHONY: all clean

all: mkdir build

mkdir:
ifeq ($(wildcard $(ObjectPath)),)
	mkdir -p $(ObjectPath)
endif

build: $(obj)

clean:
	rm -rf $(ObjectPath)

$(ObjectPath)%.o: %.c
	$(CC) $< $(CCFLAGS) -o $@
