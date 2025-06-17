## Type Category Concepts

Defined in header `<rbox/utils/concepts.hpp>`.

Provides shorthand concepts mirroring `std::is_*` type traits, plus additional composite concepts.

```cpp
namespace rbox {

// Shorthand concepts for std::is_* type traits
template <class T> concept abstract_type = std::is_abstract_v<T>;
template <class T> concept aggregate_type = std::is_aggregate_v<T>;
template <class T> concept arithmetic_type = std::is_arithmetic_v<T>;
template <class T> concept array_type = std::is_array_v<T>;
template <class T> concept bounded_array_type = std::is_bounded_array_v<T>;
template <class T> concept class_type = std::is_class_v<T>;
template <class T> concept compound_type = std::is_compound_v<T>;
template <class T> concept empty_type = std::is_empty_v<T>;
template <class T> concept enum_type = std::is_enum_v<T>;
template <class T> concept final_type = std::is_final_v<T>;
template <class T> concept function_type = std::is_function_v<T>;
template <class T> concept fundamental_type = std::is_fundamental_v<T>;
template <class T> concept implicit_lifetime_type = std::is_implicit_lifetime_v<T>;
template <class T> concept lvalue_reference_type = std::is_lvalue_reference_v<T>;
template <class T> concept member_function_pointer_type = std::is_member_function_pointer_v<T>;
template <class T> concept member_object_pointer_type = std::is_member_object_pointer_v<T>;
template <class T> concept member_pointer_type = std::is_member_pointer_v<T>;
template <class T> concept object_type = std::is_object_v<T>;
template <class T> concept pointer_type = std::is_pointer_v<T>;
template <class T> concept polymorphic_type = std::is_polymorphic_v<T>;
template <class T> concept reference_type = std::is_reference_v<T>;
template <class T> concept rvalue_reference_type = std::is_rvalue_reference_v<T>;
template <class T> concept scalar_type = std::is_scalar_v<T>;
template <class T> concept scoped_enum_type = std::is_scoped_enum_v<T>;
template <class T> concept structural_type = std::is_structural_v<T>;
template <class T> concept unbounded_array_type = std::is_unbounded_array_v<T>;
template <class T> concept union_type = std::is_union_v<T>;

// Negated variants — some traits are intentionally omitted due to potential ambiguity.
template <class T> concept non_abstract_type = !std::is_abstract_v<T>;
template <class T> concept non_aggregate_type = !std::is_aggregate_v<T>;
template <class T> concept non_arithmetic_type = !std::is_arithmetic_v<T>;
template <class T> concept non_array_type = !std::is_array_v<T>;
template <class T> concept non_class_type = !std::is_class_v<T>;
template <class T> concept non_compound_type = !std::is_compound_v<T>;
template <class T> concept non_empty_type = !std::is_empty_v<T>;
template <class T> concept non_enum_type = !std::is_enum_v<T>;
template <class T> concept non_final_type = !std::is_final_v<T>;
template <class T> concept non_function_type = !std::is_function_v<T>;
template <class T> concept non_fundamental_type = !std::is_fundamental_v<T>;
template <class T> concept non_object_type = !std::is_object_v<T>;
template <class T> concept non_pointer_type = !std::is_pointer_v<T>;
template <class T> concept non_polymorphic_type = !std::is_polymorphic_v<T>;
template <class T> concept non_reference_type = !std::is_reference_v<T>;
template <class T> concept non_scalar_type = !std::is_scalar_v<T>;
template <class T> concept non_structural_type = !std::is_structural_v<T>;
template <class T> concept non_union_type = !std::is_union_v<T>;

// Composite concepts
template <class T>
concept class_or_union_type = std::is_class_v<T> || std::is_union_v<T>;

template <class T>
concept integral_or_enum_type = std::is_integral_v<T> || std::is_enum_v<T>;

}  // namespace rbox
```
