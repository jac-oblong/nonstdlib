#include "nonstdlib/magic.h"

#include <assert.h>

void test_concat(void) {
    int NSL_CAT(x_, is_int) = 0;
    (void)x_is_int;
}

void test_nargs(void) {
    assert(NSL_NARGS(1, 2, 3) == 3);
    assert(NSL_NARGS(a, b, c, e, f, g, h, i, j, k) == 10);
    assert(NSL_NARGS() == 0);
    // this goes over the argument limit for NARGS, so it should return the
    // 128th argument, which is '1'
    // clang-format off
    assert(NSL_NARGS('0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                     '0', '0', '0', '0', '0', '0', '0', '1') == '1');
    // clang-format on
}

int main() {
    test_concat();
    test_nargs();
}
