# Generic Type Implementation ADR

NonStdLib is meant to implement multiple generic data structures. C, however, does not easily
support generic types. This document examines the different options for implementing generic types
in C that were identified and why the final design was chosen. A simple `Array` type that is a
wrapper around C's builtin arrays will be used as a case study.

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [Generic Type Implementation ADR](#generic-type-implementation-adr)
  - [Generic Types with `void*`](#generic-types-with-void)
    - [Pros](#pros)
    - [Cons](#cons)
  - [Generic Type with `char*`](#generic-type-with-char)
    - [Pros](#pros-1)
    - [Cons](#cons-1)
  - [Generic Type with Macro Magic](#generic-type-with-macro-magic)
    - [Pros](#pros-2)
    - [Cons](#cons-2)
  - [Generic Array by Not Being Generic](#generic-array-by-not-being-generic)
    - [Pros](#pros-3)
    - [Cons](#cons-3)
  - [Generic Array with `_Generic`](#generic-array-with-_generic)
    - [Pros](#pros-4)
    - [Cons](#cons-4)
  - [Chosen Implementation](#chosen-implementation)

<!-- markdown-toc end -->


## Generic Types with `void*`

One popular method for achieving a generic type in C is through the use of `void*`. C's standard
library does this in multiple cases (`realloc`, `memcpy`, `write`, etc.). A potential implementation
of generic types with an `Array` is shown below.

```c
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

typedef struct Array Array;
struct Array {
    void **items;    // Pointer to the C array that is being wrapped.
    size_t length;   // Number of items currently in the C array.
    size_t capacity; // Total number of elements that can be added to the C array.
};

// Pushes `item` into the end of `arr`. Returns `true` if success, `false` otherwise.
bool arr_push(Array *arr, void *item) {
    if (arr == nullptr || arr->length == arr->capacity) { return false; }
    arr->items[arr->length] = item;
    arr->length++;
    return true;
}

// Removes the last element of `arr` and puts it in `item`. Returns `true` if success.
bool arr_pop(Array *arr, void **item) {
    if (arr == nullptr || arr->length == 0) { return false; }
    arr->length--;
    *item = arr->items[arr->length];
    return true;
}

int main() {
    void *carr[3];
    void *x;
    Array arr = (Array) { .items = carr, .length = 0, .capacity = 3 };
    assert(arr_push(&arr, (void**)1));
    assert(arr_push(&arr, (void**)2));
    assert(arr_push(&arr, (void**)3));
    assert(!arr_push(&arr, (void**)4));
    assert(arr_pop(&arr, &x));
    assert((int)x == 3);
    assert(arr_pop(&arr, &x));
    assert((int)x == 2);
    assert(arr_pop(&arr, &x));
    assert((int)x == 1);
    assert(!arr_pop(&arr, &x));
}
```

### Pros

- Handles most use cases.
- Simple and clean interface. There are no confusing macros, or other indirections.

### Cons

- All type safety is gone. Data structures can be assumed to be of one type, but actually be of
  another type.
- Users are required to think about the lifetime of the data structure and the objects in it. If
  the item is a primitive type, it is possible (but **not** preferable) to simply cast to `void*`.
  However, if the item is a struct, it may need to be allocated somewhere. This can very easily
  lead to bugs involving dangling pointers.
- Requires casting function pointers to `void*`. This can cause problems on some architectures.

## Generic Types with `char*`

An alternate version with its own tradeoffs is a `char*` array. Here, instead of storing pointers,
the data is copied into the internal C array.

```c
#include <assert.h>
#include <stddef.h>
#include <string.h>

typedef struct Array Array;
struct Array {
    char  *items;     // Pointer to the C array that is being wrapped.
    size_t item_size; // Size of the items in the array.
    size_t length;    // Number of items currently in the C array.
    size_t capacity;  // Total number of elements that can be added to the C array.
};

// Pushes `item` into the end of `arr`. Returns `true` if success, `false` otherwise.
bool arr_push(Array *arr, void *item) {
    if (arr == nullptr || arr->length == arr->capacity) { return false; }

    memcpy(arr->items + (arr->item_size * arr->length), item, arr->item_size);
    arr->length++;
    return true;
}

// Removes the last element of `arr` and puts it in `item`. Returns `true` if success.
bool arr_pop(Array *arr, void *item) {
    if (arr == nullptr || arr->length == 0) { return false; }
    arr->length--;
    memcpy(item, (arr->items + (arr->item_size * arr->length)), arr->item_size);
    return true;
}

int main() {
    char carr[3 * sizeof(int)];
    int x;
    Array arr = (Array) { .items = carr, .item_size = sizeof(int), .length = 0, .capacity = 3 };
    x = 1;
    assert(arr_push(&arr, &x));
    x = 2;
    assert(arr_push(&arr, &x));
    x = 3;
    assert(arr_push(&arr, &x));
    x = 4;
    assert(!arr_push(&arr, &x));
    assert(arr_pop(&arr, &x));
    assert(x == 3);
    assert(arr_pop(&arr, &x));
    assert(x == 2);
    assert(arr_pop(&arr, &x));
    assert(x == 1);
    assert(!arr_pop(&arr, &x));
}
```

### Pros

- Handles most use cases.
- Simple and clean interface. There are no confusing macros, or other indirections.
- Does not require user to allocate memory for larger objects.

### Cons

- All type safety is gone. Data structures can be assumed to be of one type, but actually be of
  another type.
- Usage is slightly clunky, as it requires a pointer, so objects have to be allocated somewhere
  (can be on the stack) before being pushed into the array.
- Requires casting function pointers to `void*`. This can cause problems on some architectures.

## Generic Types with Macro Magic

Another popular method for achieving generic types is through the use of macros. In this
implementation, there is no `Array` struct and no functions for pushing / popping. Instead,
everything is done through macros.

```c
#include <assert.h>
#include <stddef.h>

#define Array(type) struct { type *items; size_t length; size_t capacity; }

// Pushes `item` into the end of `arr`. Returns `true` if success, `false` otherwise.
#define arr_push(arr, item)                                 \
    ((arr) == nullptr || (arr)->length == (arr)->capacity ? \
        false :                                             \
        ((arr)->items[(arr)->length++] = item, true))

// Removes the last element of `arr` and puts it in `item`. Returns `true` if success.
#define arr_pop(arr, item_ptr)                \
    ((arr) == nullptr || (arr)->length == 0 ? \
        false :                               \
        ((arr)->length--, *(item_ptr) = (arr)->items[(arr)->length], true))

int main() {
    int carr[3] = {0};
    int x;
    Array(int) arr = { .items = carr, .length = 0, .capacity = 3 };
    assert(arr_push(&arr, 1));
    assert(arr_push(&arr, 2));
    assert(arr_push(&arr, 3));
    assert(!arr_push(&arr, 4));
    assert(arr_pop(&arr, &x));
    assert(x == 3);
    assert(arr_pop(&arr, &x));
    assert(x == 2);
    assert(arr_pop(&arr, &x));
    assert(x == 1);
    assert(!arr_pop(&arr, &x));
}
```

### Pros

- Handles most use cases.
- Everything is type safe.
- Does not require user to allocate memory for larger objects.
- Function pointers do not need to be casted to any other type.

### Cons

- Reading and understanding implementation of macros is difficult. Current implementation uses both
  ternary and comma operators, which are not the easiest to reason about.
- Macros can lead to unexpected results (re-evaluating expression multiple times, etc.).
- Macros limit the interfaces that can be exposed to the user. For example, in order to "return" a
  value from a macro, the comma operator must be used. However, if variables need to be declared, or
  other such statements, the comma operator will not work. This makes writing the implementation a
  considerable challenge.
- Will likely increase code size as compiler may not be able to optimize away the repeated code into
  a single function. Not necessarily a major concern, though.

## Generic Types by Not Being Generic

One way to keep type safety while also being able to achieve the desired interface is to create a
new `Array` struct and associated functions for each desired type. This, however, defeats the point
of generic arrays, unless the struct and associated functions can be automatically generated. A
simple macro interface can be used to accomplish this goal.

```c
#include <assert.h>
#include <stddef.h>

#define DEFINE_ARRAY(type, type_name, func_name)                               \
    typedef struct type_name ## Array type_name ## Array;                      \
    struct type_name ## Array {                                                \
        type  *items;                                                          \
        size_t length;                                                         \
        size_t capacity;                                                       \
    };                                                                         \
                                                                               \
    bool func_name ## _arr_push(type_name ## Array *arr, type item) {          \
        if (arr == nullptr || arr->length == arr->capacity) { return false; }  \
        arr->items[arr->length] = item;                                        \
        arr->length++;                                                         \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool func_name ## _arr_pop(type_name ## Array *arr, type *item) {          \
        if (arr == nullptr || arr->length == 0) { return false; }              \
        arr->length--;                                                         \
        *item = arr->items[arr->length];                                       \
        return true;                                                           \
    }

DEFINE_ARRAY(int, Int, int)

int main() {
    int carr[3];
    int x;
    IntArray arr = (IntArray) { .items = carr, .length = 0, .capacity = 3 };
    assert(int_arr_push(&arr, 1));
    assert(int_arr_push(&arr, 2));
    assert(int_arr_push(&arr, 3));
    assert(!int_arr_push(&arr, 4));
    assert(int_arr_pop(&arr, &x));
    assert(x == 3);
    assert(int_arr_pop(&arr, &x));
    assert(x == 2);
    assert(int_arr_pop(&arr, &x));
    assert(x == 1);
    assert(!int_arr_pop(&arr, &x));
}
```

### Pros

- Handles most use cases.
- Everything is type safe.
- Does not require user to allocate memory for larger objects.

### Cons

- Will increase code size if multiple array types are used. This is not a major concern, however,
  and is the norm for languages with templates.
- Cannot handle function pointers. This can be fixed by using `typedef` to define a function
  pointer type.
- Verbose interface. The interface also prevents accepting an array of any type and doing basic
  operations with it (checking if it is empty / obtaining the length).

## Generic Types with `_Generic`

`_Generic` allows for picking between expressions based on the type of the given expression. This
can be used in conjunction with a macro to remove some of the verboseness while maintaining type
safety.

```c
#include <assert.h>
#include <stddef.h>

#define arr_push(arr, item)   \
    _Generic((arr)->items[0], \
        int : int_arr_push) (arr, item)

#define arr_pop(arr, item)   \
    _Generic((arr)->items[0], \
        int : int_arr_pop) (arr, item)

typedef struct IntArray IntArray;
struct IntArray {
    int  *items;
    size_t length;
    size_t capacity;
};

bool int_arr_push(IntArray *arr, int item) {
    if (arr == nullptr || arr->length == arr->capacity) { return false; }
    arr->items[arr->length] = item;
    arr->length++;
    return true;
}

bool int_arr_pop(IntArray *arr,int *item) {
    if (arr == nullptr || arr->length == 0) { return false; }
    arr->length--;
    *item = arr->items[arr->length];
    return true;
}

int main() {
    int carr[3];
    int x;
    IntArray arr = (IntArray) { .items = carr, .length = 0, .capacity = 3 };
    assert(arr_push(&arr, 1));
    assert(arr_push(&arr, 2));
    assert(arr_push(&arr, 3));
    assert(!arr_push(&arr, 4));
    assert(arr_pop(&arr, &x));
    assert(x == 3);
    assert(arr_pop(&arr, &x));
    assert(x == 2);
    assert(arr_pop(&arr, &x));
    assert(x == 1);
    assert(!arr_pop(&arr, &x));
}
```

### Pros

- Handles all use cases. Even works with function pointers.
- Everything is type safe.
- Does not require user to allocate memory for larger objects.

### Cons

- Will increase code size if multiple array types are used. This is not a major concern, however,
  and is the norm for languages with templates.
- Requires knowing all possible types beforehand so that `arr_push` and `arr_pop` can be correctly
  defined. This is not realistically possible.
- User is required to implement all arrays themselves.

## Generic Types with Template-esque Headers

One problem with using macros to define a generic data structure implementation is that reading the
source code becomes difficult as it is in a macro definition. A workaround for this is to use a
header file to hold the data structure implementation. An example of such a header (and the usage)
is shown below.

```c
#include <stddef.h>

#ifndef SOME_HEADER_FILE_H_
#define FIRST_ARG_(a, b) a
#define FIRST_ARG(...) FIRST_ARG_(__VA_ARGS__)
#define SECOND_ARG_(a, b) b
#define SECOND_ARG(...) SECOND_ARG_(__VA_ARGS__)
#define PREFIX(prefix, rest) prefix ## rest
#endif // SOME_HEADER_FILE_H_

#ifndef T
#error "T must be defined"
#endif // T

#define Name FIRST_ARG(T)
#define type SECOND_ARG(T)

typedef struct Name {
    type *items;
    size_t length;
    size_t capacity;
} Name;

bool PREFIX(Name, _arr_push)(Name *arr, type item) {
    if (arr == nullptr || arr->length == arr->capacity) { return false; }
    arr->items[arr->length] = item;
    arr->length++;
    return true;
}

bool PREFIX(Name, _arr_pop)(Name *arr, type item) {
    if (arr == nullptr || arr->length == 0) { return false; }
    arr->length--;
    *item = arr->items[arr->length];
    return true;
}

#undef type
#undef Name
#undef T
```

```c
#include <assert.h>

#define T Ints, int
#include "some_header_file.h"

int main() {
    int carr[3];
    int x;
    Ints arr = (Ints) { .items = carr, .length = 0, .capacity = 3 };
    assert(Ints_arr_push(&arr, 1));
    assert(Ints_arr_push(&arr, 2));
    assert(Ints_arr_push(&arr, 3));
    assert(!Ints_arr_push(&arr, 4));
    assert(Ints_arr_pop(&arr, &x));
    assert(x == 3);
    assert(Ints_arr_pop(&arr, &x));
    assert(x == 2);
    assert(Ints_arr_pop(&arr, &x));
    assert(x == 1);
    assert(!Ints_arr_pop(&arr, &x));
}
```

### Pros

- Handles most use cases.
- Everything is type safe.
- Does not require user to allocate memory for larger objects.
- Does not require implementation to be defined within macro.

### Cons

- Will increase code size if multiple array types are used. This is not a major concern, however,
  and is the norm for languages with templates.
- Cannot handle function pointers. This can be fixed by using `typedef` to define a function
  pointer type.
- Verbose interface. The interface also prevents accepting an array of any type and doing basic
  operations with it (checking if it is empty / obtaining the length).
- Requires re-including same file for each implementation, which can get weird.
- Requires some indirection with macros.

## Chosen Implementation

The usage of a template-esque header to generate an generic types is the best option because it is
robust and does not sacrifice type safety or readability. The problem with function pointers can be
solved by requiring `typedef`, which realistically is smart because function pointer syntax is not
easily digestible.

Additionally, if possible, `_Generic` may be used to reduce the verboseness of the interface. This
will only be added after the core implementation is complete, however.
