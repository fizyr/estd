# estd

`estd` is a header-only C++ library providing some additional utilities in the style of the standard library.

The include files are ordered hierarchically in the style of Boost.
You can include a whole sublibrary with a single header, or pick only the bits you want to use.

```cpp
// Include all tuple utilities in one go:
#include <estd/tuple.hpp>

// Include only the estd::for_each family of functions:
#include <estd/tuple/for_each.hpp>


```

An overview of the libraries currently contained in `estd`:

* **convert**: A standardized conversion convention, with support for custom tagged conversion functions.
* **range**: Utility functions to operate on ranges of elements.
* **result**: A type that can hold either an error or a value.
* **traits**: Some additional type traits not in #include <type_traits>
* **tuple**: Utility functions to work with tuples easily (looping, folding, zipping, transforming).
* **utility**: Assorted small utility functions and classes.

`estd` is licensed under the BSD 3-Clause license.
