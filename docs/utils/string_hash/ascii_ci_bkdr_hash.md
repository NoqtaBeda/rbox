## ASCII Case-Insensitive BKDR Hash

Defined in header `<rbox/utils/string_hash/ascii_ci_bkdr_hash.hpp>`.

```cpp
namespace rbox {

struct ascii_ci_bkdr_hash_t {
    template <char_type CharT>
    static constexpr auto operator()(const CharT* begin, const CharT* end) -> size_t;
    template <string_like StringT>
    static constexpr auto operator()(const StringT& str) -> size_t;
};

constexpr auto ascii_ci_bkdr_hash = ascii_ci_bkdr_hash_t{};

}  // namespace rbox
```

`ascii_ci_bkdr_hash` is a locale-independent, ASCII case-insensitive variant which uses `ascii_tolower` to convert characters to lowercase before hashing. Works with all character types. Same overloads as `bkdr_hash`.

Example:

```cpp
constexpr auto h1 = rbox::ascii_ci_bkdr_hash("Hello");
constexpr auto h2 = rbox::ascii_ci_bkdr_hash("HELLO");
static_assert(h1 == h2);  // case-insensitive → same hash
```
