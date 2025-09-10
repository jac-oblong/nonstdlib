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
TESTS		= $(BUILD_DIR)/todo
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

$(BUILD_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	$(Q)rm -rf $(BUILD_DIR)/*
