## Introduction

rbox contains a series of utility components for various usages. They were initially designed for internal use within the rbox library, yet they are now ready to be exposed as a public API.

> ⚠️ **NOT RECOMMENDED to use components outside this document**. rbox provides no guarantee of API stability of those components if they are not listed in this document.

## Components

### Structural Alternative Types

* [meta_span](utils/meta_span.md): Structural alternative to `std::span`
* [meta_string_view](utils/meta_string_view.md): Structural alternative to `std::basic_string_view`
* [meta_pair](utils/meta_pair.md): Structural alternative to `std::pair<T, U>`
* [meta_triple](utils/meta_triple.md): Structural alternative to `std::tuple<T, U, V>`
* [meta_tuple](utils/meta_tuple.md): Structural alternative to `std::tuple<Args...>`
* [meta_variant](utils/meta_variant.md): Multi-type container variant

Along with:

* [format](utils/format.md): `std::format` and `fmt::format` support for `meta_basic_string_view`

### Helper Utility Types
* [ref_variant](utils/ref_variant.md): Non-owning variant of references

### String & Encoding

* [bkdr_hash](utils/string_hash/bkdr_hash.md): String hash with BKDR algorithm
* [make_string_view](utils/make_string_view.md): Generic, `nullptr`-safe construction to `std::basic_string_view` from ranges or C-style string
* [ctype](utils/ctype.md): ASCII character predicates & case conversion helpers
* [trim](utils/trim.md): ASCII whitespace trimming
* [identifier_naming](utils/identifier_naming.md): Identifier name conversion
* [string_builder](utils/string_builder.md): String builder with `constexpr` support
* [string_encoding](utils/string_encoding.md): UTF conversion with `constexpr` support

### Functional

* [generic_comparison](utils/functional/generic_comparison.md): Extension to `std::less` etc.
* [integral_comparison](utils/functional/integral_comparison.md): Extension to `std::cmp_less` etc.
* [tuple_comparison](utils/functional/tuple_comparison.md): Tuple-like elementwise comparison
* [get_ith_alternative](utils/functional/get_ith_alternative.md): Variant-like access
* [get_ith_element](utils/functional/get_ith_element.md): Tuple-like access

### Type Traits & Concepts

* [concepts](utils/concepts.md): Type category concepts
* [compare](utils/compare.md): Comparison operator detection
* [ranges](utils/ranges.md): Ranges concepts extension

### Metaprogramming Helpers

* [addressable_member](utils/addressable_member.md): Testing addressable members
* [define_static_array](utils/define_static_array.md): Wrapper of `std::define_static_array` returning `meta_span`
* [define_static_string](utils/define_static_string.md): Wrapper of `std::define_static_string` returning `meta_basic_string_view`
* [meta_utility](utils/meta_utility.md): Wrappers of `<meta>` components

## Common Constants

### `rbox::npos`

Defined in `<rbox/utils/constant.hpp>`.

```cpp
namespace rbox {
constexpr auto npos = static_cast<size_t>(-1);
}  // namespace rbox
```

A constant representing an invalid/unused position, analogous to `std::string::npos`.

### `max_decimal_digits`

Defined in `<rbox/utils/max_decimal_digits.hpp>`.

```cpp
namespace rbox {
constexpr size_t max_decimal_digits_int8 = 4;    // -128
constexpr size_t max_decimal_digits_int16 = 6;   // -32768
constexpr size_t max_decimal_digits_int32 = 11;  // -2147483648
constexpr size_t max_decimal_digits_int64 = 20;  // -9223372036854775808

consteval size_t max_decimal_digits(size_t bytes);
}  // namespace rbox
```

Pre-computed constants and a `consteval` function returning the maximum number of characters (including the minus sign) needed to represent an integer of a given size in bytes.

## Common Helper Types

### `indices_view`

Defined in `<rbox/utils/indices_view.hpp>`.

A simple range-like type that iterates over `size_t` indices from `0` to `n-1`, providing a lightweight alternative to `std::views::iota(size_t(0), n)`.

Example:

```cpp
template <class Tuple, class Visitor>
void for_each_in_tuple(Tuple&& tuple, Visitor&& visitor)
{
    constexpr size_t N = std::tuple_size_v<std::remove_cvref_t<Tuple>>;
    // Equivalent to std::views::iota(size_t(0), N)
    template for (constexpr size_t I : rbox::indices_view(N)) {
        // rbox::get_ith_element<I> defined in <rbox/utils/functional/get_ith_element.hpp>
        std::invoke(visitor, rbox::get_ith_element<I>(std::forward<Tuple>(tuple)));
    }
}
```
