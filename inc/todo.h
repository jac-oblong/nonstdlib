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

/**
 * This module provides one primary macro (`NSL_TODO`) and two secondary macros
 * (`NSL_TODO_RUNTIME` and `NSL_TODO_COMPTIME`). Each of the macros take one
 * argument and are meant to stop execution of the program with an error
 * message. This is useful when a feature is currently incomplete and it should
 * be marked as such.
 *
 * # Example
 *
 * ```c
 * void function_that_is_not_yet_implemented() {
 *     NSL_TODO("implement function_that_is_not_yet_implemented");
   }
 * ```
 *
 * By default, `NSL_TODO` redirects to `NSL_TODO_RUNTIME`, so the above example
 * will cause the process to exit if `function_that_is_not_yet_implemented` is
 * ever called. `NSL_TODO` can be redirected to `NSL_TODO_COMPTIME` with a
 * define. For example, `gcc -DNSL_TODO=NSL_TODO_COMPTIME example.c`.
 *
 * # Macro Flags
 *
 * - `NSL_TODO_STRIP_PREFIX` - Defining this macro before including this file
 *   will cause the provided macros to be available without the `NSL_` prefix.
 *   For example, `NSL_TODO` would just become `TODO`.
 *
 * # Redefinable Macros
 *
 * - `NSL_TODO_EPRINTF` - This macro is intended to be used to print an error
 *   message to `stderr`. It can be redifined if the message should be
 *   redirected elsewhere.
 * - `NSL_TODO_EXIT` - This macro is intended to cause the process to exit. It
 *   can be redefined if some other function should handle the exiting.
 */

#ifndef NSL_TODO_H_
#define NSL_TODO_H_

#define NSL_TODO_MAJOR 0
#define NSL_TODO_MINOR 1
#define NSL_TODO_PATCH 0

#include <assert.h>

/**
 * This macro can optionally be user defined if using `fprintf` from `stdio.h`
 * is undesirable. The macro accepts a string literal formatter and should be
 * variadic.
 */
#ifndef NSL_TODO_EPRINTF
#    include <stdio.h>
#    define NSL_TODO_EPRINTF(format, ...) fprintf(stderr, format, __VA_ARGS__)
#endif  // NSL_TODO_EPRINTF

/**
 * This macro can optionally be user defined if using `exit(EXIT_FAILURE)` from
 * `stdlib.h` is undesirable. The macro does not have arguments and should
 * evaluate to a statement.
 */
#ifndef NSL_TODO_EXIT
#    include <stdlib.h>
#    define NSL_TODO_EXIT exit(EXIT_FAILURE)
#endif  // NSL_TODO_EXIT

#ifndef NSL_TODO
/**
 * Generates a TODO error. This macro is defined to redirect to either
 * `NSL_TODO_RUNTIME`, in order to generate an error at runtime, or
 * `NSL_TODO_COMPTIME`, in order to generate an error at compile time.
 *
 * # Arguments
 * - `message`: The message to print. Should ideally be a string literal. Look
 *   at documentation for `NSL_TODO_RUNTIME` and `NSL_TODO_COMPTIME` for more
 *   information.
 */
#    define NSL_TODO NSL_TODO_RUNTIME
#endif  // NSL_TODO

/**
 * Generates an error at runtime. The error message is printed to `stderr` and
 * will include the file and line that the macro was invoked on. The process
 * will immediately exit after printing the error.
 *
 * # Arguments
 * - `message`: The message to print. Ideally, this should be a string literal,
 *   but it can be any type that can be printed with `"%s"`.
 */
#define NSL_TODO_RUNTIME(message)                                              \
    do {                                                                       \
        NSL_TODO_EPRINTF("[TODO] %s:%d - %s\n", __FILE__, __LINE__, message);  \
        NSL_TODO_EXIT;                                                         \
    } while (0)

/**
 * Generates an error at compile time.
 *
 * # Arguments
 * - `message`: The message to print. Ideally, this should be a string literal,
 *   but it can be anything as a type mismatch will also cause a compile time
 *   error.
 */
#define NSL_TODO_COMPTIME(message) static_assert(0, "[TODO] " message)

#endif  // NSL_TODO_H_

#ifndef NSL_TODO_STRIP_PREFIX_GUARD_
#define NSL_TODO_STRIP_PREFIX_GUARD_

#ifdef NSL_TODO_STRIP_PREFIX
#    define TODO          NSL_TODO
#    define TODO_RUNTIME  NSL_TODO_RUNTIME
#    define TODO_COMPTIME NSL_TODO_COMPTIME
#endif  // NSL_TODO_STRIP_PREFIX

#endif  // NSL_TODO_STRIP_PREFIX_GUARD_
