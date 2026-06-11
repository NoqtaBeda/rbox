## `meta_span`

Defined in header `<rbox/utils/meta_span.hpp>`.

`meta_span<T>` is a structural alternative to `std::span<const T>`, which stores a pointer `head` and a size `n`. For a value-initialized `meta_span` instance, `head` is a null pointer and `n` is 0 denoting an empty range; Otherwise, they denote the contiguous range `[head, head + n)`. It is designed for contiguous ranges with static constant storage only.

A structural type is a type whose value can be used as a non-type template parameter (see [cppreference](https://en.cppreference.com/w/cpp/language/template_parameters) for details).

> ⚠️ The internal fields are _intentionally public_ to keep the type structural. **NOT RECOMMENDED to modify the data members directly**. Undefined behavior may occur if you break their data consistency via direct modification.

> ⚠️ `meta_span` is _intentionally trivial_. **BE CAREFUL with initialization**:
> ```cpp
> auto s1 = meta_span<int>{};  // ✅️ Value initialization as {nullptr, 0}
> meta_span<int> s2;           // ⚠️ Default initialization: undetermined value!
> ```

### Synopsis

```cpp
namespace rbox {

template <class T>
struct meta_span {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = const_pointer;
    using const_iterator = const_pointer;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    // Public data members (structural requirement)
    const T* head;
    size_t n;

    // Default construction
    constexpr meta_span() = default;

    // Constructors
    constexpr meta_span(const T* head, size_t n);
    constexpr meta_span(const T* head, const T* tail);
    constexpr meta_span(std::initializer_list<T> il);

    template <size_t N>
    constexpr meta_span(const T (&arr)[N]);
    template <size_t N>
    constexpr meta_span(const std::array<T, N>& arr);
    constexpr meta_span(std::span<const T> span);

    // Conversion to std::span
    constexpr operator std::span<const T>() const;

    // Element access
    constexpr auto operator[](size_t index) const -> const T&;
    constexpr auto front() const -> const T&;
    constexpr auto back() const -> const T&;

    // Capacity
    constexpr auto size() const -> size_t;
    constexpr auto empty() const -> bool;

    // Data access
    constexpr auto data() const -> const T*;

    // Iterators
    constexpr auto begin() const -> const T*;
    constexpr auto end() const -> const T*;
    constexpr auto cbegin() const -> const T*;
    constexpr auto cend() const -> const T*;

    // Subspan operations
    constexpr auto first(size_t n) const -> meta_span;
    constexpr auto last(size_t n) const -> meta_span;
    constexpr auto subspan(size_t offset, size_t count = std::dynamic_extent) const -> meta_span;
};

}  // namespace rbox
```
