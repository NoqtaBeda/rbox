## Conversion from Enum Value to Name

Defined in header `<rbox/enum/enum_name.hpp>`.

```cpp
namespace rbox {

struct enum_name_t {
    // (1)
    template <enum_type E>
    static constexpr auto operator()(E e) -> std::string_view;
};

struct enum_name_opt_t {
    // (2)
    template <enum_type E>
    static constexpr auto operator()(E e) -> std::optional<std::string_view>;
};

constexpr auto enum_name = enum_name_t{};
constexpr auto enum_name_opt = enum_name_opt_t{};

}  // namespace rbox
```

- (1) `enum_name(e)` gets the name of given enum value `e`, or `std::string_view{}` if no such value is defined in the enum type.
- (2) `enum_name_opt(e)` gets the name of given enum value, or `std::nullopt` if no such value is defined in the enum type. This version is helpful with monadic operations of `std::optional` since C++23.

If multiple enum entries have the same value as `e`, the name of the first one by definition order is returned.

Example:

```cpp
enum class foo {
    one = 1,
    two = 2,
    three = 3,
    invalid = -1,
    first = 1,
    last = 3,
};

enum bar {
    bar_one = 1,
    bar_two = 2,
    bar_three = 3,
};

static_assert(rbox::enum_name(foo::two) == "two");
// 'three' is defined before 'last' in enum class foo
static_assert(*rbox::enum_name_opt(foo::last) == "three");
// No entry defined in enum class foo
static_assert(rbox::enum_name(static_cast<foo>(-2)) == "");
static_assert(rbox::enum_name_opt(static_cast<foo>(-3)) == std::nullopt);

// Example of monadic operation with std::optional (introduced in C++23)
auto convert_to_bar(foo value) -> std::optional<bar>
{
    return rbox::enum_name_opt(value).and_then([](std::string_view foo_name) {
        auto bar_name = std::format("bar_{}", foo_name);
        return rbox::enum_cast<bar>(bar_name);  // Example of enum_cast (see below)
    });
}
```
