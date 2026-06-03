## Invalid Sequence Consumers

Defined in `<rbox/utils/string_encoding/consume_invalid_sequence.hpp>`.

```cpp
namespace rbox {

struct consume_utf_invalid_sequence_t {
    template <class CharT>
    static constexpr auto operator()(const CharT* input, const CharT* input_end) -> const CharT*;
};

struct consume_utf8_invalid_sequence_t { /* Same as above */ };
struct consume_utf16_invalid_sequence_t { /* Same as above */ };
struct consume_utf32_invalid_sequence_t { /* Same as above */ };

constexpr auto consume_utf8_invalid_sequence = consume_utf8_invalid_sequence_t{};
constexpr auto consume_utf16_invalid_sequence = consume_utf16_invalid_sequence_t{};
constexpr auto consume_utf32_invalid_sequence = consume_utf32_invalid_sequence_t{};
constexpr auto consume_utf_invalid_sequence = consume_utf_invalid_sequence_t{};

}  // namespace rbox
```

These helpers are used for recovery after a decoding failure. They inspect the input range `[input, input_end)` and consume the maximal invalid prefix starting at `input`.

`consume_utf*_invalid_sequence` consumes the maximal continuous invalid prefix starting at `input` and returns the first position where normal decoding may resume.

- `consume_utf8_invalid_sequence`: consumes invalid UTF-8 leading / continuation patterns, overlong forms, truncated sequences, surrogates encoded in UTF-8, and values above `U+10FFFF`.
- `consume_utf16_invalid_sequence`: consumes orphaned UTF-16 surrogates until a valid BMP code unit or a valid surrogate pair is reached.
- `consume_utf32_invalid_sequence`: consumes consecutive invalid UTF-32 code points.
- `consume_utf_invalid_sequence`: dispatches by `sizeof(CharT)`.

These helpers are intended for replacement-based recovery, where the whole consumed invalid slice is typically replaced by a single `replacement_code_point` (common practice according to Unicode 11 standard).

Example:

```cpp
char8_t bad_utf8[] = {u8'a', char8_t(0xFF), u8'b'};  // 0xFF is invalid UTF-8
auto next = rbox::consume_utf8_invalid_sequence(bad_utf8 + 1, bad_utf8 + 3);
// next == bad_utf8 + 2 (consumed 1 invalid byte, resuming at 'b')
```
