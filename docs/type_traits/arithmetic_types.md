## Arithmetic Types

Defined in header `<rbox/type_traits/arithmetic_types.hpp>`.

```cpp
namespace rbox {

template <class T>
concept char_type = /* ... */;

template <class T>
concept non_bool_integral = /* ... */;

template <class T>
concept integer_type = /* ... */;

template <std::integral T>
using integral_to_integer_t = /* ... */;

}  // namespace rbox
```

The concept `char_type` checks whether `T` is a (maybe cv-qualified) character type, i.e. one of `char`, `wchar_t`, `char8_t`, `char16_t` or `char32_t`.

The concept `non_bool_integral` checks whether `T` is a (maybe cv-qualified) _integral_ type which is not `bool`, i.e. `std::is_integral_v<T>` is true and `std::is_same_v<std::remove_cv_t<T>, bool>` is false.

The concept `integer_type` checks whether `T` is a (maybe cv-qualified) _integer_ type, i.e. `std::integral - char_type - bool`.

The type alias `integral_to_integer_t` converts _integral_ type `T` to an _integer_ type with the same size and signedness. The major usage is to convert `bool` and character types to the corresponding `intN_t` or `uintN_t`.

Note that `signed char` and `unsigned char` are not `char_type`s as they are defined as character's _integer representation_ rather than the character itself in C++ standard.

Example:

```cpp
static_assert(rbox::char_type<char>);
static_assert(rbox::char_type<wchar_t>);
static_assert(!rbox::char_type<signed char>);

static_assert(rbox::integer_type<int>);
static_assert(!rbox::integer_type<bool>);
static_assert(!rbox::integer_type<char>);

static_assert(rbox::non_bool_integral<int>);
static_assert(rbox::non_bool_integral<char>);
static_assert(!rbox::non_bool_integral<bool>);

static_assert(std::is_same_v<rbox::integral_to_integer_t<bool>, uint8_t>);
static_assert(std::is_same_v<rbox::integral_to_integer_t<int>, int32_t>);
```
