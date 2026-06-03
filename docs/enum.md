## Introduction

`rbox` provides common functionality for enum types, including but not limited to:

- enum metadata: `enum_entries`, `enum_names`, `enum_values`, `enum_type_name`, etc.;
- conversion between enum values and strings: `enum_name`, `enum_cast`, etc.;
- enum flag value decomposition and string parsing: `enum_flags_name`, `enum_flags_cast`, etc.;
- `std::format` and `fmt::format` support;
- operator overloading support (including bitwise and comparison operators).

Part of the API design resembles [magic_enum](https://github.com/Neargye/magic_enum), a well-known enum static reflection library based on compiler magic before C++26.

## Components

### Enum Metadata

* [enum_type_name](enum/enum_type_name.md): Gets the type name (dealiased, cv-qualifiers discarded) or display string of an enum type.
* [enum_entries](enum/enum_entries.md): Gets the list of (value, name) pairs of an enum type.
* [enum_names](enum/enum_names.md): Gets the name list of an enum type; checks whether all names are ASCII-only.
* [enum_values](enum/enum_values.md): Gets the value list of an enum type.
* [enum_meta_entries](enum/enum_meta_entries.md): Gets the reflector list (`std::meta::info`) of entries in an enum type.
* [enum_hash](enum/enum_hash.md): Gets the 64-bit hash value of an enum type for change detection.
* [enum_json](enum/enum_json.md): Dumps the enum entry set as a JSON string at compile-time or runtime.

### Enum Value Operations

* [enum_name](enum/enum_name.md): Converts an enum value to its string name.
* [enum_cast](enum/enum_cast.md): Converts a string or integer to an enum value; case-insensitive variant provided.
* [enum_contains](enum/enum_contains.md): Checks whether a given value, integer, or string exists in the enum type.
* [enum_index](enum/enum_index.md): Gets the 0-based index of an enum value by a given ordering; unique-index variant provided.

### Enum Flag Operations

* [enum_flags_name](enum/enum_flags_name.md): Converts an enum flag value to its string representation.
* [enum_flags_cast](enum/enum_flags_cast.md): Converts a flag string or integer to an enum flag value; case-insensitive variant provided.
* [enum_flags_contains](enum/enum_flags_contains.md): Checks whether a flag string or value can be decomposed into a disjunction of defined enum entries.

### Switch-Case & Formatting

* [enum_switch](enum/enum_switch.md): Compile-time switch-case over all enum values, with fallback and return-value support.
* [enum_format](enum/enum_format.md): `std::format` and `fmt::format` support for enum types.

## Common Components

### `enum_entry_order`

Defined in header `<rbox/enum/enum_entry_order.hpp>` (implicitly included by its users).

```cpp
namespace rbox {

enum class enum_entry_order {
    original,
    by_value,
    by_name,
};

}  // namespace rbox
```

`enum_entry_order` is used for customized ordering when accessing entry list of enum type:

- `original`: Keeps the order of enum definition (which is the default option);
- `by_value`: Sorts enum entries by their underlying values in ascending order. For multiple entries with the same underlying value, their original order is preserved;
- `by_name`: Sorts enum entries by their names, case-sensitive in ascending order.

### `enum_count`

Defined in header `<rbox/enum/enum_count.hpp>`.

```cpp
namespace rbox {

template <enum_type E>
constexpr size_t enum_count_v;

}  // namespace rbox
```

`enum_count_v<E>` gets the number of entries in the given enum type.

### Enum Bitwise Operators

Defined in header `<rbox/enum/enum_bitwise_operators.hpp>`.

- (1) The macro `RBOX_DEFINE_ENUM_BITWISE_BINARY_OPERATORS(E)` defines overloads of the following operators for enum type `E`:
  - `operator&` and `operator&=`;
  - `operator|` and `operator|=`;
  - `operator^` and `operator^=`.
- (2) The macro `RBOX_DEFINE_ENUM_BITWISE_OPERATORS(E)` defines overloads of `operator~` along with all the binary operators in (1) for enum type `E`;
- (3) `using namespace rbox::enum_bitwise_operators` enables all the bitwise operators in (2) for enum types in the `using` scope.

### Enum Comparison Operators

Defined in header `<rbox/enum/enum_comparison_operators.hpp>`.

- (1) The macro `RBOX_DEFINE_ENUM_COMPARISON_OPERATORS(E)` defines overloads of `operator<=>` and `operator==` for enum type `E` by its underlying value;
- (2) `using namespace rbox::enum_comparison_operators` enables `operator<=>` and `operator==` for enum types in the `using` scope.
