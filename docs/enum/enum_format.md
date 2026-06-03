## `std::format` and `fmt::format` Support for Enum Types

Defined in header `<rbox/enum/enum_format.hpp>`.

```cpp

// std::format support
template <rbox::enum_type E>
struct std::formatter<E>;

// fmt::format support (conditional)
#if __has_include(<fmt/format.h>)
template <rbox::enum_type E>
struct fmt::formatter<E>;
#endif
```

rbox provides formatting support for both `std::format` and `fmt::format`. Include this header to enable enum formatting.

- For enum non-flag values, no format specifier is needed;
- For enum flag values, the format specifier should start with character `'f'` or `'F'`, then optionally follows the delimiter string.

`enum_name` and `enum_flags_name` are used respectively. Undefined enum values (or enum flag values that cannot be decomposed into a disjunction of defined enum entries) will be output by pattern `(%1)%2`, where `%1` is the type name and `%2` is the underlying value.

Example:

```cpp
// Include this header to enable enum formatting
#include <rbox/enum/enum_format.hpp>

enum class permissions {
    read = 1,
    write = 2,
    execute = 4,
};

// As non-flag: outputs "write"
std::println("{}", permissions::write);
// As non-flag: outputs "(permissions)7"
std::println("{}", static_cast<permissions>(7));
// As non-flag: outputs "(permissions)0"
std::println("{}", static_cast<permissions>(0));
// As flag: possibly outputs "execute|write|read"
std::println("{:F}", static_cast<permissions>(7));
// As flag with custom delimiter " | ": possibly outputs "execute | write | read"
std::println("{:F | }", static_cast<permissions>(7));
// As flags: outputs "" as 0 is a valid flag value which represents empty set
std::println("{:F}", static_cast<permissions>(0));
// Outputs "(permissions)-1"
std::println("{:F}", static_cast<permissions>(-1));
```
