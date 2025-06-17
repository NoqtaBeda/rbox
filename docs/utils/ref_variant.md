## `ref_variant`

Defined in header `<rbox/utils/ref_variant.hpp>`.

`ref_variant<Args...>` is a non-owning variant that stores a pointer to one of its alternative types. Unlike `std::variant` which owns its values, `ref_variant` references externally-managed objects — analogous to `std::reference_wrapper` generalized over multiple types.

**Construction and rebinding.** Construction accepts an lvalue whose type matches or is convertible to _exactly one_ alternative. Copy and move are shallow (they rebind the internal pointer, consistent with `std::reference_wrapper`). The `rebind()` method allows changing the referenced object after construction.

**Access.** Index-based and type-based `get()` / `get_if()` are provided. `get()` throws (or aborts if exceptions are disabled by configuration macro `RBOX_NO_EXCEPTIONS`) on mismatch; `get_if()` returns `nullptr`. Reflecting its non-owning nature, `ref_variant` uses *shallow const*: `get()` always returns `T&` where `T` is the matched alternative type, regardless of the variant's own constness (consistent with the behavior of `std::optional<T&>` and `std::reference_wrapper`).

**Visitation and comparison.** `visit()` dispatches the held alternative to a callable. `operator==` and `operator<=>` compare `alternative_index` first, then the referenced values — requiring the alternatives to be equality/three-way comparable respectively.

**Structural type.** The data members `alternative_index` and `alternatives` (an aggregate of pointers generated via `std::meta::define_aggregate`) are public, making `ref_variant` a structural type.

The alternative types `Args...` must be distinct, non-void, non-reference, non-array. At least one alternative is required. Free-function equivalents of `holds_alternative`, `get`, `get_if`, and `visit` are also provided.

```cpp
namespace rbox {

template <class... Args>
struct ref_variant {
    struct underlying_type;  // compile-time generated aggregate of pointers

    // ---- Construction & Assignment ----

    template <class T>
    constexpr ref_variant(T& obj) noexcept;

    constexpr ref_variant(const ref_variant&) noexcept = default;
    constexpr ref_variant& operator=(const ref_variant&) noexcept = default;

    constexpr ref_variant(ref_variant&&) noexcept = default;
    constexpr ref_variant& operator=(ref_variant&&) noexcept = default;

    constexpr ~ref_variant() noexcept = default;

    // ---- Rebinding ----

    template <class T>
    constexpr void rebind(T&& obj);

    // ---- Querying ----

    constexpr auto index() const -> size_t;

    template <class T>
    constexpr auto holds_alternative() const -> bool;

    // ---- Access ----

    template <class T>
    constexpr auto get() const -> auto&;       // shallow const

    template <size_t I>
    constexpr auto get() const -> auto&;

    template <class T>
    constexpr auto get_if() const -> auto*;

    template <size_t I>
    constexpr auto get_if() const -> auto*;

    // ---- Visitation ----

    template <class Func>
    constexpr decltype(auto) visit(Func&& func) const;

    // ---- Data Members ----

    size_t alternative_index;
    underlying_type alternatives;
};

// ---- Comparison ----

template <class... Args>
constexpr bool operator==(const ref_variant<Args...>& lhs, const ref_variant<Args...>& rhs);

template <class... Args>
constexpr auto operator<=>(const ref_variant<Args...>& lhs, const ref_variant<Args...>& rhs)
    /* -> common category */;

// ---- Free functions ----

template <class T, class... Args>
constexpr auto holds_alternative(const ref_variant<Args...>& v) -> bool;

template <class T, class... Args>
constexpr auto get(const ref_variant<Args...>& v) -> auto&;

template <size_t I, class... Args>
constexpr auto get(const ref_variant<Args...>& v) -> auto&;

template <class T, class... Args>
constexpr auto get_if(const ref_variant<Args...>& v) -> auto*;

template <size_t I, class... Args>
constexpr auto get_if(const ref_variant<Args...>& v) -> auto*;

template <class Func, class... Args>
constexpr decltype(auto) visit(Func&& func, const ref_variant<Args...>& v);

}  // namespace rbox
```

## `optional_ref_variant`

`optional_ref_variant<Args...>` extends `ref_variant` with a valueless (empty) state — analogous to the relationship between `std::optional<T&>` and `T&`. Default construction or `reset()` enters the valueless state, where `alternative_index == npos`, `has_value()` returns `false`, `valueless()` returns `true`, and `get()` throws/aborts. `try_visit()` returns `std::nullopt` when valueless instead of invoking the callable. Comparison treats two valueless instances as equal. Valueless `optional_ref_variant` is always treated as the lowest during comparison. All other semantics (shallow const, shallow copy/move, structural type, free functions) match `ref_variant`.

```cpp
namespace rbox {

template <class... Args>
struct optional_ref_variant {
    struct underlying_type;

    // ---- Construction & Assignment ----

    constexpr optional_ref_variant() noexcept;  // valueless

    template <class T>
    constexpr optional_ref_variant(T& obj) noexcept;

    constexpr optional_ref_variant(const optional_ref_variant&) noexcept = default;
    constexpr optional_ref_variant& operator=(const optional_ref_variant&) noexcept = default;

    constexpr optional_ref_variant(optional_ref_variant&&) noexcept = default;
    constexpr optional_ref_variant& operator=(optional_ref_variant&&) noexcept = default;

    constexpr ~optional_ref_variant() noexcept = default;

    // ---- Rebinding & Reset ----

    template <class T>
    constexpr void rebind(T&& obj);

    constexpr void reset();

    // ---- Querying ----

    constexpr auto index() const -> size_t;
    constexpr auto has_value() const -> bool;
    constexpr auto valueless() const -> bool;

    template <class T>
    constexpr auto holds_alternative() const -> bool;

    // ---- Access ----

    template <class T>
    constexpr auto get() const -> auto&;       // shallow const

    template <size_t I>
    constexpr auto get() const -> auto&;

    template <class T>
    constexpr auto get_if() const -> auto*;

    template <size_t I>
    constexpr auto get_if() const -> auto*;

    // ---- Visitation ----

    template <class Func>
    constexpr auto try_visit(Func&& func) const;  // returns std::optional

    // ---- Data Members ----

    size_t alternative_index;  // npos when valueless
    underlying_type alternatives;
};

// ---- Comparison ----

template <class... Args>
constexpr bool operator==(const optional_ref_variant<Args...>& lhs,
                          const optional_ref_variant<Args...>& rhs);

template <class... Args>
constexpr auto operator<=>(const optional_ref_variant<Args...>& lhs,
                           const optional_ref_variant<Args...>& rhs)
    /* -> common category */;

// ---- Free functions ----

template <class T, class... Args>
constexpr auto holds_alternative(const optional_ref_variant<Args...>& v) -> bool;

template <class T, class... Args>
constexpr auto get(const optional_ref_variant<Args...>& v) -> auto&;

template <size_t I, class... Args>
constexpr auto get(const optional_ref_variant<Args...>& v) -> auto&;

template <class T, class... Args>
constexpr auto get_if(const optional_ref_variant<Args...>& v) -> auto*;

template <size_t I, class... Args>
constexpr auto get_if(const optional_ref_variant<Args...>& v) -> auto*;

}  // namespace rbox
```
