ROOT_PATH := $(shell pwd)/
OUTPUT_PATH := $(ROOT_PATH)Build/Canalotos/
INTERMEDIATE_PATH := $(ROOT_PATH)Build/intermediate/

ASM := nasm
C++ := g++
OBJCOPY := objcopy

export ROOT_PATH OUTPUT_PATH INTERMEDIATE_PATH
export ASM C++ OBJCOPY

.PHONY: all
all: CreateDirectory Build

.PHONY: CreateDirectory
CreateDirectory:
ifeq ($(wildcard $(OUTPUT_PATH)),)
	mkdir -p $(OUTPUT_PATH)
endif
ifeq ($(wildcard $(INTERMEDIATE_PATH)),)
	mkdir -p $(INTERMEDIATE_PATH)
endif

.PHONY: Build
Build:
	@make -C BootLoader
	@make -C Kernel

.PHONY: Clean
Clean:
	@rm -rf $(OUTPUT_PATH)