## ASCII Character Classification

Defined in header `<rbox/utils/ctype/char_categories.hpp>`.

```cpp
namespace rbox {

struct ascii_isalnum_t {
    template <non_bool_integral T>
    static constexpr auto operator()(T c) -> bool;
};
struct ascii_isalpha_t { /* Same as above */ };
struct ascii_islower_t { /* Same as above */ };
struct ascii_isupper_t { /* Same as above */ };
struct ascii_isdigit_t { /* Same as above */ };
struct ascii_isxdigit_t { /* Same as above */ };
struct ascii_isblank_t { /* Same as above */ };
struct ascii_iscntrl_t { /* Same as above */ };
struct ascii_isgraph_t { /* Same as above */ };
struct ascii_isspace_t { /* Same as above */ };
struct ascii_isprint_t { /* Same as above */ };
struct ascii_ispunct_t { /* Same as above */ };

constexpr auto ascii_isalnum = ascii_isalnum_t{};
constexpr auto ascii_isalpha = ascii_isalpha_t{};
constexpr auto ascii_islower = ascii_islower_t{};
constexpr auto ascii_isupper = ascii_isupper_t{};
constexpr auto ascii_isdigit = ascii_isdigit_t{};
constexpr auto ascii_isxdigit = ascii_isxdigit_t{};
constexpr auto ascii_isblank = ascii_isblank_t{};
constexpr auto ascii_iscntrl = ascii_iscntrl_t{};
constexpr auto ascii_isgraph = ascii_isgraph_t{};
constexpr auto ascii_isspace = ascii_isspace_t{};
constexpr auto ascii_isprint = ascii_isprint_t{};
constexpr auto ascii_ispunct = ascii_ispunct_t{};

}  // namespace rbox
```

These predicates match the behavior of their `std::` counterparts for ASCII characters (0-127). **For non-ASCII values, they always return `false`**:

| Functor          | Description                                  |
| ---------------- | -------------------------------------------- |
| `ascii_isalnum`  | Alphanumeric character (`[0-9A-Za-z]`)       |
| `ascii_isalpha`  | Alphabetic character (`[A-Za-z]`)            |
| `ascii_islower`  | Lowercase letter (`[a-z]`)                   |
| `ascii_isupper`  | Uppercase letter (`[A-Z]`)                   |
| `ascii_isdigit`  | Decimal digit (`[0-9]`)                      |
| `ascii_isxdigit` | Hexadecimal digit (`[0-9A-Fa-f]`)            |
| `ascii_isblank`  | Blank character (space or tab)               |
| `ascii_iscntrl`  | Control character (`[0-31, 127]`)            |
| `ascii_isgraph`  | Graphical character (printable except space) |
| `ascii_isspace`  | Whitespace character (`[ \f\n\r\t\v]`)       |
| `ascii_isprint`  | Printable character (`[32-126]`)             |
| `ascii_ispunct`  | Punctuation character                        |
