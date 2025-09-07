#include <assert.h>
#include <string.h>

#define NSL_ARRAY_STRIP_PREFIX
#include "array.h"

DEFINE_ARRAY(int, Int, int)
DEFINE_FUNC_PTR_ARRAY(bool, (int, int), Compare, compare)

constexpr size_t n       = 3;
int              carr[n] = {0};

void test_pushing_item_into_array_produces_correct_error(void) {
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    assert(nsl_arr_push_int(&arr, 1) == ARRAY_ERROR_NONE);
    assert(nsl_arr_push_int(&arr, 2) == ARRAY_ERROR_NONE);
    assert(nsl_arr_push_int(&arr, 3) == ARRAY_ERROR_NONE);
    assert(nsl_arr_push_int(&arr, 4) == ARRAY_ERROR_FULL);
    memset(carr, 0, sizeof(carr));
}

void test_items_pushed_into_array_modify_length_correctly(void) {
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    (void)nsl_arr_push_int(&arr, 1);
    assert(arr.length == 1);
    (void)nsl_arr_push_int(&arr, 1);
    assert(arr.length == 2);
    (void)nsl_arr_push_int(&arr, 1);
    assert(arr.length == 3);
    memset(carr, 0, sizeof(carr));
}

void test_items_pushed_into_array_are_in_correct_order(void) {
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    (void)nsl_arr_push_int(&arr, 1);
    (void)nsl_arr_push_int(&arr, 2);
    (void)nsl_arr_push_int(&arr, 3);

    int expected[n] = {1, 2, 3};
    assert(memcmp(arr.items, expected, sizeof(carr)) == 0);
    memset(carr, 0, sizeof(carr));
}

void test_popping_item_from_array_produces_correct_error(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    assert(nsl_arr_pop_int(&arr).error == ARRAY_ERROR_NONE);
    assert(nsl_arr_pop_int(&arr).error == ARRAY_ERROR_NONE);
    assert(nsl_arr_pop_int(&arr).error == ARRAY_ERROR_NONE);
    assert(nsl_arr_pop_int(&arr).error == ARRAY_ERROR_EMPTY);
    memset(carr, 0, sizeof(carr));
}

void test_items_popped_from_array_modify_length_correctly(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    (void)nsl_arr_pop_int(&arr);
    assert(arr.length == 2);
    (void)nsl_arr_pop_int(&arr);
    assert(arr.length == 1);
    (void)nsl_arr_pop_int(&arr);
    assert(arr.length == 0);
    memset(carr, 0, sizeof(carr));
}

void test_items_popped_from_array_are_in_correct_order(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    assert(nsl_arr_pop_int(&arr).value == 3);
    assert(nsl_arr_pop_int(&arr).value == 2);
    assert(nsl_arr_pop_int(&arr).value == 1);
    memset(carr, 0, sizeof(carr));
}

void test_inserting_item_into_array_produces_correct_error(void) {
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    assert(nsl_arr_insert_int(&arr, 1, 1) == ARRAY_ERROR_BAD_IDX);
    assert(nsl_arr_insert_int(&arr, 0, 1) == ARRAY_ERROR_NONE);
    assert(nsl_arr_insert_int(&arr, 0, 2) == ARRAY_ERROR_NONE);
    assert(nsl_arr_insert_int(&arr, 0, 3) == ARRAY_ERROR_NONE);
    assert(nsl_arr_insert_int(&arr, 0, 4) == ARRAY_ERROR_FULL);
    memset(carr, 0, sizeof(carr));
}

void test_items_inserted_into_array_modify_length_correctly(void) {
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    (void)nsl_arr_insert_int(&arr, 0, 1);
    assert(arr.length == 1);
    (void)nsl_arr_insert_int(&arr, 0, 2);
    assert(arr.length == 2);
    (void)nsl_arr_insert_int(&arr, 0, 3);
    assert(arr.length == 3);
    memset(carr, 0, sizeof(carr));
}

void test_items_inserted_into_array_are_in_correct_order(void) {
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    (void)nsl_arr_insert_int(&arr, 0, 1);
    (void)nsl_arr_insert_int(&arr, 0, 2);
    (void)nsl_arr_insert_int(&arr, 1, 3);

    int expected[n] = {2, 3, 1};
    assert(memcmp(arr.items, expected, sizeof(carr)) == 0);
    memset(carr, 0, sizeof(carr));
}

void test_removing_item_from_array_produces_correct_error(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    assert(nsl_arr_remove_int(&arr, 3).error == ARRAY_ERROR_BAD_IDX);
    assert(nsl_arr_remove_int(&arr, 2).error == ARRAY_ERROR_NONE);
    assert(nsl_arr_remove_int(&arr, 1).error == ARRAY_ERROR_NONE);
    assert(nsl_arr_remove_int(&arr, 0).error == ARRAY_ERROR_NONE);
    assert(nsl_arr_remove_int(&arr, 0).error == ARRAY_ERROR_EMPTY);
    memset(carr, 0, sizeof(carr));
}

void test_items_removed_from_array_modify_length_correctly(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    (void)nsl_arr_remove_int(&arr, 2);
    assert(arr.length == 2);
    (void)nsl_arr_remove_int(&arr, 1);
    assert(arr.length == 1);
    (void)nsl_arr_remove_int(&arr, 0);
    assert(arr.length == 0);
    memset(carr, 0, sizeof(carr));
}

void test_items_removed_from_array_are_in_correct_order(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    assert(nsl_arr_remove_int(&arr, 2).value == 3);
    assert(nsl_arr_remove_int(&arr, 0).value == 1);
    assert(nsl_arr_remove_int(&arr, 0).value == 2);
    memset(carr, 0, sizeof(carr));
}

void test_swap_removing_item_from_array_produces_correct_error(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    assert(nsl_arr_swap_remove_int(&arr, 3).error == ARRAY_ERROR_BAD_IDX);
    assert(nsl_arr_swap_remove_int(&arr, 2).error == ARRAY_ERROR_NONE);
    assert(nsl_arr_swap_remove_int(&arr, 1).error == ARRAY_ERROR_NONE);
    assert(nsl_arr_swap_remove_int(&arr, 0).error == ARRAY_ERROR_NONE);
    assert(nsl_arr_swap_remove_int(&arr, 0).error == ARRAY_ERROR_EMPTY);
    memset(carr, 0, sizeof(carr));
}

void test_items_swap_removed_from_array_modify_length_correctly(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    (void)nsl_arr_swap_remove_int(&arr, 2);
    assert(arr.length == 2);
    (void)nsl_arr_swap_remove_int(&arr, 1);
    assert(arr.length == 1);
    (void)nsl_arr_swap_remove_int(&arr, 0);
    assert(arr.length == 0);
    memset(carr, 0, sizeof(carr));
}

void test_items_swap_removed_from_array_are_in_correct_order(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    assert(nsl_arr_swap_remove_int(&arr, 0).value == 1);
    assert(nsl_arr_swap_remove_int(&arr, 0).value == 3);
    assert(nsl_arr_swap_remove_int(&arr, 0).value == 2);
    memset(carr, 0, sizeof(carr));
}

void test_truncate_shortens_array_length(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    nsl_arr_truncate_int(&arr, 1);
    assert(arr.length == 1);
    memset(carr, 0, sizeof(carr));
}

void test_truncate_does_nothing_when_greater_than_length(void) {
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    nsl_arr_truncate_int(&arr, 3);
    assert(arr.length == 0);
    memset(carr, 0, sizeof(carr));
}

void test_extend_does_nothing_when_array_too_short(void) {
    NSL_IntArray     arr                 = nsl_arr_from_ptr_int(carr, n);
    constexpr size_t other_n             = 5;
    int              other_carr[other_n] = {1, 2, 3, 4, 5};
    NSL_IntArray     other               = nsl_arr_from_ptr_int(other_carr, other_n);
    other.length                         = other_n;
    assert(nsl_arr_extend_int(&arr, &other) == ARRAY_ERROR_FULL);
    assert(arr.length == 0);
    assert(other.length == 5);
    memset(carr, 0, sizeof(carr));
}

void test_extend_moves_items_from_one_array_to_another_in_order(void) {
    constexpr size_t other_n             = 5;
    int              other_carr[other_n] = {1, 2, 3, 4, 5};
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr   = nsl_arr_from_ptr_int(carr, n);
    arr.length         = n;
    NSL_IntArray other = nsl_arr_from_ptr_int(other_carr, other_n);
    other.length       = other_n - n;
    assert(nsl_arr_extend_int(&other, &arr) == ARRAY_ERROR_NONE);
    assert(arr.length == 0);
    assert(other.length == 5);

    int expected[other_n] = {1, 2, 1, 2, 3};
    assert(memcmp(other.items, expected, sizeof(expected)) == 0);
    memset(carr, 0, sizeof(carr));
}

void test_clear_removes_all_items_from_array(void) {
    for (size_t i = 0; i < n; i++) { carr[i] = i + 1; }
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    arr.length       = n;
    nsl_arr_clear_int(&arr);
    assert(arr.length == 0);
    memset(carr, 0, sizeof(carr));
}

void test_is_empty_returns_the_correct_result(void) {
    NSL_IntArray arr = nsl_arr_from_ptr_int(carr, n);
    assert(nsl_arr_is_empty_int(&arr));
    memset(carr, 0, sizeof(carr));
}

int main() {
    test_pushing_item_into_array_produces_correct_error();
    test_items_pushed_into_array_modify_length_correctly();
    test_items_pushed_into_array_are_in_correct_order();
    test_popping_item_from_array_produces_correct_error();
    test_items_popped_from_array_modify_length_correctly();
    test_items_popped_from_array_are_in_correct_order();
    test_inserting_item_into_array_produces_correct_error();
    test_items_inserted_into_array_modify_length_correctly();
    test_items_inserted_into_array_are_in_correct_order();
    test_removing_item_from_array_produces_correct_error();
    test_items_removed_from_array_modify_length_correctly();
    test_items_removed_from_array_are_in_correct_order();
    test_swap_removing_item_from_array_produces_correct_error();
    test_items_swap_removed_from_array_modify_length_correctly();
    test_items_swap_removed_from_array_are_in_correct_order();
    test_truncate_does_nothing_when_greater_than_length();
    test_truncate_shortens_array_length();
    test_extend_does_nothing_when_array_too_short();
    test_extend_moves_items_from_one_array_to_another_in_order();
    test_clear_removes_all_items_from_array();
    test_is_empty_returns_the_correct_result();
}
