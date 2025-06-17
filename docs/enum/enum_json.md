## Enum Entries Dumped as JSON

Defined in header `<rbox/enum/enum_json.hpp>`.

```cpp
namespace rbox {

template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr std::string enum_json();

template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr std::string_view enum_json_static_v;

}  // namespace rbox
```

- `enum_json<E>()` gets the JSON representation of given enum class in compact style. The returned `std::string` is built (possibly) during runtime.
- `enum_json_static_v<E>` gets the same string which is built during compile-time and guaranteed to be null-terminated.

Example:

```cpp
enum class foo {
    three = 3,
    one = 1,
    zero = 0,
    two = 2,
};

constexpr auto by_value = rbox::enum_entry_order::by_value;
// original: {"three":3,"one":1,"zero":0,"two":2}
printf("original: %s\n", rbox::enum_json<foo>().c_str());
// by_value: {"zero":0,"one":1,"two":2,"three":3}
printf("by_value: %s\n", rbox::enum_json_static_v<foo, by_value>.data());
```
