## Wrappers of `<meta>` Components

Defined in header `<rbox/utils/meta_utility.hpp>`.

```cpp
namespace rbox {

// Access context helpers
consteval auto unprivileged_context();
consteval auto unchecked_context();

// Member query functions
consteval auto public_direct_members_of(std::meta::info a) -> std::vector<std::meta::info>;
consteval auto all_direct_members_of(std::meta::info a) -> std::vector<std::meta::info>;

consteval auto public_direct_bases_of(std::meta::info a) -> std::vector<std::meta::info>;
consteval auto all_direct_bases_of(std::meta::info a) -> std::vector<std::meta::info>;

consteval auto public_direct_static_data_members_of(std::meta::info a)
    -> std::vector<std::meta::info>;
consteval auto all_direct_static_data_members_of(std::meta::info a)
    -> std::vector<std::meta::info>;

consteval auto public_direct_nonstatic_data_members_of(std::meta::info a)
    -> std::vector<std::meta::info>;
consteval auto all_direct_nonstatic_data_members_of(std::meta::info a)
    -> std::vector<std::meta::info>;

// Member query values
template <class_or_union_type T>
constexpr std::span<const std::meta::info> public_direct_members_v = /* ... */;
template <class_or_union_type T>
constexpr std::span<const std::meta::info> all_direct_members_v = /* ... */;

template <class_or_union_type T>
constexpr std::span<const std::meta::info> public_direct_bases_v = /* ... */;
template <class_or_union_type T>
constexpr std::span<const std::meta::info> all_direct_bases_v = /* ... */;

template <class_or_union_type T>
constexpr std::span<const std::meta::info> public_direct_static_data_members_v = /* ... */;
template <class_or_union_type T>
constexpr std::span<const std::meta::info> all_direct_static_data_members_v = /* ... */;

template <class_or_union_type T>
constexpr std::span<const std::meta::info> public_direct_nonstatic_data_members_v = /* ... */;
template <class_or_union_type T>
constexpr std::span<const std::meta::info> all_direct_nonstatic_data_members_v = /* ... */;

// Extraction helper
template <class T, std::same_as<std::meta::info>... Args>
consteval T extract(std::meta::info templ, Args... templ_params);

}  // namespace rbox
```

### Access Contexts

These functions are wrappers of special access contexts:

- `unprivileged_context()`: Equivalent to `std::meta::access_context::unprivileged()`.
- `unchecked_context()`: Equivalent to `std::meta::access_context::unchecked()`.

### Member Query Functions

These functions query members of a type with different access levels:

- `public_direct_*_of(a)`: Equivalent to `std::meta::*_of(a, unprivileged_context())`.
- `all_direct_*_of(a)`: Equivalent to `std::meta::*_of(a, unchecked_context())`.

The `*` can be one of: `members`, `bases`, `static_data_members`, `nonstatic_data_members`.

### Member Query Values

These variable templates store the results of member queries as static arrays:

- `public_direct_*_v<T>`: Equivalent to `std::define_static_array(public_direct_*_of(^^T))`.
- `all_direct_*_v<T>`: Equivalent to `std::define_static_array(all_direct_*_of(^^T))`.

Example:

```cpp
struct MyClass { /*...*/ };

// Before: More typing required
constexpr auto members = std::define_static_array(
    nonstatic_data_members_of(^^MyClass, std::meta::access_context::unprivileged()));
template for (constexpr auto m : members) { /*...*/}
// After:
template for (constexpr auto m : rbox::public_direct_nonstatic_data_members_v<MyClass>) { /*...*/ }
```

### Extraction Helper

`extract<T>(templ, params...)`: Equivalent to `extract<T>(substitute(templ, {params...}))`.

Example:

```cpp
constexpr auto T = substitute(^^std::vector, {^^int});
// Equivalent to extract<bool>(substitute(^^std::ranges::range, {T}))
static_assert(rbox::extract<bool>(^^std::ranges::range, T));
```
