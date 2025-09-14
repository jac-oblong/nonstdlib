#include "nonstdlib/magic.h"

#include <assert.h>

void test_cat(void) {
    assert(NSL_CAT(1, 9) == 19);
    assert(NSL_CAT(4, 7) == 47);
}

void test_ncat(void) {
    // should evaluate to nothing
    NSL_NCAT()
    assert(NSL_NCAT(1) == 1);
    assert(NSL_NCAT(1, 2) == 12);
    assert(NSL_NCAT(1, 2, 3) == 123);
    assert(NSL_NCAT(1, 2, 3, 4) == 1234);
    assert(NSL_NCAT(1, 2, 3, 4, 5) == 12345);
    assert(NSL_NCAT(1, 2, 3, 4, 5, 6) == 123456);
    assert(NSL_NCAT(1, 2, 3, 4, 5, 6, 7) == 1234567);
    assert(NSL_NCAT(1, 2, 3, 4, 5, 6, 7, 8) == 12345678);
    assert(NSL_NCAT(1, 2, 3, 4, 5, 6, 7, 8, 9) == 123456789);
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

void test_va_tail(void) {
    assert(NSL_VA_TAIL(1) == 1);
    assert(NSL_VA_TAIL(1, 2, 3, 4) == 4);
    assert(NSL_VA_TAIL(NSL_VA_REST(NSL__REVERSE_SEQUENCE_N)) == 0);
}

void test_va_init(void) {
    // should evaluate to nothing
    NSL_VA_INIT(1)

    int expected[] = {1, 2, 3};
    int result[]   = {NSL_VA_INIT(1, 2, 3, 4)};
    for (unsigned long i = 0; i < sizeof(result) / sizeof(*result); i++) {
        assert(expected[i] == result[i]);
    }

    unsigned long rev_seq[] = {NSL_VA_INIT(NSL_VA_REST(NSL__REVERSE_SEQUENCE_N))};
    for (unsigned long i = 0; i < sizeof(rev_seq) / sizeof(*rev_seq); i++) {
        assert(rev_seq[i] == (126 - i));
    }
}

void test_va_reverse(void) {
    // should evaluate to nothing
    NSL_VA_REVERSE()

    int expected[] = {4, 3, 2, 1};
    int result[]   = {NSL_VA_REVERSE(1, 2, 3, 4)};
    for (unsigned long i = 0; i < sizeof(result) / sizeof(*result); i++) {
        assert(expected[i] == result[i]);
    }

    unsigned long rev_seq[] = {NSL_VA_REVERSE(NSL_VA_REST(NSL__REVERSE_SEQUENCE_N))};
    for (unsigned long i = 0; i < sizeof(rev_seq) / sizeof(*rev_seq); i++) {
        assert(rev_seq[i] == i);
    }
}

void test_foreach(void) {
    // should evaluate to nothing
    NSL_FOREACH(f)

#define TIMES_10(x) (x * 10)
    int expected[] = {10, 20, 30, 40};
    int result[]   = {NSL_FOREACH(TIMES_10, 1, 2, 3, 4)};
    for (unsigned long i = 0; i < sizeof(result) / sizeof(*result); i++) {
        assert(expected[i] == result[i]);
    }

    unsigned long rev_seq[] = {NSL_FOREACH(TIMES_10, NSL_VA_REST(NSL__REVERSE_SEQUENCE_N))};
    for (unsigned long i = 0; i < sizeof(rev_seq) / sizeof(*rev_seq); i++) {
        assert(rev_seq[i] == ((126 - i) * 10));
    }
#undef TIMES_10
}

void test_forall_rest(void) {
    // should evaluate to nothing
    NSL_FORALL_REST(f)

    int expected[] = {1234, 234, 34, 4};
    int result[]   = {NSL_FORALL_REST(NSL_NCAT, 1, 2, 3, 4)};
    for (unsigned long i = 0; i < sizeof(result) / sizeof(*result); i++) {
        assert(expected[i] == result[i]);
    }
}

void test_forall_init(void) {
    // should evaluate to nothing
    NSL_FORALL_INIT(f)

    int expected[] = {1234, 123, 12, 1};
    int result[]   = {NSL_FORALL_INIT(NSL_NCAT, 1, 2, 3, 4)};
    for (unsigned long i = 0; i < sizeof(result) / sizeof(*result); i++) {
        assert(expected[i] == result[i]);
    }
}

int main() {
    test_cat();
    test_ncat();
    test_nargs();
    test_va_head();
    test_va_rest();
    test_va_tail();
    test_va_init();
    test_va_reverse();
    test_foreach();
    test_forall_rest();
    test_forall_init();
}
