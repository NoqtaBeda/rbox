## `meta_pair`

Defined in header `<rbox/utils/meta_pair.hpp>`.

`meta_pair<T, U>` is a structural alternative to `std::pair<T, U>`. A structural type is a type whose value can be used as a non-type template parameter (see [cppreference](https://en.cppreference.com/w/cpp/language/template_parameters) for details).

> ⚠️ `meta_pair` is _intentionally trivial_. **BE CAREFUL with initialization**.

### Synopsis

```cpp
namespace rbox {

template <class T, class U>
struct meta_pair {
    T first;
    U second;

    template <size_t I>
    friend constexpr auto& get(meta_pair& self);

    template <size_t I>
    friend constexpr const auto& get(const meta_pair& self);

    template <size_t I>
    friend constexpr auto&& get(meta_pair&& self);

    template <size_t I>
    friend constexpr const auto&& get(meta_pair&& self);

    // Comparison
    template <class TupleLike>
    constexpr auto operator==(const TupleLike& rhs) const -> bool;
    template <class TupleLike>
    constexpr auto operator<=>(const TupleLike& rhs) const /* -> common category */;
};

// Standard library integration
template <class T, class U>
struct std::tuple_size<rbox::meta_pair<T, U>> : std::integral_constant<size_t, 2> {};

template <size_t I, class T, class U>
struct std::tuple_element<I, rbox::meta_pair<T, U>> { using type = /* T (I==0) or U (I==1) */; };

}  // namespace rbox
```
