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
 * A collection of helper functions / macros / etc. that are used throughout the
 * project. Most of the utilities in this file are not meant for external use,
 * but some can be used externally, so they are not marked as private.
 * Additionally, this contains user overrides for certain functions.
 *
 * # Macro Flags
 *
 * - `NSL_STRIP_PREFIX`: Defining this macro before this file is included will
 *   cause public facing utilities to have the `NSL_`/`nsl_` prefix stripped
 *   from their name.
 * - `NSL_COMMON_STRIP_PREFIX`: Same as `NSL_STRIP_PREFIX`, but will only strip
 *   prefixes for things in this module and not in the entire library.
 *
 * # Redefinable Macros
 *
 * - `nsl_eprintf`: Can be defined to redirect error messages.
 * - `nsl_abort`: Can be defined to redirect abort handler.
 * - `nsl_malloc`: Can be defined to redirect memory allocation.
 * - `nsl_realloc`: Can be defined to redirect memory re-allocation.
 * - `nsl_free`: Can be defined to redirect memory freeing.
 */

#ifndef NSL_COMMON_H_
#define NSL_COMMON_H_

/******************************************************************************/
/*                                                                            */
/*                           LIBRARY VERSION MACROS                           */
/*                                                                            */
/******************************************************************************/

#define NSL_VERSION_MAJOR 0
#define NSL_VERSION_MINOR 1
#define NSL_VERSION_PATCH 0

/******************************************************************************/
/*                                                                            */
/*                                META MACROS                                 */
/*                                                                            */
/******************************************************************************/

/*!
 * Concatenates the two provided arguments into one. For example,
 * `NSL_CONCAT(foo, bar)` would be expanded by the preprocessor to `foobar`.
 *
 * # Parameters
 * - `a`: The first part of the full name.
 * - `b`: The second part of the full name.
 *
 * # Returns
 * The concatenated full name.
 */
#define NSL_CONCAT_(a, b) a##b
#define NSL_CONCAT(a, b)  NSL_CONCAT_(a, b)

/******************************************************************************/
/*                                                                            */
/*                            TYPE CHECKING MACROS                            */
/*                                                                            */
/******************************************************************************/

/*!
 * Returns whether or not `expr` is of type `expected`.
 *
 * # Parameters
 * - `expr`: The element to for which the type assertion is made.
 * - `expected`: The expected type of `expr`.
 *
 * # Requires
 * - `expr` is any expression except the comma operator.
 * - `expected` is a complete object type that is not variably-modified (i.e.
 *   not a VLA or a pointer to a VLA).
 *
 * # Returns
 * - `true` if `expr` is of type `expected`.
 * - `false` if `expr` is not of type `expected`.
 */
#define nsl_is_of_type(expr, expected) _Generic((expr), expected: true, default: false)

/*!
 * Checks that `expr` is of type `expected` and generates a compile-time error
 * if they are not.
 *
 * # Parameters
 * - `expr`: The element to for which the type assertion is made.
 * - `expected`: The expected type of `expr`.
 * - `msg`: The error message to display if they are not the same type.
 *
 * # Requires
 * - `expr` is any expression except the comma operator.
 * - `expected` is a complete object type that is not variably-modified (i.e.
 *   not a VLA or a pointer to a VLA).
 * - `msg` is a string literal.
 *
 * # Aborts
 * - Will cause a compilation error if `expr` is not of type `expected`.
 */
#define nsl_assert_is_of_type(expr, expected, msg)                                                 \
    static_assert(nsl_is_of_type(expr, expected), msg)

/******************************************************************************/
/*                                                                            */
/*                      USER-DEFINED FUNCTION OVERRIDES                       */
/*                                                                            */
/******************************************************************************/

/*!
 * `nsl_eprintf` can optionally be defined by the user to redirect error
 * messages. By default, all messages are printed to `stderr`.
 *
 * # Example
 *
 * ```c
 * #define nsl_eprintf ignore_error_messages
 * // include NSL header files here
 *
 * int ignore_error_messages(const char *restrict fmt, ...) {
 *     return 0;
 * }
 * ```
 *
 * NOTE: The overriding function does not need to be declared prior to any
 * includes or even in the same translation unit, as the function will be
 * forward declared using `extern`.
 *
 * # Parameters
 * - `fmt`: The format string.
 * - `...`: The variadic arguments to be formatted.
 *
 * # Requires
 * - The macro evaluation has the type `void(*)(const char *restrict, ...)`.
 *
 * # Returns
 * The number of bytes printed excluding the null byte. On error, a negative
 * value is returned, and `errno` is set to indicate the error.
 */
#if defined(nsl_eprintf)
extern int nsl_eprintf(const char *restrict fmt, ...);
#else
#    include <stdarg.h>
#    include <stdio.h>
#    define nsl_eprintf nsl__eprintf

static inline int nsl__eprintf(const char *restrict fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int result = vfprintf(stderr, fmt, ap);
    va_end(ap);
    return result;
}
#endif  // defined(nsl_eprintf)
nsl_assert_is_of_type(nsl_eprintf,
                      int (*)(const char *restrict, ...),
                      "'nsl_eprintf' is of wrong type");

/*!
 * `nsl_abort` can optionally be defined by the user to redirect `abort` to a
 * different handler. This is helpful if any cleanup needs to occur before the
 * program terminates.
 *
 * # Example
 *
 * ```c
 * #define nsl_abort cleanup_and_abort
 * // include NSL header files here
 * #include <stdlib.h>
 *
 * void cleanup_and_abort(void) {
 *     // cleanup
 *     abort();
 * }
 * ```
 *
 * NOTE: The overriding function does not need to be declared prior to any
 * includes or even in the same translation unit, as the function will be
 * forward declared using `extern`.
 *
 * # Requires
 * - The macro evaluation has the type `[[noreturn]] void(*)(void)`.
 *
 * # Aborts
 * Yes.
 *
 * # Returns
 * No.
 */
#if defined(nsl_abort)
extern void nsl_abort(void);
#else
#    include <stdlib.h>
#    define nsl_abort abort
#endif  // defined(nsl_abort)
nsl_assert_is_of_type(nsl_abort, void (*)(void), "'nsl_abort' is of wrong type");

/*!
 * `nsl_malloc` can optionally be defined by the user to redirect `malloc` to a
 * different handler.
 *
 * NOTE: The overriding function does not need to be declared prior to any
 * includes or even in the same translation unit, as the function will be
 * forward declared using `extern`.
 *
 * # Requires
 * - The macro evaluation has the type `void *(*)(size_t)`.
 * - The behavior of `nsl_malloc` must match the behavior of libc's `malloc`
 *   (i.e. if there is an error, `nullptr` is returned).
 * - Redefining `nsl_malloc` requires redefining `nsl_realloc` and `nsl_free`.
 */
#if defined(nsl_malloc)
extern void *nsl_malloc(void);
#    if !defined(nsl_free) || !defined(nsl_realloc)
#        error "Defining `nsl_malloc` requires defining both `nsl_realloc` and `nsl_free`"
#    endif
#else
#    include <stdlib.h>
#    define nsl_malloc malloc
#endif  // defined(nsl_malloc)
nsl_assert_is_of_type(nsl_malloc, void *(*)(size_t), "'nsl_malloc' is of wrong type");

/*!
 * `nsl_realloc` can optionally be defined by the user to redirect `realloc` to
 * a different handler.
 *
 * NOTE: The overriding function does not need to be declared prior to any
 * includes or even in the same translation unit, as the function will be
 * forward declared using `extern`.
 *
 * # Requires
 * - The macro evaluation has the type `void *(*)(void *, size_t)`.
 * - The behavior of `nsl_realloc` must match the behavior of libc's `realloc`
 *   (i.e. if there is an error, `nullptr` is returned).
 * - Redefining `nsl_realloc` requires redefining `nsl_malloc` and `nsl_free`.
 */
#if defined(nsl_realloc)
extern void *nsl_realloc(void);
#    if !defined(nsl_free) || !defined(nsl_malloc)
#        error "Defining `nsl_realloc` requires defining both `nsl_malloc` and `nsl_free`"
#    endif
#else
#    include <stdlib.h>
#    define nsl_realloc realloc
#endif  // defined(nsl_realloc)
nsl_assert_is_of_type(nsl_realloc, void *(*)(void *, size_t), "'nsl_realloc' is of wrong type");

/*!
 * `nsl_free` can optionally be defined by the user to redirect `free` to a
 * different handler.
 *
 * NOTE: The overriding function does not need to be declared prior to any
 * includes or even in the same translation unit, as the function will be
 * forward declared using `extern`.
 *
 * # Requires
 * - The macro evaluation has the type `void (*)(void *)`.
 * - The behavior of `nsl_free` must match the behavior of libc's `free`.
 * - Redefining `nsl_free` requires redefining `nsl_malloc` and `nsl_realloc`.
 */
#if defined(nsl_free)
extern void *nsl_free(void);
#    if !defined(nsl_malloc) || !defined(nsl_realloc)
#        error "Defining `nsl_free` requires defining both `nsl_malloc` and `nsl_realloc`"
#    endif
#else
#    include <stdlib.h>
#    define nsl_free free
#endif  // defined(nsl_free)
nsl_assert_is_of_type(nsl_free, void (*)(void *), "'nsl_free' is of wrong type");

/******************************************************************************/
/*                                                                            */
/*              MACROS FOR GENERATING ERRORS ON UNFINISHED CODE               */
/*                                                                            */
/******************************************************************************/

/*!
 * Generates a **TODO** error. This macro can be defined to redirect to either
 * `nsl_todo_runtime` or `nsl_todo_comptime`. This will cause the error to be
 * generated during program execution or at compile time, respectively. By
 * default, it redirects to `nsl_todo_runtime`. It can easily be redirected to
 * `nsl_todo_comptime` by adding `-Dnsl_todo=nsl_todo_comptime` as a flag when
 * compiling, making it easy to find unfinished code.
 *
 * # Parameters:
 * - `message`: The message to print.
 *
 * # Requires
 * - Look at documentation for `nsl_todo_runtime` and `nsl_todo_comptime` to see
 *   the potential requirements.
 *
 * # Aborts
 * Is guarantee to cause the program to abort.
 */
#if defined(nsl_todo)
#    if nsl_todo != nsl_todo_runtime && nsl_todo != nsl_todo_comptime
#        error "'nsl_todo' must redirect to either 'nsl_todo_runtime' or 'nsl_todo_comptime'"
#    endif  // nsl_todo != nsl_todo_runtime && nsl_todo != nsl_todo_comptime
#else
#    define nsl_todo nsl_todo_runtime
#endif      // defined(nsl_todo)

/*!
 * Generates a **TODO** error at runtime. The error will only be generated if
 * the function is called. The error message is printed using `nsl_eprintf` and
 * includes the file and line of the todo message. The process immediately exits
 * using `nsl_abort`.
 *
 * # Parameters
 * - `message`: The message to print.
 *
 * # Requires
 * - `message` must be formatable with `"%s"`.
 *
 * # Aborts
 * Is guaranteed to cause the program to abort using `nsl_abort`.
 */
#define nsl_todo_runtime(message) nsl__todo_runtime(message, __FILE__, __LINE__)

[[noreturn]]
void nsl__todo_runtime(const char *message, const char *file, int line) {
    nsl_eprintf("[TODO] %s:%d - %s\n", file, line, message);
    nsl_abort();
}

/*!
 * Generates a **TODO** error at compile time. The error will be generated no
 * matter what, and is generated using `static_assert`.
 *
 * # Parameters
 * - `message`: The message to print.
 *
 * # Requires
 * - `message` should ideally be a string literal, however, it can be anything,
 *   as a type mismatch will also cause a compile time error.
 *
 * # Aborts
 * Will prevent program from being compiled by generating an error.
 */
#define nsl_todo_comptime(message) static_assert(false, "[TODO] " message)

/******************************************************************************/
/*                                                                            */
/*                                STRIP PREFIX                                */
/*                                                                            */
/******************************************************************************/

#if defined(NSL_STRIP_PREFIX) || defined(NSL_COMMON_STRIP_PREFIX)
#    define is_of_type        nsl_is_of_type
#    define assert_is_of_type nsl_assert_is_of_type
#    define todo              nsl_todo
#    define todo_runtime      nsl_todo_runtime
#    define todo_comptime     nsl_todo_comptime
#endif  // defined(NSL_STRIP_PREFIX) || defined(NSL_COMMON_STRIP_PREFIX)

#endif  // NSL_COMMON_H_
