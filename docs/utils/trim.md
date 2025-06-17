## ASCII Whitespace Trimming

Defined in header `<rbox/utils/trim.hpp>`.

```cpp
namespace rbox {

struct ascii_trim_t {
    template <string_like T>
    static constexpr auto operator()(const T& str) -> std::basic_string_view</* CharT */>;
};
constexpr auto ascii_trim = ascii_trim_t{};

}  // namespace rbox
```

`ascii_trim(str)` removes leading and trailing ASCII whitespace characters (`' '`, `'\f'`, `'\n'`, `'\r'`, `'\t'`, `'\v'`). Returns a `std::basic_string_view`.
