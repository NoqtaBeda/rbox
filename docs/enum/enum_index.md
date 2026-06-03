## Index of Enum Value

Defined in header `<rbox/enum/enum_index.hpp>`.

```cpp
namespace rbox {

template <enum_entry_order Order>
struct enum_index_by_t {
    // (1)
    template <enum_type E>
    static constexpr size_t operator()(E e);
};

template <enum_entry_order Order>
struct enum_index_opt_by_t {
    // (2)
    template <enum_type E>
    static constexpr auto operator()(E e) -> std::optional<size_t>;
};

struct enum_unique_index_t {
    // (3)
    template <enum_type E>
    static constexpr size_t operator()(E e);
};

struct enum_unique_index_opt_t {
    // (4)
    template <enum_type E>
    static constexpr auto operator()(E e) -> std::optional<size_t>;
};

template <enum_entry_order Order>
constexpr auto enum_index_by = enum_index_by_t<Order>{};

template <enum_entry_order Order>
constexpr auto enum_index_opt_by = enum_index_opt_by_t<Order>{};

constexpr auto enum_index = enum_index_by<enum_entry_order::original>;
constexpr auto enum_index_opt = enum_index_opt_by<enum_entry_order::original>;

constexpr auto enum_unique_index = enum_unique_index_t{};
constexpr auto enum_unique_index_opt = enum_unique_index_opt_t{};

}  // namespace rbox
```

- (1) `enum_index_by<Order>(e)` or `enum_index(e)` (taking the original definition order) gets the index (0-based) of given enum value by specific order, or returns `npos` (which is -1 of type `size_t`) if such enum value is not defined. For multiple enum entries with duplicated underlying value, the first one is taken.
- (2) `enum_index_opt_by<Order>(e)` or `enum_index_opt(e)` (taking the original definition order) is similar to (1) yet returns `std::nullopt` on undefined enum values, which is useful for monadic operations;
- (3) `enum_unique_index(e)` gets the index (0-based) of given value after sorting all the enum entries by underlying value and removing all duplicated entries, or `npos` if such enum value is not defined;
- (4) `enum_unique_index_opt(e)` is similar to (3) yet returns `std::nullopt` on undefined enum values.

Example:

```cpp
enum class foo : int {  // | original | by_value | by_name | enum_unique_index |
    one = 1,            // |     0    |     1    |    4    |         1         |
    two = 2,            // |     1    |     3    |    6    |         2         |
    three = 3,          // |     2    |     4    |    5    |         3         |
    invalid = -1,       // |     3    |     0    |    2    |         0         |
    first = 1,          // |               (same as foo::one)                  |
    last = 3,           // |              (same as foo::three)                 |
    four = 4,           // |     6    |     6    |    1    |         4         |
};

using enum rbox::enum_entry_order;

static_assert(rbox::enum_index_by<by_value>(foo::three) == 4);
static_assert(rbox::enum_index_opt_by<by_name>(foo::four) == 1);

constexpr auto foo_null = static_cast<foo>(0);
static_assert(rbox::enum_index(foo_null) == rbox::npos);  // which is static_cast<size_t>(-1)
static_assert(rbox::enum_index_opt(foo_null) == std::nullopt);

using handler_t = bool (*)(const char*);
extern handler_t handlers[5];

// Example on monadic operations
bool call_handler(foo key, const char* message)
{
    return rbox::enum_unique_index_opt(key)
        .transform([](size_t index) {
            return (*handlers[index])(message);
        })
        .value_or(false);
}
```
