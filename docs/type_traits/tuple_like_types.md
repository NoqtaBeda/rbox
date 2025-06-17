## Tuple-like Types

Defined in header `<rbox/type_traits/tuple_like_types.hpp>`.

```cpp
namespace rbox {

template <class T>
concept tuple_like = /* ... */;

template <class T>
concept pair_like = /* ... */;

template <class T, class... Args>
concept tuple_like_of = /* ... */;

template <class T, class... Args>
concept tuple_like_of_exactly = /* ... */;

}  // namespace rbox
```

The concept `tuple_like` tests whether `T` is a (maybe cv-qualified) [tuple-like](https://en.cppreference.com/w/cpp/utility/tuple/tuple-like) type. Tuple-like types are those whose `std::tuple_size` and `std::tuple_element` specializations are provided, and members can be accessed via either `std::get<I>(t)` or `t.get<I>()` for all valid indices. This includes but is not limited to:

- `std::tuple`, `std::pair`, `std::array`;
- Custom tuple-like types that satisfy the tuple interface.

The concept `pair_like` tests whether `T` is a (maybe cv-qualified) pair-like type, i.e. a tuple-like type with size exactly 2.

The concept `tuple_like_of` tests whether `T` is a (maybe cv-qualified) tuple-like type whose element types can be converted to `Args...` respectively. The element types are compared using `std::is_convertible_v`.

The concept `tuple_like_of_exactly` tests whether `T` is a (maybe cv-qualified) tuple-like type whose element types are exactly `Args...` respectively. The element types are compared using `std::is_same_v`.
