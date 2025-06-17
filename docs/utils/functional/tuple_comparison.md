## Tuple-Like Elementwise Comparison

Defined in header `<rbox/utils/functional/tuple_comparison.hpp>`, also included in the umbrella header `<rbox/utils/functional.hpp>`.

```cpp
namespace rbox {

// Comparator concepts
template <class T, class U>
constexpr auto is_elementwise_less_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_elementwise_greater_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_elementwise_less_equal_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_elementwise_greater_equal_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_elementwise_equal_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_elementwise_not_equal_comparable_v = /* ... */;
template <class T, class U>
constexpr auto is_elementwise_compare_three_way_comparable_v = /* ... */;

// Elementwise comparison functors
struct elementwise_less_t {
    template <class T, class U>
    static constexpr auto operator()(const T& t, const U& u) -> bool;
};
struct elementwise_greater_t { /* same as above */ };
struct elementwise_less_equal_t { /* same as above */ };
struct elementwise_greater_equal_t { /* same as above */ };
struct elementwise_equal_t { /* same as above */ };
struct elementwise_not_equal_t { /* same as above */ };

struct elementwise_compare_three_way_t {
    template <class T, class U>
    static constexpr auto operator()(const T& t, const U& u)
        /* -> common comparison category */;
};

constexpr auto elementwise_less = elementwise_less_t{};
constexpr auto elementwise_greater = elementwise_greater_t{};
constexpr auto elementwise_less_equal = elementwise_less_equal_t{};
constexpr auto elementwise_greater_equal = elementwise_greater_equal_t{};
constexpr auto elementwise_equal = elementwise_equal_t{};
constexpr auto elementwise_not_equal = elementwise_not_equal_t{};
constexpr auto elementwise_compare_three_way = elementwise_compare_three_way_t{};

}  // namespace rbox
```

These functors provide lexicographical (elementwise) comparison for two tuple-like objects of the same size. They use the rbox generic comparison functors (`less`, `greater`, `equal`, `not_equal`, `compare_three_way`) elementwise.

- **`elementwise_less`**: Returns `true` if the first differing element in `t` is less than the corresponding element in `u`.
- **`elementwise_equal`**: Returns `true` only if all elements are pairwise equal.
- **`elementwise_not_equal`**: Returns `true` if any element differs.
- **`elementwise_compare_three_way`**: Performs lexicographical three-way comparison. Common comparison category is taken as the result type.

Example:
```cpp
auto t1 = std::tuple{1, 2, 3};
auto t2 = std::tuple{1, 2, 4};
rbox::elementwise_less(t1, t2);               // true
rbox::elementwise_equal(t1, t2);              // false
rbox::elementwise_not_equal(t1, t2);          // true
rbox::elementwise_compare_three_way(t1, t2);  // std::strong_ordering::less
```
