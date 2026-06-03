## Enum Reflection Entry List

Defined in header `<rbox/enum/enum_meta_entries.hpp>`.

```cpp
namespace rbox {

template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr std::span<const std::meta::info> enum_meta_entries_v;

}  // namespace rbox
```

`enum_meta_entries_v<E>` gets the reflector list of entries in given enum type.
For the default order, `enum_meta_entries_v<E>` is equivalent to `std::define_static_array(enumerators_of(^^E))` with less typing.

Example:

```cpp
enum class foo {
    three = 3,
    one = 1,
    zero = 0,
    two = 2,
};

using enum enum_entry_order;

template for (constexpr std::meta::info order : rbox::enum_meta_entries_v<rbox::enum_entry_order>)
{
    std::print("{}:", identifier_of(order));
    for (auto value : rbox::enum_values_v<foo, [:order:]>) {  // Example of enum_values_v
        std::print(" {}", std::to_underlying(value));
    }
    std::print("; ");
}
// original: 3 1 0 2; by_value: 0 1 2 3; by_name: 1 3 2 0;
```
