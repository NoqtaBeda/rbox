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

Overloads in `enum_flags_contains<E>` are similar to those in `enum_flags_cast<E>` while only a presence check is performed (i.e., whether the input string or value is a disjunction of enum entries defined). If you do not need the casted enum value, `enum_flags_contains<E>` has better performance.
