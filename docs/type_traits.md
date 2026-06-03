## Type Traits

rbox provides functionality for advanced type traits useful in common cases, some of which could not be implemented without compiler magic before C++26.

### Core Components

* [Enum Types & Enum Flag Types](type_traits/enum_types.md): Tests whether a type is an enum, scoped enum, or enum flag type; provides `RBOX_AS_ENUM_FLAG` attribute for marking enum flags.
* [Flattenable Type](type_traits/flattenable.md): Tests whether a type can be "flattened" into an aggregate containing all non-static data members without ambiguity.
* [Serializable Types](type_traits/serializable.md): Tests whether a type can be recursively [serialized](serializer.md) in a memberwise manner.

### Type Classification

* [Arithmetic Types](type_traits/arithmetic_types.md): Classifies integral types — distinguishes character types, booleans, and integers; provides conversion from any integral to the corresponding integer type of the same size and signedness.
* [String-like Types](type_traits/string_like_types.md): Tests whether a type can be converted to `std::basic_string_view`, covering C-style strings, `std::string`, `std::string_view`, contiguous character ranges like `std::vector<char>`, etc.
* [Tuple-like Types](type_traits/tuple_like_types.md): Tests whether a type satisfies the tuple protocol (`std::tuple_size` / `std::tuple_element` / `std::get`), including `std::tuple`, `std::pair`, `std::array`, and custom tuple-like types.
* [Variant-like Types](type_traits/variant_like_types.md): Tests whether a type satisfies the variant protocol (`std::variant_size` / `std::variant_alternative` / `holds_alternative` / `get`), covering `std::variant` and custom variant-like types.

### Qualifier & Comparison

* [Qualifier Manipulation](type_traits/cvref.md): Compile-time utilities to add or propagate const, volatile, and reference qualifiers from one type to another via reflection — useful in template metaprogramming where qualifiers must be transferred between related types.
* [Type Comparison](type_traits/type_comparison.md): Extended type comparison concepts beyond `std::is_same` — exact match, one-of match, negation, and cvref-stripping variants.

### Invocability

* [Advanced Invocability Testing](type_traits/is_invocable.md): Extended invocability traits — exact return type matching (`invocable_exactly_r`), repetitive argument invocation (`invocable_n`, `invocable_r_n`), and corresponding `nothrow` variants.

### Class Type Properties

* [Class Type Property Testing](type_traits/class_types.md): Tests structural properties of class types relevant for reflection and serialization — no ambiguous base classes, no non-public non-static data members, no virtual inheritance.
* [Trivial Type](type_traits/trivial_types.md): Tests whether all six special member functions (default constructor, destructor, copy/move constructors, copy/move assignment operators) of a type are trivial — a stricter and more specific check than `std::is_trivial`.

### Template & Member Utilities

* [Template Instance Detection](type_traits/template_instance.md): Tests whether a type is an instance of a given type template or non-type template — useful for pattern matching in template metaprogramming.
* [Pointer-to-Member Utilities](type_traits/pointer_to_member.md): Convenience aliases for pointer-to-member types, plus extraction of the member type or parent class from a pointer-to-member type.
