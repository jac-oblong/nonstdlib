/*******************************************************************************/
/*                                                                             */
/* NonStdLib - A Non-Standard Library for C                                    */
/* Copyright (C) 2026  Jacob Long                                              */
/*                                                                             */
/* This program is free software: you can redistribute it and/or modify        */
/* it under the terms of the GNU Affero General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or           */
/* (at your option) any later version.                                         */
/*                                                                             */
/* This program is distributed in the hope that it will be useful,             */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of              */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               */
/* GNU Affero General Public License for more details.                         */
/*                                                                             */
/* You should have received a copy of the GNU Affero General Public License    */
/* along with this program.  If not, see <https://www.gnu.org/licenses/>.      */
/*                                                                             */
/*******************************************************************************/

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
