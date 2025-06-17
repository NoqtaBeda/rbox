## Checking Existence of Enum Value

Defined in header `<rbox/enum/enum_contains.hpp>`.

```cpp
namespace rbox {

template <enum_type E>
struct enum_contains_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    // (1)
    static constexpr bool operator()(ENoCV e);
    // (2)
    static constexpr bool operator()(std::integral auto value);
    // (3)
    static constexpr bool operator()(std::string_view str);
};

template <enum_type E>
constexpr auto enum_contains = enum_contains_t<std::remove_cv_t<E>>{};

template <enum_type E>
struct ascii_ci_enum_contains_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    static constexpr bool operator()(std::string_view str);
};

template <enum_type E>
constexpr auto ascii_ci_enum_contains = ascii_ci_enum_contains_t<std::remove_cv_t<E>>{};

}  // namespace rbox
```

- (1) `enum_contains<E>(e)` checks whether given enum value is defined in the enum type;
- (2) `enum_contains<E>(value)` checks whether some enum entry with given underlying value exists in the enum type. Signedness-safe and narrowing-safe comparison is performed;
- (3) `enum_contains<E>(str)` checks whether some enum entry with given name exists in the enum type.

Additionally, `ascii_ci_enum_contains<E>(str)` provides a case-insensitive variant. A compilation error will be raised in case of non-ASCII characters in enum entry definition.

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

static_assert(rbox::enum_contains<foo>("first"));
static_assert(rbox::ascii_ci_enum_contains<foo>("LAST"));
static_assert(rbox::enum_contains<foo>(2));
static_assert(rbox::enum_contains<foo>(foo::three));
// Signedness-safe comparison: -1u does not match -1 despite identical bit representation.
static_assert(!rbox::enum_contains<foo>(-1u));
// Narrowing-safe comparison.
static_assert(!rbox::enum_contains<foo>(0x1'0000'0001));
```
