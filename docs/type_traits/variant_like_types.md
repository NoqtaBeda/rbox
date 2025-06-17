## Variant-like Types

Defined in header `<rbox/type_traits/variant_like_types.hpp>`.

```cpp
namespace rbox {

template <class T>
concept variant_like = /* ... */;

}  // namespace rbox
```

The concept `variant_like` tests whether `T` is a (maybe cv-qualified) variant-like type. Variant-like types are those which satisfy all the requirements below:
- `std::variant_size` and `std::variant_alternative` specializations are provided
- For each alternative type `A`, either `holds_alternative<A>(v)` and `get<A>(v)` (ADL free functions) or `v.template holds_alternative<A>()` and `v.template get<A>()` (member functions) are well-defined for all cvref-qualified combinations.

This includes but is not limited to:
- `std::variant`;
- `meta_variant<Args...>` (defined in `<rbox/utils/meta_variant.hpp>`, a [structural](https://en.cppreference.com/w/cpp/language/template_parameters) replacement of `std::variant`);
- Custom variant-like types that satisfy the variant interface.

See [unit test](../../tests/type_traits/test_variant_like_types.cpp) for examples and details.
