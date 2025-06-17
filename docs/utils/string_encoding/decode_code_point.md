## Code Point Decoders

Defined in `<rbox/utils/string_encoding/decode_code_point.hpp>`.

```cpp
namespace rbox {

struct decode_code_point_t {
    template <class CharT>
    static constexpr auto operator()(const CharT* input, const CharT* input_end)
        -> std::pair<char32_t, const CharT*>;
};

struct decode_code_point_from_utf8_t {
    template <class CharT>
    static constexpr auto operator()(const CharT* input, const CharT* input_end)
        -> std::pair<char32_t, const CharT*>;
};

struct decode_code_point_from_utf16_t {
    template <class CharT>
    static constexpr auto operator()(const CharT* input, const CharT* input_end)
        -> std::pair<char32_t, const CharT*>;
};

constexpr auto decode_code_point = decode_code_point_t{};
constexpr auto decode_code_point_from_utf8 = decode_code_point_from_utf8_t{};
constexpr auto decode_code_point_from_utf16 = decode_code_point_from_utf16_t{};

}  // namespace rbox
```

These helpers decode one code point from the input range `[input, input_end)`.

- `decode_code_point_from_utf8` decodes one UTF-8 scalar value from a 1-byte code unit sequence (`char8_t`, `char`, `uint8_t`, etc.).
- `decode_code_point_from_utf16` decodes one UTF-16 scalar value from a 2-byte code unit sequence (`char16_t`, `uint16_t`, etc.).
- `decode_code_point` dispatches by `sizeof(CharT)`:
  - `1` byte: UTF-8
  - `2` bytes: UTF-16
  - `4` bytes: UTF-32 validation + direct return

All decoders return `{code_point, next}` whose type is `std::pair<char32_t, const CharT*>`.

- The first element `code_point` is the decoded result code point, or `invalid_code_point` on failure.
- The second element `next` is an input pointer:
  - on success, it points to the first unconsumed code unit after the decoded character;
  - on failure, it is exactly the original `input` pointer passed to the function.

So `{code_point, next}` means "one scalar value was decoded and the input cursor advanced to `next`", while `{invalid_code_point, next = input}` means "decoding failed and no input was consumed".
