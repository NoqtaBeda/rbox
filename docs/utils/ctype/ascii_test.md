## ASCII Character/String Check

Defined in header `<rbox/utils/ctype/ascii_test.hpp>`.

```cpp
namespace rbox {

struct is_ascii_char_t {
    template <non_bool_integral T>
    static constexpr bool operator()(T c);
};
struct is_ascii_string_t {
    template <string_like T>
    static constexpr bool operator()(const T& str);
};
constexpr auto is_ascii_char = is_ascii_char_t{};
constexpr auto is_ascii_string = is_ascii_string_t{};

}  // namespace rbox
```

- `is_ascii_char(c)`: Returns `true` if `c` is in range `[0, 127]`.
- `is_ascii_string(str)`: Returns `true` if all characters in `str` are in range `[0, 127]`.
