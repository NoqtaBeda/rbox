## Trivial Type

Defined in header `<rbox/type_traits/trivial_types.hpp>`.

```cpp
namespace rbox {

template <class T>
concept trivial_type =
       std::is_trivially_default_constructible_v<T>
    && std::is_trivially_destructible_v<T>
    && std::is_trivially_copy_constructible_v<T>
    && std::is_trivially_copy_assignable_v<T>
    && std::is_trivially_move_constructible_v<T>
    && std::is_trivially_move_assignable_v<T>;

}  // namespace rbox
```

The concept `trivial_type` is an alternative to the deprecated `std::is_trivial_v`.
