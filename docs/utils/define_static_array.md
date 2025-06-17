## `define_static_array`

Defined in header `<rbox/utils/define_static_array.hpp>`.

```cpp
namespace rbox {

template <class Range>
consteval auto define_static_array(Range&& range) /* -> meta_span<T> */;

}  // namespace rbox
```

A thin wrapper around `std::define_static_array` that returns a `meta_span<T>` instead of a `std::span<const T>`. This allows the result to be used as a non-type template parameter (since `meta_span` is a structural type).
