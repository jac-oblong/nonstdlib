#include "nonstdlib/magic.h"

#include <assert.h>

void test_concat(void) {
    int NSL_CONCAT(x_, is_int) = 0;
    (void)x_is_int;
}

int main() {
    test_concat();
}
