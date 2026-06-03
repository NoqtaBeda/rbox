## Enum Entry Value List

Defined in header `<rbox/enum/enum_values.hpp>`.

```cpp
namespace rbox {

template <class E, enum_entry_order Order = enum_entry_order::original>
constexpr std::span<const std::remove_cv_t<E>> enum_values_v;

}  // namespace rbox
```

`enum_values_v<E>` gets the value list of given enum type.

Example:

```cpp
enum class foo {
    three = 3,
    one = 1,
    zero = 0,
    two = 2,
};

using enum rbox::enum_entry_order;
for (auto value : rbox::enum_values_v<foo, by_value>) {
    std::println("{}", std::to_underlying(value));  // 0, 1, 2, 3
}
```
