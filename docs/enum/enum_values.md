## Enum Entry Value List

Defined in header `<rbox/enum/enum_values.hpp>`.

```cpp
namespace rbox {

template <class E, enum_entry_order Order = enum_entry_order::original>
constexpr std::span<const std::remove_cv_t<E>> enum_values_v;

}  // namespace rbox
```

`enum_values_v<E>` gets the value list of given enum type.

> See [Enum Reflection Entry List](enum_meta_entries.md) for a combined example.
