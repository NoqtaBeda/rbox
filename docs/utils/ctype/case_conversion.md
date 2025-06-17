## ASCII Case Conversion

Defined in header `<rbox/utils/ctype/case_conversion.hpp>`.

```cpp
namespace rbox {

struct ascii_tolower_t {
    template <non_bool_integral T>
    static constexpr auto operator()(T c);

    template <string_like T>
    static constexpr auto operator()(const T& str) -> std::basic_string</* CharT */>;
};

struct ascii_toupper_t {
    template <non_bool_integral T>
    static constexpr auto operator()(T c);

    template <string_like T>
    static constexpr auto operator()(const T& str) -> std::basic_string</* CharT */>;
};

constexpr auto ascii_tolower = ascii_tolower_t{};
constexpr auto ascii_toupper = ascii_toupper_t{};

}  // namespace rbox
```

- `ascii_tolower(c)`: Converts uppercase ASCII letter to lowercase, otherwise returns `c` unchanged.
- `ascii_toupper(c)`: Converts lowercase ASCII letter to uppercase, otherwise returns `c` unchanged.
- `ascii_tolower(str)`: Returns a new string with all ASCII letters converted to lowercase.
- `ascii_toupper(str)`: Returns a new string with all ASCII letters converted to uppercase.

For non-ASCII characters, the functions above always return the character unchanged.
