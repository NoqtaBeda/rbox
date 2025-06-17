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

The concept `trivial_type` tests whether `T` is a fully trivial type — all six special member functions (default constructor, destructor, copy/move constructors, copy/move assignment operators) are trivial. This is a stricter constraint than `std::is_trivial_v<T>`, which also requires the type to have a trivial default constructor and be trivially copyable.
