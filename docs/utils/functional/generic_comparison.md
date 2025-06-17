## Generic Comparison

Defined in header `<rbox/utils/functional/generic_comparison.hpp>`, also included in the umbrella header `<rbox/utils/functional.hpp>`.

```cpp
namespace rbox {

// Comparator concepts
template <class T, class U>
constexpr auto is_less_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_greater_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_less_equal_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_greater_equal_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_equal_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_not_equal_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_compare_three_way_comparable_v = /* ... */;

// Generic comparison functors
struct less_t {
    template <class T, class U>
    static constexpr bool operator()(const T& t, const U& u);
};

struct greater_t { /* same as above */ };
struct less_equal_t { /* same as above */ };
struct greater_equal_t { /* same as above */ };
struct equal_t { /* same as above */ };
struct not_equal_t { /* same as above */ };

struct compare_three_way_t {
    template <class T, class U>
    static constexpr auto operator()(const T& t, const U& u) /* -> three-way comparison result */;
};

constexpr auto less = less_t{};
constexpr auto greater = greater_t{};
constexpr auto less_equal = less_equal_t{};
constexpr auto greater_equal = greater_equal_t{};
constexpr auto equal = equal_t{};
constexpr auto not_equal = not_equal_t{};
constexpr auto compare_three_way = compare_three_way_t{};

template <class T, class U = T>
    requires (is_compare_three_way_comparable_v<T, U>)
using compare_three_way_result = std::invoke_result_t<compare_three_way_t, const T&, const U&>;

}  // namespace rbox
```

These are generic comparison functors that extend `std::ranges::less` and `std::compare_three_way` with less constraints:

1. **Integer types** are compared in a signedness-safe manner (delegates to `cmp_*` from [Integral Comparison](integral_comparison.md));
2. **Other types** use the corresponding operators directly (e.g., `operator <`).

The fallback policies provide maximum compatibility:
- `greater_t` falls back to inverted `operator <` if `operator >` is not defined.
- `less_equal_t` falls back to `operator < || operator ==` if `operator <=` is not defined.
- `greater_equal_t` has multi-level fallbacks for legacy types with incomplete operator sets.
- `not_equal_t` falls back to `!operator ==` if `operator !=` is not defined.
- `compare_three_way_t` synthesizes three-way comparison from `less`, `greater` and `equal` when `operator <=>` is not available, falling back to `std::partial_ordering` as the most conservative policy.

Example:

```cpp
// Integers: signedness-safe
rbox::less(-1, 1u);   // true

// Custom type with only operator < defined
struct Legacy { int v; };
bool operator<(Legacy a, Legacy b) { return a.v < b.v; }
rbox::greater(Legacy{2}, Legacy{1});      // OK: falls back to inverted operator <
rbox::compare_three_way(Legacy{1}, Legacy{2});  // OK: synthesizes from less/greater/equal
```
