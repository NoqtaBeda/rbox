## `make_string_view`

Defined in header `<rbox/utils/make_string_view.hpp>`.

```cpp
namespace rbox {

template <string_like StringT>
constexpr auto make_string_view(const StringT& str) -> std::basic_string_view</* CharT */>;

}  // namespace rbox
```

A `nullptr`-safe helper to convert a string-like type to `std::basic_string_view`. If `str` is a null pointer, returns an empty `basic_string_view` instead of invoking undefined behavior.

Example:

```cpp
std::string s = "hello";
auto sv = rbox::make_string_view(s);  // std::string_view{"hello"}
const char* p = nullptr;
auto empty = rbox::make_string_view(p);  // std::string_view{} (safe)
```
