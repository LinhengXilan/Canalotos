ROOT_PATH := $(shell pwd)/
BUILD_PATH := $(ROOT_PATH)build/
OUTPUT_PATH := $(BUILD_PATH)Canalotos/
INTERMEDIATE_PATH := $(BUILD_PATH)intermediate/

ASM := nasm
C++ := g++
OBJCOPY := objcopy

BUILD_TYPE := Release

export ROOT_PATH OUTPUT_PATH INTERMEDIATE_PATH
export ASM C++ OBJCOPY
export BUILD_TYPE

.PHONY: all
all: CreateDirectory Build

CreateDirectory:
ifeq ($(wildcard $(OUTPUT_PATH)), )
	@mkdir -p $(OUTPUT_PATH)
endif
ifeq ($(wildcard $(INTERMEDIATE_PATH)), )
	@mkdir -p $(INTERMEDIATE_PATH)
endif

.PHONY: Build
Build:
	@make -C BootLoader
	@make -C Kernel

.PHONY: clean
clean:
	@rm -rf $(BUILD_PATH)