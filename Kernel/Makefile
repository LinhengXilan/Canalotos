CURRENT_OUTPUT_PATH := $(OUTPUT_PATH)Kernel/
CURRENT_INTERMEDIATE_PATH := $(INTERMEDIATE_PATH)Kernel/
RELATIVE_OUTPUT_PATH := Kernel/

BUILD := Head.bin

.PHONY: all
all: MakeDirectory Build

.PHONY: MakeDirectory
MakeDirectory:
ifeq ($(wildcard $(CURRENT_OUTPUT_PATH)),)
	mkdir $(CURRENT_OUTPUT_PATH)
endif
ifeq ($(wildcard $(CURRENT_INTERMEDIATE_PATH)),)
	mkdir $(CURRENT_INTERMEDIATE_PATH)
endif

.PHONY: Build
Build: $(BUILD)

%.bin: %.asm
	@printf "\tASM\t%s\n" $(RELATIVE_OUTPUT_PATH)$@
	@$(ASM) $< $(ASM_FLAGS) -o $(CURRENT_OUTPUT_PATH)$@
