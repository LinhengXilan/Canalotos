# @file Memory/Makefile
# @author LinhengXilan
# @version 0.0.1.10
# @date 2026-3-22

CURRENT_INTERMEDIATE_PATH := $(INTERMEDIATE_PATH)Kernel/Memory/
RELATIVE_OUTPUT_PATH := Kernel/Memory/

BUILD := Memory.a
__OBJECT := Memory.o
_OBJECT := $(addprefix $(RELATIVE_OUTPUT_PATH), $(__OBJECT))
OBJECT := $(addprefix $(CURRENT_INTERMEDIATE_PATH), $(__OBJECT))


.PHONY: all
all: MakeDirectory Build

MakeDirectory:
ifeq ($(wildcard $(CURRENT_INTERMEDIATE_PATH)), )
	mkdir $(CURRENT_INTERMEDIATE_PATH)
endif

Build: $(__OBJECT)
	@printf "\tAR\t%s\n" $(RELATIVE_OUTPUT_PATH)Memory.a
	@ar rcs $(CURRENT_INTERMEDIATE_PATH)Memory.a $(OBJECT)

%.o: %.cpp
	@printf "\tC++\t%s\n" $(RELATIVE_OUTPUT_PATH)$@
	@$(C++) $< $(C++_FLAGS) -I $(CURRENT_WORK_PATH)Include -o $(CURRENT_INTERMEDIATE_PATH)$@
