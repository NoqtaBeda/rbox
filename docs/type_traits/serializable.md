## Serializable Types

Defined in header `<rbox/type_traits/serializable_types.hpp>`.

```cpp
namespace rbox {

template <class T>
concept serializable = /* ... */;

}  // namespace rbox
```

These concepts test whether types can be serialized (converted to/from a portable format).

The concept `serializable<T>` tests whether `std::remove_cv_t<T>` can be serialized in a memberwise manner _recursively_. A type is memberwise serializable if it satisfies one of the following:

- **`std::monostate`**: serves as a null placeholder;
- **Arithmetic types**: all integral types (including `bool` and character types) and floating-point types;
- **Enumeration types**: all enum types;
- **String-like types**: types that satisfy the `string_like` concept (see [String-like Types](string_like_types.md));
- **Array types**: bounded and unbounded arrays whose element type is `serializable`;
- **Range types**: types satisfying `std::ranges::range` whose `value_type` is `serializable`;
- **Tuple-like types**: types satisfying `tuple_like` whose all element types are `serializable`;
- **`std::optional<T>`**: if `T` is `serializable`;
- **`std::variant<Ts...>`**: if all alternative types `Ts...` are `serializable`.
- **Memberwise serializable class types**: It is a `flattenable_class` and:
  - all of its non-static data members (including inherited ones) are `serializable`;
  - no duplicated field name after flattening.

Note: pointers and references are NOT `serializable`.

See [unit test](../../tests/type_traits/test_serializable_types.cpp) for examples and details.
