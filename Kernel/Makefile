CURRENT_OUTPUT_PATH := $(OUTPUT_PATH)Kernel/
CURRENT_INTERMEDIATE_PATH := $(INTERMEDIATE_PATH)Kernel/
RELATIVE_OUTPUT_PATH := Kernel/

BUILD := Kernel.bin
__OBJECT := Kernel.o
_OBJECT := $(addprefix $(RELATIVE_OUTPUT_PATH), $(__OBJECT))
OBJECT := $(addprefix $(CURRENT_INTERMEDIATE_PATH), $(__OBJECT))


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

$(BUILD): $(__OBJECT)
	@printf "\tLD\t%s\n" $(_OBJECT)
	@$(LD) $(OBJECT) $(LD_FLAGS) -T Kernel.lds -o $(CURRENT_OUTPUT_PATH)$@

%.: %.asm
	@printf "\tASM\t%s\n" $(RELATIVE_OUTPUT_PATH)$@
	@$(ASM) $< $(ASM_FLAGS) -o $(CURRENT_OUTPUT_PATH)$@

%.o: %.cpp
	@printf "\tC++\t%s\n" $(RELATIVE_OUTPUT_PATH)$@
	@$(C++) $< $(C++_FLAGS) -I include -o $(CURRENT_INTERMEDIATE_PATH)$@
