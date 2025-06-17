## Integral Comparison

Defined in header `<rbox/utils/functional/integral_comparison.hpp>`, also included in the umbrella header `<rbox/utils/functional.hpp>`.

```cpp
namespace rbox {

// Integer comparison functors
struct cmp_equal_t {
    template <std::integral T, std::integral U>
    static constexpr bool operator()(T t, U u) noexcept;
};
struct cmp_not_equal_t { /* same as above */ };
struct cmp_less_t { /* same as above */ };
struct cmp_greater_t { /* same as above */ };
struct cmp_less_equal_t { /* same as above */ };
struct cmp_greater_equal_t { /* same as above */ };

struct cmp_three_way_t {
    template <std::integral T, std::integral U>
    static constexpr auto operator()(T t, U u) noexcept -> std::strong_ordering;
};

constexpr auto cmp_equal = cmp_equal_t{};
constexpr auto cmp_not_equal = cmp_not_equal_t{};
constexpr auto cmp_less = cmp_less_t{};
constexpr auto cmp_greater = cmp_greater_t{};
constexpr auto cmp_less_equal = cmp_less_equal_t{};
constexpr auto cmp_greater_equal = cmp_greater_equal_t{};
constexpr auto cmp_three_way = cmp_three_way_t{};

// In-range check functor
template <std::integral R>
struct in_range_t {
    template <std::integral T>
    static constexpr bool operator()(T t);
};
template <class R>
constexpr auto in_range = in_range_t<R>{};

}  // namespace rbox
```

The integer comparison functors (`cmp_equal`, `cmp_not_equal`, `cmp_less`, `cmp_greater`, `cmp_less_equal`, `cmp_greater_equal`, `cmp_three_way`) are relaxed alternatives to `std::cmp_*` functions. Unlike the standard library versions which only accept integer types (excluding `bool` and character types), these functors accept all integral types including `bool`, `char`, etc.

Example:

```cpp
// Standard library rejects these:
// std::cmp_less('a', 100);  // Error: char not allowed
// std::cmp_less(true, 1);   // Error: bool not allowed

// rbox accepts them:
rbox::cmp_less('a', 100);  // OK: compares as int
rbox::cmp_less(true, 1);   // OK: compares as int
```

`in_range<R>(t)` checks whether the integer value `t` can be represented by type `R`. It is equivalent to `std::in_range<R>(t)` with additional support to character types and `bool`.

The `in_range_t<bool>` specialization checks whether the value is exactly `0` or `1`.

Example:

```cpp
rbox::in_range<uint8_t>(255);  // true
rbox::in_range<uint8_t>(256);  // false
rbox::in_range<uint8_t>(-1);   // false
rbox::in_range<int8_t>(127);   // true
rbox::in_range<int8_t>(128);   // false
rbox::in_range<bool>(0);       // true
rbox::in_range<bool>(1);       // true
rbox::in_range<bool>(2);       // false
rbox::in_range<bool>(-1);      // false
```
