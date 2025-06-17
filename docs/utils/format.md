## `std::format` Support for `meta_basic_string_view`

Defined in header `<rbox/utils/format.hpp>`.

Provides `std::formatter` (and `fmt::formatter` if available) specializations for `rbox::meta_basic_string_view<CharT>`, enabling use with `std::format` and `fmt::format`.

```cpp
template <class CharT>
struct std::formatter<rbox::meta_basic_string_view<CharT>, CharT>
    : std::formatter<std::basic_string_view<CharT>, CharT> {
    /* ... */
};

#if __has_include(<fmt/format.h>)
template <class CharT>
struct fmt::formatter<rbox::meta_basic_string_view<CharT>, CharT>
    : fmt::formatter<std::basic_string_view<CharT>, CharT> {
    /* ... */
};
#endif
```

This allows `meta_string_view` and related types to be formatted directly:

```cpp
auto sv = rbox::meta_string_view{"hello"};
auto str = std::format("{}", sv);  // "hello"
```

> `format` support for `meta_basic_string_view` is detached from `<rbox/utils/meta_string_view.hpp>` _intentionally_ to minimize header propagation.
