## Code Point Encoders

Defined in `<rbox/utils/string_encoding/encode_code_point.hpp>`.

```cpp
namespace rbox {

struct encode_code_point_t {
    template <class CharT>
    static constexpr auto operator()(CharT* dest, const CharT* dest_end, char32_t code_point)
        -> std::pair<CharT*, encoding_status>;
};

struct encode_code_point_to_utf8_t { /* Same as above */ };
struct encode_code_point_to_utf16_t { /* Same as above */ };

struct encode_code_point_unsafe_t {
    template <class CharT>
    static constexpr auto operator()(CharT* dest, const CharT* dest_end, char32_t code_point)
        -> CharT*;

    template <class CharT>
    static constexpr auto operator()(CharT* dest, char32_t code_point) -> CharT*;
};

struct encode_code_point_to_utf8_unsafe_t { /* Same as above */ };
struct encode_code_point_to_utf16_unsafe_t { /* Same as above */ };

constexpr auto encode_code_point = encode_code_point_t{};
constexpr auto encode_code_point_unsafe = encode_code_point_unsafe_t{};

constexpr auto encode_code_point_to_utf8 = encode_code_point_to_utf8_t{};
constexpr auto encode_code_point_to_utf8_unsafe = encode_code_point_to_utf8_unsafe_t{};

constexpr auto encode_code_point_to_utf16 = encode_code_point_to_utf16_t{};
constexpr auto encode_code_point_to_utf16_unsafe = encode_code_point_to_utf16_unsafe_t{};

}  // namespace rbox
```

These helpers encode one Unicode code point into the destination range `[dest, dest_end)`.

- `encode_code_point_to_utf8` / `encode_code_point_to_utf16` validate both `code_point` and output capacity, then return `{next_dest, status}` whose type is `std::pair<CharT*, encoding_status>`.
- `encode_code_point` dispatches by `sizeof(CharT)` and also supports trivial UTF-32 output (after validation).
- `*_unsafe` variants skip validation and return only `next_dest` (in this case `status` is always `encoding_status::done`). They **assume** a valid code point and enough buffer space.

For the safe variants, the returned pair has the following meaning:

- the first element `next_dest` is the first output position after the written code units on success, or `dest` unchanged on failure;
- the second element `status` is the corresponding `encoding_status`.

So `{next_dest, status}` means "writing stopped at `next_dest` and produced status `status`".

The possible status values are:

- `encoding_status::done`: the code point was written successfully;
- `encoding_status::invalid_character`: the input code point is not a valid Unicode scalar value;
- `encoding_status::buffer_run_out`: the destination range is too small.
