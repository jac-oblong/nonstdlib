#include "nonstdlib/magic.h"

#include <assert.h>

void test_cat(void) {
    assert(NSL_CAT(1, 9) == 19);
    assert(NSL_CAT(4, 7) == 47);
}

void test_cat_sep(void) {
    assert(NSL_CAT_SEP(0, 1, 9) == 109);
    assert(NSL_CAT_SEP(1, 4, 7) == 417);
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

void test_ncat_sep(void) {
    // should evaluate to nothing
    NSL_NCAT_SEP(_)
    assert(NSL_NCAT_SEP(0, 1) == 1);
    assert(NSL_NCAT_SEP(0, 1, 2) == 102);
    assert(NSL_NCAT_SEP(0, 1, 2, 3) == 10203);
    assert(NSL_NCAT_SEP(0, 1, 2, 3, 4) == 1020304);
    assert(NSL_NCAT_SEP(0, 1, 2, 3, 4, 5) == 102030405);
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

void test_arg_head(void) {
    assert(NSL_ARG_HEAD(1) == 1);
    assert(NSL_ARG_HEAD(1, 2, 3, 4) == 1);
    assert(NSL_ARG_HEAD(NSL__REVERSE_SEQUENCE_N) == 127);
}

void test_arg_rest(void) {
    // should evaluate to nothing
    NSL_ARG_REST(1)

    int expected[] = {2, 3, 4};
    int result[]   = {NSL_ARG_REST(1, 2, 3, 4)};
    for (unsigned long i = 0; i < sizeof(result) / sizeof(*result); i++) {
        assert(expected[i] == result[i]);
    }

    unsigned long rev_seq[] = {NSL_ARG_REST(NSL__REVERSE_SEQUENCE_N)};
    for (unsigned long i = 0; i < sizeof(rev_seq) / sizeof(*rev_seq); i++) {
        assert(rev_seq[i] == (126 - i));
    }
}

void test_arg_tail(void) {
    assert(NSL_ARG_TAIL(1) == 1);
    assert(NSL_ARG_TAIL(1, 2, 3, 4) == 4);
    assert(NSL_ARG_TAIL(NSL_ARG_REST(NSL__REVERSE_SEQUENCE_N)) == 0);
}

void test_arg_init(void) {
    // should evaluate to nothing
    NSL_ARG_INIT(1)

    int expected[] = {1, 2, 3};
    int result[]   = {NSL_ARG_INIT(1, 2, 3, 4)};
    for (unsigned long i = 0; i < sizeof(result) / sizeof(*result); i++) {
        assert(expected[i] == result[i]);
    }

    unsigned long rev_seq[] = {NSL_ARG_INIT(NSL_ARG_REST(NSL__REVERSE_SEQUENCE_N))};
    for (unsigned long i = 0; i < sizeof(rev_seq) / sizeof(*rev_seq); i++) {
        assert(rev_seq[i] == (126 - i));
    }
}

void test_arg_reverse(void) {
    // should evaluate to nothing
    NSL_ARG_REVERSE()

    int expected[] = {4, 3, 2, 1};
    int result[]   = {NSL_ARG_REVERSE(1, 2, 3, 4)};
    for (unsigned long i = 0; i < sizeof(result) / sizeof(*result); i++) {
        assert(expected[i] == result[i]);
    }

    unsigned long rev_seq[] = {NSL_ARG_REVERSE(NSL_ARG_REST(NSL__REVERSE_SEQUENCE_N))};
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

    unsigned long rev_seq[] = {NSL_FOREACH(TIMES_10, NSL_ARG_REST(NSL__REVERSE_SEQUENCE_N))};
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
    test_cat_sep();
    test_ncat();
    test_ncat_sep();
    test_nargs();
    test_arg_head();
    test_arg_rest();
    test_arg_tail();
    test_arg_init();
    test_arg_reverse();
    test_foreach();
    test_forall_rest();
    test_forall_init();
}
