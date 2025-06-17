## `define_static_string`

Defined in header `<rbox/utils/define_static_string.hpp>`.

```cpp
namespace rbox {

template <class Range>
consteval auto define_static_string(Range&& range) /* -> meta_basic_string_view<CharT> */;

}  // namespace rbox
```

A thin wrapper around `std::define_static_string` that returns a `meta_basic_string_view<CharT>` instead of a `const CharT*`. This allows the result to be used as a non-type template parameter (since `meta_basic_string_view` is a structural type). The input range must consist of character-type elements.
