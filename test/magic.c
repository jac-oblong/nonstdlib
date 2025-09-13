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

void test_va_head(void) {
    assert(NSL_VA_HEAD(1) == 1);
    assert(NSL_VA_HEAD(1, 2, 3, 4) == 1);
    assert(NSL_VA_HEAD(NSL__REVERSE_SEQUENCE_N) == 127);
}

void test_va_rest(void) {
    // should evaluate to nothing
    NSL_VA_REST(1)

    int expected[] = {2, 3, 4};
    int result[]   = {NSL_VA_REST(1, 2, 3, 4)};
    for (unsigned long i = 0; i < sizeof(result) / sizeof(*result); i++) {
        assert(expected[i] == result[i]);
    }

    unsigned long rev_seq[] = {NSL_VA_REST(NSL__REVERSE_SEQUENCE_N)};
    for (unsigned long i = 0; i < sizeof(rev_seq) / sizeof(*rev_seq); i++) {
        assert(rev_seq[i] == (126 - i));
    }
}

int main() {
    test_concat();
    test_nargs();
    test_va_head();
    test_va_rest();
}
