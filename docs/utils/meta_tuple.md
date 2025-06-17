## `meta_tuple`

Defined in header `<rbox/utils/meta_tuple.hpp>`.

`meta_tuple<Args...>` is a structural alternative to `std::tuple<Args...>`. It uses `std::meta::define_aggregate()` to generate an underlying aggregate type `underlying_type` at compile time, whose data members `v1`, `v2`, ..., `vN` correspond to the tuple elements. This indirection via `define_aggregate` makes `meta_tuple` itself a structural type (as long as all element types are structural), since the generated aggregate is always structural.

A structural type is a type whose value can be used as a non-type template parameter (see [cppreference](https://en.cppreference.com/w/cpp/language/template_parameters) for details).

> ⚠️ `meta_tuple` is _intentionally trivial_. **BE CAREFUL with initialization**.

### Synopsis

```cpp
namespace rbox {

template <class... Args>
struct meta_tuple {
    union underlying_type;  // Aggregate generated via define_aggregate

    // data members are v1, v2, ..., vN inside underlying_type
    underlying_type elements;

    // Default construction
    constexpr meta_tuple() = default;

    // Forwarding construction
    template <class... CtorArgs>
    constexpr meta_tuple(CtorArgs&&... args);

    // Copy / move (defaulted)
    constexpr meta_tuple(const meta_tuple&) = default;
    constexpr meta_tuple(meta_tuple&&) = default;
    constexpr auto operator=(const meta_tuple&) -> meta_tuple& = default;
    constexpr auto operator=(meta_tuple&&) -> meta_tuple& = default;

    template <size_t I>
    friend constexpr auto& get(meta_tuple& tuple);

    template <size_t I>
    friend constexpr const auto& get(const meta_tuple& tuple);

    template <size_t I>
    friend constexpr auto&& get(meta_tuple&& tuple);

    template <size_t I>
    friend constexpr const auto&& get(const meta_tuple&& tuple);

    // Comparison
    template <class TupleLike>
    constexpr auto operator==(const TupleLike& rhs) const -> bool;
    template <class TupleLike>
    constexpr auto operator<=>(const TupleLike& rhs) const /* -> common category */;
};

// Deduction guide
template <class... Args>
meta_tuple(Args...) -> meta_tuple<Args...>;

// Standard library integration
template <class... Args>
struct std::tuple_size<rbox::meta_tuple<Args...>>
    : std::integral_constant<size_t, sizeof...(Args)> {};

template <size_t I, class... Args>
struct std::tuple_element<I, rbox::meta_tuple<Args...>> { using type = Args...[I]; };

}  // namespace rbox
```
