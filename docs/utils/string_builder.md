## String Builder

Defined in header `<rbox/utils/string_builder.hpp>`.

```cpp
namespace rbox {

template <class CharT, class Allocator = std::allocator<CharT>>
class basic_string_builder {
public:
    // Construction / Destruction
    constexpr basic_string_builder();
    explicit constexpr basic_string_builder(Allocator alloc);
    explicit constexpr basic_string_builder(size_t initial_size);
    explicit constexpr basic_string_builder(size_t initial_size, Allocator alloc);

    // No copy semantics
    basic_string_builder(const basic_string_builder&) = delete;
    auto operator=(const basic_string_builder&) -> basic_string_builder& = delete;

    // Trivial move semantics
    constexpr basic_string_builder(basic_string_builder&&) = default;
    constexpr auto operator=(basic_string_builder&&) -> basic_string_builder& = default;

    constexpr ~basic_string_builder();

    // Querying
    constexpr size_t size() const;
    constexpr auto str() const -> std::basic_string<CharT>;
    constexpr auto strview() const& -> std::basic_string_view<CharT>;

    // Buffer management
    constexpr auto reserve_at_least(size_t n) -> basic_string_builder&;

    // -------- Append single char --------
    constexpr auto append_char(CharT c) -> basic_string_builder&;
    constexpr auto append_char(CharT c, size_t count) -> basic_string_builder&;
    constexpr auto append_char_unsafe(CharT c) -> basic_string_builder&;
    constexpr auto append_char_unsafe(CharT c, size_t count) -> basic_string_builder&;

    template <escaping_mode Mode>
    constexpr auto append_char_by(CharT c) -> basic_string_builder&;
    template <escaping_mode Mode>
    constexpr auto append_char_by_unsafe(CharT c) -> basic_string_builder&;

    // -------- Append single code point --------
    constexpr auto append_utf_code_point(char32_t code_point) -> basic_string_builder&;
    constexpr auto append_utf_code_point_unsafe(char32_t code_point) -> basic_string_builder&;

    template <escaping_mode Mode>
    constexpr auto append_utf_code_point_by(char32_t code_point) -> basic_string_builder&;
    template <escaping_mode Mode>
    constexpr auto append_utf_code_point_by_unsafe(char32_t code_point) -> basic_string_builder&;

    // -------- Append string (no UTF conversion) --------
    constexpr auto append_string(const CharT* str, const CharT* str_end) -> basic_string_builder&;
    constexpr auto append_string(const CharT* str) -> basic_string_builder&;
    constexpr auto append_string(std::basic_string_view<CharT> str) -> basic_string_builder&;

    constexpr auto append_string_unsafe(const CharT* str, const CharT* str_end) -> basic_string_builder&;
    constexpr auto append_string_unsafe(const CharT* str) -> basic_string_builder&;
    constexpr auto append_string_unsafe(std::basic_string_view<CharT> str) -> basic_string_builder&;

    template <escaping_mode Mode>
    constexpr auto append_string_by(const CharT* str, const CharT* str_end) -> basic_string_builder&;
    template <escaping_mode Mode>
    constexpr auto append_string_by(const CharT* str) -> basic_string_builder&;
    template <escaping_mode Mode>
    constexpr auto append_string_by(std::basic_string_view<CharT> str) -> basic_string_builder&;

    template <escaping_mode Mode>
    constexpr auto append_string_by_unsafe(const CharT* str, const CharT* str_end) -> basic_string_builder&;
    template <escaping_mode Mode>
    constexpr auto append_string_by_unsafe(const CharT* str) -> basic_string_builder&;
    template <escaping_mode Mode>
    constexpr auto append_string_by_unsafe(std::basic_string_view<CharT> str) -> basic_string_builder&;

    // -------- Append C-style string (no UTF conversion) --------
    constexpr auto append_c_string_unsafe(const char* str, const char* str_end) -> basic_string_builder&;
    constexpr auto append_c_string_unsafe(const char* str) -> basic_string_builder&;
    constexpr auto append_c_string_unsafe(std::string_view str) -> basic_string_builder&;

    constexpr auto append_c_string(const char* str, const char* str_end) -> basic_string_builder&;
    constexpr auto append_c_string(const char* str) -> basic_string_builder&;
    constexpr auto append_c_string(std::string_view str) -> basic_string_builder&;

    template <escaping_mode Mode>
    constexpr auto append_c_string_by_unsafe(const char* str, const char* str_end) -> basic_string_builder&;
    template <escaping_mode Mode>
    constexpr auto append_c_string_by_unsafe(const char* str) -> basic_string_builder&;
    template <escaping_mode Mode>
    constexpr auto append_c_string_by_unsafe(std::string_view str) -> basic_string_builder&;

    template <escaping_mode Mode>
    constexpr auto append_c_string_by(const char* str, const char* str_end) -> basic_string_builder&;
    template <escaping_mode Mode>
    constexpr auto append_c_string_by(const char* str) -> basic_string_builder&;
    template <escaping_mode Mode>
    constexpr auto append_c_string_by(std::string_view str) -> basic_string_builder&;

    // -------- Append string (with UTF conversion) --------
    template <char_type CharU>
    constexpr auto append_utf_string(const CharU* str, const CharU* str_end) -> basic_string_builder&;
    template <char_type CharU>
    constexpr auto append_utf_string(const CharU* str) -> basic_string_builder&;
    template <char_type CharU>
    constexpr auto append_utf_string(std::basic_string_view<CharU> str) -> basic_string_builder&;
    template <char_type CharU, class TraitsU, class AllocU>
    constexpr auto append_utf_string(const std::basic_string<CharU, TraitsU, AllocU>& str) -> basic_string_builder&;

    template <char_type CharU>
    constexpr auto append_utf_string_unsafe(const CharU* str, const CharU* str_end) -> basic_string_builder&;
    template <char_type CharU>
    constexpr auto append_utf_string_unsafe(const CharU* str) -> basic_string_builder&;
    template <char_type CharU>
    constexpr auto append_utf_string_unsafe(std::basic_string_view<CharU> str) -> basic_string_builder&;
    template <char_type CharU, class TraitsU, class AllocU>
    constexpr auto append_utf_string_unsafe(const std::basic_string<CharU, TraitsU, AllocU>& str) -> basic_string_builder&;

    template <escaping_mode Mode, char_type CharU>
    constexpr auto append_utf_string_by(const CharU* str, const CharU* str_end) -> basic_string_builder&;
    template <escaping_mode Mode, char_type CharU>
    constexpr auto append_utf_string_by(const CharU* str) -> basic_string_builder&;
    template <escaping_mode Mode, char_type CharU>
    constexpr auto append_utf_string_by(std::basic_string_view<CharU> str) -> basic_string_builder&;
    template <escaping_mode Mode, char_type CharU, class TraitsU, class AllocU>
    constexpr auto append_utf_string_by(const std::basic_string<CharU, TraitsU, AllocU>& str) -> basic_string_builder&;

    template <escaping_mode Mode, char_type CharU>
    constexpr auto append_utf_string_by_unsafe(const CharU* str, const CharU* str_end) -> basic_string_builder&;
    template <escaping_mode Mode, char_type CharU>
    constexpr auto append_utf_string_by_unsafe(const CharU* str) -> basic_string_builder&;
    template <escaping_mode Mode, char_type CharU>
    constexpr auto append_utf_string_by_unsafe(std::basic_string_view<CharU> str) -> basic_string_builder&;
    template <escaping_mode Mode, char_type CharU, class TraitsU, class AllocU>
    constexpr auto append_utf_string_by_unsafe(const std::basic_string<CharU, TraitsU, AllocU>& str) -> basic_string_builder&;

    // -------- Append arithmetic types --------
    constexpr auto append_bool(bool value) -> basic_string_builder&;

    template <non_bool_integral IntegerT>
    constexpr auto append_integer(IntegerT value, int base = 10) -> basic_string_builder&;

    constexpr auto append_floating_point(float value) -> basic_string_builder&;
    constexpr auto append_floating_point(double value) -> basic_string_builder&;
    constexpr auto append_floating_point(long double value) -> basic_string_builder&;

    template <std::floating_point FloatT>
    constexpr auto append_floating_point(FloatT value, std::chars_format fmt)
        -> basic_string_builder&;

    template <std::floating_point FloatT>
    constexpr auto append_floating_point(FloatT value, std::chars_format fmt, int precision)
        -> basic_string_builder&;

    // -------- Output iterators --------
    class safe_output_iterator;
    constexpr auto out() -> safe_output_iterator;

    class unsafe_output_iterator;
    constexpr auto out_unsafe() -> unsafe_output_iterator;
};

using string_builder = basic_string_builder<char>;
using u8string_builder = basic_string_builder<char8_t>;
using u16string_builder = basic_string_builder<char16_t>;
using u32string_builder = basic_string_builder<char32_t>;
using wstring_builder = basic_string_builder<wchar_t>;

using pmr_string_builder =
    basic_string_builder<char, std::pmr::polymorphic_allocator<char>>;
using pmr_u8string_builder =
    basic_string_builder<char8_t, std::pmr::polymorphic_allocator<char8_t>>;
using pmr_u16string_builder =
    basic_string_builder<char16_t, std::pmr::polymorphic_allocator<char16_t>>;
using pmr_u32string_builder =
    basic_string_builder<char32_t, std::pmr::polymorphic_allocator<char32_t>>;
using pmr_wstring_builder =
    basic_string_builder<wchar_t, std::pmr::polymorphic_allocator<wchar_t>>;

}  // namespace rbox
```

The `basic_string_builder` class provides a constexpr-compatible string builder with automatic buffer management and support for [UTF encoding conversion](string_encoding/utf_convert.md).

### Construction / Destruction

- Default constructor creates a builder with no initial buffer.
- Constructor taking `Allocator` creates a builder with a custom allocator.
- Constructor taking `initial_size` allocates an initial buffer of that many characters.
- Copy construction and copy assignment are **deleted**.
- Move construction and move assignment are **defaulted**.
- Destructor deallocates the internal buffer (if any) using the allocator.

### Type Aliases

| Type Alias          | Character Type | Allocator                         |
| ------------------- | -------------- | --------------------------------- |
| `string_builder`    | `char`         | `std::allocator<char>`            |
| `u8string_builder`  | `char8_t`      | `std::allocator<char8_t>`         |
| `u16string_builder` | `char16_t`     | `std::allocator<char16_t>`        |
| `u32string_builder` | `char32_t`     | `std::allocator<char32_t>`        |
| `wstring_builder`   | `wchar_t`      | `std::allocator<wchar_t>`         |
| `pmr_*_builder`     | Same as above  | `std::pmr::polymorphic_allocator` |

> ⚠️ **BE CAREFUL** with UTF conversion to `wchar_t` string. The behavior is platform-dependent (see [string_encoding](./string_encoding.md) for details)

### Core Methods

- `size()`: Returns the current number of characters in the builder.
- `str()`: Returns a `std::basic_string<CharT>` with a copy of the current content.
- `strview()`: Returns a `std::basic_string_view<CharT>` referencing the current content.
- `reserve_at_least(n)`: Ensures the buffer has capacity for at least `n` additional characters. Grows exponentially if needed.

### Safe vs Unsafe Variants

Most `append_*` methods come in two forms:

| Variant    | Naming            | Behavior                                                                                                                                             |
| ---------- | ----------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Safe**   | `append_*`        | Calls `reserve_at_least` before writing. **Preferred in most cases.**                                                                                |
| **Unsafe** | `append_*_unsafe` | Assumes the caller has already ensured sufficient buffer space. Skips `reserve_at_least` for performance when buffer capacity is known to be enough. |

Suggested buffer reservation size for each `_unsafe` function is shown in the following table (Constant 6 refers to the max escaped size `\u00xx`. `s, t` refers to UTF-`s` -> UTF-`t` conversion):

| `_unsafe` Method                                                                       | Worst-Case Buffer Usage                         |
| -------------------------------------------------------------------------------------- | ----------------------------------------------- |
| `append_char_unsafe(c)`                                                                | 1                                               |
| `append_char_unsafe(c, count)`                                                         | `count`                                         |
| `append_utf_code_point_unsafe(cp)`                                                     | `32/t` (UTF-8: max 4 bytes)                     |
| `append_string_unsafe(s, s+n)` <br> `append_c_string_unsafe(s, s+n)`                   | `n`                                             |
| `append_string_by_unsafe<Mode>(s, s+n)` <br> `append_c_string_by_unsafe<Mode>(s, s+n)` | `6 × n` (escaped: worst-case 6×)                |
| `append_utf_string_unsafe(s, s+n)`                                                     | `max(1, s/t) × n` (same-type or UTF conversion) |

Typical usage of `_unsafe` variants is chained operation sequence whose total buffer usage is known as prior knowledge. For example:

```cpp
rbox::u8string_builder builder;

// Known: 6 chars for "Name: " + at most 256 chars for name + 12 chars for ", status: OK"
builder.reserve_at_least(6 + 256 + 12);

// Now all _unsafe variants can be chained without intermediate reserve checks
builder.append_string_unsafe(u8"Name: ")
    .append_utf_string_unsafe(name)  // u16string, with at most 128 UTF-16 units
    .append_string_unsafe(u8", status: OK");

auto result = builder.str();
```

### Escaping-mode Template Methods

Methods suffixed `_by` accept an `escaping_mode` template parameter, enabling arbitrary [escaping modes](string_encoding/escaping.md):

- `append_char_by<Mode>`: Append a single character using the specified escaping mode.
- `append_string_by<Mode>`: Append a same-type string using the specified escaping mode.
- `append_utf_string_by<Mode>`: Append a (potentially different-type) string using the specified escaping mode.
- `append_utf_code_point_by<Mode>`: Append a code point using the specified escaping mode.

Each `_by` method also has a `_by_unsafe` counterpart.

### Appending Characters

- `append_char(c)`: Appends a single character.
- `append_char(c, count)`: Appends `count` copies of character `c`.
- `append_char_unsafe(c)` / `append_char_unsafe(c, count)`: Unsafe variants (no reserve).

### Appending Code Points

- `append_utf_code_point(code_point)`: Appends a Unicode code point, [encoding](string_encoding/encode_code_point.md) it appropriately for the builder's character type. Invalid code points are replaced with U+FFFD.
- `append_utf_code_point_unsafe(code_point)`: Unsafe variant.

### Appending Strings

The builder supports the following types of string append operations:

1. **`append_string`**: Appends raw characters without UTF conversion or escaping.
   - Safe overloads: pointer with end, null-terminated pointer, `std::basic_string_view`
   - Unsafe variants: `append_string_unsafe` with the same overloads

2. **`append_utf_string`**: Appends strings with automatic [UTF encoding conversion](string_encoding/utf_convert.md). Invalid UTF sequences are replaced with U+FFFD.
   - Safe overloads: pointer with end, null-terminated pointer, `std::basic_string_view`, `std::basic_string`
   - Unsafe variants: `append_utf_string_unsafe` with the same overloads
   - Supports all UTF conversions (UTF-8↔UTF-16, UTF-8↔UTF-32, UTF-16↔UTF-32, and same-type)

### Appending C-style Strings

The `append_c_string` series accepts `const char*` or `std::string_view` input regardless of the builder's character type. Characters are converted to `CharT` via `static_cast`—no UTF encoding conversion is performed.

### Output Iterators

The builder provides two output iterator classes, following the same idiom as `std::back_insert_iterator` (`operator*` and `operator++` are no-ops returning `*this`; `operator=(CharT c)` does the actual work):

**`safe_output_iterator`** — Calls `append_char(c)` on each assignment (calling `reserve_at_least` internally each time):

```cpp
auto iter = builder.out();  // safe_output_iterator
*iter++ = 'A';              // calls builder.append_char('A')
```

**`unsafe_output_iterator`** — Writes directly to the internal buffer. Automatic buffer expansion is **not** performed and you need to ensure the buffer has enough space via manual `reserve_at_least()` call.

```cpp
auto iter = builder.out_unsafe();  // unsafe_output_iterator
*iter++ = 'A';
```

These iterators satisfy `std::output_iterator<CharT>` and can be used with standard algorithms or functions that write through output iterators (e.g. `std::ranges::copy`).

Factory methods:

- `out()`: Returns a `safe_output_iterator` bound to this builder.
- `out_unsafe()`: Returns an `unsafe_output_iterator` bound to this builder.

### Appending Numbers

- `append_bool(value)`: Appends "true" or "false".
- `append_integer(value, base)`: Appends an integer in the specified base (default: decimal). Supports bases 2-36.
- `append_floating_point(value)`: Appends a floating-point number.
- `append_floating_point(value, fmt)`: Appends with specified format (`std::chars_format::general`, `scientific`, `fixed`, `hex`).
- `append_floating_point(value, fmt, precision)`: Appends with specified format and precision.

### Buffer Management

The builder manages a buffer allocated via the provided allocator:

- Initial size can be specified in the constructor
- When the buffer is full, it grows exponentially (doubles capacity)
- `reserve_at_least(n)` can be called explicitly to pre-allocate space
- All operations are constexpr-compatible
- Buffer memory is deallocated in the destructor using the allocator

### Chaining

All `append_*` methods return `basic_string_builder&`, enabling method chaining:

```cpp
auto result = rbox::u8string_builder{}
                  .append_string(u8"Name: ")
                  .append_string(u8"John")
                  .append_char(' ')
                  .append_integer(30)
                  .append_string(u8", Score: ")
                  .append_floating_point(98.5)
                  .append_bool(true)
                  .strview();
```

### Error Handling for UTF Conversion

When `append_utf_string` encounters [invalid UTF sequences](string_encoding/consume_invalid_sequence.md), it:

1. Replaces the invalid sequence with the Unicode replacement character (U+FFFD, which is '�')
2. Continues processing the remaining input

```cpp
rbox::u16string_builder builder;
const char8_t invalid[] = u8"hello\xFF\xFEworld";
builder.append_utf_string(invalid, invalid + sizeof(invalid) - 1);
// Result: u"hello�world"
```
