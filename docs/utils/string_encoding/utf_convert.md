## UTF Encoding Conversion

Defined in header `<rbox/utils/string_encoding/utf_convert.hpp>` and included in the umbrella header `<rbox/utils/string_encoding.hpp>`.

This module provides constexpr-compatible UTF encoding conversion between UTF-8, UTF-16, and UTF-32. It also supports escaping by various modes (including but not limited to JSON escaping rule).

### Synopsis

```cpp
namespace rbox {

enum class escaping_mode {
    none = 0,
    json = 1,
    display_char = 2,
    display_string = 3,
};

enum class encoding_status {
    done = 0,
    invalid_character = 1,
    buffer_run_out = 2,
};

template <class OutT, class InT>
struct encode_result_t {
    OutT* out_ptr;
    const InT* in_ptr;
    encoding_status status;
};

template <escaping_mode Mode>
struct utf_convert_by_t {
    template <class OutT, class InT>
    static constexpr auto operator()(
        OutT* dest, const OutT* dest_end, const InT* input, const InT* input_end)
        -> encode_result_t<OutT, InT>;
};

template <escaping_mode Mode>
constexpr auto utf_convert_by = utf_convert_by_t<Mode>{};

using utf_convert_t = utf_convert_by_t<escaping_mode::none>;
constexpr auto utf_convert = utf_convert_t{};

template <escaping_mode Mode>
struct utf8_to_utf16_by_t { /* Same as above */ };

template <escaping_mode Mode>
constexpr auto utf8_to_utf16_by = utf8_to_utf16_by_t<Mode>{};

using utf8_to_utf16_t = utf8_to_utf16_by_t<escaping_mode::none>;
constexpr auto utf8_to_utf16 = utf8_to_utf16_by_t<escaping_mode::none>{};

template <escaping_mode Mode>
struct utf8_to_utf32_by_t { /* Same as above */ };

template <escaping_mode Mode>
constexpr auto utf8_to_utf32_by = utf8_to_utf32_by_t<Mode>{};

using utf8_to_utf32_t = utf8_to_utf32_by_t<escaping_mode::none>;
constexpr auto utf8_to_utf32 = utf8_to_utf32_by_t<escaping_mode::none>{};

template <escaping_mode Mode>
struct utf16_to_utf8_by_t { /* Same as above */ };

template <escaping_mode Mode>
constexpr auto utf16_to_utf8_by = utf16_to_utf8_by_t<Mode>{};

using utf16_to_utf8_t = utf16_to_utf8_by_t<escaping_mode::none>;
constexpr auto utf16_to_utf8 = utf16_to_utf8_by_t<escaping_mode::none>{};

template <escaping_mode Mode>
struct utf16_to_utf32_by_t { /* Same as above */ };

template <escaping_mode Mode>
constexpr auto utf16_to_utf32_by = utf16_to_utf32_by_t<Mode>{};

using utf16_to_utf32_t = utf16_to_utf32_by_t<escaping_mode::none>;
constexpr auto utf16_to_utf32 = utf16_to_utf32_by_t<escaping_mode::none>{};

template <escaping_mode Mode>
struct utf32_to_utf8_by_t { /* Same as above */ };

template <escaping_mode Mode>
constexpr auto utf32_to_utf8_by = utf32_to_utf8_by_t<Mode>{};

using utf32_to_utf8_t = utf32_to_utf8_by_t<escaping_mode::none>;
constexpr auto utf32_to_utf8 = utf32_to_utf8_by_t<escaping_mode::none>{};

template <escaping_mode Mode>
struct utf32_to_utf16_by_t { /* Same as above */ };

template <escaping_mode Mode>
constexpr auto utf32_to_utf16_by = utf32_to_utf16_by_t<Mode>{};

using utf32_to_utf16_t = utf32_to_utf16_by_t<escaping_mode::none>;
constexpr auto utf32_to_utf16 = utf32_to_utf16_by_t<escaping_mode::none>{};

}  // namespace rbox
```

### Description

This submodule provides range-based transcoding between UTF-8, UTF-16, and UTF-32. The public API is organized around dedicated converters such as `utf8_to_utf16` and the size-based dispatcher `utf_convert`.

Each converter reads from an explicit input range `[input, input_end)` and writes to an explicit destination range `[dest, dest_end)`. Conversion stops when:

- the whole input range has been consumed successfully;
- an invalid input sequence is encountered;
- the destination buffer has insufficient remaining space.

Besides the default non-escaping variants, the header also provides `*_by<Mode>` variable templates for explicitly selecting an `escaping_mode`.

### Result Semantics

`encode_result_t<OutT, InT>` contains:

- `out_ptr`: one-past-the-end of the output already written;
- `in_ptr`: next input position to process;
- `status`: operation status.

Its status semantics are:

- `encoding_status::done`: all input in `[input, input_end)` was consumed;
- `encoding_status::invalid_character`: `in_ptr` points at the first invalid code unit sequence;
- `encoding_status::buffer_run_out`: `in_ptr` points at the next still-unprocessed input position.

### Character Type Support

- UTF-8 code units: any integral type except `bool` of size 1 (`char8_t`, `char`, `signed char`, `unsigned char`, `int8_t`, `uint8_t`);
- UTF-16 code units: any integral type of size 2 (`char16_t`, `int16_t`, `uint16_t`);
- UTF-32 code units: any integral type of size 4 (`char32_t`, `int32_t`, `uint32_t`, etc.).

> ⚠️ **BE CAREFUL with platform-dependent types** like `int`, `wchar_t`, `wint_t`, etc.

Example:

```cpp
// UTF-8 to UTF-16 conversion
char8_t input[] = u8"Hello 😀";
char16_t output[16] = {};
auto result = rbox::utf8_to_utf16(output, output + 16, input, std::end(input));
// result.status == encoding_status::done
// result.out_ptr - output == 8 (6 BMP + 2 surrogates... depends on encoding)
```

The dedicated converters further restrict source and destination size bytes by encoding direction:

- `utf8_to_utf16`: output type size 2, input type size 1
- `utf8_to_utf32`: output type size 4, input type size 1
- `utf16_to_utf8`: output type size 1, input type size 2
- `utf16_to_utf32`: output type size 4, input type size 2
- `utf32_to_utf8`: output type size 1, input type size 4
- `utf32_to_utf16`: output type size 2, input type size 4
- `utf_convert`: dispatches to one of above by `sizeof(OutT)` and `sizeof(InT)`

### Escaping Modes

For transcoding, `escaping_mode::json` escapes `"`, `\\`, `/`, `\b`, `\f`, `\n`, `\r`, `\t`, and other ASCII control characters in range `[0x0, 0x1F]` as the standard pattern `\u00XX`. Details of all `escaping_mode` candidates are shown in [Escaping](../string_encoding/escaping.md).
