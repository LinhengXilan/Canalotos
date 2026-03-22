# @file Makefile
# @author LinhengXilan
# @version 0.0.1.10
# @date 2026-3-22

CURRENT_WORK_PATH := $(shell pwd)/
CURRENT_OUTPUT_PATH := $(OUTPUT_PATH)Canalotos/
CURRENT_INTERMEDIATE_PATH := $(INTERMEDIATE_PATH)Kernel/
RELATIVE_OUTPUT_PATH := Kernel/

ASM_FLAGS := -f bin
C++_FLAGS := -std=c++23 -m64 -c -nostdlib -fno-builtin -nostdinc -O2 -fno-stack-protector -Werror
LD_FLAGS :=
OBJCOPY_FLAGS := -I binary -g -S -O binary

export CURRENT_WORK_PATH
export ASM_FLAGS C++_FLAGS LD_FLAGS

_OBJECT := Kernel/Kernel.a Graphics/Graphics.a Shell/Shell.a Lib/Lib.a Memory/Memory.a
OBJECT := $(addprefix $(CURRENT_INTERMEDIATE_PATH), $(_OBJECT))

SUBDIRS := Kernel Graphics Shell Lib Memory

.PHONY: all
all: MakeDirectory Build

MakeDirectory:
ifeq ($(wildcard $(CURRENT_OUTPUT_PATH)), )
	@mkdir $(CURRENT_OUTPUT_PATH)
endif
ifeq ($(wildcard $(CURRENT_INTERMEDIATE_PATH)), )
	@mkdir $(CURRENT_INTERMEDIATE_PATH)
endif

Build: Subdir
	@printf "\tLD\t%s\n" Kernel.bin
	@$(LD) $(OBJECT) $(LD_FLAGS) -T Kernel.lds -o $(CURRENT_INTERMEDIATE_PATH)Kernel.bin
	@printf "\tOBJCOPY\t%s\n" Kernel
	@$(OBJCOPY) $(CURRENT_INTERMEDIATE_PATH)Kernel.bin $(OBJCOPY_FLAGS) $(CURRENT_OUTPUT_PATH)Kernel

Subdir: $(SUBDIRS)
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done