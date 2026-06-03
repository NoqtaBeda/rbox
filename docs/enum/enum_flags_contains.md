## Checking Existence of Enum Flag Value

Defined in header `<rbox/enum/enum_flags_contains.hpp>`.

```cpp
namespace rbox {

template <enum_type E>
struct enum_flags_contains_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    // (1.1)
    static constexpr bool operator()(std::string_view str, char delim = '|');
    // (1.2)
    static constexpr bool operator()(std::string_view str, std::string_view delim);
    // (2)
    static constexpr bool operator()(E flags);
    // (3)
    static constexpr bool operator()(std::integral auto flags);
};

template <enum_type E>
constexpr auto enum_flags_contains = enum_flags_contains_t<std::remove_cv_t<E>>{};

template <enum_type E>
struct ascii_ci_enum_flags_contains_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    static constexpr bool operator()(std::string_view str, char delim = '|');
    static constexpr bool operator()(std::string_view str, std::string_view delim);
};

template <enum_type E>
constexpr auto ascii_ci_enum_flags_contains =
    ascii_ci_enum_flags_contains_t<std::remove_cv_t<E>>{};

}  // namespace rbox
```

- (1.1, 1.2) `enum_flags_contains<E>(str, delim)` checks whether the input string can be split by the given delimiter into a valid disjunction of defined enum entry names. Each segment in input `str` is trimmed before matching;
- (2) `enum_flags_contains<E>(flags)` checks whether the given enum flag value is a valid disjunction of defined entries;
- (3) `enum_flags_contains<E>(flags)` checks whether the given integer, interpreted as a flag value, is a valid disjunction of defined entries. Signedness-safe and narrowing-safe comparison is performed.

Additionally, `ascii_ci_enum_flags_contains<E>(str, delim)` provides a case-insensitive variant. Compilation error will be raised if non-ASCII characters exist in enum entry definition.

If you do not need the casted enum value, `enum_flags_contains<E>` has better performance than `enum_flags_cast<E>`.

Example:

```cpp
enum class permissions {
    read = 1,
    write = 2,
    execute = 4,
};

static_assert(rbox::enum_flags_contains<permissions>("read|write"));
static_assert(rbox::enum_flags_contains<permissions>(3));  // read | write
static_assert(!rbox::enum_flags_contains<permissions>(8));
```
