## `meta_triple`

Defined in header `<rbox/utils/meta_pair.hpp>`.

`meta_triple<T, U, V>` is a structural alternative to `std::tuple<T, U, V>` for exactly three elements. A structural type is a type whose value can be used as a non-type template parameter (see [cppreference](https://en.cppreference.com/w/cpp/language/template_parameters) for details).

> ⚠️ `meta_triple` is _intentionally trivial_. **BE CAREFUL with initialization**.

### Synopsis

```cpp
namespace rbox {

template <class T, class U, class V>
struct meta_triple {
    T first;
    U second;
    V third;

    template <size_t I>
    friend constexpr auto& get(meta_triple& self);

    template <size_t I>
    friend constexpr const auto& get(const meta_triple& self);

    template <size_t I>
    friend constexpr auto&& get(meta_triple&& self);

    template <size_t I>
    friend constexpr const auto&& get(const meta_triple&& self);

    // Comparison
    template <class TupleLike>
    constexpr auto operator==(const TupleLike& rhs) const -> bool;
    template <class TupleLike>
    constexpr auto operator<=>(const TupleLike& rhs) const /* -> common category */;
};

// Standard library integration
template <class T, class U, class V>
struct std::tuple_size<rbox::meta_triple<T, U, V>> : std::integral_constant<size_t, 3> {};

template <size_t I, class T, class U, class V>
struct std::tuple_element<I, rbox::meta_triple<T, U, V>> { using type = /* ... */; };

}  // namespace rbox
```
