## Conversion from String or Integer to Enum Value

Defined in header `<rbox/enum/enum_cast.hpp>`.

```cpp
namespace rbox {

template <enum_type E>
struct enum_cast_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    // (1)
    static constexpr auto operator()(std::string_view str) -> std::optional<ENoCV>;
    // (2)
    static constexpr auto operator()(std::integral auto value) -> std::optional<ENoCV>;
};

template <enum_type E>
constexpr auto enum_cast = enum_cast_t<std::remove_cv_t<E>>{};

template <enum_type E>
struct ascii_ci_enum_cast_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    static constexpr auto operator()(std::string_view str) -> std::optional<ENoCV>;
};

template <enum_type E>
constexpr auto ascii_ci_enum_cast = ascii_ci_enum_cast_t<std::remove_cv_t<E>>{};

}  // namespace rbox
```

- (1) `enum_cast<E>(str)` converts given string to the enum value whose name matches, or `std::nullopt` if such enum entry does not exist;
- (2) `enum_cast<E>(value)` converts given integer to the enum value whose underlying value matches, or `std::nullopt` if such enum entry does not exist. Signedness-safe and narrowing-safe comparison is performed.

Additionally, `ascii_ci_enum_cast<E>(str)` provides a case-insensitive variant that finds enum entries in an ASCII case-insensitive manner. A compilation error will be raised in case of entry name duplication or non-ASCII characters in enum entry definition.

Example:

```cpp
enum class foo : int {
    one = 1,
    two = 2,
    three = 3,
    invalid = -1,
    first = 1,
    last = 3,
};

static_assert(rbox::enum_cast<foo>("first") == foo::one);
static_assert(rbox::ascii_ci_enum_cast<foo>("LAST") == foo::three);
static_assert(rbox::enum_cast<foo>(2) == foo::two);
// Signedness-safe comparison: -1u does not match -1 despite identical bit representation.
static_assert(rbox::enum_cast<foo>(-1u) == std::nullopt);
// Narrowing-safe comparison.
static_assert(rbox::enum_cast<foo>(0x1'0000'0001) == std::nullopt);
```
