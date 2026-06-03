## BKDR Hash (Case-Sensitive)

Defined in header `<rbox/utils/string_hash/bkdr_hash.hpp>`.

```cpp
namespace rbox {

struct bkdr_hash_t {
    template <char_type CharT>
    static constexpr auto operator()(const CharT* begin, const CharT* end) -> size_t;
    template <string_like StringT>
    static constexpr auto operator()(const StringT& str) -> size_t;
};

constexpr auto bkdr_hash = bkdr_hash_t{};

}  // namespace rbox
```

`bkdr_hash` implements the modified BKDR hash algorithm:

```
result = 0
for each c in the input string:
    result = result * 131 + c
return result
```

Works with all character types. Provides two overloads:
1. Pointer range `[begin, end)`
2. Any string-like type (including C-style null-terminated strings)

Example:

```cpp
constexpr auto h1 = rbox::bkdr_hash("hello");       // hashes null-terminated string
constexpr auto h2 = rbox::bkdr_hash(std::string{"hello"});  // hashes std::string
static_assert(h1 == h2);  // same input → same hash
```
