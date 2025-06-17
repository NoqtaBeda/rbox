## Enum Entry Name List

Defined in header `<rbox/enum/enum_names.hpp>`.

```cpp
namespace rbox {

template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr std::span<const std::string_view> enum_names_v;

template <enum_type E>
constexpr bool enum_names_are_ascii_only_v;

}  // namespace rbox
```

- `enum_names_v<E>` gets the name list of given enum type.
- `enum_names_are_ascii_only_v<E>` checks whether all enum entry names contain only ASCII characters. Non-ASCII characters are NOT supported in enum entry names for case-insensitive operations.

Example:

```cpp
enum foo {
    foo_First = 1,
    foo_Second = 2,
    foo_Invalid = -1,
    foo_FIRST = 1,
};

using enum enum_entry_order;
template for (constexpr auto order : {original, by_value, by_name})
{
    for (size_t i = 0; i < rbox::enum_count_v<foo>; i++) {
        std::println("{0:>11s}", rbox::enum_names_v<foo, order>[i]);
    }
}
// Output of each order:
// |        original          |        by_value          |         by_name          |
// |--------------------------|--------------------------|--------------------------|
// |              foo_First   |             foo_Invalid   |               foo_FIRST   |
// |             foo_Second   |               foo_First   |               foo_First   |
// |            foo_Invalid   |               foo_FIRST   |             foo_Invalid   |
// |              foo_FIRST   |              foo_Second   |              foo_Second   |

static_assert(rbox::enum_names_are_ascii_only_v<foo>);
```
