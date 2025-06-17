## `meta_basic_string_view`

Defined in header `<rbox/utils/meta_string_view.hpp>`.

`meta_basic_string_view<CharT>` is a structural alternative to `std::basic_string_view<CharT>`, which stores a pointer `head` and a size `n`. For a value-initialized `meta_basic_string_view` instance, `head` is a null pointer and `n` is 0 denoting an empty string; Otherwise, they denote the character range `[head, head + n)`. It is ensured that the referenced string is always null-terminated (i.e., `head == nullptr || head[n] == '\0'` always holds). Type aliases are provided for all character types: `meta_string_view`, `meta_u8string_view`, etc.

A structural type is a type whose value can be used as a non-type template parameter (see [cppreference](https://en.cppreference.com/w/cpp/language/template_parameters) for details).

> ⚠️ The internal fields are _intentionally public_ to keep the type structural. **NOT RECOMMENDED to modify the data members directly**. Undefined behavior may occur if you break their data consistency via direct modification.

> ⚠️ `meta_basic_string_view` is _intentionally trivial_. **BE CAREFUL with initialization**:
> ```cpp
> auto s1 = meta_string_view{};  // ✅️ Value initialization as {nullptr, 0}
> meta_string_view s2;           // ⚠️ Trivial initialization: undetermined value!
> ```

### Synopsis

```cpp
namespace rbox {

template <class CharT>
struct meta_basic_string_view {
    using value_type = CharT;
    using pointer = CharT*;
    using const_pointer = const CharT*;
    using reference = CharT&;
    using const_reference = const CharT&;
    using iterator = const_pointer;
    using const_iterator = const_pointer;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    // Public data members (structural requirement)
    const CharT* head;
    size_t n;

    // Default construction
    constexpr meta_basic_string_view() = default;

    // consteval constructors (with compile-time trailing '\0' check)
    consteval meta_basic_string_view(const CharT* literal);
    template <size_t N>
    consteval meta_basic_string_view(const CharT (&literal)[N]);
    template <size_t N>
    consteval meta_basic_string_view(const std::array<CharT, N>& arr);
    consteval meta_basic_string_view(std::basic_string_view<CharT> sv);

    // Conversion to std::basic_string_view
    constexpr operator std::basic_string_view<CharT>() const;

    // Element access
    constexpr auto operator[](size_t index) const -> CharT;
    constexpr auto front() const -> CharT;
    constexpr auto back() const -> CharT;

    // Capacity
    constexpr auto size() const -> size_t;
    constexpr auto length() const -> size_t;
    constexpr auto empty() const -> bool;

    // Data access
    constexpr auto data() const -> const CharT*;

    // Iterators
    constexpr auto begin() const -> const CharT*;
    constexpr auto end() const -> const CharT*;
    constexpr auto cbegin() const -> const CharT*;
    constexpr auto cend() const -> const CharT*;

    // Modification (returns new view)
    constexpr auto remove_prefix(size_t n) const -> meta_basic_string_view;

    // Comparison
    constexpr auto operator<=>(meta_basic_string_view rhs) const -> std::strong_ordering;
    constexpr bool operator==(meta_basic_string_view rhs) const;
    constexpr auto operator<=>(std::basic_string_view<CharT> rhs) const -> std::strong_ordering;
    constexpr bool operator==(std::basic_string_view<CharT> rhs) const;
    constexpr auto operator<=>(const CharT* rhs) const -> std::strong_ordering;
    constexpr bool operator==(const CharT* rhs) const;
};

// Type aliases for common character types
using meta_string_view = meta_basic_string_view<char>;
using meta_wstring_view = meta_basic_string_view<wchar_t>;
using meta_u8string_view = meta_basic_string_view<char8_t>;
using meta_u16string_view = meta_basic_string_view<char16_t>;
using meta_u32string_view = meta_basic_string_view<char32_t>;

}  // namespace rbox
```
