## Ranges Concepts Extension

Defined in header `<rbox/utils/ranges.hpp>`.

Provides extended range concepts and convenience aliases.

```cpp
namespace rbox {

template <class T>
concept non_range = !std::ranges::range<T>;

template <class Range, class T>
concept range_of =
    std::ranges::range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept range_of_exactly =
    std::ranges::range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept borrowed_range_of =
    std::ranges::borrowed_range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept borrowed_range_of_exactly =
    std::ranges::borrowed_range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept sized_range_of =
    std::ranges::sized_range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept sized_range_of_exactly =
    std::ranges::sized_range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept view_of =
    std::ranges::view<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept view_of_exactly =
    std::ranges::view<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept input_range_of =
    std::ranges::input_range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept input_range_of_exactly =
    std::ranges::input_range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept forward_range_of =
    std::ranges::forward_range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept forward_range_of_exactly =
    std::ranges::forward_range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept bidirectional_range_of =
    std::ranges::bidirectional_range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept bidirectional_range_of_exactly =
    std::ranges::bidirectional_range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept random_access_range_of =
    std::ranges::random_access_range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept random_access_range_of_exactly =
    std::ranges::random_access_range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept contiguous_range_of =
    std::ranges::contiguous_range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept contiguous_range_of_exactly =
    std::ranges::contiguous_range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept common_range_of =
    std::ranges::common_range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept common_range_of_exactly =
    std::ranges::common_range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept viewable_range_of =
    std::ranges::viewable_range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept viewable_range_of_exactly =
    std::ranges::viewable_range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

#if __cpp_lib_ranges_as_const >= 202207L
template <class Range, class T>
concept constant_range_of =
    std::ranges::constant_range<Range> &&
    std::is_convertible_v<std::ranges::range_value_t<Range>, T>;

template <class Range, class T>
concept constant_range_of_exactly =
    std::ranges::constant_range<Range> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;
#endif

template <class Range, class T>
concept output_range_of =
    std::ranges::output_range<Range, T>;

template <class Range, class T>
concept output_range_of_exactly =
    std::ranges::output_range<Range, T> &&
    std::is_same_v<std::ranges::range_value_t<Range>, T>;

}  // namespace rbox
```

The `*_of` and `*_of_exactly` concepts are convenient shortcuts for constraining both the range category and its value type in a single concept.
