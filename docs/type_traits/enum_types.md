## Enum Types & Enum Flag Types

Defined in header `<rbox/type_traits/enum_types.hpp>`.

```cpp
namespace rbox {

template <class T>
concept enum_type = std::is_enum_v<T>;

template <class T>
concept scoped_enum_type = std::is_scoped_enum_v<T>;

#define RBOX_AS_ENUM_FLAG [[= /* unspecified */]]

// Supports specialization
template <class T>
constexpr bool is_enum_flag_v = false;

template <class T>
concept enum_flag_type;

}  // namespace rbox
```

The concepts `enum_type` and `scoped_enum_type` test whether the given type `T` is any enum type, or scoped enum type (`enum class`).

The concept `enum_flag_type` tests whether the given type `T` is an enum flag type. rbox identifies an enum type `T` as `enum_flag_type` only if `T` satisfies one of the following:

- `is_enum_flag_v<T>` is specialized as `true`;
- `T` is annotated via macro `RBOX_AS_ENUM_FLAG`.

Example:

```cpp
enum class permissions_t : uint8_t {
    read = 1,
    write = 2,
    execute = 4,
};

// (1) Specialize rbox::is_enum_flag_v
template <>
constexpr bool rbox::is_enum_flag_v<permissions_t>;

// (2) Annotate via macro RBOX_AS_ENUM_FLAG
enum RBOX_AS_ENUM_FLAG format_t {
    format_bold = 1,
    format_italic = 2,
    format_underline = 4,
};

// cv-qualification is supported
static_assert(rbox::enum_flag_type<const permissions_t>);
static_assert(rbox::enum_flag_type<format_t>);
```
