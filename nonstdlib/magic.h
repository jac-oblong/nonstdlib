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
#define NSL__CONCAT(a, b) a##b
#define NSL_CONCAT(a, b)  NSL__CONCAT(a, b)

#endif  // NSL_MAGIC_H_
