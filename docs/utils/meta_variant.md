## `meta_variant`

Defined in header `<rbox/utils/meta_variant.hpp>`.

`meta_variant<Args...>` is a structural alternative to `std::variant<Args...>`. It uses `std::meta::define_aggregate()` to create a tagged union at compile time, making the variant itself a structural type (provided all of its alternatives are structural). `meta_variant` strictly requires all alternative types to be trivial (due to constraints on `union` members by C++ language).

A structural type is a type whose value can be used as a non-type template parameter (see [cppreference](https://en.cppreference.com/w/cpp/language/template_parameters) for details).

> ⚠️ The internal fields are _intentionally public_ to keep the type structural. **NOT RECOMMENDED to modify the data members directly**. Undefined behavior may occur if you break their data consistency via direct modification.

> ⚠️ `meta_variant` is _intentionally trivial_. **BE CAREFUL with initialization**.

### Synopsis

```cpp
namespace rbox {

template <class... Args>
struct meta_variant {
    union underlying_type;  // Implementation-defined

    // Default-constructs the 1st alternative
    constexpr meta_variant() noexcept;

    // Value construction (exact match or unique conversion)
    template <class T>
    constexpr meta_variant(T value) noexcept;

    // In-place assignment
    template <class T>
    constexpr meta_variant& operator=(T value) noexcept;

    // Trivial copy/move
    constexpr meta_variant(const meta_variant&) noexcept = default;
    constexpr meta_variant(meta_variant&&) noexcept = default;
    constexpr meta_variant& operator=(const meta_variant&) noexcept = default;
    constexpr meta_variant& operator=(meta_variant&&) noexcept = default;
    constexpr ~meta_variant() noexcept = default;

    // In-place construction
    template <class T>
    constexpr void emplace(T value);

    // Querying
    constexpr auto index() const -> size_t;

    template <class T>
    constexpr auto holds_alternative() const -> bool;

    template <class T, class Self>
    constexpr decltype(auto) get(this Self&& self);

    template <size_t I, class Self>
    constexpr decltype(auto) get(this Self&& self);

    template <class T>
    constexpr auto get_if() & -> auto*;
    template <class T>
    constexpr auto get_if() const& -> const auto*;
    template <size_t I>
    constexpr auto get_if() & -> auto*;
    template <size_t I>
    constexpr auto get_if() const& -> const auto*;

    // Visitation
    template <class Self, class Func>
    constexpr decltype(auto) visit(this Self&& self, Func&& func);

    // Public for structural type compliance
    size_t alternative_index;
    underlying_type alternatives;
};

// Comparison operators (defined iff all alternatives support them)
template <class... Args>
constexpr bool operator==(const meta_variant<Args...>& lhs, const meta_variant<Args...>& rhs);
template <class... Args>
constexpr auto operator<=>(const meta_variant<Args...>& lhs, const meta_variant<Args...>& rhs);

// -------- Free API functions --------

template <class T, class... Args>
constexpr auto holds_alternative(const meta_variant<Args...>& v) -> bool;

template <class T, class Variant>
constexpr decltype(auto) get(Variant&& v);
template <size_t I, class Variant>
constexpr decltype(auto) get(Variant&& v);

template <class T, class... Args>
constexpr auto* get_if(meta_variant<Args...>& v);  // also const&
template <size_t I, class... Args>
constexpr auto* get_if(meta_variant<Args...>& v);  // also const&

template <class Func, class Variant>
constexpr decltype(auto) visit(Func&& func, Variant&& v);

}  // namespace rbox
```

### Description

**Comparison**: `operator==` and `operator<=>` are defined only when all alternatives support the corresponding operator. Similar to `std::variant`, comparison first checks `alternative_index` — if indices differ, `<=>` compares by index; if equal, the underlying values are compared.

**Exception**: Both member `get()` and free `get()` throws `std::bad_variant_access` on alternative mismatch. If exceptions are to be disabled, use the configuration macro `RBOX_NO_EXCEPTIONS` and then `get()` will simply call `std::abort()` instead of throwing an exception.
