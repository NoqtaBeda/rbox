## Enum Type Name

Defined in header `<rbox/enum/enum_type_name.hpp>`.

```cpp
namespace rbox {

template <enum_type E>
constexpr std::string_view enum_type_name_v;

template <enum_type E>
constexpr std::string_view enum_type_display_string_v;

}  // namespace rbox
```

- `enum_type_name_v<E>` gets the type name (dealiased, cv-qualifiers discarded) of given enum type.
- `enum_type_display_string_v<E>` gets the prettier type name of given enum type, which is implementation-defined by the compiler.

Example:

```cpp
enum class foo : int;
namespace bar {
enum class values : long;
}  // namespace bar
using bar_values = bar::values;

static_assert(rbox::enum_type_name_v<foo> == "foo");
static_assert(rbox::enum_type_name_v<const bar::values> == "values");  // cv discarded
static_assert(rbox::enum_type_name_v<bar_values> == "values");         // Dealiased
// Implementation defined, may be "const bar::values"
printf("%s\n", rbox::enum_type_display_string_v<const bar_values>.data());
```
