## Conversion from Enum Flag String to Value

Defined in header `<rbox/enum/enum_flags_cast.hpp>`.

```cpp
namespace rbox {

template <enum_type E>
struct enum_flags_cast_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    // (1.1)
    static constexpr auto operator()(std::string_view str, char delim = '|')
        -> std::optional<ENoCV>;
    // (1.2)
    static constexpr auto operator()(std::string_view str, std::string_view delim)
        -> std::optional<ENoCV>;
    // (2)
    static constexpr auto operator()(std::integral auto value) -> std::optional<ENoCV>;
};

template <enum_type E>
constexpr auto enum_flags_cast = enum_flags_cast_t<std::remove_cv_t<E>>{};

template <enum_type E>
struct ascii_ci_enum_flags_cast_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    static constexpr auto operator()(std::string_view str, char delim = '|')
        -> std::optional<ENoCV>;
    static constexpr auto operator()(std::string_view str, std::string_view delim)
        -> std::optional<ENoCV>;
};

template <enum_type E>
constexpr auto ascii_ci_enum_flags_cast = ascii_ci_enum_flags_cast_t<std::remove_cv_t<E>>{};

}  // namespace rbox
```

- (1.1, 1.2) `enum_flags_cast<E>(str, delim)` returns the enum flag value if input str can be decomposed as enum entry names split by given delimiter, or `std::nullopt` otherwise. Each segment in input `str` is trimmed before parsing such that leading and trailing ASCII space characters of the segment are removed;
- (2) `enum_flags_cast<E>(value)` casts value to enum type `E` if it can be decomposed into a disjunction of defined enum entries, or returns `std::nullopt` otherwise. Signedness-safe and narrowing-safe comparison is performed.

Additionally, `ascii_ci_enum_flags_cast<E>(str, delim)` provides a case-insensitive variant where input segments are matched in an ASCII case-insensitive manner. Compilation error will be raised if duplicated names exist in enum definition.

Example:

```cpp
enum class permissions {
    read = 1,
    write = 2,
    execute = 4,
};

using namespace rbox::enum_bitwise_operators;

static_assert(
    rbox::enum_flags_cast<permissions>("read | write | execute") == permissions::read
    | permissions::write | permissions::execute);
// Space characters around "read " and "\twrite " will be trimmed.
// The empty token after the last '\n' will be ignored.
static_assert(
    rbox::enum_flags_cast<permissions>("read \n\twrite \n", '\n') == permissions::read
    | permissions::write);

static_assert(
    rbox::enum_flags_cast<permissions>(6) == permissions::write
    | permissions::execute);
static_assert(rbox::enum_flags_cast<permissions>(8) == std::nullopt);
```
