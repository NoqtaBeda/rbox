## Qualifier Manipulation

Defined in header `<rbox/type_traits/cvref.hpp>`.

```cpp
namespace rbox {

consteval auto add_cv_like(std::meta::info dest, std::meta::info src) -> std::meta::info;

consteval auto add_cvref_like(std::meta::info dest, std::meta::info src) -> std::meta::info;

consteval auto propagate_cv_like(std::meta::info dest, std::meta::info src) -> std::meta::info;

template <non_reference_type Dest, class Src>
using add_cv_like_t = [:add_cv_like(^^Dest, ^^Src):];

template <non_reference_type Dest, class Src>
using add_cvref_like_t = [:add_cvref_like(^^Dest, ^^Src):];

template <class Dest, class Src>
using propagate_cv_like_t = [:propagate_cv_like(^^Dest, ^^Src):];

}  // namespace rbox
```

The function `add_cv_like()` takes 2 reflections designating types `dest` and `src` as arguments. The function adds const-qualifier to `dest` if `remove_reference(src)` is const-qualified, and adds volatile-qualifier to `dest` if `remove_reference(src)` is volatile-qualified. Compilation error is raised on either of the following:

- `dest` or `src` is not a reflection designating some type;
- `dest` is reference-qualified.

The function `add_cvref_like()` takes 2 reflections designating types `dest` and `src` as arguments. The function adds const-qualifier to `dest` if `remove_reference(src)` is const-qualified, and adds volatile-qualifier to `dest` if `remove_reference(src)` is volatile-qualified, and finally adds lvalue-reference or rvalue-reference qualifier to `dest` if `src` is lvalue-reference or rvalue-reference. Compilation error is raised on either of the following:

- `dest` or `src` is not a reflection designating some type;
- `dest` is reference-qualified.

The function `propagate_cv_like()` takes 2 reflections designating types `dest` and `src` as arguments.

- If `dest` reflects a pointer type, the function propagates cv-qualifiers of `remove_reference(src)` to the pointee type of `dest` _recursively_, then adds cv-qualifiers of `remove_reference(src)` to `dest` itself;
- If `dest` reflects a reference type, the function propagates cv-qualifiers of `remove_reference(src)` to `remove_reference(dest)`, then adds the reference-qualifier of `dest` back;
- Otherwise, `propagate_cv_like()` is equivalent to `add_cv_like()`.
  Compilation error is raised if `dest` or `src` is not a reflection designating some type.

The type aliases `add_cv_like_t`, `add_cvref_like_t` and `propagate_cv_like_t` are template-based interfaces to query the type conversion result.

Example (see [unit test](../../tests/type_traits/test_cvref.cpp) for more examples):

```cpp


// Adds const-qualifier to T = int*
static_assert(std::is_same_v<
              rbox::add_cv_like_t<int*, const double&>,
              int* const>);  // const T = int* const

// Adds volatile-qualifier to T = const int*
static_assert(std::is_same_v<
              rbox::add_cv_like_t<const int*, volatile double&&>,
              const int* volatile>);  // volatile T = const int* volatile

// Adds volatile-qualifier to T = int* const
static_assert(std::is_same_v<
              rbox::add_cv_like_t<int* const, volatile double&&>,
              int* const volatile>);  // volatile T = int* const volatile

// Adds const-qualifier and lvalue-reference qualifier to T = int*
static_assert(std::is_same_v<
              rbox::add_cvref_like_t<int*, const double&>,
              int* const&>);  // const T& = int* const &

// Adds volatile-qualifier and rvalue-reference qualifier to T = const int*
static_assert(std::is_same_v<
              rbox::add_cvref_like_t<const int*, volatile double&&>,
              const int* volatile&&>);  // volatile T&& = const int* volatile &&

// Propagates volatile-qualifier to every level, including the pointer type itself.
// For a k-level pointer, the cv-qualifiers are applied (k+1) times.
static_assert(std::is_same_v<
              rbox::propagate_cv_like_t<const int***, volatile double>,
              const volatile int* volatile* volatile* volatile>);

// Propagates volatile-qualifier to the rvalue-referenced type.
// Note that the result is still a rvalue-reference.
static_assert(std::is_same_v<
              rbox::propagate_cv_like_t<const int***&&, volatile double&>,
              const volatile int* volatile* volatile* volatile&&>);
```
