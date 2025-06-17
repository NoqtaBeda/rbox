## Variant-Like Access

Defined in header `<rbox/utils/functional/get_ith_alternative.hpp>`, also included in the umbrella header `<rbox/utils/functional.hpp>`.

```cpp
namespace rbox {

template <size_t I>
struct holds_ith_alternative_t {
    template <variant_like T>
    static constexpr auto operator()(const T& variant) -> bool;
};

template <size_t I>
constexpr auto holds_ith_alternative = holds_ith_alternative_t<I>{};

template <size_t I>
struct get_ith_alternative_t {
    template <variant_like T>
    static constexpr decltype(auto) operator()(T&& variant);
};

template <size_t I>
constexpr auto get_ith_alternative = get_ith_alternative_t<I>{};

}  // namespace rbox
```

Provides a unified access interface for variant-like types (including `std::variant`, `meta_variant`, `ref_variant`, and user-defined variant-like types).

- `holds_ith_alternative<I>(variant)`: Checks whether the variant currently holds the `I`-th alternative. Either calls `holds_alternative<A>(variant)` via ADL or calls the member function `.template holds_alternative<A>()`.
- `get_ith_alternative<I>(variant)`: Retrieves the value of the `I`-th alternative from the variant. Either calls `get<A>(variant)` via ADL or calls the member function `.template get<A>()`.

Example:

```cpp
auto v = std::variant<int, double, std::string>{42};
rbox::holds_ith_alternative<0>(v);  // true (holds int)
rbox::holds_ith_alternative<1>(v);  // false
rbox::get_ith_alternative<0>(v);    // returns 42
```
