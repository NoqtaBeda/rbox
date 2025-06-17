## Escape Writers

Defined in `<rbox/utils/string_encoding/escaping.hpp>`.

```cpp
namespace rbox {

enum class escaping_status {
    done = 0,
    no_escape = 1,
    error = 2,
};

template <escaping_mode Mode>
struct write_escaped_character_by_t {
    template <class CharT>
    static constexpr auto operator()(CharT* dest, const CharT* dest_end, char32_t c)
        -> std::pair<escaping_status, CharT*>;
};

template <escaping_mode Mode>
struct write_escaped_character_by_unsafe_t {
    template <class CharT>
    static constexpr auto operator()(CharT* dest, const CharT* dest_end, char32_t c)
        -> std::pair<escaping_status, CharT*>;

    template <class CharT>
    static constexpr auto operator()(CharT* dest, char32_t c)
        -> std::pair<escaping_status, CharT*>;
};

using write_escaped_character_for_json_t =
    write_escaped_character_by_t<escaping_mode::json>;
using write_escaped_character_for_char_t =
    write_escaped_character_by_t<escaping_mode::display_char>;
using write_escaped_character_for_string_t =
    write_escaped_character_by_t<escaping_mode::display_string>;

using write_escaped_character_for_json_unsafe_t =
    write_escaped_character_by_unsafe_t<escaping_mode::json>;
using write_escaped_character_for_char_unsafe_t =
    write_escaped_character_by_unsafe_t<escaping_mode::display_char>;
using write_escaped_character_for_string_unsafe_t =
    write_escaped_character_by_unsafe_t<escaping_mode::display_string>;

template <escaping_mode Mode>
constexpr auto write_escaped_character_by = write_escaped_character_by_t<Mode>{};
template <escaping_mode Mode>
constexpr auto write_escaped_character_by_unsafe = write_escaped_character_by_unsafe_t<Mode>{};

constexpr auto write_escaped_character_for_json = write_escaped_character_for_json_t{};
constexpr auto write_escaped_character_for_char = write_escaped_character_for_char_t{};
constexpr auto write_escaped_character_for_string = write_escaped_character_for_string_t{};

constexpr auto write_escaped_character_for_json_unsafe =
    write_escaped_character_for_json_unsafe_t{};
constexpr auto write_escaped_character_for_char_unsafe =
    write_escaped_character_for_char_unsafe_t{};
constexpr auto write_escaped_character_for_string_unsafe =
    write_escaped_character_for_string_unsafe_t{};

}  // namespace rbox
```

These helpers try to write an escaped form of a single character value `c` into the destination range `[dest, dest_end)`.

`write_escaped_character_*` functions return a `std::pair<escaping_status, CharT*>`.

- Safe variants (e.g., `write_escaped_character_for_json`, `write_escaped_character_by<Mode>`) take `(dest, dest_end, c)` and check capacity.
- Unsafe variants (e.g., `write_escaped_character_for_json_unsafe`, `write_escaped_character_by_unsafe<Mode>`) additionally support `(dest, c)` and assume enough space.

For the returned pair:

- the first element is the escape result status;
- the second element is the first output position after the characters written; below this pointer is denoted as `next_dest`.

So `{status, next_dest}` means "the escape attempt finished with status `status`, and the output cursor is now `next_dest`".

`escaping_status` is one of the following:

- `done`: an escaped form was written;
- `no_escape`: `c` does not need escaping in the selected mode;
- `error`: the destination buffer is too small.

Escaping behavior:

| Mode                            | Behavior                                                                                                          |
| ------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| `escaping_mode::json`           | Escapes `"`, `\\`, `/`, `\b`, `\f`, `\n`, `\r`, `\t`, and other ASCII control characters as `\u00XX`.             |
| `escaping_mode::display_char`   | Escapes `\\`, `\0`, `\a`, `\b`, `\f`, `\n`, `\r`, `\t`, `\v`, and other non-printable ASCII bytes as `\xHH`.      |
| `escaping_mode::display_string` | Escapes `"`, `\0`, `\\`, `\a`, `\b`, `\f`, `\n`, `\r`, `\t`, `\v`, and other non-printable ASCII bytes as `\xHH`. |
