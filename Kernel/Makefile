# @file Makefile
# @author LinhengXilan
# @version 0.0.0.6
# @date 2026-2-13

CURRENT_WORK_PATH := $(shell pwd)/
CURRENT_OUTPUT_PATH := $(OUTPUT_PATH)Canalotos/
CURRENT_INTERMEDIATE_PATH := $(INTERMEDIATE_PATH)Kernel/
RELATIVE_OUTPUT_PATH := Kernel/

ASM_FLAGS := -f bin
C++_FLAGS := -std=c++23 -m64 -c -nostdlib -fno-builtin -nostdinc -O2 -fno-stack-protector

export CURRENT_WORK_PATH
export ASM_FLAGS C++_FLAGS LDFLAGS

_OBJECT := Kernel/Kernel.a Graphics/Graphics.a Shell/Shell.a Lib/Lib.a
OBJECT := $(addprefix $(CURRENT_INTERMEDIATE_PATH), $(_OBJECT))

SUBDIRS := Kernel Graphics Shell Lib

.PHONY: all
all: MakeDirectory Build

.PHONY: MakeDirectory
MakeDirectory:
ifeq ($(wildcard $(CURRENT_OUTPUT_PATH)),)
	@mkdir $(CURRENT_OUTPUT_PATH)
endif

.PHONY: Build
Build: Subdir
	@printf "\tLD\t%s\n" Kernel.bin
	@$(LD) $(OBJECT) $(LD_FLAGS) -T Kernel.lds -o $(CURRENT_OUTPUT_PATH)Kernel.bin

Subdir: $(SUBDIRS)
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done