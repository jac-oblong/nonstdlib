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
 * Implementation of the macro magic that makes **NonStdLib** possible. None of
 * the macros here are public facing (i.e. designed for public use). However,
 * many of the macros can be used without causing problems. These macros follow
 * the same prefix pattern used by the rest of the library. Any macro that can
 * be safely used is prefixed with `NSL_` and any that cannot is prefixed with
 * `NSL__`.
 */

#ifndef NSL_MAGIC_H_
#define NSL_MAGIC_H_

/*!
 * Returns the number of arguments provided. For example, `NSL_NARGS(a, b, c)`
 * would evaluate to `3` because there are 3 arguments.
 *
 * `NSL_NARGS` first redirects to `NSL__NARGS`. Without this redirection,
 * `NSL__REVERSE_SEQUENCE_N` will not be expanded and instead treated as one
 * token. `NSL__NARGS` is a straight redirection to `NSL__ARG_N` which just
 * chooses the Nth argument. As more arguments are provided to `NSL_NARGS`, the
 * location of the Nth is pushed towards the front of `NSL__REVERSE_SEQUENCE_N`,
 * which correlates with a higher value. In short, `NSL__ARG_N` acts as a
 * sliding window over `NSL__REVERSE_SEQUENCE_N`. `NSL__NARGS` ensures that all
 * necessary layers of indirection are present to fully expand the macros. And
 * finally, `NSL_NARGS` provides the extra arguments to push the sliding window.
 *
 * # Requirements
 * - The number of arguments is less than 128. If 128 or more arguments are
 *   provided, the macro will evaluate to the 128th argument.
 *
 * # Returns
 * The number of arguments, N, provided. If N >= 128, then the 128th argument.
 */
// clang-format off
#define NSL__ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, \
                   _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26,  \
                   _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38,  \
                   _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
                   _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62,  \
                   _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74,  \
                   _75, _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86,  \
                   _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98,  \
                   _99, _100, _101, _102, _103, _104, _105, _106, _107, _108,   \
                   _109, _110, _111, _112, _113, _114, _115, _116, _117, _118,  \
                   _119, _120, _121, _122, _123, _124, _125, _126, _127,        \
                   N, ...) N
#define NSL__REVERSE_SEQUENCE_N                                                 \
                                       127, 126, 125, 124, 123, 122, 121, 120,  \
    119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105,  \
    104, 103, 102, 101, 100,  99,  98,  97,  96,  95,  94,  93,  92,  91,  90,  \
     89,  88,  87,  86,  85,  84,  83,  82,  81,  80,  79,  78,  77,  76,  75,  \
     74,  73,  72,  71,  70,  69,  68,  67,  66,  65,  64,  63,  62,  61,  60,  \
     59,  58,  57,  56,  55,  54,  53,  52,  51,  50,  49,  48,  47,  46,  45,  \
     44,  43,  42,  41,  40,  39,  38,  37,  36,  35,  34,  33,  32,  31,  30,  \
     29,  28,  27,  26,  25,  24,  23,  22,  21,  20,  19,  18,  17,  16,  15,  \
     14,  13,  12,  11,  10,   9,   8,   7,   6,   5,   4,   3,   2,   1,   0
// clang-format on
#define NSL__NARGS(...) NSL__ARG_N(__VA_ARGS__)
#define NSL_NARGS(...)  NSL__NARGS(__VA_ARGS__ __VA_OPT__(, ) NSL__REVERSE_SEQUENCE_N)

/*!
 * Concatenates the two provided arguments into one. For example,
 * `NSL_CAT(foo, bar)` would be expanded by the preprocessor to `foobar`.
 *
 * `NSL_CAT` requires two levels of indirection. This is because the
 * preprocessor does not recursively expand macros if `#` or `##` are present.
 * For example, `A ## __LINE__` would result in `A__LINE__` instead of `A15` (or
 * whatever line number it is). By introducing a second layer of indirection,
 * the `__LINE__` is forced to be expanded to `15`, at which point `A ## 15`
 * will provide the correct result.
 *
 * # Parameters
 * - `a`: The first part of the full name.
 * - `b`: The second part of the full name.
 *
 * # Returns
 * The concatenated full name.
 */
#define NSL__CAT(a, b) a##b
#define NSL_CAT(a, b)  NSL__CAT(a, b)

/*!
 * Evaluates to the head of the variadic arguments (i.e. the first argument).
 *
 * `NSL_VA_HEAD` requires two levels of indirection. This is to ensure that
 * passing a macro that expands to the variadic arguments works correctly and is
 * expanded.
 *
 * # Returns
 * The first argument in the variadic list of arguments, if any exists.
 */
#define NSL__VA_HEAD(head, ...) head
#define NSL_VA_HEAD(...)        NSL__VA_HEAD(__VA_ARGS__)

/*!
 * Evaluates to the rest of the variadic arguments (i.e. all arguments except
 * for the first).
 *
 * `NSL_VA_REST` requires two levels of indirection. This is to ensure that
 * passing a macro that expands to the variadic arguments works correctly and is
 * expanded.
 *
 * # Returns
 * The rest of the arguments in the variadic list of arguments, if any exists.
 */
#define NSL__VA_REST(head, ...) __VA_ARGS__
#define NSL_VA_REST(...)        NSL__VA_REST(__VA_ARGS__)

#endif  // NSL_MAGIC_H_
