## Enum Entry List

Defined in header `<rbox/enum/enum_entries.hpp>`.

```cpp
namespace rbox {

template <enum_type E>
using enum_entry_t = std::pair<std::remove_cv_t<E>, std::string_view>;

template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr std::span<const enum_entry_t<E>> enum_entries_v;

}  // namespace rbox
```

`enum_entries_v<E>` gets the (value, string) pair list of given enum type.

Example:

```cpp
enum class foo {
    three = 3,
    one = 1,
    zero = 0,
    two = 2,
};

using enum enum_entry_order;
template for (constexpr auto order : {original, by_value, by_name})
{
    for (auto [value, name] : rbox::enum_entries_v<foo>) {
        std::println("{:>5s}: {}", name, std::to_underlying(value));
    }
}
// Output of each order:
// | original | by_value |  by_name |
// |----------|----------|----------|
// | three: 3 |  zero: 0 |   one: 1 |
// |   one: 1 |   one: 1 | three: 3 |
// |  zero: 0 |   two: 2 |   two: 2 |
// |   two: 2 | three: 3 |  zero: 0 |
```
