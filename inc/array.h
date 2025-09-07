/**************************************************************************/
/* The MIT License (MIT)                                                  */
/*                                                                        */
/* Copyright (c) 2025 Jacob Long                                          */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

/*!
 * This module provides a wrapper around C's builtin arrays. The wrapper is
 * typesafe and keeps track of the length and capacity of the array, avoiding
 * the primary problem of arrays decaying to pointers. The `Array` wrapper is
 * implemented using the `NSL_DEFINE_ARRAY` / `NSL_DEFINE_FUNC_PTR_ARRAY`
 * macros. These macros will produce the implementation of the array for a
 * specific type.
 *
 * # Example
 *
 * ```c
 * NSL_DEFINE_ARRAY(int, Int, int)
 *
 * void some_function(void) {
 *     int carr[5] = {0};
 *     NSL_IntArray = nsl_arr_from_ptr_int(carr, sizeof(carr) / sizeof(carr[0]));
 *     (void)nsl_arr_push_int(&arr, 1);
 *     (void)nsl_arr_push_int(&arr, 2);
 *     (void)nsl_arr_push_int(&arr, 3);
 *     assert(arr.length == 3);
 *     assert(nsl_arr_pop_int(&arr).value == 3);
 *     assert(nsl_arr_pop_int(&arr).value == 2);
 *     assert(nsl_arr_pop_int(&arr).value == 1);
 *     assert(nsl_arr_pop_int(&arr).error == NSL_ARRAY_ERROR_EMPTY);
 * }
 * ```
 *
 * # Macro Flags
 *
 * - `NSL_ARRAY_STRIP_PREFIX` - Defining this macro before including this file
 *   will cause the provided macros to be available without the `NSL_` prefix.
 *   For example, `NSL_DEFINE_ARRAY` will be available as `DEFINE_ARRAY`.
 *   NOTE: This does not remove prefix of array types or array functions (e.g.
 *   `NSL_IntArray` would **not** become `IntArray`).
 */

#ifndef NSL_ARRAY_H_
#define NSL_ARRAY_H_

#define NSL_ARRAY_MAJOR 0
#define NSL_ARRAY_MINOR 1
#define NSL_ARRAY_PATCH 0

#include <stddef.h>

#define NSL__DEFINE_GENERIC_ARRAY(ArrayType,                                                       \
                                  ArrayResultType,                                                 \
                                  func_suffix,                                                     \
                                  items_with_type,                                                 \
                                  item_with_type,                                                  \
                                  value_with_type,                                                 \
                                  declaration_specifiers)                                          \
    typedef struct ArrayType {                                                                     \
        items_with_type;                                                                           \
        size_t       length;                                                                       \
        const size_t capacity;                                                                     \
    } ArrayType;                                                                                   \
                                                                                                   \
    typedef struct ArrayResultType {                                                               \
        value_with_type;                                                                           \
        ArrayError error;                                                                          \
    } ArrayResultType;                                                                             \
                                                                                                   \
    [[nodiscard]]                                                                                  \
    declaration_specifiers ArrayType nsl_arr_from_ptr_##func_suffix(items_with_type,               \
                                                                    size_t capacity) {             \
        return (ArrayType) {.items = items, .length = 0, .capacity = capacity};                    \
    }                                                                                              \
                                                                                                   \
    [[nodiscard]]                                                                                  \
    declaration_specifiers ArrayError nsl_arr_push_##func_suffix(ArrayType *arr, item_with_type) { \
        if (arr->length == arr->capacity) { return NSL_ARRAY_ERROR_FULL; }                         \
        arr->items[arr->length] = item;                                                            \
        arr->length++;                                                                             \
        return NSL_ARRAY_ERROR_NONE;                                                               \
    }                                                                                              \
                                                                                                   \
    [[nodiscard]]                                                                                  \
    declaration_specifiers ArrayResultType nsl_arr_pop_##func_suffix(ArrayType *arr) {             \
        if (arr->length == 0) { return (ArrayResultType) {.error = NSL_ARRAY_ERROR_EMPTY}; }       \
        arr->length--;                                                                             \
        return (ArrayResultType) {.error = NSL_ARRAY_ERROR_NONE,                                   \
                                  .value = arr->items[arr->length]};                               \
    }                                                                                              \
                                                                                                   \
    [[nodiscard]]                                                                                  \
    declaration_specifiers ArrayError nsl_arr_insert_##func_suffix(ArrayType *arr,                 \
                                                                   size_t     idx,                 \
                                                                   item_with_type) {               \
        if (arr->length == arr->capacity) { return NSL_ARRAY_ERROR_FULL; }                         \
        if (idx > arr->length) { return NSL_ARRAY_ERROR_BAD_IDX; }                                 \
                                                                                                   \
        for (size_t i = arr->length; i > idx; i--) { arr->items[i] = arr->items[i - 1]; }          \
        arr->items[idx] = item;                                                                    \
        arr->length++;                                                                             \
        return NSL_ARRAY_ERROR_NONE;                                                               \
    }                                                                                              \
                                                                                                   \
    [[nodiscard]]                                                                                  \
    declaration_specifiers ArrayResultType nsl_arr_remove_##func_suffix(ArrayType *arr,            \
                                                                        size_t     idx) {              \
        if (arr->length == 0) { return (ArrayResultType) {.error = NSL_ARRAY_ERROR_EMPTY}; }       \
        if (idx >= arr->length) { return (ArrayResultType) {.error = NSL_ARRAY_ERROR_BAD_IDX}; }   \
        ArrayResultType ret_val =                                                                  \
            (ArrayResultType) {.error = NSL_ARRAY_ERROR_NONE, .value = arr->items[idx]};           \
        for (size_t i = idx; i < arr->length - 1; i++) { arr->items[i] = arr->items[i + 1]; }      \
        arr->length--;                                                                             \
        return ret_val;                                                                            \
    }                                                                                              \
                                                                                                   \
    [[nodiscard]]                                                                                  \
    declaration_specifiers ArrayResultType nsl_arr_swap_remove_##func_suffix(ArrayType *arr,       \
                                                                             size_t     idx) {         \
        if (arr->length == 0) { return (ArrayResultType) {.error = NSL_ARRAY_ERROR_EMPTY}; }       \
        if (idx >= arr->length) { return (ArrayResultType) {.error = NSL_ARRAY_ERROR_BAD_IDX}; }   \
        ArrayResultType ret_val =                                                                  \
            (ArrayResultType) {.error = NSL_ARRAY_ERROR_NONE, .value = arr->items[idx]};           \
        arr->length--;                                                                             \
        arr->items[idx] = arr->items[arr->length];                                                 \
        return ret_val;                                                                            \
    }                                                                                              \
                                                                                                   \
    declaration_specifiers void nsl_arr_truncate_##func_suffix(ArrayType *arr, size_t length) {    \
        if (arr->length > length) { arr->length = length; }                                        \
    }                                                                                              \
                                                                                                   \
    [[nodiscard]]                                                                                  \
    declaration_specifiers ArrayError nsl_arr_extend_##func_suffix(ArrayType *arr,                 \
                                                                   ArrayType *other) {             \
        if (arr->capacity - arr->length < other->length) { return NSL_ARRAY_ERROR_FULL; }          \
        for (size_t i = 0; i < other->length; i++, arr->length++) {                                \
            arr->items[arr->length] = other->items[i];                                             \
        }                                                                                          \
        other->length = 0;                                                                         \
        return NSL_ARRAY_ERROR_NONE;                                                               \
    }                                                                                              \
                                                                                                   \
    declaration_specifiers void nsl_arr_clear_##func_suffix(ArrayType *arr) {                      \
        arr->length = 0;                                                                           \
    }                                                                                              \
                                                                                                   \
    [[nodiscard]]                                                                                  \
    declaration_specifiers bool nsl_arr_is_empty_##func_suffix(ArrayType *arr) {                   \
        return arr->length == 0;                                                                   \
    }

/*!
 * Defines a new array that contains elements of `type`. This includes defining
 * the relevant structs and functions for interacting with the array. The names
 * chosen should be unique to the type, otherwise there will be type / function
 * conflicts at compile / link time. NOTE: The same type can be used for
 * multiple different names, but not the other way around. This is useful if you
 * want distinction between objects that have the same type, but different
 * meanings (e.g. velocity and acceleration both defined using `float`, but they
 * mean different things and should be kept separate).
 *
 * ```c
 * NSL_DEFINE_ARRAY(int, Int, int)
 *
 * void some_function(void) {
 *    int carr[3];
 *    NSL_IntArray arr = { .items = carr, .length = 0, .capacity = 3 };
 *
 *    assert(nsl_arr_push_int(&arr, 1) == NSL_ARRAY_ERROR_NONE);
 *    assert(nsl_arr_push_int(&arr, 2) == NSL_ARRAY_ERROR_NONE);
 *    assert(nsl_arr_push_int(&arr, 3) == NSL_ARRAY_ERROR_NONE);
 *    assert(nsl_arr_push_int(&arr, 4) == NSL_ARRAY_ERROR_FULL);
 *
 *    assert(nsl_arr_pop_int(&arr).value == 3);
 *    assert(nsl_arr_pop_int(&arr).value == 2);
 *    assert(nsl_arr_pop_int(&arr).value == 1);
 *    assert(nsl_arr_pop_int(&arr).error == NSL_ARRAY_ERROR_EMPTY);
 * }
 * ```
 *
 * # Parameters
 * - `type` - The type of object that the array should hold (e.g. `int`,
 *   `float`, `struct ...`, etc.).
 * - `uppername` - The name to associate with the type. This should be pascal
 *   case so that it fits in with NSL's naming scheme.
 * - `lowername` - The name to associate with functions. This should be snake
 *   case so that it fits in with NSL's naming scheme.
 *
 * # Requires
 * - `type` is a valid type accessible in the current scope.
 * - `type` is not the type of a function pointer.
 * - `uppername` and `lowername` are both valid parts of an identifier. In other
 *   words, they can be part of a type / function name, respectively.
 * - `uppername` and `lowername` have not been used to define other arrays.
 */
#define NSL_DEFINE_ARRAY(type, uppername, lowername)                                               \
    NSL_DEFINE_ARRAY_WITH_SPECS(type, uppername, lowername, )

/*!
 * This produces the same results as `NSL_DEFINE_ARRAY`, but all of the
 * associated functions will have `specs` appended to their declarations. For
 * example, `NSL_DEFINE_ARRAY_DEF(int, Int, int, inline)` would result in all
 * functions associated with `IntArray` being declared as inline.
 *
 * # Parameters
 * - `type` - The type of object that the array should hold (e.g. `int`,
 *   `float`, `struct ...`, etc.).
 * - `uppername` - The name to associate with the type. This should be pascal
 *   case so that it fits in with NSL's naming scheme.
 * - `lowername` - The name to associate with functions. This should be snake
 *   case so that it fits in with NSL's naming scheme.
 * - `specs` - The funciton specifier(s) that all generated functions should be
 *   declared with.
 *
 * # Requires
 * - Everything required by `NSL_DEFINE_ARRAY`.
 * - `specs` must be a valid sequence of function specifier(s).
 */
#define NSL_DEFINE_ARRAY_WITH_SPECS(type, uppername, lowername, specs)                             \
    NSL__DEFINE_GENERIC_ARRAY(NSL_##uppername##Array,                                              \
                              NSL_##uppername##ArrayResult,                                        \
                              lowername,                                                           \
                              type *const items,                                                   \
                              type        item,                                                    \
                              type        value,                                                   \
                              specs)
/*!
 * Defines a new array that contains function pointers that return `result` and
 * have `parameters`. This functions in the same way as `NSL_DEFINE_ARRAY`, but
 * is specifically for arrays of function pointers.
 *
 * ```c NSL_DEFINE_FUNC_PTR_ARRAY(bool, (int, int), Compare, compare)
 *
 * bool greater_than(int x, int y) { return x > y; }
 * bool less_than(int x, int y) { return x < y; }
 * bool equal(int x, int y) { return x == y; }
 * bool unequal(int x, int y) { return x != y; }
 *
 * void some_function(void) {
 *    bool (*carr)(int, int)[3];
 *    NSL_CompareFuncPtrArray arr = { .items = carr, .length = 0, .capacity = 3 };
 *
 *    assert(nsl_arr_push_compare(&arr, greater_than) == NSL_ARRAY_ERROR_NONE);
 *    assert(nsl_arr_push_compare(&arr, less_than) == NSL_ARRAY_ERROR_NONE);
 *    assert(nsl_arr_push_compare(&arr, equal) == NSL_ARRAY_ERROR_NONE);
 *    assert(nsl_arr_push_compare(&arr, unequal) == NSL_ARRAY_ERROR_FULL);
 *
 *    assert(nsl_arr_pop_compare(&arr).value == equal);
 *    assert(nsl_arr_pop_compare(&arr).value == less_than);
 *    assert(nsl_arr_pop_compare(&arr).value == greater_than);
 *    assert(nsl_arr_pop_compare(&arr).error == NSL_ARRAY_ERROR_EMPTY);
 * }
 * ```
 *
 * # Parameters
 * - `result` - The type of the result that is returned by the function.
 * - `parameters` - The type of the parameters accepted by the function.
 * - `uppername` - The name to associate with the type. This should be pascal
 *   case so that it fits in with NSL's naming scheme.
 * - `lowername` - The name to associate with functions. This should be snake
 *   case so that it fits in with NSL's naming scheme.
 *
 * # Requires
 * - `result` is a valid type accessible in the current scope.
 * - `parameters` is zero or more types separated by commas and enclosed in
 *   parenthesis. All types are valid and accessible in the current scope.
 * - `uppername` and `lowername` are both valid parts of an identifier. In other
 *   words, they can be part of a type / function name, respectively.
 * - `uppername` and `lowername` have not been used to define other arrays.
 */
#define NSL_DEFINE_FUNC_PTR_ARRAY(result, parameters, uppername, lowername)                        \
    NSL_DEFINE_FUNC_PTR_ARRAY_WITH_SPECS(result, parameters, uppername, lowername, )

/*!
 * This produces the same results as `NSL_DEFINE_FUNC_PTR_ARRAY`, but all of the
 * generated functions will have `specs` in their declarations. For example,
 * `NSL_DEFINE_FUNC_PTR_ARRAY_DEF(bool, (int, int), Compare, compare, inline)`
 * would result in all functions associated with `CompareArray` being declared
 * as inline.
 *
 * # Parameters
 * - `result` - The type of the result that is returned by the function.
 * - `parameters` - The type of the parameters accepted by the function.
 * - `uppername` - The name to associate with the type. This should be pascal
 *   case so that it fits in with NSL's naming scheme.
 * - `lowername` - The name to associate with functions. This should be snake
 *   case so that it fits in with NSL's naming scheme.
 * - `specs` - The funciton specifier(s) that all generated functions should be
 *   declared with.
 *
 * # Requires
 * - Everything required by `NSL_DEFINE_FUNC_PTR_ARRAY`.
 * - `specs` must be a valid sequence of function specifier(s).
 */
#define NSL_DEFINE_FUNC_PTR_ARRAY_WITH_SPECS(result, parameters, uppername, lowername, specs)      \
    NSL__DEFINE_GENERIC_ARRAY(NSL_##uppername##FuncPtrArray,                                       \
                              NSL_##uppername##FuncPtrArrayResult,                                 \
                              lowername,                                                           \
                              result(**const items) parameters,                                    \
                              result(*item) parameters,                                            \
                              result(*value) parameters,                                           \
                              specs)

//! Specifies why the operation failed.
enum NSL_ArrayError {
    //! No error. The operation succeeded.
    NSL_ARRAY_ERROR_NONE,
    //! The array is full. More items cannot be added to it.
    NSL_ARRAY_ERROR_FULL,
    //! The array is empty. No items can be removed from it.
    NSL_ARRAY_ERROR_EMPTY,
    //! The provided index is not valid.
    NSL_ARRAY_ERROR_BAD_IDX,
};
typedef enum NSL_ArrayError NSL_ArrayError;

#ifndef NSL_ARRAY_DEF
#    define NSL_ARRAY_DEF
#endif  // NSL_ARRAY_DEF

#endif  // NSL_ARRAY_H_

#ifndef NSL_ARRAY_STRIP_PREFIX_GUARD_
#define NSL_ARRAY_STRIP_PREFIX_GUARD_

#ifdef NSL_ARRAY_STRIP_PREFIX
#    define DEFINE_ARRAY                     NSL_DEFINE_ARRAY
#    define DEFINE_ARRAY_WITH_SPECS          NSL_DEFINE_ARRAY_WITH_SPECS
#    define DEFINE_FUNC_PTR_ARRAY            NSL_DEFINE_FUNC_PTR_ARRAY
#    define DEFINE_FUNC_PTR_ARRAY_WITH_SPECS NSL_DEFINE_FUNC_PTR_ARRAY_WITH_SPECS
#    define ArrayError                       NSL_ArrayError
#    define ARRAY_ERROR_NONE                 NSL_ARRAY_ERROR_NONE
#    define ARRAY_ERROR_FULL                 NSL_ARRAY_ERROR_FULL
#    define ARRAY_ERROR_EMPTY                NSL_ARRAY_ERROR_EMPTY
#    define ARRAY_ERROR_BAD_IDX              NSL_ARRAY_ERROR_BAD_IDX
#endif  // NSL_ARRAY_STRIP_PREFIX

#endif  // NSL_ARRAY_STRIP_PREFIX_GUARD_
