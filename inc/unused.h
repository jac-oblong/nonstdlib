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
 * This module provides one macro (`NSL_UNUSED`). This provides a simple and
 * explicit way to mark a variable as unused.
 *
 * # Example
 *
 * ```c
 * void example(int variable_that_is_unused) {
 *     NSL_UNUSED(variable_that_is_unused);
   }
 * ```
 *
 * # Macro Flags
 *
 * - `NSL_UNUSED_STRIP_PREFIX` - Defining this macro before including this file
 *   will cause `NSL_UNUSED` to be available as `UNUSED`.
 */

#ifndef NSL_UNUSED_H_
#define NSL_UNUSED_H_

#define NSL_UNUSED_MAJOR 0
#define NSL_UNUSED_MINOR 1
#define NSL_UNUSED_PATCH 0

/**
 * Marks a variable as being unused to prevent errors.
 */
#define NSL_UNUSED(variable) (void)(variable)

#endif  // NSL_UNUSED_H_

#ifndef NSL_UNUSED_STRIP_PREFIX_GUARD_
#define NSL_UNUSED_STRIP_PREFIX_GUARD_

#ifdef NSL_UNUSED_STRIP_PREFIX
#    define UNUSED NSL_UNUSED
#endif  // NSL_UNUSED_STRIP_PREFIX

#endif  // NSL_UNUSED_STRIP_PREFIX_GUARD_
