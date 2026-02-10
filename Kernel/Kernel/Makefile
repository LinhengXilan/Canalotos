# @file Kernel/Makefile
# @author LinhengXilan
# @version 0.0.0.4
# @date 2026-2-10

CURRENT_OUTPUT_PATH := $(OUTPUT_PATH)Canalotos/
CURRENT_INTERMEDIATE_PATH := $(INTERMEDIATE_PATH)Kernel/Kernel/
RELATIVE_OUTPUT_PATH := Kernel/

__OBJECT := Kernel.o
_OBJECT := $(addprefix $(RELATIVE_OUTPUT_PATH), $(__OBJECT))
OBJECT := $(addprefix $(CURRENT_INTERMEDIATE_PATH), $(__OBJECT))

.PHONY: all
all: MakeDirectory Build

.PHONY: MakeDirectory
MakeDirectory:
ifeq ($(wildcard $(CURRENT_OUTPUT_PATH)),)
	@mkdir $(CURRENT_OUTPUT_PATH)
endif
ifeq ($(wildcard $(CURRENT_INTERMEDIATE_PATH)),)
	@mkdir $(CURRENT_INTERMEDIATE_PATH)
endif

.PHONY: Build
Build: $(__OBJECT)
	@printf "\tAR\t%s\n" $(RELATIVE_OUTPUT_PATH)Kernel.a
	@ar rcs $(CURRENT_INTERMEDIATE_PATH)Kernel.a $(OBJECT)

Graphics:
	@make -c Graphics

%.: %.asm
	@printf "\tASM\t%s\n" $(RELATIVE_OUTPUT_PATH)$@
	@$(ASM) $< $(ASM_FLAGS) -o $(CURRENT_OUTPUT_PATH)$@

%.o: %.cpp
	@printf "\tC++\t%s\n" $(RELATIVE_OUTPUT_PATH)$@
	@$(C++) $< $(C++_FLAGS) -I $(CURRENT_WORK_PATH)Include -o $(CURRENT_INTERMEDIATE_PATH)$@
