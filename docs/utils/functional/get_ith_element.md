## Tuple-Like Access

Defined in header `<rbox/utils/functional/get_ith_element.hpp>`, also included in the umbrella header `<rbox/utils/functional.hpp>`.

```cpp
namespace rbox {

template <size_t I>
struct get_ith_element_t {
    template <tuple_like T>
    static constexpr decltype(auto) operator()(T&& tuple);
};

using get_first_t = get_ith_element_t<0>;
using get_second_t = get_ith_element_t<1>;
using get_third_t = get_ith_element_t<2>;

constexpr auto get_first = get_ith_element_t<0>{};
constexpr auto get_second = get_ith_element_t<1>{};
constexpr auto get_third = get_ith_element_t<2>{};

template <size_t I>
constexpr auto get_ith_element = get_ith_element_t<I>{};

}  // namespace rbox
```

Provides a unified access interface for tuple-like types (including `std::tuple`, `std::pair`, `std::array`, `meta_tuple`, `meta_pair`, `meta_triple`, and user-defined tuple-like types). Calls either `get<I>(tuple)` via ADL or the member function `.template get<I>()`.

Example:

```cpp
auto t = std::tuple{1, 2.0, "hello"};
rbox::get_first(t);         // returns 1
rbox::get_ith_element<2>(t); // returns "hello"
rbox::get_second(t);        // returns 2.0

auto p = std::pair{1, 2};
rbox::get_first(p);   // returns 1
rbox::get_second(p);  // returns 2
```
