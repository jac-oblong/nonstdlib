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

ifneq ($(V),1)
	Q = @
endif

ifneq ($(DEBUG),1)
	OPTIMIZATION = -O3
else
	OPTIMIZATION = -ggdb
endif

BUILD_DIR   = build
TEST_DIR    = test
TESTS		= $(BUILD_DIR)/todo  \
			  $(BUILD_DIR)/magic
WARNINGS	= -Wall -Wextra -Werror -Wpedantic -Wconversion -Wwrite-strings -pedantic-errors
CC			= gcc
CC_FLAGS	= -std=c23 $(WARNINGS) -I. -fsanitize=address $(OPTIMIZATION)


.PHONY: all
all: test

.PHONY: test
test: $(BUILD_DIR) $(TESTS)

$(BUILD_DIR)/todo: $(TEST_DIR)/todo.c
	$(Q)$(CC) $(CC_FLAGS) $< -o $@
	$(Q)if $@ &>/dev/null; then false; else true; fi
	$(Q)if $(CC) $(CC_FLAGS) $< -o $@ -Dnsl_todo=nsl_todo_comptime &>/dev/null; then false; else true; fi
	$(Q)echo "Todo - Test(s) Passed"

$(BUILD_DIR)/magic: $(TEST_DIR)/magic.c
	$(Q)$(CC) $(CC_FLAGS) $< -o $@
	$(Q)$@
	$(Q)echo "Magic - Test(s) Passed"

$(BUILD_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	$(Q)rm -rf $(BUILD_DIR)/*
