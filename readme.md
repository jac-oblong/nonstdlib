# NonStdLib

A non-standard library for C. This contains a collection of types, functions, utilities, etc.
that are designed to work together. The library utilizes the 
[stb-style header-only design](https://github.com/nothings/stb). As such, parts of the library can 
be used simply by copying the header file. Not all files are capable of this, however, as they rely 
on other parts of the library. Additionally, if desired, source files exist for each part of the 
library.

> [!WARNING]
> This library has only been testing using gcc on Linux. There is no guarantee that it will function
> correctly with any other compiler / on any other platform.

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [NonStdLib](#nonstdlib)
  - [File Structure](#file-structure)
  - [Features](#features)
    - [Miscellaneous](#miscellaneous)
    - [Data Structures](#data-structures)
    - [String Types](#string-types)
    - [Allocators](#allocators)
    - [Larger Tools](#larger-tools)

<!-- markdown-toc end -->

## File Structure

- [Header Files](file:inc) - The actual implementation of the library units. Everything of note
  exists within this folder.
- [Source Files](file:src) - Optional individual library units. These files simply include and
  corresponding header file and implementation.
- [Tests](file:test) - Unit tests for the library.
- [Documentation](file:doc) - Various pieces of documentation. The documentation / examples for each
  module are in the header files themselves. This folder holds other pieces of documentation that
  do not fit elsewhere.

## Features

### Miscellaneous

- **Todo** - Provides a macro for marking things that need to be done.
- **Logging** - TODO
- **Pretty Printer** - TODO
- **Sequencer** - TODO
- **Command Line Arguments Parser** - TODO
- **JSON Serializer / De-Serializer** - TODO

### Data Structures

- **Array** - Auto-Generated Wrapper for C Arrays
- **DynamicArray** - TODO
- **LinkedList** - TODO
- **SinglyLinkedList** - TODO
- **BinaryTree** - TODO
- **Tree** - TODO
- **Map** - TODO
- **HashMap** - TODO
- **RedBlackTree** - TODO

### String Types

- **String** - TODO
- **StringView** - TODO
- **StringBuilder** - TODO
- **Rope** - TODO

### Allocators

[Reference](http://www.gingerbill.org/series/memory-allocation-strategies/)

- **Pool** - TODO
- **Stack** - TODO
- **Arena** - TODO
- **FreeList** - TODO
- **Buddy** - TODO

### Larger Tools

- **Unit Test** - TODO
- **Build Tool** - TODO
