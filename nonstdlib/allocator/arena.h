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
 *
 */

#ifndef NSL_ALLOCATOR_ARENA_H_
#define NSL_ALLOCATOR_ARENA_H_

/******************************************************************************/
/*                                                                            */
/*                                  INCLUDES                                  */
/*                                                                            */
/******************************************************************************/

#include "nonstdlib/common.h"

#include <stddef.h>

/******************************************************************************/
/*                                                                            */
/*                                 CONSTANTS                                  */
/*                                                                            */
/******************************************************************************/

const size_t NSL__ARENA_DEFAULT_CAPACITY = 4 * 1024;

/******************************************************************************/
/*                                                                            */
/*                                  STRUCTS                                   */
/*                                                                            */
/******************************************************************************/

typedef struct NSL_ArenaAllocator        NSL_ArenaAllocator;
typedef struct NSL__ArenaAllocatorRegion NSL__ArenaAllocatorRegion;

/*!
 * Owns a region of memory within the arena allocator. Separating the arena's
 * memory into regions allow for dynamically increasing the size of the arena if
 * more memory is requested.
 */
struct NSL__ArenaAllocatorRegion {
    //! The next region in the linked list of regions.
    NSL__ArenaAllocatorRegion *next;
    //! Pointer to the beginning of memory that this region owns.
    unsigned char *const buffer;
    //! Amount of the buffer that is currently allocated.
    size_t length;
    //! Total size of the buffer.
    const size_t capacity;
};

/*!
 * Simple arena/linear allocator. Allocations are made into an internal buffer,
 * with each new allocation being somewhere after the previous. Freeing of
 * individual memory allocations is not possible. This allocator is best used
 * for situations in which all allocated data can be freed at the same time.
 */
struct NSL_ArenaAllocator {
    //! The start of the linked list of regions
    NSL__ArenaAllocatorRegion begin;
    //! The end of the linked list of regions. This is also the currently active
    //! region.
    NSL__ArenaAllocatorRegion *end;
    //! Controls whether more memory can be allocated if the arena runs out of
    //! memory.
    const bool resizable;
};

/******************************************************************************/
/*                                                                            */
/*                                INITIALIZER                                 */
/*                                                                            */
/******************************************************************************/

//! Options accepted by `NSL_ArenaAllocator_init_opt`.
typedef struct NSL_ArenaAllocatorInitOpt {
    //! Initialize the new Arena from the buffer given.
    unsigned char *const from_buffer;
    //! The capacity of the arena allocator. If specified with `from_buffer`,
    //! this is the size of the buffer, otherwise memory will be allocated.
    const size_t capacity;
    //! Whether more memory can be allocated if the arena runs out.
    const bool resizable;
} NSL_ArenaAllocatorInitOpt;

/*!
 * Initializes an Arena. If no arguments are provided, the default values will
 * be used. Any member field of `NSL_ArenaAllocatorInitOpt` can be provided as
 * an argument.
 *
 * # Parameters
 * All parameters are defined within the `NSL_ArenaAllocatorInitOpt` struct.
 *
 * # Requires
 * - Arguments are specified using struct designator initialization syntax.
 *
 * # Returns
 * An initialized Arena. There is no guarantee that the struct is valid.
 */
#define NSL_ArenaAllocator_init(...)                                                               \
    NSL_ArenaAllocator_init_opt(                                                                   \
        (NSL_ArenaAllocatorInitOpt) {.capacity  = NSL__ARENA_DEFAULT_CAPACITY,                     \
                                     .resizable = true __VA_OPT__(, ) __VA_ARGS__})
[[nodiscard]]
NSL_ArenaAllocator NSL_ArenaAllocator_init_opt(NSL_ArenaAllocatorInitOpt options);

/******************************************************************************/
/*                                                                            */
/*                               DEINITIALIZER                                */
/*                                                                            */
/******************************************************************************/

//! Options accepted by `NSL_ArenaAllocator_deinit_opt`.
typedef struct NSL_ArenaAllocatorDeinitOpt {
    //! Whether or not the Arena was initialized from a buffer. This prevents
    //! attempts to free that buffer.
    const bool init_from_buffer;
} NSL_ArenaAllocatorDeinitOpt;

/*!
 * Deinitializes the provided Arena. If only the arena is provided, default
 * values will be used for everything else. Any member field of
 * `NSL_ArenaAllocatorDeinitOpt` can be provided as an argument.
 *
 * * # Parameters
 * All parameters are defined within the `NSL_ArenaAllocatorDeinitOpt` struct.
 *
 * # Requires
 * - Arguments are specified using struct designator initialization syntax.
 */
#define NSL_ArenaAllocator_deinit(arena, ...)                                                      \
    NSL_ArenaAllocator_deinit_opt((arena), (NSL_ArenaAllocatorDeinitOpt) {__VA_ARGS__})
void NSL_ArenaAllocator_deinit_opt(NSL_ArenaAllocator *arena, NSL_ArenaAllocatorDeinitOpt options);

#endif  // NSL_ALLOCATOR_ARENA_H_

/******************************************************************************/
/*                                                                            */
/*                               IMPLEMENTATION                               */
/*                                                                            */
/******************************************************************************/

#if NSL_SHOULD_INCLUDE_IMPLEMENTATION(ALLOCATOR, ARENA)

#endif  // NSL_SHOULD_INCLUDE_IMPLEMENTATION(ALLOCATOR, ARENA)

/******************************************************************************/
/*                                                                            */
/*                                STRIP PREFIX                                */
/*                                                                            */
/******************************************************************************/

#if NSL_SHOULD_STRIP_PREFIX(ALLOCATOR, ARENA)
#define ArenaAllocator            NSL_ArenaAllocator
#define ArenaAllocatorInitOpt     NSL_ArenaAllocatorInitOpt
#define ArenaAllocator_init       NSL_ArenaAllocator_init
#define ArenaAllocator_init_opt   NSL_ArenaAllocator_init_opt
#define ArenaAllocatorDeinitOpt   NSL_ArenaAllocatorDeinitOpt
#define ArenaAllocator_deinit     NSL_ArenaAllocator_deinit
#define ArenaAllocator_deinit_opt NSL_ArenaAllocator_deinit_opt
#endif  // NSL_SHOULD_STRIP_PREFIX(ALLOCATOR, ARENA)
