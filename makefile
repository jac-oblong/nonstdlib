###############################################################################
# 																			  #
# NonStdLib - A Non-Standard Library for C									  #
# Copyright (C) 2026  Jacob Long											  #
# 																			  #
# This program is free software: you can redistribute it and/or modify		  #
# it under the terms of the GNU Affero General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or			  #
# (at your option) any later version.										  #
# 																			  #
# This program is distributed in the hope that it will be useful,			  #
# but WITHOUT ANY WARRANTY; without even the implied warranty of			  #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				  #
# GNU Affero General Public License for more details.						  #
# 																			  #
# You should have received a copy of the GNU Affero General Public License	  #
# along with this program.  If not, see <https://www.gnu.org/licenses/>.	  #
# 																			  #
###############################################################################

# By default, all commands are silenced. This reduces noise when running tests.
# Run make with V=1 to enable verbose output.
ifneq ($(V),1)
	Q := @
endif

# By default, everything is compiled with the highest level of optimization. Run
# make with DEBUG=1 to enable debugging.
ifneq ($(DEBUG),1)
	OPTIMIZATION := -O3
else
	OPTIMIZATION := -ggdb
endif

# This controls the depth to which the macro language functions.
ifeq ($(MACRO_DEPTH),)
	MACRO_DEPTH := 32
endif

BUILD_DIR   := build
SCRIPT_DIR  := script
SRC_DIR     := nonstdlib
TEST_DIR    := test

# Scripts are used to generate parts of the library that would be difficult or
# impossible to maintain manually.
SCRIPTS     := $(wildcard $(SCRIPT_DIR)/*.c)
SCRIPT_BINS := $(SCRIPTS:$(SCRIPT_DIR)/%.c=$(BUILD_DIR)/script/%)
SCRIPT_DEPS := $(SCRIPTS:$(SCRIPT_DIR)/%.c=$(BUILD_DIR)/script/%.d)

TESTS		:= $(wildcard $(TEST_DIR)/*.c)
TEST_BINS   := $(TESTS:$(TEST_DIR)/%.c=$(BUILD_DIR)/test/%)
TEST_DEPS   := $(TESTS:$(TEST_DIR)/%.c=$(BUILD_DIR)/test/%.d)

WARNINGS	:= -Wall -Wextra -Werror -Wpedantic -Wconversion -Wwrite-strings -pedantic-errors
CC			:= gcc
CC_FLAGS	:= -std=c23 $(WARNINGS) -I. -fsanitize=address $(OPTIMIZATION) -MMD


.PHONY: all
all: scripts tests

.PHONY: tests
tests: $(BUILD_DIR) scripts $(TEST_BINS)

.PHONY: scripts
scripts: $(BUILD_DIR) $(SCRIPT_BINS)

-include $(SCRIPT_DEPS)
-include $(TEST_DEPS)

################################################################################
#                                                                              #
# TESTS                                                                        #
#                                                                              #
################################################################################

$(BUILD_DIR)/test/todo: $(TEST_DIR)/todo.c
	$(Q)$(CC) $(CC_FLAGS) $< -o $@
	$(Q)if $@ &>/dev/null; then false; else true; fi
	$(Q)if $(CC) $(CC_FLAGS) $< -o $@ -Dnsl_todo=nsl_todo_comptime &>/dev/null; then false; else true; fi
	$(Q)echo "Todo - Test(s) Passed"

$(BUILD_DIR)/test/magic: $(TEST_DIR)/magic.c
	$(Q)$(CC) $(CC_FLAGS) $< -o $@
	$(Q)$@
	$(Q)echo "Magic - Test(s) Passed"

################################################################################
#                                                                              #
# SCRIPTS                                                                      #
#                                                                              #
################################################################################

$(BUILD_DIR)/script/magic: $(SCRIPT_DIR)/magic.c
	$(Q)$(CC) $(CC_FLAGS) $< -o $@
$(SRC_DIR)/magic.h: $(BUILD_DIR)/script/magic
	$(Q)$< $@ $(MACRO_DEPTH)

################################################################################
#                                                                              #
# DEPENDENCIES                                                                 #
#                                                                              #
################################################################################

$(BUILD_DIR):
	mkdir -p $@
	mkdir -p $@/script
	mkdir -p $@/test

.PHONY: clean
clean:
	$(Q)find $(BUILD_DIR) -type f -delete
