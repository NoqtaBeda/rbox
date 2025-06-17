## Comparison Operator Detection

Defined in header `<rbox/utils/compare.hpp>`.

Provides concepts and variable templates for detecting whether two types support specific comparison operator.

```cpp
namespace rbox {

template <class T>
concept three_way_comparison_result =
       std::is_same_v<std::remove_cv_t<T>, std::strong_ordering>
    || std::is_same_v<std::remove_cv_t<T>, std::weak_ordering>
    || std::is_same_v<std::remove_cv_t<T>, std::partial_ordering>;

template <class T, class U>
constexpr bool is_operator_eq_comparable_v = /* ... */;
template <class T, class U>
constexpr bool is_operator_ne_comparable_v = /* ... */;
template <class T, class U>
constexpr bool is_operator_lt_comparable_v = /* ... */;
template <class T, class U>
constexpr bool is_operator_gt_comparable_v = /* ... */;
template <class T, class U>
constexpr bool is_operator_le_comparable_v = /* ... */;
template <class T, class U>
constexpr bool is_operator_ge_comparable_v = /* ... */;
template <class T, class U>
constexpr bool is_operator_3way_comparable_v = /* ... */;

}  // namespace rbox
```

These variable templates detect whether `const T&` and `const U&` support the corresponding operator (e.g., `==`, `!=`, `<`, `>`, `<=`, `>=`, `<=>`) and the result is convertible to `bool` (or a three-way comparison result type for `<=>`).
