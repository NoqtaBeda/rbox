## String-like Types

Defined in header `<rbox/type_traits/string_like_types.hpp>`.

```cpp
namespace rbox {

template <class T>
concept string_like = /* ... */;

template <class T, class CharT>
concept string_like_of = /* ... */;

template <string_like T>
using char_type_t = /* ... */;

}  // namespace rbox
```

The concept `string_like` tests whether `T` is a (maybe cv-qualified) string-like type. String-like types are those which can be converted to `std::basic_string_view<CharT>` for some character type `CharT` via the `std::basic_string_view` constructor, including but not limited to:

- C-style non-volatile character arrays: `CharT[]`, `const CharT[N]`, etc.
- C-style string pointer: `CharT*` and `const CharT*`;
- `std::basic_string<CharT, Traits, Alloc>` for any `Traits` and `Alloc`;
- `std::basic_string_view<CharT, Traits>` for any `Traits`;
- `meta_basic_string_view<CharT>` (defined in `<rbox/utils/meta_string_view.hpp>`, a [structural](https://cppreference.com/w/cpp/language/template_parameters.html) replacement of `std::basic_string_view`);
- Contiguous range of characters: `std::vector<CharT>`, `std::array<CharT, N>`, etc.

The concept `string_like_of` tests whether `T` is a (maybe cv-qualified) string-like type whose character type is exactly `CharT`.

The type alias `char_type_t` extracts the character type of the string-like type `T`.

- For contiguous ranges, `char_type_t` is its `value_type`;
- For C-style strings that decay to `CharT*` or `const CharT*`, `char_type_t` is `CharT`.

See [unit test](../../tests/type_traits/test_string_like_types.cpp) for examples and details.
