# @file Shell/Makefile
# @author LinhengXilan
# @version 0.0.0.3
# @date 2026-2-9

CURRENT_INTERMEDIATE_PATH := $(INTERMEDIATE_PATH)Kernel/Shell/
RELATIVE_OUTPUT_PATH := Kernel/Shell/

BUILD := Shell.a
__OBJECT := Shell.o
_OBJECT := $(addprefix $(RELATIVE_OUTPUT_PATH), $(__OBJECT))
OBJECT := $(addprefix $(CURRENT_INTERMEDIATE_PATH), $(__OBJECT))


.PHONY: all
all: MakeDirectory Build

.PHONY: MakeDirectory
MakeDirectory:
ifeq ($(wildcard $(CURRENT_INTERMEDIATE_PATH)),)
	mkdir $(CURRENT_INTERMEDIATE_PATH)
endif

.PHONY: Build
Build: $(__OBJECT)
	@printf "\tAR\t%s\n" $(RELATIVE_OUTPUT_PATH)Shell.a
	@ar rcs $(CURRENT_INTERMEDIATE_PATH)Shell.a $(OBJECT)

%.o: %.cpp
	@printf "\tC++\t%s\n" $(RELATIVE_OUTPUT_PATH)$@
	@$(C++) $< $(C++_FLAGS) -I $(CURRENT_WORK_PATH)Include -o $(CURRENT_INTERMEDIATE_PATH)$@
