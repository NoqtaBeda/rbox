## Advanced Invocability Testing

Defined in header `<rbox/type_traits/is_invocable.hpp>`.

```cpp
namespace rbox {

// ---- Group 1.1: Exact result type testing ----

template <class R, class Func, class... Args>
constexpr auto is_invocable_exactly_r_v = /* ... */;

template <class R, class Func, class... Args>
constexpr auto is_nothrow_invocable_exactly_r_v = /* ... */;

// ---- Group 1.2: Repetitive argument type testing ----

template <class Func, class Arg, size_t N>
constexpr auto is_invocable_n_v = /* ... */;

template <class Func, class Arg, size_t N>
constexpr auto is_nothrow_invocable_n_v = /* ... */;

template <class R, class Func, class Arg, size_t N>
constexpr auto is_invocable_r_n_v = /* ... */;

template <class R, class Func, class Arg, size_t N>
constexpr auto is_nothrow_invocable_r_n_v = /* ... */;

template <class R, class Func, class Arg, size_t N>
constexpr auto is_invocable_exactly_r_n_v = /* ... */;

template <class R, class Func, class Arg, size_t N>
constexpr auto is_nothrow_invocable_exactly_r_n_v = /* ... */;

// ---- Group 2.1: Common testing ----

template <class F, class... Args>
concept invocable = /* ... */;

template <class F, class... Args>
concept nothrow_invocable = /* ... */;

template <class F, class R, class... Args>
concept invocable_r = /* ... */;

template <class F, class R, class... Args>
concept nothrow_invocable_r = /* ... */;

// ---- Group 2.2: Exact result type testing ----

template <class F, class R, class... Args>
concept invocable_exactly_r = /* ... */;

template <class F, class R, class... Args>
concept nothrow_invocable_exactly_r = /* ... */;

// ---- Group 2.3: Repetitive argument type testing ----

template <class F, class Arg, size_t N>
concept invocable_n = /* ... */;

template <class F, class Arg, size_t N>
concept nothrow_invocable_n = /* ... */;

template <class F, class R, class Arg, size_t N>
concept invocable_r_n = /* ... */;

template <class F, class R, class Arg, size_t N>
concept nothrow_invocable_r_n = /* ... */;

template <class F, class R, class Arg, size_t N>
concept invocable_exactly_r_n = /* ... */;

template <class F, class R, class Arg, size_t N>
concept nothrow_invocable_exactly_r_n = /* ... */;

}  // namespace rbox
```

Various concepts are provided for testing callable types:

- `invocable` - checks whether `F` is invocable with `Args...`;
- `invocable_r` - checks whether `F` is invocable with `Args...` and the result is convertible to `R`;
- `invocable_exactly_r` - checks whether `F` is invocable with `Args...` and the result type is _exactly_ `R` (no implicit conversion allowed). This is a stronger constraint than `invocable_r` when implicit conversion might cause ambiguity;
- `invocable_n` - checks whether `F` is invocable with `N` arguments of type `Arg`;
- `invocable_r_n` - checks whether `F` is invocable with `N` arguments of type `Arg` and the result is convertible to `R`;
- `invocable_exactly_r_n` - checks whether `F` is invocable with `N` arguments of type `Arg` and the result type is _exactly_ `R`.

For each concept listed above, an additional `nothrow_*` variant above is provided which tests whether `F` is nothrow-invocable, i.e. the `operator()` is `noexcept`-qualified.

Each concept `name` is equivalent to the corresponding variable template `is_name_v`:

| Concept                                                                                 | Equivalent Variable Template                                                                      |
| --------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- |
| `invocable<F, Args...>`<br/>`nothrow_invocable<F, Args...>`                             | `std::is_invocable_v<F, Args...>`<br/>`std::is_nothrow_invocable_v<F, Args...>`                   |
| `invocable_r<F, R, Args...>`<br/>`nothrow_invocable_r<F, R, Args...>`                   | `std::is_invocable_r_v<R, F, Args...>`<br/>`std::is_nothrow_invocable_r_v<R, F, Args...>`         |
| `invocable_exactly_r<F, R, Args...>`<br/>`nothrow_invocable_exactly_r<F, R, Args...>`   | `is_invocable_exactly_r_v<R, F, Args...>`<br/>`is_nothrow_invocable_exactly_r_v<R, F, Args...>`   |
| `invocable_n<F, Arg, N>`<br/>`nothrow_invocable_n<F, Arg, N>`                           | `is_invocable_n_v<F, Arg, N>`<br/>`is_nothrow_invocable_n_v<F, Arg, N>`                           |
| `invocable_r_n<F, R, Arg, N>`<br/>`nothrow_invocable_r_n<F, R, Arg, N>`                 | `is_invocable_r_n_v<R, F, Arg, N>`<br/>`is_nothrow_invocable_r_n_v<R, F, Arg, N>`                 |
| `invocable_exactly_r_n<F, R, Arg, N>`<br/>`nothrow_invocable_exactly_r_n<F, R, Arg, N>` | `is_invocable_exactly_r_n_v<R, F, Arg, N>`<br/>`is_nothrow_invocable_exactly_r_n_v<R, F, Arg, N>` |

Example:

```cpp
auto add = [](int a, int b) noexcept -> int { return a + b; };
auto mul = [](int a, int b) -> long { return static_cast<long>(a) * b; };

// Exact return type matching
static_assert(rbox::invocable_exactly_r<decltype(add), int, int, int>);
static_assert(rbox::nothrow_invocable_exactly_r<decltype(add), int, int, int>);
// mul returns long, not int — invocable_r passes (convertible), invocable_exactly_r fails
static_assert(rbox::invocable_r<decltype(mul), int, int, int>);
static_assert(!rbox::invocable_exactly_r<decltype(mul), int, int, int>);

// Repetitive argument invocation
static_assert(rbox::invocable_n<decltype(add), int, 2>);
static_assert(rbox::nothrow_invocable_n<decltype(add), int, 2>);
static_assert(rbox::invocable_exactly_r_n<decltype(add), int, int, 2>);

static_assert(rbox::invocable_r_n<decltype(mul), int, int, 2>);
// Return type mismatch: long vs. int
static_assert(!rbox::invocable_exactly_r_n<decltype(mul), int, int, 2>);
```
