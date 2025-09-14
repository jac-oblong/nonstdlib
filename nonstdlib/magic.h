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
 * `NSL_VA_INIT` requires two levels of indirection. This is to ensure that
 * passing a macro that expands to the variadic arguments works correctly and is
 * expanded. `NSL__VA_INIT` concatentates `NSL__VA_INIT` with the number
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
#define NSL_VA_INIT(...)  NSL__VA_INIT(__VA_ARGS__)
#define NSL__VA_INIT(...) NSL_CAT(NSL__VA_INIT, NSL_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define NSL__VA_INIT0()
#define NSL__VA_INIT1(x)
#define NSL__VA_INIT2(x, ...)   x
#define NSL__VA_INIT3(x, ...)   x, NSL__VA_INIT2(__VA_ARGS__)
#define NSL__VA_INIT4(x, ...)   x, NSL__VA_INIT3(__VA_ARGS__)
#define NSL__VA_INIT5(x, ...)   x, NSL__VA_INIT4(__VA_ARGS__)
#define NSL__VA_INIT6(x, ...)   x, NSL__VA_INIT5(__VA_ARGS__)
#define NSL__VA_INIT7(x, ...)   x, NSL__VA_INIT6(__VA_ARGS__)
#define NSL__VA_INIT8(x, ...)   x, NSL__VA_INIT7(__VA_ARGS__)
#define NSL__VA_INIT9(x, ...)   x, NSL__VA_INIT8(__VA_ARGS__)
#define NSL__VA_INIT10(x, ...)  x, NSL__VA_INIT9(__VA_ARGS__)
#define NSL__VA_INIT11(x, ...)  x, NSL__VA_INIT10(__VA_ARGS__)
#define NSL__VA_INIT12(x, ...)  x, NSL__VA_INIT11(__VA_ARGS__)
#define NSL__VA_INIT13(x, ...)  x, NSL__VA_INIT12(__VA_ARGS__)
#define NSL__VA_INIT14(x, ...)  x, NSL__VA_INIT13(__VA_ARGS__)
#define NSL__VA_INIT15(x, ...)  x, NSL__VA_INIT14(__VA_ARGS__)
#define NSL__VA_INIT16(x, ...)  x, NSL__VA_INIT15(__VA_ARGS__)
#define NSL__VA_INIT17(x, ...)  x, NSL__VA_INIT16(__VA_ARGS__)
#define NSL__VA_INIT18(x, ...)  x, NSL__VA_INIT17(__VA_ARGS__)
#define NSL__VA_INIT19(x, ...)  x, NSL__VA_INIT18(__VA_ARGS__)
#define NSL__VA_INIT20(x, ...)  x, NSL__VA_INIT19(__VA_ARGS__)
#define NSL__VA_INIT21(x, ...)  x, NSL__VA_INIT20(__VA_ARGS__)
#define NSL__VA_INIT22(x, ...)  x, NSL__VA_INIT21(__VA_ARGS__)
#define NSL__VA_INIT23(x, ...)  x, NSL__VA_INIT22(__VA_ARGS__)
#define NSL__VA_INIT24(x, ...)  x, NSL__VA_INIT23(__VA_ARGS__)
#define NSL__VA_INIT25(x, ...)  x, NSL__VA_INIT24(__VA_ARGS__)
#define NSL__VA_INIT26(x, ...)  x, NSL__VA_INIT25(__VA_ARGS__)
#define NSL__VA_INIT27(x, ...)  x, NSL__VA_INIT26(__VA_ARGS__)
#define NSL__VA_INIT28(x, ...)  x, NSL__VA_INIT27(__VA_ARGS__)
#define NSL__VA_INIT29(x, ...)  x, NSL__VA_INIT28(__VA_ARGS__)
#define NSL__VA_INIT30(x, ...)  x, NSL__VA_INIT29(__VA_ARGS__)
#define NSL__VA_INIT31(x, ...)  x, NSL__VA_INIT30(__VA_ARGS__)
#define NSL__VA_INIT32(x, ...)  x, NSL__VA_INIT31(__VA_ARGS__)
#define NSL__VA_INIT33(x, ...)  x, NSL__VA_INIT32(__VA_ARGS__)
#define NSL__VA_INIT34(x, ...)  x, NSL__VA_INIT33(__VA_ARGS__)
#define NSL__VA_INIT35(x, ...)  x, NSL__VA_INIT34(__VA_ARGS__)
#define NSL__VA_INIT36(x, ...)  x, NSL__VA_INIT35(__VA_ARGS__)
#define NSL__VA_INIT37(x, ...)  x, NSL__VA_INIT36(__VA_ARGS__)
#define NSL__VA_INIT38(x, ...)  x, NSL__VA_INIT37(__VA_ARGS__)
#define NSL__VA_INIT39(x, ...)  x, NSL__VA_INIT38(__VA_ARGS__)
#define NSL__VA_INIT40(x, ...)  x, NSL__VA_INIT39(__VA_ARGS__)
#define NSL__VA_INIT41(x, ...)  x, NSL__VA_INIT40(__VA_ARGS__)
#define NSL__VA_INIT42(x, ...)  x, NSL__VA_INIT41(__VA_ARGS__)
#define NSL__VA_INIT43(x, ...)  x, NSL__VA_INIT42(__VA_ARGS__)
#define NSL__VA_INIT44(x, ...)  x, NSL__VA_INIT43(__VA_ARGS__)
#define NSL__VA_INIT45(x, ...)  x, NSL__VA_INIT44(__VA_ARGS__)
#define NSL__VA_INIT46(x, ...)  x, NSL__VA_INIT45(__VA_ARGS__)
#define NSL__VA_INIT47(x, ...)  x, NSL__VA_INIT46(__VA_ARGS__)
#define NSL__VA_INIT48(x, ...)  x, NSL__VA_INIT47(__VA_ARGS__)
#define NSL__VA_INIT49(x, ...)  x, NSL__VA_INIT48(__VA_ARGS__)
#define NSL__VA_INIT50(x, ...)  x, NSL__VA_INIT49(__VA_ARGS__)
#define NSL__VA_INIT51(x, ...)  x, NSL__VA_INIT50(__VA_ARGS__)
#define NSL__VA_INIT52(x, ...)  x, NSL__VA_INIT51(__VA_ARGS__)
#define NSL__VA_INIT53(x, ...)  x, NSL__VA_INIT52(__VA_ARGS__)
#define NSL__VA_INIT54(x, ...)  x, NSL__VA_INIT53(__VA_ARGS__)
#define NSL__VA_INIT55(x, ...)  x, NSL__VA_INIT54(__VA_ARGS__)
#define NSL__VA_INIT56(x, ...)  x, NSL__VA_INIT55(__VA_ARGS__)
#define NSL__VA_INIT57(x, ...)  x, NSL__VA_INIT56(__VA_ARGS__)
#define NSL__VA_INIT58(x, ...)  x, NSL__VA_INIT57(__VA_ARGS__)
#define NSL__VA_INIT59(x, ...)  x, NSL__VA_INIT58(__VA_ARGS__)
#define NSL__VA_INIT60(x, ...)  x, NSL__VA_INIT59(__VA_ARGS__)
#define NSL__VA_INIT61(x, ...)  x, NSL__VA_INIT60(__VA_ARGS__)
#define NSL__VA_INIT62(x, ...)  x, NSL__VA_INIT61(__VA_ARGS__)
#define NSL__VA_INIT63(x, ...)  x, NSL__VA_INIT62(__VA_ARGS__)
#define NSL__VA_INIT64(x, ...)  x, NSL__VA_INIT63(__VA_ARGS__)
#define NSL__VA_INIT65(x, ...)  x, NSL__VA_INIT64(__VA_ARGS__)
#define NSL__VA_INIT66(x, ...)  x, NSL__VA_INIT65(__VA_ARGS__)
#define NSL__VA_INIT67(x, ...)  x, NSL__VA_INIT66(__VA_ARGS__)
#define NSL__VA_INIT68(x, ...)  x, NSL__VA_INIT67(__VA_ARGS__)
#define NSL__VA_INIT69(x, ...)  x, NSL__VA_INIT68(__VA_ARGS__)
#define NSL__VA_INIT70(x, ...)  x, NSL__VA_INIT69(__VA_ARGS__)
#define NSL__VA_INIT71(x, ...)  x, NSL__VA_INIT70(__VA_ARGS__)
#define NSL__VA_INIT72(x, ...)  x, NSL__VA_INIT71(__VA_ARGS__)
#define NSL__VA_INIT73(x, ...)  x, NSL__VA_INIT72(__VA_ARGS__)
#define NSL__VA_INIT74(x, ...)  x, NSL__VA_INIT73(__VA_ARGS__)
#define NSL__VA_INIT75(x, ...)  x, NSL__VA_INIT74(__VA_ARGS__)
#define NSL__VA_INIT76(x, ...)  x, NSL__VA_INIT75(__VA_ARGS__)
#define NSL__VA_INIT77(x, ...)  x, NSL__VA_INIT76(__VA_ARGS__)
#define NSL__VA_INIT78(x, ...)  x, NSL__VA_INIT77(__VA_ARGS__)
#define NSL__VA_INIT79(x, ...)  x, NSL__VA_INIT78(__VA_ARGS__)
#define NSL__VA_INIT80(x, ...)  x, NSL__VA_INIT79(__VA_ARGS__)
#define NSL__VA_INIT81(x, ...)  x, NSL__VA_INIT80(__VA_ARGS__)
#define NSL__VA_INIT82(x, ...)  x, NSL__VA_INIT81(__VA_ARGS__)
#define NSL__VA_INIT83(x, ...)  x, NSL__VA_INIT82(__VA_ARGS__)
#define NSL__VA_INIT84(x, ...)  x, NSL__VA_INIT83(__VA_ARGS__)
#define NSL__VA_INIT85(x, ...)  x, NSL__VA_INIT84(__VA_ARGS__)
#define NSL__VA_INIT86(x, ...)  x, NSL__VA_INIT85(__VA_ARGS__)
#define NSL__VA_INIT87(x, ...)  x, NSL__VA_INIT86(__VA_ARGS__)
#define NSL__VA_INIT88(x, ...)  x, NSL__VA_INIT87(__VA_ARGS__)
#define NSL__VA_INIT89(x, ...)  x, NSL__VA_INIT88(__VA_ARGS__)
#define NSL__VA_INIT90(x, ...)  x, NSL__VA_INIT89(__VA_ARGS__)
#define NSL__VA_INIT91(x, ...)  x, NSL__VA_INIT90(__VA_ARGS__)
#define NSL__VA_INIT92(x, ...)  x, NSL__VA_INIT91(__VA_ARGS__)
#define NSL__VA_INIT93(x, ...)  x, NSL__VA_INIT92(__VA_ARGS__)
#define NSL__VA_INIT94(x, ...)  x, NSL__VA_INIT93(__VA_ARGS__)
#define NSL__VA_INIT95(x, ...)  x, NSL__VA_INIT94(__VA_ARGS__)
#define NSL__VA_INIT96(x, ...)  x, NSL__VA_INIT95(__VA_ARGS__)
#define NSL__VA_INIT97(x, ...)  x, NSL__VA_INIT96(__VA_ARGS__)
#define NSL__VA_INIT98(x, ...)  x, NSL__VA_INIT97(__VA_ARGS__)
#define NSL__VA_INIT99(x, ...)  x, NSL__VA_INIT98(__VA_ARGS__)
#define NSL__VA_INIT100(x, ...) x, NSL__VA_INIT99(__VA_ARGS__)
#define NSL__VA_INIT101(x, ...) x, NSL__VA_INIT100(__VA_ARGS__)
#define NSL__VA_INIT102(x, ...) x, NSL__VA_INIT101(__VA_ARGS__)
#define NSL__VA_INIT103(x, ...) x, NSL__VA_INIT102(__VA_ARGS__)
#define NSL__VA_INIT104(x, ...) x, NSL__VA_INIT103(__VA_ARGS__)
#define NSL__VA_INIT105(x, ...) x, NSL__VA_INIT104(__VA_ARGS__)
#define NSL__VA_INIT106(x, ...) x, NSL__VA_INIT105(__VA_ARGS__)
#define NSL__VA_INIT107(x, ...) x, NSL__VA_INIT106(__VA_ARGS__)
#define NSL__VA_INIT108(x, ...) x, NSL__VA_INIT107(__VA_ARGS__)
#define NSL__VA_INIT109(x, ...) x, NSL__VA_INIT108(__VA_ARGS__)
#define NSL__VA_INIT110(x, ...) x, NSL__VA_INIT109(__VA_ARGS__)
#define NSL__VA_INIT111(x, ...) x, NSL__VA_INIT110(__VA_ARGS__)
#define NSL__VA_INIT112(x, ...) x, NSL__VA_INIT111(__VA_ARGS__)
#define NSL__VA_INIT113(x, ...) x, NSL__VA_INIT112(__VA_ARGS__)
#define NSL__VA_INIT114(x, ...) x, NSL__VA_INIT113(__VA_ARGS__)
#define NSL__VA_INIT115(x, ...) x, NSL__VA_INIT114(__VA_ARGS__)
#define NSL__VA_INIT116(x, ...) x, NSL__VA_INIT115(__VA_ARGS__)
#define NSL__VA_INIT117(x, ...) x, NSL__VA_INIT116(__VA_ARGS__)
#define NSL__VA_INIT118(x, ...) x, NSL__VA_INIT117(__VA_ARGS__)
#define NSL__VA_INIT119(x, ...) x, NSL__VA_INIT118(__VA_ARGS__)
#define NSL__VA_INIT120(x, ...) x, NSL__VA_INIT119(__VA_ARGS__)
#define NSL__VA_INIT121(x, ...) x, NSL__VA_INIT120(__VA_ARGS__)
#define NSL__VA_INIT122(x, ...) x, NSL__VA_INIT121(__VA_ARGS__)
#define NSL__VA_INIT123(x, ...) x, NSL__VA_INIT122(__VA_ARGS__)
#define NSL__VA_INIT124(x, ...) x, NSL__VA_INIT123(__VA_ARGS__)
#define NSL__VA_INIT125(x, ...) x, NSL__VA_INIT124(__VA_ARGS__)
#define NSL__VA_INIT126(x, ...) x, NSL__VA_INIT125(__VA_ARGS__)
#define NSL__VA_INIT127(x, ...) x, NSL__VA_INIT126(__VA_ARGS__)

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

/*!
 * Applies the function `f` to all arguments provided. For example,
 * `NSL_FOREACH(abs, 1, 2, 3)` would evaluate to `abs(1), abs(2), abs(3)`.
 *
 * `NSL_FOREACH` requires two levels of indirection. This is to ensure that
 * passing a macro that expands to the variadic arguments works correctly and is
 * expanded. `NSL__FOREACH` concatentates `NSL__FOREACH` with the number
 * of arguments provided (using `NSL_NARGS`, which is restricted to [0-127]).
 * The corresponding macro is called and it applies the function to the current
 * head and then calls the next macro in the chain.
 *
 * # Arguments
 * - `f`: The name of a function, macro, etc.
 *
 * # Requires
 * - `f` only accepts one argument and can be applied using `f(x)`.
 * - The number of arguments, N, is less than 128. If 128 or more arguments are
 *   provided, a compile time error is likely to be generated. There is no
 *   guarantee of this, however.
 *
 * # Returns
 * The function `f` mapped to all arguments provided.
 */
#define NSL_FOREACH(f, ...) NSL__FOREACH(f, __VA_ARGS__)
#define NSL__FOREACH(f, ...)                                                                       \
    NSL_CAT(NSL__FOREACH, NSL_NARGS(__VA_ARGS__))(f __VA_OPT__(, ) __VA_ARGS__)
#define NSL__FOREACH0(f)
#define NSL__FOREACH1(f, x)        f(x)
#define NSL__FOREACH2(f, x, ...)   f(x), NSL__FOREACH1(f, __VA_ARGS__)
#define NSL__FOREACH3(f, x, ...)   f(x), NSL__FOREACH2(f, __VA_ARGS__)
#define NSL__FOREACH4(f, x, ...)   f(x), NSL__FOREACH3(f, __VA_ARGS__)
#define NSL__FOREACH5(f, x, ...)   f(x), NSL__FOREACH4(f, __VA_ARGS__)
#define NSL__FOREACH6(f, x, ...)   f(x), NSL__FOREACH5(f, __VA_ARGS__)
#define NSL__FOREACH7(f, x, ...)   f(x), NSL__FOREACH6(f, __VA_ARGS__)
#define NSL__FOREACH8(f, x, ...)   f(x), NSL__FOREACH7(f, __VA_ARGS__)
#define NSL__FOREACH9(f, x, ...)   f(x), NSL__FOREACH8(f, __VA_ARGS__)
#define NSL__FOREACH10(f, x, ...)  f(x), NSL__FOREACH9(f, __VA_ARGS__)
#define NSL__FOREACH11(f, x, ...)  f(x), NSL__FOREACH10(f, __VA_ARGS__)
#define NSL__FOREACH12(f, x, ...)  f(x), NSL__FOREACH11(f, __VA_ARGS__)
#define NSL__FOREACH13(f, x, ...)  f(x), NSL__FOREACH12(f, __VA_ARGS__)
#define NSL__FOREACH14(f, x, ...)  f(x), NSL__FOREACH13(f, __VA_ARGS__)
#define NSL__FOREACH15(f, x, ...)  f(x), NSL__FOREACH14(f, __VA_ARGS__)
#define NSL__FOREACH16(f, x, ...)  f(x), NSL__FOREACH15(f, __VA_ARGS__)
#define NSL__FOREACH17(f, x, ...)  f(x), NSL__FOREACH16(f, __VA_ARGS__)
#define NSL__FOREACH18(f, x, ...)  f(x), NSL__FOREACH17(f, __VA_ARGS__)
#define NSL__FOREACH19(f, x, ...)  f(x), NSL__FOREACH18(f, __VA_ARGS__)
#define NSL__FOREACH20(f, x, ...)  f(x), NSL__FOREACH19(f, __VA_ARGS__)
#define NSL__FOREACH21(f, x, ...)  f(x), NSL__FOREACH20(f, __VA_ARGS__)
#define NSL__FOREACH22(f, x, ...)  f(x), NSL__FOREACH21(f, __VA_ARGS__)
#define NSL__FOREACH23(f, x, ...)  f(x), NSL__FOREACH22(f, __VA_ARGS__)
#define NSL__FOREACH24(f, x, ...)  f(x), NSL__FOREACH23(f, __VA_ARGS__)
#define NSL__FOREACH25(f, x, ...)  f(x), NSL__FOREACH24(f, __VA_ARGS__)
#define NSL__FOREACH26(f, x, ...)  f(x), NSL__FOREACH25(f, __VA_ARGS__)
#define NSL__FOREACH27(f, x, ...)  f(x), NSL__FOREACH26(f, __VA_ARGS__)
#define NSL__FOREACH28(f, x, ...)  f(x), NSL__FOREACH27(f, __VA_ARGS__)
#define NSL__FOREACH29(f, x, ...)  f(x), NSL__FOREACH28(f, __VA_ARGS__)
#define NSL__FOREACH30(f, x, ...)  f(x), NSL__FOREACH29(f, __VA_ARGS__)
#define NSL__FOREACH31(f, x, ...)  f(x), NSL__FOREACH30(f, __VA_ARGS__)
#define NSL__FOREACH32(f, x, ...)  f(x), NSL__FOREACH31(f, __VA_ARGS__)
#define NSL__FOREACH33(f, x, ...)  f(x), NSL__FOREACH32(f, __VA_ARGS__)
#define NSL__FOREACH34(f, x, ...)  f(x), NSL__FOREACH33(f, __VA_ARGS__)
#define NSL__FOREACH35(f, x, ...)  f(x), NSL__FOREACH34(f, __VA_ARGS__)
#define NSL__FOREACH36(f, x, ...)  f(x), NSL__FOREACH35(f, __VA_ARGS__)
#define NSL__FOREACH37(f, x, ...)  f(x), NSL__FOREACH36(f, __VA_ARGS__)
#define NSL__FOREACH38(f, x, ...)  f(x), NSL__FOREACH37(f, __VA_ARGS__)
#define NSL__FOREACH39(f, x, ...)  f(x), NSL__FOREACH38(f, __VA_ARGS__)
#define NSL__FOREACH40(f, x, ...)  f(x), NSL__FOREACH39(f, __VA_ARGS__)
#define NSL__FOREACH41(f, x, ...)  f(x), NSL__FOREACH40(f, __VA_ARGS__)
#define NSL__FOREACH42(f, x, ...)  f(x), NSL__FOREACH41(f, __VA_ARGS__)
#define NSL__FOREACH43(f, x, ...)  f(x), NSL__FOREACH42(f, __VA_ARGS__)
#define NSL__FOREACH44(f, x, ...)  f(x), NSL__FOREACH43(f, __VA_ARGS__)
#define NSL__FOREACH45(f, x, ...)  f(x), NSL__FOREACH44(f, __VA_ARGS__)
#define NSL__FOREACH46(f, x, ...)  f(x), NSL__FOREACH45(f, __VA_ARGS__)
#define NSL__FOREACH47(f, x, ...)  f(x), NSL__FOREACH46(f, __VA_ARGS__)
#define NSL__FOREACH48(f, x, ...)  f(x), NSL__FOREACH47(f, __VA_ARGS__)
#define NSL__FOREACH49(f, x, ...)  f(x), NSL__FOREACH48(f, __VA_ARGS__)
#define NSL__FOREACH50(f, x, ...)  f(x), NSL__FOREACH49(f, __VA_ARGS__)
#define NSL__FOREACH51(f, x, ...)  f(x), NSL__FOREACH50(f, __VA_ARGS__)
#define NSL__FOREACH52(f, x, ...)  f(x), NSL__FOREACH51(f, __VA_ARGS__)
#define NSL__FOREACH53(f, x, ...)  f(x), NSL__FOREACH52(f, __VA_ARGS__)
#define NSL__FOREACH54(f, x, ...)  f(x), NSL__FOREACH53(f, __VA_ARGS__)
#define NSL__FOREACH55(f, x, ...)  f(x), NSL__FOREACH54(f, __VA_ARGS__)
#define NSL__FOREACH56(f, x, ...)  f(x), NSL__FOREACH55(f, __VA_ARGS__)
#define NSL__FOREACH57(f, x, ...)  f(x), NSL__FOREACH56(f, __VA_ARGS__)
#define NSL__FOREACH58(f, x, ...)  f(x), NSL__FOREACH57(f, __VA_ARGS__)
#define NSL__FOREACH59(f, x, ...)  f(x), NSL__FOREACH58(f, __VA_ARGS__)
#define NSL__FOREACH60(f, x, ...)  f(x), NSL__FOREACH59(f, __VA_ARGS__)
#define NSL__FOREACH61(f, x, ...)  f(x), NSL__FOREACH60(f, __VA_ARGS__)
#define NSL__FOREACH62(f, x, ...)  f(x), NSL__FOREACH61(f, __VA_ARGS__)
#define NSL__FOREACH63(f, x, ...)  f(x), NSL__FOREACH62(f, __VA_ARGS__)
#define NSL__FOREACH64(f, x, ...)  f(x), NSL__FOREACH63(f, __VA_ARGS__)
#define NSL__FOREACH65(f, x, ...)  f(x), NSL__FOREACH64(f, __VA_ARGS__)
#define NSL__FOREACH66(f, x, ...)  f(x), NSL__FOREACH65(f, __VA_ARGS__)
#define NSL__FOREACH67(f, x, ...)  f(x), NSL__FOREACH66(f, __VA_ARGS__)
#define NSL__FOREACH68(f, x, ...)  f(x), NSL__FOREACH67(f, __VA_ARGS__)
#define NSL__FOREACH69(f, x, ...)  f(x), NSL__FOREACH68(f, __VA_ARGS__)
#define NSL__FOREACH70(f, x, ...)  f(x), NSL__FOREACH69(f, __VA_ARGS__)
#define NSL__FOREACH71(f, x, ...)  f(x), NSL__FOREACH70(f, __VA_ARGS__)
#define NSL__FOREACH72(f, x, ...)  f(x), NSL__FOREACH71(f, __VA_ARGS__)
#define NSL__FOREACH73(f, x, ...)  f(x), NSL__FOREACH72(f, __VA_ARGS__)
#define NSL__FOREACH74(f, x, ...)  f(x), NSL__FOREACH73(f, __VA_ARGS__)
#define NSL__FOREACH75(f, x, ...)  f(x), NSL__FOREACH74(f, __VA_ARGS__)
#define NSL__FOREACH76(f, x, ...)  f(x), NSL__FOREACH75(f, __VA_ARGS__)
#define NSL__FOREACH77(f, x, ...)  f(x), NSL__FOREACH76(f, __VA_ARGS__)
#define NSL__FOREACH78(f, x, ...)  f(x), NSL__FOREACH77(f, __VA_ARGS__)
#define NSL__FOREACH79(f, x, ...)  f(x), NSL__FOREACH78(f, __VA_ARGS__)
#define NSL__FOREACH80(f, x, ...)  f(x), NSL__FOREACH79(f, __VA_ARGS__)
#define NSL__FOREACH81(f, x, ...)  f(x), NSL__FOREACH80(f, __VA_ARGS__)
#define NSL__FOREACH82(f, x, ...)  f(x), NSL__FOREACH81(f, __VA_ARGS__)
#define NSL__FOREACH83(f, x, ...)  f(x), NSL__FOREACH82(f, __VA_ARGS__)
#define NSL__FOREACH84(f, x, ...)  f(x), NSL__FOREACH83(f, __VA_ARGS__)
#define NSL__FOREACH85(f, x, ...)  f(x), NSL__FOREACH84(f, __VA_ARGS__)
#define NSL__FOREACH86(f, x, ...)  f(x), NSL__FOREACH85(f, __VA_ARGS__)
#define NSL__FOREACH87(f, x, ...)  f(x), NSL__FOREACH86(f, __VA_ARGS__)
#define NSL__FOREACH88(f, x, ...)  f(x), NSL__FOREACH87(f, __VA_ARGS__)
#define NSL__FOREACH89(f, x, ...)  f(x), NSL__FOREACH88(f, __VA_ARGS__)
#define NSL__FOREACH90(f, x, ...)  f(x), NSL__FOREACH89(f, __VA_ARGS__)
#define NSL__FOREACH91(f, x, ...)  f(x), NSL__FOREACH90(f, __VA_ARGS__)
#define NSL__FOREACH92(f, x, ...)  f(x), NSL__FOREACH91(f, __VA_ARGS__)
#define NSL__FOREACH93(f, x, ...)  f(x), NSL__FOREACH92(f, __VA_ARGS__)
#define NSL__FOREACH94(f, x, ...)  f(x), NSL__FOREACH93(f, __VA_ARGS__)
#define NSL__FOREACH95(f, x, ...)  f(x), NSL__FOREACH94(f, __VA_ARGS__)
#define NSL__FOREACH96(f, x, ...)  f(x), NSL__FOREACH95(f, __VA_ARGS__)
#define NSL__FOREACH97(f, x, ...)  f(x), NSL__FOREACH96(f, __VA_ARGS__)
#define NSL__FOREACH98(f, x, ...)  f(x), NSL__FOREACH97(f, __VA_ARGS__)
#define NSL__FOREACH99(f, x, ...)  f(x), NSL__FOREACH98(f, __VA_ARGS__)
#define NSL__FOREACH100(f, x, ...) f(x), NSL__FOREACH99(f, __VA_ARGS__)
#define NSL__FOREACH101(f, x, ...) f(x), NSL__FOREACH100(f, __VA_ARGS__)
#define NSL__FOREACH102(f, x, ...) f(x), NSL__FOREACH101(f, __VA_ARGS__)
#define NSL__FOREACH103(f, x, ...) f(x), NSL__FOREACH102(f, __VA_ARGS__)
#define NSL__FOREACH104(f, x, ...) f(x), NSL__FOREACH103(f, __VA_ARGS__)
#define NSL__FOREACH105(f, x, ...) f(x), NSL__FOREACH104(f, __VA_ARGS__)
#define NSL__FOREACH106(f, x, ...) f(x), NSL__FOREACH105(f, __VA_ARGS__)
#define NSL__FOREACH107(f, x, ...) f(x), NSL__FOREACH106(f, __VA_ARGS__)
#define NSL__FOREACH108(f, x, ...) f(x), NSL__FOREACH107(f, __VA_ARGS__)
#define NSL__FOREACH109(f, x, ...) f(x), NSL__FOREACH108(f, __VA_ARGS__)
#define NSL__FOREACH110(f, x, ...) f(x), NSL__FOREACH109(f, __VA_ARGS__)
#define NSL__FOREACH111(f, x, ...) f(x), NSL__FOREACH110(f, __VA_ARGS__)
#define NSL__FOREACH112(f, x, ...) f(x), NSL__FOREACH111(f, __VA_ARGS__)
#define NSL__FOREACH113(f, x, ...) f(x), NSL__FOREACH112(f, __VA_ARGS__)
#define NSL__FOREACH114(f, x, ...) f(x), NSL__FOREACH113(f, __VA_ARGS__)
#define NSL__FOREACH115(f, x, ...) f(x), NSL__FOREACH114(f, __VA_ARGS__)
#define NSL__FOREACH116(f, x, ...) f(x), NSL__FOREACH115(f, __VA_ARGS__)
#define NSL__FOREACH117(f, x, ...) f(x), NSL__FOREACH116(f, __VA_ARGS__)
#define NSL__FOREACH118(f, x, ...) f(x), NSL__FOREACH117(f, __VA_ARGS__)
#define NSL__FOREACH119(f, x, ...) f(x), NSL__FOREACH118(f, __VA_ARGS__)
#define NSL__FOREACH120(f, x, ...) f(x), NSL__FOREACH119(f, __VA_ARGS__)
#define NSL__FOREACH121(f, x, ...) f(x), NSL__FOREACH120(f, __VA_ARGS__)
#define NSL__FOREACH122(f, x, ...) f(x), NSL__FOREACH121(f, __VA_ARGS__)
#define NSL__FOREACH123(f, x, ...) f(x), NSL__FOREACH122(f, __VA_ARGS__)
#define NSL__FOREACH124(f, x, ...) f(x), NSL__FOREACH123(f, __VA_ARGS__)
#define NSL__FOREACH125(f, x, ...) f(x), NSL__FOREACH124(f, __VA_ARGS__)
#define NSL__FOREACH126(f, x, ...) f(x), NSL__FOREACH125(f, __VA_ARGS__)
#define NSL__FOREACH127(f, x, ...) f(x), NSL__FOREACH126(f, __VA_ARGS__)

/*!
 * Calls the function `f` with all arguments provided. This is done recusively,
 * taking the rest of the provided arguments each time. For example,
 * `NSL_FORALL_REST(NSL_NCAT, 1, 2, 3)` would evaluate to
 * `NSL_NCAT(1, 2, 3), NSL_NCAT(2, 3), NSL_NCAT(3)`.
 *
 * `NSL_FORALL_REST` requires two levels of indirection. This is to ensure that
 * passing a macro that expands to the variadic arguments works correctly and is
 * expanded. `NSL__FORALL_REST` concatentates `NSL__FORALL_REST` with the number
 * of arguments provided (using `NSL_NARGS`, which is restricted to [0-127]).
 * The corresponding macro is called and it applies the function to the current
 * list of arguments, pops the head, and then calls the next macro in the chain.
 *
 * # Arguments
 * - `f`: The name of a function, macro, etc.
 *
 * # Requires
 * - `f` accepts a variadic number of arguments and can be applied using `f(x)`.
 * - The number of arguments, N, is less than 128. If 128 or more arguments are
 *   provided, a compile time error is likely to be generated. There is no
 *   guarantee of this, however.
 *
 * # Returns
 * The function `f` called with all arguments provided, recursing until there
 * are no arguments left.
 */
#define NSL_FORALL_REST(f, ...) NSL__FORALL_REST(f, __VA_ARGS__)
#define NSL__FORALL_REST(f, ...)                                                                   \
    NSL_CAT(NSL__FORALL_REST, NSL_NARGS(__VA_ARGS__))(f __VA_OPT__(, ) __VA_ARGS__)
#define NSL__FORALL_REST0(f)
#define NSL__FORALL_REST1(f, x)        f(x)
#define NSL__FORALL_REST2(f, x, ...)   f(x, __VA_ARGS__), NSL__FORALL_REST1(f, __VA_ARGS__)
#define NSL__FORALL_REST3(f, x, ...)   f(x, __VA_ARGS__), NSL__FORALL_REST2(f, __VA_ARGS__)
#define NSL__FORALL_REST4(f, x, ...)   f(x, __VA_ARGS__), NSL__FORALL_REST3(f, __VA_ARGS__)
#define NSL__FORALL_REST5(f, x, ...)   f(x, __VA_ARGS__), NSL__FORALL_REST4(f, __VA_ARGS__)
#define NSL__FORALL_REST6(f, x, ...)   f(x, __VA_ARGS__), NSL__FORALL_REST5(f, __VA_ARGS__)
#define NSL__FORALL_REST7(f, x, ...)   f(x, __VA_ARGS__), NSL__FORALL_REST6(f, __VA_ARGS__)
#define NSL__FORALL_REST8(f, x, ...)   f(x, __VA_ARGS__), NSL__FORALL_REST7(f, __VA_ARGS__)
#define NSL__FORALL_REST9(f, x, ...)   f(x, __VA_ARGS__), NSL__FORALL_REST8(f, __VA_ARGS__)
#define NSL__FORALL_REST10(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST9(f, __VA_ARGS__)
#define NSL__FORALL_REST11(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST10(f, __VA_ARGS__)
#define NSL__FORALL_REST12(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST11(f, __VA_ARGS__)
#define NSL__FORALL_REST13(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST12(f, __VA_ARGS__)
#define NSL__FORALL_REST14(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST13(f, __VA_ARGS__)
#define NSL__FORALL_REST15(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST14(f, __VA_ARGS__)
#define NSL__FORALL_REST16(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST15(f, __VA_ARGS__)
#define NSL__FORALL_REST17(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST16(f, __VA_ARGS__)
#define NSL__FORALL_REST18(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST17(f, __VA_ARGS__)
#define NSL__FORALL_REST19(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST18(f, __VA_ARGS__)
#define NSL__FORALL_REST20(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST19(f, __VA_ARGS__)
#define NSL__FORALL_REST21(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST20(f, __VA_ARGS__)
#define NSL__FORALL_REST22(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST21(f, __VA_ARGS__)
#define NSL__FORALL_REST23(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST22(f, __VA_ARGS__)
#define NSL__FORALL_REST24(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST23(f, __VA_ARGS__)
#define NSL__FORALL_REST25(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST24(f, __VA_ARGS__)
#define NSL__FORALL_REST26(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST25(f, __VA_ARGS__)
#define NSL__FORALL_REST27(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST26(f, __VA_ARGS__)
#define NSL__FORALL_REST28(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST27(f, __VA_ARGS__)
#define NSL__FORALL_REST29(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST28(f, __VA_ARGS__)
#define NSL__FORALL_REST30(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST29(f, __VA_ARGS__)
#define NSL__FORALL_REST31(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST30(f, __VA_ARGS__)
#define NSL__FORALL_REST32(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST31(f, __VA_ARGS__)
#define NSL__FORALL_REST33(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST32(f, __VA_ARGS__)
#define NSL__FORALL_REST34(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST33(f, __VA_ARGS__)
#define NSL__FORALL_REST35(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST34(f, __VA_ARGS__)
#define NSL__FORALL_REST36(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST35(f, __VA_ARGS__)
#define NSL__FORALL_REST37(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST36(f, __VA_ARGS__)
#define NSL__FORALL_REST38(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST37(f, __VA_ARGS__)
#define NSL__FORALL_REST39(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST38(f, __VA_ARGS__)
#define NSL__FORALL_REST40(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST39(f, __VA_ARGS__)
#define NSL__FORALL_REST41(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST40(f, __VA_ARGS__)
#define NSL__FORALL_REST42(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST41(f, __VA_ARGS__)
#define NSL__FORALL_REST43(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST42(f, __VA_ARGS__)
#define NSL__FORALL_REST44(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST43(f, __VA_ARGS__)
#define NSL__FORALL_REST45(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST44(f, __VA_ARGS__)
#define NSL__FORALL_REST46(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST45(f, __VA_ARGS__)
#define NSL__FORALL_REST47(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST46(f, __VA_ARGS__)
#define NSL__FORALL_REST48(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST47(f, __VA_ARGS__)
#define NSL__FORALL_REST49(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST48(f, __VA_ARGS__)
#define NSL__FORALL_REST50(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST49(f, __VA_ARGS__)
#define NSL__FORALL_REST51(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST50(f, __VA_ARGS__)
#define NSL__FORALL_REST52(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST51(f, __VA_ARGS__)
#define NSL__FORALL_REST53(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST52(f, __VA_ARGS__)
#define NSL__FORALL_REST54(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST53(f, __VA_ARGS__)
#define NSL__FORALL_REST55(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST54(f, __VA_ARGS__)
#define NSL__FORALL_REST56(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST55(f, __VA_ARGS__)
#define NSL__FORALL_REST57(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST56(f, __VA_ARGS__)
#define NSL__FORALL_REST58(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST57(f, __VA_ARGS__)
#define NSL__FORALL_REST59(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST58(f, __VA_ARGS__)
#define NSL__FORALL_REST60(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST59(f, __VA_ARGS__)
#define NSL__FORALL_REST61(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST60(f, __VA_ARGS__)
#define NSL__FORALL_REST62(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST61(f, __VA_ARGS__)
#define NSL__FORALL_REST63(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST62(f, __VA_ARGS__)
#define NSL__FORALL_REST64(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST63(f, __VA_ARGS__)
#define NSL__FORALL_REST65(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST64(f, __VA_ARGS__)
#define NSL__FORALL_REST66(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST65(f, __VA_ARGS__)
#define NSL__FORALL_REST67(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST66(f, __VA_ARGS__)
#define NSL__FORALL_REST68(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST67(f, __VA_ARGS__)
#define NSL__FORALL_REST69(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST68(f, __VA_ARGS__)
#define NSL__FORALL_REST70(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST69(f, __VA_ARGS__)
#define NSL__FORALL_REST71(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST70(f, __VA_ARGS__)
#define NSL__FORALL_REST72(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST71(f, __VA_ARGS__)
#define NSL__FORALL_REST73(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST72(f, __VA_ARGS__)
#define NSL__FORALL_REST74(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST73(f, __VA_ARGS__)
#define NSL__FORALL_REST75(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST74(f, __VA_ARGS__)
#define NSL__FORALL_REST76(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST75(f, __VA_ARGS__)
#define NSL__FORALL_REST77(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST76(f, __VA_ARGS__)
#define NSL__FORALL_REST78(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST77(f, __VA_ARGS__)
#define NSL__FORALL_REST79(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST78(f, __VA_ARGS__)
#define NSL__FORALL_REST80(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST79(f, __VA_ARGS__)
#define NSL__FORALL_REST81(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST80(f, __VA_ARGS__)
#define NSL__FORALL_REST82(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST81(f, __VA_ARGS__)
#define NSL__FORALL_REST83(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST82(f, __VA_ARGS__)
#define NSL__FORALL_REST84(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST83(f, __VA_ARGS__)
#define NSL__FORALL_REST85(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST84(f, __VA_ARGS__)
#define NSL__FORALL_REST86(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST85(f, __VA_ARGS__)
#define NSL__FORALL_REST87(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST86(f, __VA_ARGS__)
#define NSL__FORALL_REST88(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST87(f, __VA_ARGS__)
#define NSL__FORALL_REST89(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST88(f, __VA_ARGS__)
#define NSL__FORALL_REST90(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST89(f, __VA_ARGS__)
#define NSL__FORALL_REST91(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST90(f, __VA_ARGS__)
#define NSL__FORALL_REST92(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST91(f, __VA_ARGS__)
#define NSL__FORALL_REST93(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST92(f, __VA_ARGS__)
#define NSL__FORALL_REST94(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST93(f, __VA_ARGS__)
#define NSL__FORALL_REST95(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST94(f, __VA_ARGS__)
#define NSL__FORALL_REST96(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST95(f, __VA_ARGS__)
#define NSL__FORALL_REST97(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST96(f, __VA_ARGS__)
#define NSL__FORALL_REST98(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST97(f, __VA_ARGS__)
#define NSL__FORALL_REST99(f, x, ...)  f(x, __VA_ARGS__), NSL__FORALL_REST98(f, __VA_ARGS__)
#define NSL__FORALL_REST100(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST99(f, __VA_ARGS__)
#define NSL__FORALL_REST101(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST100(f, __VA_ARGS__)
#define NSL__FORALL_REST102(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST101(f, __VA_ARGS__)
#define NSL__FORALL_REST103(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST102(f, __VA_ARGS__)
#define NSL__FORALL_REST104(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST103(f, __VA_ARGS__)
#define NSL__FORALL_REST105(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST104(f, __VA_ARGS__)
#define NSL__FORALL_REST106(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST105(f, __VA_ARGS__)
#define NSL__FORALL_REST107(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST106(f, __VA_ARGS__)
#define NSL__FORALL_REST108(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST107(f, __VA_ARGS__)
#define NSL__FORALL_REST109(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST108(f, __VA_ARGS__)
#define NSL__FORALL_REST110(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST109(f, __VA_ARGS__)
#define NSL__FORALL_REST111(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST110(f, __VA_ARGS__)
#define NSL__FORALL_REST112(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST111(f, __VA_ARGS__)
#define NSL__FORALL_REST113(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST112(f, __VA_ARGS__)
#define NSL__FORALL_REST114(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST113(f, __VA_ARGS__)
#define NSL__FORALL_REST115(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST114(f, __VA_ARGS__)
#define NSL__FORALL_REST116(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST115(f, __VA_ARGS__)
#define NSL__FORALL_REST117(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST116(f, __VA_ARGS__)
#define NSL__FORALL_REST118(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST117(f, __VA_ARGS__)
#define NSL__FORALL_REST119(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST118(f, __VA_ARGS__)
#define NSL__FORALL_REST120(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST119(f, __VA_ARGS__)
#define NSL__FORALL_REST121(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST120(f, __VA_ARGS__)
#define NSL__FORALL_REST122(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST121(f, __VA_ARGS__)
#define NSL__FORALL_REST123(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST122(f, __VA_ARGS__)
#define NSL__FORALL_REST124(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST123(f, __VA_ARGS__)
#define NSL__FORALL_REST125(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST124(f, __VA_ARGS__)
#define NSL__FORALL_REST126(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST125(f, __VA_ARGS__)
#define NSL__FORALL_REST127(f, x, ...) f(x, __VA_ARGS__), NSL__FORALL_REST126(f, __VA_ARGS__)

/*!
 * Calls the function `f` with all arguments provided. This is done recusively,
 * taking the initial part of the provided arguments each time. For example,
 * `NSL_FORALL_INIT(NSL_NCAT, 1, 2, 3)` would evaluate to
 * `NSL_NCAT(1, 2, 3), NSL_NCAT(1, 2), NSL_NCAT(1)`.
 *
 * `NSL_FORALL_INIT` requires two levels of indirection. This is to ensure
 * that passing a macro that expands to the variadic arguments works correctly
 * and is expanded. `NSL__FORALL_INIT` concatentates `NSL__FORALL_INIT`
 * with the number of arguments provided (using `NSL_NARGS`, which is restricted
 * to [0-127]). The corresponding macro is called and it applies the function to
 * the current list of arguments, pops the tail, and then calls the next macro
 * in the chain.
 *
 * # Arguments
 * - `f`: The name of a function, macro, etc.
 *
 * # Requires
 * - `f` accepts a variadic number of arguments and can be applied using `f(x)`.
 * - The number of arguments, N, is less than 128. If 128 or more arguments are
 *   provided, a compile time error is likely to be generated. There is no
 *   guarantee of this, however.
 *
 * # Returns
 * The function `f` called with all arguments provided, recursing until there
 * are no arguments left.
 */
#define NSL_FORALL_INIT(f, ...) NSL__FORALL_INIT(f, __VA_ARGS__)
#define NSL__FORALL_INIT(f, ...)                                                                   \
    NSL_CAT(NSL__FORALL_INIT, NSL_NARGS(__VA_ARGS__))(f __VA_OPT__(, ) __VA_ARGS__)
#define NSL__FORALL_INIT0(f)
#define NSL__FORALL_INIT1(f, x)     f(x)
#define NSL__FORALL_INIT2(f, ...)   f(__VA_ARGS__), NSL__FORALL_INIT1(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT3(f, ...)   f(__VA_ARGS__), NSL__FORALL_INIT2(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT4(f, ...)   f(__VA_ARGS__), NSL__FORALL_INIT3(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT5(f, ...)   f(__VA_ARGS__), NSL__FORALL_INIT4(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT6(f, ...)   f(__VA_ARGS__), NSL__FORALL_INIT5(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT7(f, ...)   f(__VA_ARGS__), NSL__FORALL_INIT6(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT8(f, ...)   f(__VA_ARGS__), NSL__FORALL_INIT7(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT9(f, ...)   f(__VA_ARGS__), NSL__FORALL_INIT8(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT10(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT9(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT11(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT10(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT12(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT11(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT13(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT12(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT14(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT13(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT15(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT14(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT16(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT15(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT17(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT16(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT18(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT17(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT19(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT18(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT20(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT19(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT21(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT20(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT22(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT21(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT23(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT22(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT24(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT23(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT25(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT24(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT26(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT25(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT27(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT26(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT28(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT27(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT29(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT28(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT30(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT29(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT31(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT30(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT32(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT31(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT33(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT32(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT34(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT33(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT35(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT34(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT36(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT35(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT37(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT36(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT38(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT37(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT39(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT38(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT40(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT39(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT41(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT40(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT42(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT41(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT43(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT42(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT44(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT43(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT45(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT44(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT46(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT45(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT47(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT46(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT48(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT47(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT49(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT48(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT50(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT49(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT51(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT50(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT52(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT51(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT53(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT52(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT54(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT53(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT55(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT54(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT56(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT55(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT57(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT56(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT58(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT57(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT59(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT58(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT60(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT59(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT61(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT60(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT62(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT61(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT63(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT62(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT64(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT63(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT65(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT64(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT66(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT65(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT67(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT66(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT68(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT67(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT69(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT68(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT70(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT69(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT71(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT70(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT72(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT71(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT73(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT72(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT74(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT73(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT75(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT74(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT76(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT75(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT77(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT76(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT78(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT77(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT79(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT78(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT80(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT79(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT81(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT80(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT82(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT81(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT83(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT82(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT84(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT83(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT85(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT84(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT86(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT85(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT87(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT86(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT88(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT87(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT89(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT88(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT90(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT89(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT91(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT90(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT92(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT91(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT93(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT92(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT94(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT93(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT95(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT94(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT96(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT95(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT97(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT96(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT98(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT97(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT99(f, ...)  f(__VA_ARGS__), NSL__FORALL_INIT98(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT100(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT99(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT101(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT100(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT102(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT101(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT103(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT102(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT104(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT103(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT105(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT104(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT106(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT105(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT107(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT106(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT108(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT107(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT109(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT108(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT110(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT109(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT111(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT110(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT112(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT111(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT113(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT112(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT114(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT113(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT115(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT114(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT116(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT115(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT117(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT116(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT118(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT117(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT119(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT118(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT120(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT119(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT121(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT120(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT122(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT121(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT123(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT122(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT124(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT123(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT125(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT124(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT126(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT125(f, NSL_VA_INIT(__VA_ARGS__))
#define NSL__FORALL_INIT127(f, ...) f(__VA_ARGS__), NSL__FORALL_INIT126(f, NSL_VA_INIT(__VA_ARGS__))

#endif  // NSL_MAGIC_H_
