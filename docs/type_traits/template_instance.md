## Template Instance Detection

Defined in header `<rbox/type_traits/template_instance.hpp>`.

```cpp
namespace rbox {

template <class T, template <class...> class Templ>
concept template_instance_of = /* ... */;

template <class T, template <auto...> class Templ>
concept nontype_template_instance_of = /* ... */;

}  // namespace rbox
```

The concept `template_instance_of<T, Templ>` tests whether `std::remove_cv_t<T>` is an instance of the given type template `Templ`.

The concept `nontype_template_instance_of<T, Templ>` tests whether `std::remove_cv_t<T>` is an instance of the given non-type template `Templ`.

> **Note:** Templates with mixed type and non-type parameters (e.g. `std::array<T, N>`) are not supported by these concepts. Use `std::meta::template_of()` for such cases.

Example:

```cpp
static_assert(rbox::template_instance_of<
    std::vector<int, std::allocator<int>>,
    std::vector>);

static_assert(rbox::nontype_template_instance_of<
    std::ratio<2, 5>,
    std::ratio>);
```
