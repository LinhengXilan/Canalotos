# @file Makefile
# @author LinhengXilan
# @version 0.0.2.13
# @date 2026-3-20

CURRENT_OUTPUT_PATH := $(OUTPUT_PATH)EFI/Boot/

.PHONY: all
all: CreateDirectory Build

CreateDirectory:
ifeq ($(wildcard $(CURRENT_OUTPUT_PATH)), )
	@mkdir -p $(CURRENT_OUTPUT_PATH)
endif

Build:
	@export WORKSPACE=$(ROOT_PATH)edk2
	@export EDK_TOOLS_PATH=$(ROOT_PATH)edk2/BaseTools
	@export CONF_PATH=$(ROOT_PATH)edk2/Conf
	@cp -r . $(ROOT_PATH)edk2/BootLoader
	@printf "\tEFI\tBootx64.efi\n"
	@cd ../edk2 && build -p BootLoader/BootLoader.dsc -a X64 -t GCC -b RELEASE
	@cp ../edk2/Build/BootLoader/RELEASE_GCC/X64/Bootx64.efi $(CURRENT_OUTPUT_PATH)