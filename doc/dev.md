# Developer Documentation

The majority of documentation for developers exists within each module. The documentation there
is designed primarily for users of this library, but it is also assumed that they may want to
know about internal workings, so that information is included.

This document serves to fill in the gaps and provides a general, high-level overview of the
patterns, conventions, and decisions that exist throughout the entire library.

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [Developer Documentation](#developer-documentation)
  - [Naming Convention](#naming-convention)
  - [Version Convention](#version-convention)
  - [Module Organization](#module-organization)
  - [Design Patterns / Philosophies](#design-patterns--philosophies)
    - [Using the C Standard Library](#using-the-c-standard-library)
    - [Handling Library Inter-Dependencies](#handling-library-inter-dependencies)
    - [Handling External Dependencies](#handling-external-dependencies)
  - [Documentation Comments](#documentation-comments)

<!-- markdown-toc end -->


## Naming Convention

**EVERYTHING** that is public facing / publicly accessible is prefixed with `nsl_`. An additional
prefix will be used that corresponds with the name of the module. This is not always the full
name of the module, but may be a shortened name (similar to how `nsl` is the shorted form of
`NonStdLib`). Some, but not all, prefixes can be removed. If removal of a prefix would cause a
name collision, it will not be removed.

- **Functions** - All functions follow the `snake_case` naming convention. Functions that belong to 
  a specific type will be prefixed with the type name. This means that it will be a mixture of 
  `PascalCase` and `snake_case`. For example, if the type `Array` has a function `push`, the full 
  name of the function will be `Array_push`.
- **Structs** - All structs follow the `PascalCase` naming convention. This includes any macros that
  evaluates to a struct. All struct member variables follow the `snake_case` naming convention.
- **Enums** - All enums follow the `PascalCase` naming convention. All enum values follow the
  `ALL_CAPS` naming convention.
- **Macros** - Naming conventions for macros are mixed. 
  - If the macro is intended to be used as a function (i.e. it is a wrapper for a function or it 
    acts as a generic function) it should follow the function naming convention.
  - If the macro is intended to be used as a type (e.g. it is in alias for a specific type) it 
    should follow the naming convention of that type.
  - If the macro does not fit into any of the above categories, it should follow the `ALL_CAPS` 
    naming convention.
- **Local Variables** - All local variables follow the `snake_case` naming convention. This is not
  particularly relevant to users, but it is important to be consistent.
- **Global Variables** - All global variables follow the `snake_case` naming convention and are
  prefixed with `g_` (this prefix cannot be removed and should go in front of any library /
  module prefix). Usage of global variables should be avoided if possible.
- **Private Identifiers** - Any identifier that is meant to be private **will not** be included in 
  the declaration section of the header file, if possible. Its name will be prefixed with an
  additional underscore (e.g. `nsl__` instead of `nsl_`) which will be after the module name (if
  one exists). They will not support stripping the prefix, and will not be included in the
  global scope (i.e. declared as static), if possible.

## Version Convention

[Semantic Versioning](https://semver.org) is used for all version needs. The macros 
`NSL_VERSION_MAJOR`, `NSL_VERSION_MINOR`, and `NSL_VERSION_PATCH` can be used to query the current
version of the library.

- Editing comments will not change the version at all.
- **Patch** will be incremented for bug fixes and refactoring that do not touch the API.
- **Minor** will be incremented for new functions, types, etc. that do not touch the existing API.
  This also applies for marking items as deprecated.
- **Major** will be incremented for changes to the existing API. The API should only change when 
  items that have been marked as deprecated are removed.

## Module Organization

All modules should follow the organizational layout described below. Some parts of the layout
are not needed for every module, and so they can be skipped.

- **License** - Include a copy of the license.
- **Module Documentation** - A high-level overview of the module. This generally follows the format
  below.
  - Explanation of what the module provides. This includes pointing out a few types, functions,
    etc. that are good places to get started.
  - Basic example code for using the module.
  - Explanation of each macro flag and how they should be used.
  - Explanation of macros that can be user defined and what they control.
- **Header Section** - The declaration of the interface. This section should be wrapped in a macro
  include guard. The name of the guard should follow the pattern `NSL_<MODULE>_H_`. The order of
  elements is listed below. If anything is meant to be public facing, it should have an
  associated comment with the definition / declaration.
  - The macros containing the version of the module.
  - Any potential includes.
  - Macros that can be user defined.
  - Everything else is pretty free in terms of order. Generally, the order macros, types, public
    functions, private functions should be followed. But if a macro is a wrapper for a function,
    it may make sense to put it next to that function. Or if a type is only ever meant for one
    function, the same applies.
  - There should be a `NSL_<MODULE>_DEF` macro that users can define to restrict functions to be
    `static` and/or `inline`.
- **Implementation Section** - The implementation of the interface. The order of elements here does
  not matter as much, as most users should not need to look through this section. This section
  should be surrounded by a guard with the format `NSL_<MODULE>_IMPLEMENTATION`.
- **Prefix-Stripping Section** - The definition of macros to strip prefixes. Again, the order of
  elements does not matter too much. This should be placed after everything else, however, so
  that there are no potential conflicts. This section should be surrounded by a guard with the
  format `NSL_<MODULE>_STRIP_PREFIX_GUARD_`.

## Design Patterns / Philosophies

This section contains some common design patterns and philosophies that are present throughout
this library.

### Using the C Standard Library

Avoiding use of the C standard library is often impossible and providing a complete and robust
method for users to redefine standard library functions quickly becomes excessive. This is
further exacerbated by the fact that very few systems require avoiding the standard library.
That being said, supporting redefining specific library functions is a good idea.
Specifically, allocating memory, accessing the file system, printing text, and other similar
functions should support wrappers that the user can redefine.

### Handling External Dependencies

As each module is meant to be usable on their own, requiring external dependencies should be
avoided if possible. Obviously, if the module is designed to be a wrapper around some other
library, avoiding the dependency is impossible. However, if reasonable, it is better to create
macros that can be redefined by the user or functions that the user can implement.

## Documentation Comments

All documentation comments are written as though they were markdown. Comments that are meant to be
treated as documentation will use the `/*! ... */` or `//! ...` format. For structs, enums, etc.
the type itself will be documented and each member will also have their documentation. For
functions, in addition to a general overview of the function, the following sections will be
implemented as necessary:

- **Parameters**: Details about each parameter that the function accepts.
- **Requires**: Lists any conditions that must be met prior to calling the function. This can be
  constraints on the parameters, on global variables, etc.
- **Modifies**: Lists any values (either parameters or global variables) that are modified by the
  function. This also includes the effects of the modification.
- **Aborts**: Describes any behavior / conditions that might cause the program to crash.
- **Returns**: Details about the value returned by the function.
