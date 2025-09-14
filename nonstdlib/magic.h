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
#define NSL_NARGS(...)  NSL__NARGS(__VA_ARGS__ __VA_OPT__(, ) NSL__REVERSE_SEQUENCE_N)
#define NSL__NARGS(...) NSL__ARG_N(__VA_ARGS__)
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
#define NSL_CAT(a, b)  NSL__CAT(a, b)
#define NSL__CAT(a, b) a##b

/*!
 * Concatenates all provided arguments into one. For example,
 * `NSL_NCAT(foo, bar, bar, baz, qux)` would be expanded by the preprocessor to
 * `foobarbarbazqux`.
 *
 * `NSL_NCAT` requires two levels of indirection. This is because the
 * preprocessor does not recursively expand macros if `#` or `##` are present.
 * `NSL__NCAT` will call the correct "recursive" macro to concatenate all
 * arguments.
 *
 * # Requirements
 * - The number of parameters is less than 128. If 128 or more arguments are
 *   provided, a compile time error is likely to be generated. There is no
 *   guarantee of this, however.
 *
 * # Returns
 * The concatenated full name.
 */
#define NSL_NCAT(...)  NSL__NCAT(__VA_ARGS__)
#define NSL__NCAT(...) NSL_CAT(NSL__NCAT, NSL_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define NSL__NCAT0()
#define NSL__NCAT1(x)        x
#define NSL__NCAT2(x, ...)   NSL_CAT(x, NSL__NCAT1(__VA_ARGS__))
#define NSL__NCAT3(x, ...)   NSL_CAT(x, NSL__NCAT2(__VA_ARGS__))
#define NSL__NCAT4(x, ...)   NSL_CAT(x, NSL__NCAT3(__VA_ARGS__))
#define NSL__NCAT5(x, ...)   NSL_CAT(x, NSL__NCAT4(__VA_ARGS__))
#define NSL__NCAT6(x, ...)   NSL_CAT(x, NSL__NCAT5(__VA_ARGS__))
#define NSL__NCAT7(x, ...)   NSL_CAT(x, NSL__NCAT6(__VA_ARGS__))
#define NSL__NCAT8(x, ...)   NSL_CAT(x, NSL__NCAT7(__VA_ARGS__))
#define NSL__NCAT9(x, ...)   NSL_CAT(x, NSL__NCAT8(__VA_ARGS__))
#define NSL__NCAT10(x, ...)  NSL_CAT(x, NSL__NCAT9(__VA_ARGS__))
#define NSL__NCAT11(x, ...)  NSL_CAT(x, NSL__NCAT10(__VA_ARGS__))
#define NSL__NCAT12(x, ...)  NSL_CAT(x, NSL__NCAT11(__VA_ARGS__))
#define NSL__NCAT13(x, ...)  NSL_CAT(x, NSL__NCAT12(__VA_ARGS__))
#define NSL__NCAT14(x, ...)  NSL_CAT(x, NSL__NCAT13(__VA_ARGS__))
#define NSL__NCAT15(x, ...)  NSL_CAT(x, NSL__NCAT14(__VA_ARGS__))
#define NSL__NCAT16(x, ...)  NSL_CAT(x, NSL__NCAT15(__VA_ARGS__))
#define NSL__NCAT17(x, ...)  NSL_CAT(x, NSL__NCAT16(__VA_ARGS__))
#define NSL__NCAT18(x, ...)  NSL_CAT(x, NSL__NCAT17(__VA_ARGS__))
#define NSL__NCAT19(x, ...)  NSL_CAT(x, NSL__NCAT18(__VA_ARGS__))
#define NSL__NCAT20(x, ...)  NSL_CAT(x, NSL__NCAT19(__VA_ARGS__))
#define NSL__NCAT21(x, ...)  NSL_CAT(x, NSL__NCAT20(__VA_ARGS__))
#define NSL__NCAT22(x, ...)  NSL_CAT(x, NSL__NCAT21(__VA_ARGS__))
#define NSL__NCAT23(x, ...)  NSL_CAT(x, NSL__NCAT22(__VA_ARGS__))
#define NSL__NCAT24(x, ...)  NSL_CAT(x, NSL__NCAT23(__VA_ARGS__))
#define NSL__NCAT25(x, ...)  NSL_CAT(x, NSL__NCAT24(__VA_ARGS__))
#define NSL__NCAT26(x, ...)  NSL_CAT(x, NSL__NCAT25(__VA_ARGS__))
#define NSL__NCAT27(x, ...)  NSL_CAT(x, NSL__NCAT26(__VA_ARGS__))
#define NSL__NCAT28(x, ...)  NSL_CAT(x, NSL__NCAT27(__VA_ARGS__))
#define NSL__NCAT29(x, ...)  NSL_CAT(x, NSL__NCAT28(__VA_ARGS__))
#define NSL__NCAT30(x, ...)  NSL_CAT(x, NSL__NCAT29(__VA_ARGS__))
#define NSL__NCAT31(x, ...)  NSL_CAT(x, NSL__NCAT30(__VA_ARGS__))
#define NSL__NCAT32(x, ...)  NSL_CAT(x, NSL__NCAT31(__VA_ARGS__))
#define NSL__NCAT33(x, ...)  NSL_CAT(x, NSL__NCAT32(__VA_ARGS__))
#define NSL__NCAT34(x, ...)  NSL_CAT(x, NSL__NCAT33(__VA_ARGS__))
#define NSL__NCAT35(x, ...)  NSL_CAT(x, NSL__NCAT34(__VA_ARGS__))
#define NSL__NCAT36(x, ...)  NSL_CAT(x, NSL__NCAT35(__VA_ARGS__))
#define NSL__NCAT37(x, ...)  NSL_CAT(x, NSL__NCAT36(__VA_ARGS__))
#define NSL__NCAT38(x, ...)  NSL_CAT(x, NSL__NCAT37(__VA_ARGS__))
#define NSL__NCAT39(x, ...)  NSL_CAT(x, NSL__NCAT38(__VA_ARGS__))
#define NSL__NCAT40(x, ...)  NSL_CAT(x, NSL__NCAT39(__VA_ARGS__))
#define NSL__NCAT41(x, ...)  NSL_CAT(x, NSL__NCAT40(__VA_ARGS__))
#define NSL__NCAT42(x, ...)  NSL_CAT(x, NSL__NCAT41(__VA_ARGS__))
#define NSL__NCAT43(x, ...)  NSL_CAT(x, NSL__NCAT42(__VA_ARGS__))
#define NSL__NCAT44(x, ...)  NSL_CAT(x, NSL__NCAT43(__VA_ARGS__))
#define NSL__NCAT45(x, ...)  NSL_CAT(x, NSL__NCAT44(__VA_ARGS__))
#define NSL__NCAT46(x, ...)  NSL_CAT(x, NSL__NCAT45(__VA_ARGS__))
#define NSL__NCAT47(x, ...)  NSL_CAT(x, NSL__NCAT46(__VA_ARGS__))
#define NSL__NCAT48(x, ...)  NSL_CAT(x, NSL__NCAT47(__VA_ARGS__))
#define NSL__NCAT49(x, ...)  NSL_CAT(x, NSL__NCAT48(__VA_ARGS__))
#define NSL__NCAT50(x, ...)  NSL_CAT(x, NSL__NCAT49(__VA_ARGS__))
#define NSL__NCAT51(x, ...)  NSL_CAT(x, NSL__NCAT50(__VA_ARGS__))
#define NSL__NCAT52(x, ...)  NSL_CAT(x, NSL__NCAT51(__VA_ARGS__))
#define NSL__NCAT53(x, ...)  NSL_CAT(x, NSL__NCAT52(__VA_ARGS__))
#define NSL__NCAT54(x, ...)  NSL_CAT(x, NSL__NCAT53(__VA_ARGS__))
#define NSL__NCAT55(x, ...)  NSL_CAT(x, NSL__NCAT54(__VA_ARGS__))
#define NSL__NCAT56(x, ...)  NSL_CAT(x, NSL__NCAT55(__VA_ARGS__))
#define NSL__NCAT57(x, ...)  NSL_CAT(x, NSL__NCAT56(__VA_ARGS__))
#define NSL__NCAT58(x, ...)  NSL_CAT(x, NSL__NCAT57(__VA_ARGS__))
#define NSL__NCAT59(x, ...)  NSL_CAT(x, NSL__NCAT58(__VA_ARGS__))
#define NSL__NCAT60(x, ...)  NSL_CAT(x, NSL__NCAT59(__VA_ARGS__))
#define NSL__NCAT61(x, ...)  NSL_CAT(x, NSL__NCAT60(__VA_ARGS__))
#define NSL__NCAT62(x, ...)  NSL_CAT(x, NSL__NCAT61(__VA_ARGS__))
#define NSL__NCAT63(x, ...)  NSL_CAT(x, NSL__NCAT62(__VA_ARGS__))
#define NSL__NCAT64(x, ...)  NSL_CAT(x, NSL__NCAT63(__VA_ARGS__))
#define NSL__NCAT65(x, ...)  NSL_CAT(x, NSL__NCAT64(__VA_ARGS__))
#define NSL__NCAT66(x, ...)  NSL_CAT(x, NSL__NCAT65(__VA_ARGS__))
#define NSL__NCAT67(x, ...)  NSL_CAT(x, NSL__NCAT66(__VA_ARGS__))
#define NSL__NCAT68(x, ...)  NSL_CAT(x, NSL__NCAT67(__VA_ARGS__))
#define NSL__NCAT69(x, ...)  NSL_CAT(x, NSL__NCAT68(__VA_ARGS__))
#define NSL__NCAT70(x, ...)  NSL_CAT(x, NSL__NCAT69(__VA_ARGS__))
#define NSL__NCAT71(x, ...)  NSL_CAT(x, NSL__NCAT70(__VA_ARGS__))
#define NSL__NCAT72(x, ...)  NSL_CAT(x, NSL__NCAT71(__VA_ARGS__))
#define NSL__NCAT73(x, ...)  NSL_CAT(x, NSL__NCAT72(__VA_ARGS__))
#define NSL__NCAT74(x, ...)  NSL_CAT(x, NSL__NCAT73(__VA_ARGS__))
#define NSL__NCAT75(x, ...)  NSL_CAT(x, NSL__NCAT74(__VA_ARGS__))
#define NSL__NCAT76(x, ...)  NSL_CAT(x, NSL__NCAT75(__VA_ARGS__))
#define NSL__NCAT77(x, ...)  NSL_CAT(x, NSL__NCAT76(__VA_ARGS__))
#define NSL__NCAT78(x, ...)  NSL_CAT(x, NSL__NCAT77(__VA_ARGS__))
#define NSL__NCAT79(x, ...)  NSL_CAT(x, NSL__NCAT78(__VA_ARGS__))
#define NSL__NCAT80(x, ...)  NSL_CAT(x, NSL__NCAT79(__VA_ARGS__))
#define NSL__NCAT81(x, ...)  NSL_CAT(x, NSL__NCAT80(__VA_ARGS__))
#define NSL__NCAT82(x, ...)  NSL_CAT(x, NSL__NCAT81(__VA_ARGS__))
#define NSL__NCAT83(x, ...)  NSL_CAT(x, NSL__NCAT82(__VA_ARGS__))
#define NSL__NCAT84(x, ...)  NSL_CAT(x, NSL__NCAT83(__VA_ARGS__))
#define NSL__NCAT85(x, ...)  NSL_CAT(x, NSL__NCAT84(__VA_ARGS__))
#define NSL__NCAT86(x, ...)  NSL_CAT(x, NSL__NCAT85(__VA_ARGS__))
#define NSL__NCAT87(x, ...)  NSL_CAT(x, NSL__NCAT86(__VA_ARGS__))
#define NSL__NCAT88(x, ...)  NSL_CAT(x, NSL__NCAT87(__VA_ARGS__))
#define NSL__NCAT89(x, ...)  NSL_CAT(x, NSL__NCAT88(__VA_ARGS__))
#define NSL__NCAT90(x, ...)  NSL_CAT(x, NSL__NCAT89(__VA_ARGS__))
#define NSL__NCAT91(x, ...)  NSL_CAT(x, NSL__NCAT90(__VA_ARGS__))
#define NSL__NCAT92(x, ...)  NSL_CAT(x, NSL__NCAT91(__VA_ARGS__))
#define NSL__NCAT93(x, ...)  NSL_CAT(x, NSL__NCAT92(__VA_ARGS__))
#define NSL__NCAT94(x, ...)  NSL_CAT(x, NSL__NCAT93(__VA_ARGS__))
#define NSL__NCAT95(x, ...)  NSL_CAT(x, NSL__NCAT94(__VA_ARGS__))
#define NSL__NCAT96(x, ...)  NSL_CAT(x, NSL__NCAT95(__VA_ARGS__))
#define NSL__NCAT97(x, ...)  NSL_CAT(x, NSL__NCAT96(__VA_ARGS__))
#define NSL__NCAT98(x, ...)  NSL_CAT(x, NSL__NCAT97(__VA_ARGS__))
#define NSL__NCAT99(x, ...)  NSL_CAT(x, NSL__NCAT98(__VA_ARGS__))
#define NSL__NCAT100(x, ...) NSL_CAT(x, NSL__NCAT99(__VA_ARGS__))
#define NSL__NCAT101(x, ...) NSL_CAT(x, NSL__NCAT100(__VA_ARGS__))
#define NSL__NCAT102(x, ...) NSL_CAT(x, NSL__NCAT101(__VA_ARGS__))
#define NSL__NCAT103(x, ...) NSL_CAT(x, NSL__NCAT102(__VA_ARGS__))
#define NSL__NCAT104(x, ...) NSL_CAT(x, NSL__NCAT103(__VA_ARGS__))
#define NSL__NCAT105(x, ...) NSL_CAT(x, NSL__NCAT104(__VA_ARGS__))
#define NSL__NCAT106(x, ...) NSL_CAT(x, NSL__NCAT105(__VA_ARGS__))
#define NSL__NCAT107(x, ...) NSL_CAT(x, NSL__NCAT106(__VA_ARGS__))
#define NSL__NCAT108(x, ...) NSL_CAT(x, NSL__NCAT107(__VA_ARGS__))
#define NSL__NCAT109(x, ...) NSL_CAT(x, NSL__NCAT108(__VA_ARGS__))
#define NSL__NCAT110(x, ...) NSL_CAT(x, NSL__NCAT109(__VA_ARGS__))
#define NSL__NCAT111(x, ...) NSL_CAT(x, NSL__NCAT110(__VA_ARGS__))
#define NSL__NCAT112(x, ...) NSL_CAT(x, NSL__NCAT111(__VA_ARGS__))
#define NSL__NCAT113(x, ...) NSL_CAT(x, NSL__NCAT112(__VA_ARGS__))
#define NSL__NCAT114(x, ...) NSL_CAT(x, NSL__NCAT113(__VA_ARGS__))
#define NSL__NCAT115(x, ...) NSL_CAT(x, NSL__NCAT114(__VA_ARGS__))
#define NSL__NCAT116(x, ...) NSL_CAT(x, NSL__NCAT115(__VA_ARGS__))
#define NSL__NCAT117(x, ...) NSL_CAT(x, NSL__NCAT116(__VA_ARGS__))
#define NSL__NCAT118(x, ...) NSL_CAT(x, NSL__NCAT117(__VA_ARGS__))
#define NSL__NCAT119(x, ...) NSL_CAT(x, NSL__NCAT118(__VA_ARGS__))
#define NSL__NCAT120(x, ...) NSL_CAT(x, NSL__NCAT119(__VA_ARGS__))
#define NSL__NCAT121(x, ...) NSL_CAT(x, NSL__NCAT120(__VA_ARGS__))
#define NSL__NCAT122(x, ...) NSL_CAT(x, NSL__NCAT121(__VA_ARGS__))
#define NSL__NCAT123(x, ...) NSL_CAT(x, NSL__NCAT122(__VA_ARGS__))
#define NSL__NCAT124(x, ...) NSL_CAT(x, NSL__NCAT123(__VA_ARGS__))
#define NSL__NCAT125(x, ...) NSL_CAT(x, NSL__NCAT124(__VA_ARGS__))
#define NSL__NCAT126(x, ...) NSL_CAT(x, NSL__NCAT125(__VA_ARGS__))
#define NSL__NCAT127(x, ...) NSL_CAT(x, NSL__NCAT126(__VA_ARGS__))

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
#define NSL_VA_HEAD(...)        NSL__VA_HEAD(__VA_ARGS__)
#define NSL__VA_HEAD(head, ...) head

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
#define NSL_VA_REST(...)        NSL__VA_REST(__VA_ARGS__)
#define NSL__VA_REST(head, ...) __VA_ARGS__

/*!
 * Evaluates to the tail of the variadic arguments (i.e. the last argument).
 *
 * `NSL_VA_TAIL` requires two levels of indirection. This is to ensure that
 * passing a macro that expands to the variadic arguments works correctly and is
 * expanded. `NSL__VA_TAIl` concatentates `NSL__VA_TAIL` with the number of
 * arguments provided (using `NSL_NARGS`, which is restricted to [0-127]). The
 * corresponding macro is called and it pops off elements in the list until only
 * the last is left which returned.
 *
 * # Requires
 * - The number of arguments, N, is less than 128. If 128 or more arguments are
 *   provided, a compile time error is likely to be generated. There is no
 *   guarantee of this, however.
 *
 * # Returns
 * The last argument in the variadic list of arguments, if any exists.
 */
#define NSL_VA_TAIL(...)  NSL__VA_TAIL(__VA_ARGS__)
#define NSL__VA_TAIL(...) NSL_CAT(NSL__VA_TAIL, NSL_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define NSL__VA_TAIL0()
#define NSL__VA_TAIL1(x)        x
#define NSL__VA_TAIL2(x, ...)   NSL__VA_TAIL1(__VA_ARGS__)
#define NSL__VA_TAIL3(x, ...)   NSL__VA_TAIL2(__VA_ARGS__)
#define NSL__VA_TAIL4(x, ...)   NSL__VA_TAIL3(__VA_ARGS__)
#define NSL__VA_TAIL5(x, ...)   NSL__VA_TAIL4(__VA_ARGS__)
#define NSL__VA_TAIL6(x, ...)   NSL__VA_TAIL5(__VA_ARGS__)
#define NSL__VA_TAIL7(x, ...)   NSL__VA_TAIL6(__VA_ARGS__)
#define NSL__VA_TAIL8(x, ...)   NSL__VA_TAIL7(__VA_ARGS__)
#define NSL__VA_TAIL9(x, ...)   NSL__VA_TAIL8(__VA_ARGS__)
#define NSL__VA_TAIL10(x, ...)  NSL__VA_TAIL9(__VA_ARGS__)
#define NSL__VA_TAIL11(x, ...)  NSL__VA_TAIL10(__VA_ARGS__)
#define NSL__VA_TAIL12(x, ...)  NSL__VA_TAIL11(__VA_ARGS__)
#define NSL__VA_TAIL13(x, ...)  NSL__VA_TAIL12(__VA_ARGS__)
#define NSL__VA_TAIL14(x, ...)  NSL__VA_TAIL13(__VA_ARGS__)
#define NSL__VA_TAIL15(x, ...)  NSL__VA_TAIL14(__VA_ARGS__)
#define NSL__VA_TAIL16(x, ...)  NSL__VA_TAIL15(__VA_ARGS__)
#define NSL__VA_TAIL17(x, ...)  NSL__VA_TAIL16(__VA_ARGS__)
#define NSL__VA_TAIL18(x, ...)  NSL__VA_TAIL17(__VA_ARGS__)
#define NSL__VA_TAIL19(x, ...)  NSL__VA_TAIL18(__VA_ARGS__)
#define NSL__VA_TAIL20(x, ...)  NSL__VA_TAIL19(__VA_ARGS__)
#define NSL__VA_TAIL21(x, ...)  NSL__VA_TAIL20(__VA_ARGS__)
#define NSL__VA_TAIL22(x, ...)  NSL__VA_TAIL21(__VA_ARGS__)
#define NSL__VA_TAIL23(x, ...)  NSL__VA_TAIL22(__VA_ARGS__)
#define NSL__VA_TAIL24(x, ...)  NSL__VA_TAIL23(__VA_ARGS__)
#define NSL__VA_TAIL25(x, ...)  NSL__VA_TAIL24(__VA_ARGS__)
#define NSL__VA_TAIL26(x, ...)  NSL__VA_TAIL25(__VA_ARGS__)
#define NSL__VA_TAIL27(x, ...)  NSL__VA_TAIL26(__VA_ARGS__)
#define NSL__VA_TAIL28(x, ...)  NSL__VA_TAIL27(__VA_ARGS__)
#define NSL__VA_TAIL29(x, ...)  NSL__VA_TAIL28(__VA_ARGS__)
#define NSL__VA_TAIL30(x, ...)  NSL__VA_TAIL29(__VA_ARGS__)
#define NSL__VA_TAIL31(x, ...)  NSL__VA_TAIL30(__VA_ARGS__)
#define NSL__VA_TAIL32(x, ...)  NSL__VA_TAIL31(__VA_ARGS__)
#define NSL__VA_TAIL33(x, ...)  NSL__VA_TAIL32(__VA_ARGS__)
#define NSL__VA_TAIL34(x, ...)  NSL__VA_TAIL33(__VA_ARGS__)
#define NSL__VA_TAIL35(x, ...)  NSL__VA_TAIL34(__VA_ARGS__)
#define NSL__VA_TAIL36(x, ...)  NSL__VA_TAIL35(__VA_ARGS__)
#define NSL__VA_TAIL37(x, ...)  NSL__VA_TAIL36(__VA_ARGS__)
#define NSL__VA_TAIL38(x, ...)  NSL__VA_TAIL37(__VA_ARGS__)
#define NSL__VA_TAIL39(x, ...)  NSL__VA_TAIL38(__VA_ARGS__)
#define NSL__VA_TAIL40(x, ...)  NSL__VA_TAIL39(__VA_ARGS__)
#define NSL__VA_TAIL41(x, ...)  NSL__VA_TAIL40(__VA_ARGS__)
#define NSL__VA_TAIL42(x, ...)  NSL__VA_TAIL41(__VA_ARGS__)
#define NSL__VA_TAIL43(x, ...)  NSL__VA_TAIL42(__VA_ARGS__)
#define NSL__VA_TAIL44(x, ...)  NSL__VA_TAIL43(__VA_ARGS__)
#define NSL__VA_TAIL45(x, ...)  NSL__VA_TAIL44(__VA_ARGS__)
#define NSL__VA_TAIL46(x, ...)  NSL__VA_TAIL45(__VA_ARGS__)
#define NSL__VA_TAIL47(x, ...)  NSL__VA_TAIL46(__VA_ARGS__)
#define NSL__VA_TAIL48(x, ...)  NSL__VA_TAIL47(__VA_ARGS__)
#define NSL__VA_TAIL49(x, ...)  NSL__VA_TAIL48(__VA_ARGS__)
#define NSL__VA_TAIL50(x, ...)  NSL__VA_TAIL49(__VA_ARGS__)
#define NSL__VA_TAIL51(x, ...)  NSL__VA_TAIL50(__VA_ARGS__)
#define NSL__VA_TAIL52(x, ...)  NSL__VA_TAIL51(__VA_ARGS__)
#define NSL__VA_TAIL53(x, ...)  NSL__VA_TAIL52(__VA_ARGS__)
#define NSL__VA_TAIL54(x, ...)  NSL__VA_TAIL53(__VA_ARGS__)
#define NSL__VA_TAIL55(x, ...)  NSL__VA_TAIL54(__VA_ARGS__)
#define NSL__VA_TAIL56(x, ...)  NSL__VA_TAIL55(__VA_ARGS__)
#define NSL__VA_TAIL57(x, ...)  NSL__VA_TAIL56(__VA_ARGS__)
#define NSL__VA_TAIL58(x, ...)  NSL__VA_TAIL57(__VA_ARGS__)
#define NSL__VA_TAIL59(x, ...)  NSL__VA_TAIL58(__VA_ARGS__)
#define NSL__VA_TAIL60(x, ...)  NSL__VA_TAIL59(__VA_ARGS__)
#define NSL__VA_TAIL61(x, ...)  NSL__VA_TAIL60(__VA_ARGS__)
#define NSL__VA_TAIL62(x, ...)  NSL__VA_TAIL61(__VA_ARGS__)
#define NSL__VA_TAIL63(x, ...)  NSL__VA_TAIL62(__VA_ARGS__)
#define NSL__VA_TAIL64(x, ...)  NSL__VA_TAIL63(__VA_ARGS__)
#define NSL__VA_TAIL65(x, ...)  NSL__VA_TAIL64(__VA_ARGS__)
#define NSL__VA_TAIL66(x, ...)  NSL__VA_TAIL65(__VA_ARGS__)
#define NSL__VA_TAIL67(x, ...)  NSL__VA_TAIL66(__VA_ARGS__)
#define NSL__VA_TAIL68(x, ...)  NSL__VA_TAIL67(__VA_ARGS__)
#define NSL__VA_TAIL69(x, ...)  NSL__VA_TAIL68(__VA_ARGS__)
#define NSL__VA_TAIL70(x, ...)  NSL__VA_TAIL69(__VA_ARGS__)
#define NSL__VA_TAIL71(x, ...)  NSL__VA_TAIL70(__VA_ARGS__)
#define NSL__VA_TAIL72(x, ...)  NSL__VA_TAIL71(__VA_ARGS__)
#define NSL__VA_TAIL73(x, ...)  NSL__VA_TAIL72(__VA_ARGS__)
#define NSL__VA_TAIL74(x, ...)  NSL__VA_TAIL73(__VA_ARGS__)
#define NSL__VA_TAIL75(x, ...)  NSL__VA_TAIL74(__VA_ARGS__)
#define NSL__VA_TAIL76(x, ...)  NSL__VA_TAIL75(__VA_ARGS__)
#define NSL__VA_TAIL77(x, ...)  NSL__VA_TAIL76(__VA_ARGS__)
#define NSL__VA_TAIL78(x, ...)  NSL__VA_TAIL77(__VA_ARGS__)
#define NSL__VA_TAIL79(x, ...)  NSL__VA_TAIL78(__VA_ARGS__)
#define NSL__VA_TAIL80(x, ...)  NSL__VA_TAIL79(__VA_ARGS__)
#define NSL__VA_TAIL81(x, ...)  NSL__VA_TAIL80(__VA_ARGS__)
#define NSL__VA_TAIL82(x, ...)  NSL__VA_TAIL81(__VA_ARGS__)
#define NSL__VA_TAIL83(x, ...)  NSL__VA_TAIL82(__VA_ARGS__)
#define NSL__VA_TAIL84(x, ...)  NSL__VA_TAIL83(__VA_ARGS__)
#define NSL__VA_TAIL85(x, ...)  NSL__VA_TAIL84(__VA_ARGS__)
#define NSL__VA_TAIL86(x, ...)  NSL__VA_TAIL85(__VA_ARGS__)
#define NSL__VA_TAIL87(x, ...)  NSL__VA_TAIL86(__VA_ARGS__)
#define NSL__VA_TAIL88(x, ...)  NSL__VA_TAIL87(__VA_ARGS__)
#define NSL__VA_TAIL89(x, ...)  NSL__VA_TAIL88(__VA_ARGS__)
#define NSL__VA_TAIL90(x, ...)  NSL__VA_TAIL89(__VA_ARGS__)
#define NSL__VA_TAIL91(x, ...)  NSL__VA_TAIL90(__VA_ARGS__)
#define NSL__VA_TAIL92(x, ...)  NSL__VA_TAIL91(__VA_ARGS__)
#define NSL__VA_TAIL93(x, ...)  NSL__VA_TAIL92(__VA_ARGS__)
#define NSL__VA_TAIL94(x, ...)  NSL__VA_TAIL93(__VA_ARGS__)
#define NSL__VA_TAIL95(x, ...)  NSL__VA_TAIL94(__VA_ARGS__)
#define NSL__VA_TAIL96(x, ...)  NSL__VA_TAIL95(__VA_ARGS__)
#define NSL__VA_TAIL97(x, ...)  NSL__VA_TAIL96(__VA_ARGS__)
#define NSL__VA_TAIL98(x, ...)  NSL__VA_TAIL97(__VA_ARGS__)
#define NSL__VA_TAIL99(x, ...)  NSL__VA_TAIL98(__VA_ARGS__)
#define NSL__VA_TAIL100(x, ...) NSL__VA_TAIL99(__VA_ARGS__)
#define NSL__VA_TAIL101(x, ...) NSL__VA_TAIL100(__VA_ARGS__)
#define NSL__VA_TAIL102(x, ...) NSL__VA_TAIL101(__VA_ARGS__)
#define NSL__VA_TAIL103(x, ...) NSL__VA_TAIL102(__VA_ARGS__)
#define NSL__VA_TAIL104(x, ...) NSL__VA_TAIL103(__VA_ARGS__)
#define NSL__VA_TAIL105(x, ...) NSL__VA_TAIL104(__VA_ARGS__)
#define NSL__VA_TAIL106(x, ...) NSL__VA_TAIL105(__VA_ARGS__)
#define NSL__VA_TAIL107(x, ...) NSL__VA_TAIL106(__VA_ARGS__)
#define NSL__VA_TAIL108(x, ...) NSL__VA_TAIL107(__VA_ARGS__)
#define NSL__VA_TAIL109(x, ...) NSL__VA_TAIL108(__VA_ARGS__)
#define NSL__VA_TAIL110(x, ...) NSL__VA_TAIL109(__VA_ARGS__)
#define NSL__VA_TAIL111(x, ...) NSL__VA_TAIL110(__VA_ARGS__)
#define NSL__VA_TAIL112(x, ...) NSL__VA_TAIL111(__VA_ARGS__)
#define NSL__VA_TAIL113(x, ...) NSL__VA_TAIL112(__VA_ARGS__)
#define NSL__VA_TAIL114(x, ...) NSL__VA_TAIL113(__VA_ARGS__)
#define NSL__VA_TAIL115(x, ...) NSL__VA_TAIL114(__VA_ARGS__)
#define NSL__VA_TAIL116(x, ...) NSL__VA_TAIL115(__VA_ARGS__)
#define NSL__VA_TAIL117(x, ...) NSL__VA_TAIL116(__VA_ARGS__)
#define NSL__VA_TAIL118(x, ...) NSL__VA_TAIL117(__VA_ARGS__)
#define NSL__VA_TAIL119(x, ...) NSL__VA_TAIL118(__VA_ARGS__)
#define NSL__VA_TAIL120(x, ...) NSL__VA_TAIL119(__VA_ARGS__)
#define NSL__VA_TAIL121(x, ...) NSL__VA_TAIL120(__VA_ARGS__)
#define NSL__VA_TAIL122(x, ...) NSL__VA_TAIL121(__VA_ARGS__)
#define NSL__VA_TAIL123(x, ...) NSL__VA_TAIL122(__VA_ARGS__)
#define NSL__VA_TAIL124(x, ...) NSL__VA_TAIL123(__VA_ARGS__)
#define NSL__VA_TAIL125(x, ...) NSL__VA_TAIL124(__VA_ARGS__)
#define NSL__VA_TAIL126(x, ...) NSL__VA_TAIL125(__VA_ARGS__)
#define NSL__VA_TAIL127(x, ...) NSL__VA_TAIL126(__VA_ARGS__)

/*!
 * Evaluates to the initial list of variadic arguments (i.e. all arguments
 * except for the last).
 *
 * `NSL_VA_INITIAL` requires two levels of indirection. This is to ensure that
 * passing a macro that expands to the variadic arguments works correctly and is
 * expanded. `NSL__VA_INITIAL` concatentates `NSL__VA_INITIAL` with the number
 * of arguments provided (using `NSL_NARGS`, which is restricted to [0-127]).
 * The corresponding macro is called and it pops off and replaces elements in
 * the list until only the last is left which is popped off without replacement.
 *
 * # Requires
 * - The number of arguments, N, is less than 128. If 128 or more arguments are
 *   provided, a compile time error is likely to be generated. There is no
 *   guarantee of this, however.
 *
 * # Returns
 * The initial argument in the variadic list of arguments, if any exists.
 */
#define NSL_VA_INITIAL(...)  NSL__VA_INITIAL(__VA_ARGS__)
#define NSL__VA_INITIAL(...) NSL_CAT(NSL__VA_INITIAL, NSL_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define NSL__VA_INITIAL0()
#define NSL__VA_INITIAL1(x)
#define NSL__VA_INITIAL2(x, ...)   x
#define NSL__VA_INITIAL3(x, ...)   x, NSL__VA_INITIAL2(__VA_ARGS__)
#define NSL__VA_INITIAL4(x, ...)   x, NSL__VA_INITIAL3(__VA_ARGS__)
#define NSL__VA_INITIAL5(x, ...)   x, NSL__VA_INITIAL4(__VA_ARGS__)
#define NSL__VA_INITIAL6(x, ...)   x, NSL__VA_INITIAL5(__VA_ARGS__)
#define NSL__VA_INITIAL7(x, ...)   x, NSL__VA_INITIAL6(__VA_ARGS__)
#define NSL__VA_INITIAL8(x, ...)   x, NSL__VA_INITIAL7(__VA_ARGS__)
#define NSL__VA_INITIAL9(x, ...)   x, NSL__VA_INITIAL8(__VA_ARGS__)
#define NSL__VA_INITIAL10(x, ...)  x, NSL__VA_INITIAL9(__VA_ARGS__)
#define NSL__VA_INITIAL11(x, ...)  x, NSL__VA_INITIAL10(__VA_ARGS__)
#define NSL__VA_INITIAL12(x, ...)  x, NSL__VA_INITIAL11(__VA_ARGS__)
#define NSL__VA_INITIAL13(x, ...)  x, NSL__VA_INITIAL12(__VA_ARGS__)
#define NSL__VA_INITIAL14(x, ...)  x, NSL__VA_INITIAL13(__VA_ARGS__)
#define NSL__VA_INITIAL15(x, ...)  x, NSL__VA_INITIAL14(__VA_ARGS__)
#define NSL__VA_INITIAL16(x, ...)  x, NSL__VA_INITIAL15(__VA_ARGS__)
#define NSL__VA_INITIAL17(x, ...)  x, NSL__VA_INITIAL16(__VA_ARGS__)
#define NSL__VA_INITIAL18(x, ...)  x, NSL__VA_INITIAL17(__VA_ARGS__)
#define NSL__VA_INITIAL19(x, ...)  x, NSL__VA_INITIAL18(__VA_ARGS__)
#define NSL__VA_INITIAL20(x, ...)  x, NSL__VA_INITIAL19(__VA_ARGS__)
#define NSL__VA_INITIAL21(x, ...)  x, NSL__VA_INITIAL20(__VA_ARGS__)
#define NSL__VA_INITIAL22(x, ...)  x, NSL__VA_INITIAL21(__VA_ARGS__)
#define NSL__VA_INITIAL23(x, ...)  x, NSL__VA_INITIAL22(__VA_ARGS__)
#define NSL__VA_INITIAL24(x, ...)  x, NSL__VA_INITIAL23(__VA_ARGS__)
#define NSL__VA_INITIAL25(x, ...)  x, NSL__VA_INITIAL24(__VA_ARGS__)
#define NSL__VA_INITIAL26(x, ...)  x, NSL__VA_INITIAL25(__VA_ARGS__)
#define NSL__VA_INITIAL27(x, ...)  x, NSL__VA_INITIAL26(__VA_ARGS__)
#define NSL__VA_INITIAL28(x, ...)  x, NSL__VA_INITIAL27(__VA_ARGS__)
#define NSL__VA_INITIAL29(x, ...)  x, NSL__VA_INITIAL28(__VA_ARGS__)
#define NSL__VA_INITIAL30(x, ...)  x, NSL__VA_INITIAL29(__VA_ARGS__)
#define NSL__VA_INITIAL31(x, ...)  x, NSL__VA_INITIAL30(__VA_ARGS__)
#define NSL__VA_INITIAL32(x, ...)  x, NSL__VA_INITIAL31(__VA_ARGS__)
#define NSL__VA_INITIAL33(x, ...)  x, NSL__VA_INITIAL32(__VA_ARGS__)
#define NSL__VA_INITIAL34(x, ...)  x, NSL__VA_INITIAL33(__VA_ARGS__)
#define NSL__VA_INITIAL35(x, ...)  x, NSL__VA_INITIAL34(__VA_ARGS__)
#define NSL__VA_INITIAL36(x, ...)  x, NSL__VA_INITIAL35(__VA_ARGS__)
#define NSL__VA_INITIAL37(x, ...)  x, NSL__VA_INITIAL36(__VA_ARGS__)
#define NSL__VA_INITIAL38(x, ...)  x, NSL__VA_INITIAL37(__VA_ARGS__)
#define NSL__VA_INITIAL39(x, ...)  x, NSL__VA_INITIAL38(__VA_ARGS__)
#define NSL__VA_INITIAL40(x, ...)  x, NSL__VA_INITIAL39(__VA_ARGS__)
#define NSL__VA_INITIAL41(x, ...)  x, NSL__VA_INITIAL40(__VA_ARGS__)
#define NSL__VA_INITIAL42(x, ...)  x, NSL__VA_INITIAL41(__VA_ARGS__)
#define NSL__VA_INITIAL43(x, ...)  x, NSL__VA_INITIAL42(__VA_ARGS__)
#define NSL__VA_INITIAL44(x, ...)  x, NSL__VA_INITIAL43(__VA_ARGS__)
#define NSL__VA_INITIAL45(x, ...)  x, NSL__VA_INITIAL44(__VA_ARGS__)
#define NSL__VA_INITIAL46(x, ...)  x, NSL__VA_INITIAL45(__VA_ARGS__)
#define NSL__VA_INITIAL47(x, ...)  x, NSL__VA_INITIAL46(__VA_ARGS__)
#define NSL__VA_INITIAL48(x, ...)  x, NSL__VA_INITIAL47(__VA_ARGS__)
#define NSL__VA_INITIAL49(x, ...)  x, NSL__VA_INITIAL48(__VA_ARGS__)
#define NSL__VA_INITIAL50(x, ...)  x, NSL__VA_INITIAL49(__VA_ARGS__)
#define NSL__VA_INITIAL51(x, ...)  x, NSL__VA_INITIAL50(__VA_ARGS__)
#define NSL__VA_INITIAL52(x, ...)  x, NSL__VA_INITIAL51(__VA_ARGS__)
#define NSL__VA_INITIAL53(x, ...)  x, NSL__VA_INITIAL52(__VA_ARGS__)
#define NSL__VA_INITIAL54(x, ...)  x, NSL__VA_INITIAL53(__VA_ARGS__)
#define NSL__VA_INITIAL55(x, ...)  x, NSL__VA_INITIAL54(__VA_ARGS__)
#define NSL__VA_INITIAL56(x, ...)  x, NSL__VA_INITIAL55(__VA_ARGS__)
#define NSL__VA_INITIAL57(x, ...)  x, NSL__VA_INITIAL56(__VA_ARGS__)
#define NSL__VA_INITIAL58(x, ...)  x, NSL__VA_INITIAL57(__VA_ARGS__)
#define NSL__VA_INITIAL59(x, ...)  x, NSL__VA_INITIAL58(__VA_ARGS__)
#define NSL__VA_INITIAL60(x, ...)  x, NSL__VA_INITIAL59(__VA_ARGS__)
#define NSL__VA_INITIAL61(x, ...)  x, NSL__VA_INITIAL60(__VA_ARGS__)
#define NSL__VA_INITIAL62(x, ...)  x, NSL__VA_INITIAL61(__VA_ARGS__)
#define NSL__VA_INITIAL63(x, ...)  x, NSL__VA_INITIAL62(__VA_ARGS__)
#define NSL__VA_INITIAL64(x, ...)  x, NSL__VA_INITIAL63(__VA_ARGS__)
#define NSL__VA_INITIAL65(x, ...)  x, NSL__VA_INITIAL64(__VA_ARGS__)
#define NSL__VA_INITIAL66(x, ...)  x, NSL__VA_INITIAL65(__VA_ARGS__)
#define NSL__VA_INITIAL67(x, ...)  x, NSL__VA_INITIAL66(__VA_ARGS__)
#define NSL__VA_INITIAL68(x, ...)  x, NSL__VA_INITIAL67(__VA_ARGS__)
#define NSL__VA_INITIAL69(x, ...)  x, NSL__VA_INITIAL68(__VA_ARGS__)
#define NSL__VA_INITIAL70(x, ...)  x, NSL__VA_INITIAL69(__VA_ARGS__)
#define NSL__VA_INITIAL71(x, ...)  x, NSL__VA_INITIAL70(__VA_ARGS__)
#define NSL__VA_INITIAL72(x, ...)  x, NSL__VA_INITIAL71(__VA_ARGS__)
#define NSL__VA_INITIAL73(x, ...)  x, NSL__VA_INITIAL72(__VA_ARGS__)
#define NSL__VA_INITIAL74(x, ...)  x, NSL__VA_INITIAL73(__VA_ARGS__)
#define NSL__VA_INITIAL75(x, ...)  x, NSL__VA_INITIAL74(__VA_ARGS__)
#define NSL__VA_INITIAL76(x, ...)  x, NSL__VA_INITIAL75(__VA_ARGS__)
#define NSL__VA_INITIAL77(x, ...)  x, NSL__VA_INITIAL76(__VA_ARGS__)
#define NSL__VA_INITIAL78(x, ...)  x, NSL__VA_INITIAL77(__VA_ARGS__)
#define NSL__VA_INITIAL79(x, ...)  x, NSL__VA_INITIAL78(__VA_ARGS__)
#define NSL__VA_INITIAL80(x, ...)  x, NSL__VA_INITIAL79(__VA_ARGS__)
#define NSL__VA_INITIAL81(x, ...)  x, NSL__VA_INITIAL80(__VA_ARGS__)
#define NSL__VA_INITIAL82(x, ...)  x, NSL__VA_INITIAL81(__VA_ARGS__)
#define NSL__VA_INITIAL83(x, ...)  x, NSL__VA_INITIAL82(__VA_ARGS__)
#define NSL__VA_INITIAL84(x, ...)  x, NSL__VA_INITIAL83(__VA_ARGS__)
#define NSL__VA_INITIAL85(x, ...)  x, NSL__VA_INITIAL84(__VA_ARGS__)
#define NSL__VA_INITIAL86(x, ...)  x, NSL__VA_INITIAL85(__VA_ARGS__)
#define NSL__VA_INITIAL87(x, ...)  x, NSL__VA_INITIAL86(__VA_ARGS__)
#define NSL__VA_INITIAL88(x, ...)  x, NSL__VA_INITIAL87(__VA_ARGS__)
#define NSL__VA_INITIAL89(x, ...)  x, NSL__VA_INITIAL88(__VA_ARGS__)
#define NSL__VA_INITIAL90(x, ...)  x, NSL__VA_INITIAL89(__VA_ARGS__)
#define NSL__VA_INITIAL91(x, ...)  x, NSL__VA_INITIAL90(__VA_ARGS__)
#define NSL__VA_INITIAL92(x, ...)  x, NSL__VA_INITIAL91(__VA_ARGS__)
#define NSL__VA_INITIAL93(x, ...)  x, NSL__VA_INITIAL92(__VA_ARGS__)
#define NSL__VA_INITIAL94(x, ...)  x, NSL__VA_INITIAL93(__VA_ARGS__)
#define NSL__VA_INITIAL95(x, ...)  x, NSL__VA_INITIAL94(__VA_ARGS__)
#define NSL__VA_INITIAL96(x, ...)  x, NSL__VA_INITIAL95(__VA_ARGS__)
#define NSL__VA_INITIAL97(x, ...)  x, NSL__VA_INITIAL96(__VA_ARGS__)
#define NSL__VA_INITIAL98(x, ...)  x, NSL__VA_INITIAL97(__VA_ARGS__)
#define NSL__VA_INITIAL99(x, ...)  x, NSL__VA_INITIAL98(__VA_ARGS__)
#define NSL__VA_INITIAL100(x, ...) x, NSL__VA_INITIAL99(__VA_ARGS__)
#define NSL__VA_INITIAL101(x, ...) x, NSL__VA_INITIAL100(__VA_ARGS__)
#define NSL__VA_INITIAL102(x, ...) x, NSL__VA_INITIAL101(__VA_ARGS__)
#define NSL__VA_INITIAL103(x, ...) x, NSL__VA_INITIAL102(__VA_ARGS__)
#define NSL__VA_INITIAL104(x, ...) x, NSL__VA_INITIAL103(__VA_ARGS__)
#define NSL__VA_INITIAL105(x, ...) x, NSL__VA_INITIAL104(__VA_ARGS__)
#define NSL__VA_INITIAL106(x, ...) x, NSL__VA_INITIAL105(__VA_ARGS__)
#define NSL__VA_INITIAL107(x, ...) x, NSL__VA_INITIAL106(__VA_ARGS__)
#define NSL__VA_INITIAL108(x, ...) x, NSL__VA_INITIAL107(__VA_ARGS__)
#define NSL__VA_INITIAL109(x, ...) x, NSL__VA_INITIAL108(__VA_ARGS__)
#define NSL__VA_INITIAL110(x, ...) x, NSL__VA_INITIAL109(__VA_ARGS__)
#define NSL__VA_INITIAL111(x, ...) x, NSL__VA_INITIAL110(__VA_ARGS__)
#define NSL__VA_INITIAL112(x, ...) x, NSL__VA_INITIAL111(__VA_ARGS__)
#define NSL__VA_INITIAL113(x, ...) x, NSL__VA_INITIAL112(__VA_ARGS__)
#define NSL__VA_INITIAL114(x, ...) x, NSL__VA_INITIAL113(__VA_ARGS__)
#define NSL__VA_INITIAL115(x, ...) x, NSL__VA_INITIAL114(__VA_ARGS__)
#define NSL__VA_INITIAL116(x, ...) x, NSL__VA_INITIAL115(__VA_ARGS__)
#define NSL__VA_INITIAL117(x, ...) x, NSL__VA_INITIAL116(__VA_ARGS__)
#define NSL__VA_INITIAL118(x, ...) x, NSL__VA_INITIAL117(__VA_ARGS__)
#define NSL__VA_INITIAL119(x, ...) x, NSL__VA_INITIAL118(__VA_ARGS__)
#define NSL__VA_INITIAL120(x, ...) x, NSL__VA_INITIAL119(__VA_ARGS__)
#define NSL__VA_INITIAL121(x, ...) x, NSL__VA_INITIAL120(__VA_ARGS__)
#define NSL__VA_INITIAL122(x, ...) x, NSL__VA_INITIAL121(__VA_ARGS__)
#define NSL__VA_INITIAL123(x, ...) x, NSL__VA_INITIAL122(__VA_ARGS__)
#define NSL__VA_INITIAL124(x, ...) x, NSL__VA_INITIAL123(__VA_ARGS__)
#define NSL__VA_INITIAL125(x, ...) x, NSL__VA_INITIAL124(__VA_ARGS__)
#define NSL__VA_INITIAL126(x, ...) x, NSL__VA_INITIAL125(__VA_ARGS__)
#define NSL__VA_INITIAL127(x, ...) x, NSL__VA_INITIAL126(__VA_ARGS__)

/*!
 * Reverses the arguments provided. For example, `NSL_REVERSE(1, 2, 3, 4)` would
 * evaluate to `4, 3, 2, 1`.
 *
 * `NSL_VA_REVERSE` requires two levels of indirection. This is to ensure that
 * passing a macro that expands to the variadic arguments works correctly and is
 * expanded. `NSL__VA_REVERSE` concatentates `NSL__VA_REVERSE` with the number
 * of arguments provided (using `NSL_NARGS`, which is restricted to [0-127]).
 * The corresponding macro is called and it pops off the current head, calls the
 * next macro in the chain, and then puts the head at the back.
 *
 * # Requires
 * - The number of arguments, N, is less than 128. If 128 or more arguments are
 *   provided, a compile time error is likely to be generated. There is no
 *   guarantee of this, however.
 *
 * # Returns
 * The reversed variadic list of arguments, if any exists.
 */
#define NSL_VA_REVERSE(...)  NSL__VA_REVERSE(__VA_ARGS__)
#define NSL__VA_REVERSE(...) NSL_CAT(NSL__VA_REVERSE, NSL_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define NSL__VA_REVERSE0()
#define NSL__VA_REVERSE1(x)        x
#define NSL__VA_REVERSE2(x, ...)   NSL__VA_REVERSE1(__VA_ARGS__), x
#define NSL__VA_REVERSE3(x, ...)   NSL__VA_REVERSE2(__VA_ARGS__), x
#define NSL__VA_REVERSE4(x, ...)   NSL__VA_REVERSE3(__VA_ARGS__), x
#define NSL__VA_REVERSE5(x, ...)   NSL__VA_REVERSE4(__VA_ARGS__), x
#define NSL__VA_REVERSE6(x, ...)   NSL__VA_REVERSE5(__VA_ARGS__), x
#define NSL__VA_REVERSE7(x, ...)   NSL__VA_REVERSE6(__VA_ARGS__), x
#define NSL__VA_REVERSE8(x, ...)   NSL__VA_REVERSE7(__VA_ARGS__), x
#define NSL__VA_REVERSE9(x, ...)   NSL__VA_REVERSE8(__VA_ARGS__), x
#define NSL__VA_REVERSE10(x, ...)  NSL__VA_REVERSE9(__VA_ARGS__), x
#define NSL__VA_REVERSE11(x, ...)  NSL__VA_REVERSE10(__VA_ARGS__), x
#define NSL__VA_REVERSE12(x, ...)  NSL__VA_REVERSE11(__VA_ARGS__), x
#define NSL__VA_REVERSE13(x, ...)  NSL__VA_REVERSE12(__VA_ARGS__), x
#define NSL__VA_REVERSE14(x, ...)  NSL__VA_REVERSE13(__VA_ARGS__), x
#define NSL__VA_REVERSE15(x, ...)  NSL__VA_REVERSE14(__VA_ARGS__), x
#define NSL__VA_REVERSE16(x, ...)  NSL__VA_REVERSE15(__VA_ARGS__), x
#define NSL__VA_REVERSE17(x, ...)  NSL__VA_REVERSE16(__VA_ARGS__), x
#define NSL__VA_REVERSE18(x, ...)  NSL__VA_REVERSE17(__VA_ARGS__), x
#define NSL__VA_REVERSE19(x, ...)  NSL__VA_REVERSE18(__VA_ARGS__), x
#define NSL__VA_REVERSE20(x, ...)  NSL__VA_REVERSE19(__VA_ARGS__), x
#define NSL__VA_REVERSE21(x, ...)  NSL__VA_REVERSE20(__VA_ARGS__), x
#define NSL__VA_REVERSE22(x, ...)  NSL__VA_REVERSE21(__VA_ARGS__), x
#define NSL__VA_REVERSE23(x, ...)  NSL__VA_REVERSE22(__VA_ARGS__), x
#define NSL__VA_REVERSE24(x, ...)  NSL__VA_REVERSE23(__VA_ARGS__), x
#define NSL__VA_REVERSE25(x, ...)  NSL__VA_REVERSE24(__VA_ARGS__), x
#define NSL__VA_REVERSE26(x, ...)  NSL__VA_REVERSE25(__VA_ARGS__), x
#define NSL__VA_REVERSE27(x, ...)  NSL__VA_REVERSE26(__VA_ARGS__), x
#define NSL__VA_REVERSE28(x, ...)  NSL__VA_REVERSE27(__VA_ARGS__), x
#define NSL__VA_REVERSE29(x, ...)  NSL__VA_REVERSE28(__VA_ARGS__), x
#define NSL__VA_REVERSE30(x, ...)  NSL__VA_REVERSE29(__VA_ARGS__), x
#define NSL__VA_REVERSE31(x, ...)  NSL__VA_REVERSE30(__VA_ARGS__), x
#define NSL__VA_REVERSE32(x, ...)  NSL__VA_REVERSE31(__VA_ARGS__), x
#define NSL__VA_REVERSE33(x, ...)  NSL__VA_REVERSE32(__VA_ARGS__), x
#define NSL__VA_REVERSE34(x, ...)  NSL__VA_REVERSE33(__VA_ARGS__), x
#define NSL__VA_REVERSE35(x, ...)  NSL__VA_REVERSE34(__VA_ARGS__), x
#define NSL__VA_REVERSE36(x, ...)  NSL__VA_REVERSE35(__VA_ARGS__), x
#define NSL__VA_REVERSE37(x, ...)  NSL__VA_REVERSE36(__VA_ARGS__), x
#define NSL__VA_REVERSE38(x, ...)  NSL__VA_REVERSE37(__VA_ARGS__), x
#define NSL__VA_REVERSE39(x, ...)  NSL__VA_REVERSE38(__VA_ARGS__), x
#define NSL__VA_REVERSE40(x, ...)  NSL__VA_REVERSE39(__VA_ARGS__), x
#define NSL__VA_REVERSE41(x, ...)  NSL__VA_REVERSE40(__VA_ARGS__), x
#define NSL__VA_REVERSE42(x, ...)  NSL__VA_REVERSE41(__VA_ARGS__), x
#define NSL__VA_REVERSE43(x, ...)  NSL__VA_REVERSE42(__VA_ARGS__), x
#define NSL__VA_REVERSE44(x, ...)  NSL__VA_REVERSE43(__VA_ARGS__), x
#define NSL__VA_REVERSE45(x, ...)  NSL__VA_REVERSE44(__VA_ARGS__), x
#define NSL__VA_REVERSE46(x, ...)  NSL__VA_REVERSE45(__VA_ARGS__), x
#define NSL__VA_REVERSE47(x, ...)  NSL__VA_REVERSE46(__VA_ARGS__), x
#define NSL__VA_REVERSE48(x, ...)  NSL__VA_REVERSE47(__VA_ARGS__), x
#define NSL__VA_REVERSE49(x, ...)  NSL__VA_REVERSE48(__VA_ARGS__), x
#define NSL__VA_REVERSE50(x, ...)  NSL__VA_REVERSE49(__VA_ARGS__), x
#define NSL__VA_REVERSE51(x, ...)  NSL__VA_REVERSE50(__VA_ARGS__), x
#define NSL__VA_REVERSE52(x, ...)  NSL__VA_REVERSE51(__VA_ARGS__), x
#define NSL__VA_REVERSE53(x, ...)  NSL__VA_REVERSE52(__VA_ARGS__), x
#define NSL__VA_REVERSE54(x, ...)  NSL__VA_REVERSE53(__VA_ARGS__), x
#define NSL__VA_REVERSE55(x, ...)  NSL__VA_REVERSE54(__VA_ARGS__), x
#define NSL__VA_REVERSE56(x, ...)  NSL__VA_REVERSE55(__VA_ARGS__), x
#define NSL__VA_REVERSE57(x, ...)  NSL__VA_REVERSE56(__VA_ARGS__), x
#define NSL__VA_REVERSE58(x, ...)  NSL__VA_REVERSE57(__VA_ARGS__), x
#define NSL__VA_REVERSE59(x, ...)  NSL__VA_REVERSE58(__VA_ARGS__), x
#define NSL__VA_REVERSE60(x, ...)  NSL__VA_REVERSE59(__VA_ARGS__), x
#define NSL__VA_REVERSE61(x, ...)  NSL__VA_REVERSE60(__VA_ARGS__), x
#define NSL__VA_REVERSE62(x, ...)  NSL__VA_REVERSE61(__VA_ARGS__), x
#define NSL__VA_REVERSE63(x, ...)  NSL__VA_REVERSE62(__VA_ARGS__), x
#define NSL__VA_REVERSE64(x, ...)  NSL__VA_REVERSE63(__VA_ARGS__), x
#define NSL__VA_REVERSE65(x, ...)  NSL__VA_REVERSE64(__VA_ARGS__), x
#define NSL__VA_REVERSE66(x, ...)  NSL__VA_REVERSE65(__VA_ARGS__), x
#define NSL__VA_REVERSE67(x, ...)  NSL__VA_REVERSE66(__VA_ARGS__), x
#define NSL__VA_REVERSE68(x, ...)  NSL__VA_REVERSE67(__VA_ARGS__), x
#define NSL__VA_REVERSE69(x, ...)  NSL__VA_REVERSE68(__VA_ARGS__), x
#define NSL__VA_REVERSE70(x, ...)  NSL__VA_REVERSE69(__VA_ARGS__), x
#define NSL__VA_REVERSE71(x, ...)  NSL__VA_REVERSE70(__VA_ARGS__), x
#define NSL__VA_REVERSE72(x, ...)  NSL__VA_REVERSE71(__VA_ARGS__), x
#define NSL__VA_REVERSE73(x, ...)  NSL__VA_REVERSE72(__VA_ARGS__), x
#define NSL__VA_REVERSE74(x, ...)  NSL__VA_REVERSE73(__VA_ARGS__), x
#define NSL__VA_REVERSE75(x, ...)  NSL__VA_REVERSE74(__VA_ARGS__), x
#define NSL__VA_REVERSE76(x, ...)  NSL__VA_REVERSE75(__VA_ARGS__), x
#define NSL__VA_REVERSE77(x, ...)  NSL__VA_REVERSE76(__VA_ARGS__), x
#define NSL__VA_REVERSE78(x, ...)  NSL__VA_REVERSE77(__VA_ARGS__), x
#define NSL__VA_REVERSE79(x, ...)  NSL__VA_REVERSE78(__VA_ARGS__), x
#define NSL__VA_REVERSE80(x, ...)  NSL__VA_REVERSE79(__VA_ARGS__), x
#define NSL__VA_REVERSE81(x, ...)  NSL__VA_REVERSE80(__VA_ARGS__), x
#define NSL__VA_REVERSE82(x, ...)  NSL__VA_REVERSE81(__VA_ARGS__), x
#define NSL__VA_REVERSE83(x, ...)  NSL__VA_REVERSE82(__VA_ARGS__), x
#define NSL__VA_REVERSE84(x, ...)  NSL__VA_REVERSE83(__VA_ARGS__), x
#define NSL__VA_REVERSE85(x, ...)  NSL__VA_REVERSE84(__VA_ARGS__), x
#define NSL__VA_REVERSE86(x, ...)  NSL__VA_REVERSE85(__VA_ARGS__), x
#define NSL__VA_REVERSE87(x, ...)  NSL__VA_REVERSE86(__VA_ARGS__), x
#define NSL__VA_REVERSE88(x, ...)  NSL__VA_REVERSE87(__VA_ARGS__), x
#define NSL__VA_REVERSE89(x, ...)  NSL__VA_REVERSE88(__VA_ARGS__), x
#define NSL__VA_REVERSE90(x, ...)  NSL__VA_REVERSE89(__VA_ARGS__), x
#define NSL__VA_REVERSE91(x, ...)  NSL__VA_REVERSE90(__VA_ARGS__), x
#define NSL__VA_REVERSE92(x, ...)  NSL__VA_REVERSE91(__VA_ARGS__), x
#define NSL__VA_REVERSE93(x, ...)  NSL__VA_REVERSE92(__VA_ARGS__), x
#define NSL__VA_REVERSE94(x, ...)  NSL__VA_REVERSE93(__VA_ARGS__), x
#define NSL__VA_REVERSE95(x, ...)  NSL__VA_REVERSE94(__VA_ARGS__), x
#define NSL__VA_REVERSE96(x, ...)  NSL__VA_REVERSE95(__VA_ARGS__), x
#define NSL__VA_REVERSE97(x, ...)  NSL__VA_REVERSE96(__VA_ARGS__), x
#define NSL__VA_REVERSE98(x, ...)  NSL__VA_REVERSE97(__VA_ARGS__), x
#define NSL__VA_REVERSE99(x, ...)  NSL__VA_REVERSE98(__VA_ARGS__), x
#define NSL__VA_REVERSE100(x, ...) NSL__VA_REVERSE99(__VA_ARGS__), x
#define NSL__VA_REVERSE101(x, ...) NSL__VA_REVERSE100(__VA_ARGS__), x
#define NSL__VA_REVERSE102(x, ...) NSL__VA_REVERSE101(__VA_ARGS__), x
#define NSL__VA_REVERSE103(x, ...) NSL__VA_REVERSE102(__VA_ARGS__), x
#define NSL__VA_REVERSE104(x, ...) NSL__VA_REVERSE103(__VA_ARGS__), x
#define NSL__VA_REVERSE105(x, ...) NSL__VA_REVERSE104(__VA_ARGS__), x
#define NSL__VA_REVERSE106(x, ...) NSL__VA_REVERSE105(__VA_ARGS__), x
#define NSL__VA_REVERSE107(x, ...) NSL__VA_REVERSE106(__VA_ARGS__), x
#define NSL__VA_REVERSE108(x, ...) NSL__VA_REVERSE107(__VA_ARGS__), x
#define NSL__VA_REVERSE109(x, ...) NSL__VA_REVERSE108(__VA_ARGS__), x
#define NSL__VA_REVERSE110(x, ...) NSL__VA_REVERSE109(__VA_ARGS__), x
#define NSL__VA_REVERSE111(x, ...) NSL__VA_REVERSE110(__VA_ARGS__), x
#define NSL__VA_REVERSE112(x, ...) NSL__VA_REVERSE111(__VA_ARGS__), x
#define NSL__VA_REVERSE113(x, ...) NSL__VA_REVERSE112(__VA_ARGS__), x
#define NSL__VA_REVERSE114(x, ...) NSL__VA_REVERSE113(__VA_ARGS__), x
#define NSL__VA_REVERSE115(x, ...) NSL__VA_REVERSE114(__VA_ARGS__), x
#define NSL__VA_REVERSE116(x, ...) NSL__VA_REVERSE115(__VA_ARGS__), x
#define NSL__VA_REVERSE117(x, ...) NSL__VA_REVERSE116(__VA_ARGS__), x
#define NSL__VA_REVERSE118(x, ...) NSL__VA_REVERSE117(__VA_ARGS__), x
#define NSL__VA_REVERSE119(x, ...) NSL__VA_REVERSE118(__VA_ARGS__), x
#define NSL__VA_REVERSE120(x, ...) NSL__VA_REVERSE119(__VA_ARGS__), x
#define NSL__VA_REVERSE121(x, ...) NSL__VA_REVERSE120(__VA_ARGS__), x
#define NSL__VA_REVERSE122(x, ...) NSL__VA_REVERSE121(__VA_ARGS__), x
#define NSL__VA_REVERSE123(x, ...) NSL__VA_REVERSE122(__VA_ARGS__), x
#define NSL__VA_REVERSE124(x, ...) NSL__VA_REVERSE123(__VA_ARGS__), x
#define NSL__VA_REVERSE125(x, ...) NSL__VA_REVERSE124(__VA_ARGS__), x
#define NSL__VA_REVERSE126(x, ...) NSL__VA_REVERSE125(__VA_ARGS__), x
#define NSL__VA_REVERSE127(x, ...) NSL__VA_REVERSE126(__VA_ARGS__), x

#endif  // NSL_MAGIC_H_
