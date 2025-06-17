## String Encoding Utilities

rbox provides `constexpr`-compatible UTF encoding conversion between UTF-8, UTF-16, and UTF-32, along with code point encoding/decoding, invalid sequence consumption, and character escaping. All components are locale-independent and suitable for both compile-time and runtime use.

Defined in header `<rbox/utils/string_encoding.hpp>`.

> **Warning: BE CAREFUL with platform-specific types** like `int`, `wchar_t`, etc. — The encoding conversion functions dispatch on `sizeof(T)` to select the UTF variant (1-byte → UTF-8, 2-byte → UTF-16, 4-byte → UTF-32). `wchar_t` is 2 bytes on Windows (UTF-16) but 4 bytes on most Unix-like platforms (UTF-32). Passing `wchar_t` strings to these utilities therefore yields platform-dependent behavior. Prefer the fixed-width types `char8_t`, `char16_t`, and `char32_t` for portable code.

### Components

* [encode_code_point](string_encoding/encode_code_point.md): Code point encoders (UTF-8/16/32)
* [decode_code_point](string_encoding/decode_code_point.md): Code point decoders (UTF-8/16/32)
* [utf_convert](string_encoding/utf_convert.md): Full UTF conversion with escaping support
* [escaping](string_encoding/escaping.md): Character escaping by various rules
* [consume_invalid_sequence](string_encoding/consume_invalid_sequence.md): Invalid sequence consumers for error recovery

### Constants & Code Point Validator

Defined in `<rbox/utils/string_encoding/common.hpp>`.

```cpp
namespace rbox {

constexpr auto invalid_code_point = static_cast<char32_t>(-1);
constexpr auto replacement_code_point = static_cast<char32_t>(0xFFFD);

constexpr bool is_valid_code_point(char32_t code_point);

}  // namespace rbox
```

`invalid_code_point` is the sentinel returned by the decoders on failure. `replacement_code_point` is the Unicode replacement character U+FFFD (which is '�'), typically used by higher-level recovery code after `consume_utf*_invalid_sequence`.

`is_valid_code_point(code_point)` checks whether a `char32_t` value is a valid Unicode scalar value. It accepts exactly `U+0000..U+D7FF` and `U+E000..U+10FFFF`. Surrogates and values above `U+10FFFF` are invalid.
