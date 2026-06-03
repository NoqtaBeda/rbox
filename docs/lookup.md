## Introduction

rbox provides functionality to generate compile-time lookup table for class or namespace members. The lookup table is a fixed map whose keys are of either integral, enum or string type, and values are either pointers (to member variables), function pointers or member function pointers.

Lookup table utilizes [compile-time fixed map](./fixed_map.md) as underlying data structure, which automatically selects the most suitable data structure (hash table, binary search, etc.). Read the [fixed map documentation](./fixed_map.md#candidate-data-structures) for its implementation details.

The primary usage is to replace hand-written dispatching patterns of selecting candidate member or function, which is lengthy, inefficient and error-prone. The following example shows how rbox library helps to simplify dispatching with an auto-generated lookup table:

> Even in LLM era, rbox has its unique advantage that an efficient data structure can be applied automatically with your code kept clean. You don't have to worry that (1) you forget to instruct the agent to update all related components; (2) the agent generates flawed code for several items under illusion which is hard to be detected by human reviewing (especially when the number of items or related components is large); (3) the agent either fails to pick some good data structure, or generates a huge block of boilerplate code which harms the overall readability.

```cpp
namespace binary_ops {
int do_add(int x, int y);
int do_sub(int x, int y);
int do_mul(int x, int y);
int do_div(int x, int y);
// A lot more ...
}  // namespace binary_ops

// ⚠️ You may forget to keep consistency with namespace binary_ops
int do_binary_op_before(std::string_view key, int x, int y)
{
    // Complexity: O(N) times of string comparison.
    if (key == "add") {
        return binary_ops::do_add(x, y);
    } else if (key == "sub") {
        return binary_ops::do_sub(x, y);
    } else if (key == "nul") {
        return binary_ops::do_mul(x, y);
    }  // ⚠️ TYPO, maybe hard to debug
    else if (key == "div") {
        return binary_ops::do_div(x, y);
    }
    // A lot more ...
    return 0;  // On mismatch
}

int do_binary_op_after(std::string_view key, int x, int y)
{
    // Complexity: Depends on the underlying data structure. Typically better than O(N).
    constexpr auto dispatch_table = RBOX_FUNCTION_FIXED_MAP(binary_ops, "do_*");
    if (auto fptr = dispatch_table[key]; fptr != nullptr) {
        return fptr(x, y);
    }
    return 0;  // On mismatch
}
```

Components of lookup table are defined in header `<rbox/lookup.hpp>`.

## Quick Start

More examples can be found in examples/lookup folder.

```cpp
#include <print>
#include <rbox/lookup.hpp>

// Parent namespaces are ignored.
int g_ignored = 0;

namespace stats {
// Identifier "g_call_count" matches the pattern "g_*_count".
// '*' -> "call".
int g_call_count = 0;
// '*' -> "gc"
int g_gc_count = 0;
// '*' -> "throw"
int g_throw_count = 0;
// '*' -> "catch"
int g_catch_count = 0;

// Nested namespaces are ignored as well.
namespace verbose {
int g_jump_count = 0;
int g_load_field_count = 0;
int g_store_field_count = 0;
}  // namespace verbose

// For the function itself where the table is built:
// Identifier "increment_counter" does not match the pattern "g_*_count". Ignored.
bool increment_counter(std::string_view event_name, int n = 1)
{
    // How does it work:
    // For each addressable member whose identifier matches pattern "g_*_count",
    // the matched part is key and pointer to the member is value.
    constexpr auto table = RBOX_VARIABLE_FIXED_MAP(
        stats,       // 1st arg: The namespace whose direct members are to be traversed
        "g_*_count"  // 2nd arg: The name pattern
        // 3rd arg (optional): Configuation of the underlying fixed map.
    );
    // Contents in table:
    //   ("call", &g_call_count)
    //   ("gc", &g_gc_count)
    //   ("throw", &g_throw_count)
    //   ("catch", &g_catch_count)

    // Use decltype(table)::value_type to get the value type.
    static_assert(std::is_same_v<int*, decltype(table)::value_type>);
    // Use size() member function to get the size, i.e. how many entries in the table.
    static_assert(table.size() == 4);

    // Use operator[] to access the value, i.e. pointer to the variables.
    if (int* p = table[event_name]; p != nullptr) {
        // p != nullptr: event_name is a key in table.
        *p += n;
        return true;
    } else {
        // p == nullptr: event_name is not a key in table.
        std::println("WARN: Ignores unknown event '{}'.", event_name);
        return false;
    }
}
}  // namespace stats

int main()
{
    stats::increment_counter("call");
    stats::increment_counter("gc", 2);
    stats::increment_counter("load_field");

    std::println("Function call: {} time(s).", stats::g_call_count);
    std::println("GC: {} time(s).", stats::g_gc_count);
    return 0;
}

// Expected output:
// WARN: Ignores unknown event 'load_field'.
// Function call: 1 time(s).
// GC: 2 time(s).
```

## Description

The lookup API consists of a family of `consteval` functions and their companion macros. Each function target is specialized to a specific member category, producing a fixed map whose value type is a pointer or member pointer to that category. The functions and macros are defined in header `<rbox/lookup.hpp>`.

### API Macros

The public interface is a set of macros. Each macro wraps a `make_*_fixed_map()` function: the `consteval` function returns a `std::meta::info` reflector of the generated lookup table, and the macro extracts the concrete object via the splice operator `[: ... :]`.

**Class members**:

| Macro (Common prefix `RBOX_` omitted)                                                                   | Wrapped function                                                                                  | Category                    |
| ------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | --------------------------- |
| `PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(T, ...)`                                                        | `make_public_nonstatic_data_member_fixed_map`                                                     | Non-static data members     |
| `NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...)` <br> `PUBLIC_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...)` | `make_nonstatic_member_function_fixed_map` <br> `make_public_nonstatic_member_function_fixed_map` | Non-static member functions |
| `STATIC_DATA_MEMBER_FIXED_MAP(T, ...)` <br> `PUBLIC_STATIC_DATA_MEMBER_FIXED_MAP(T, ...)`               | `make_static_data_member_fixed_map` <br> `make_public_static_data_member_fixed_map`               | Static data members         |
| `STATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...)` <br> `PUBLIC_STATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...)`       | `make_static_member_function_fixed_map` <br> `make_public_static_member_function_fixed_map`       | Static member functions     |

**Namespace members**:

| Macro (Common prefix `RBOX_` omitted) | Wrapped function          | Category  |
| ------------------------------------- | ------------------------- | --------- |
| `VARIABLE_FIXED_MAP(ns, ...)`         | `make_variable_fixed_map` | Variables |
| `FUNCTION_FIXED_MAP(ns, ...)`         | `make_function_fixed_map` | Functions |

**Notes**:

- Class non-static data members only have public API due to a GCC 16.1 reflection limitation (confirmed to be a GCC bug. See [demo code](./code/meta_bug_1.cpp) for details).
- The default access context is `RBOX_CURRENT_CONTEXT` (see [Access Contexts](./utils/meta_utility.md#access-contexts)), which is equivalent to `std::meta::access_context::current()`.

### Rules of Member Traversal

Class member lookup traverses **both direct and inherited** members of the given class recursively. Nested class members are not included.

Namespace member lookup traverses **direct** members of the given namespace only. Nested namespace members and parent namespace members are not included.

### Value Types

The value type of the generated lookup table is automatically determined:

- If all matched members share **exactly** the same type, the value type is a plain pointer or member-pointer to that common type.
- If members have different types (including difference on qualifiers), the value type is `meta_variant<Ptr1, Ptr2, ...>`, i.e. a structural variant of the distinct pointer types. Use `.visit()` to dispatch on the variant.

Specifically:

| Category                   | Mapped value type                     |
| -------------------------- | ------------------------------------- |
| Non-static data member     | `M T::*`                              |
| Non-static member function | `R (T::*)(Args...) [cvref][noexcept]` |
| Static data member         | `M*`                                  |
| Static member function     | `R (*)(Args...) [noexcept]`           |
| Namespace variable         | `M*`                                  |
| Namespace function         | `R (*)(Args...) [noexcept]`           |

### Common Constraints

All lookup functions share these constraints:

- Members must be **addressable** (see [utils: "Testing Addressable Members"](./utils.md#testing-addressable-members)).
- **Reference-typed members are excluded.**
- **Deleted functions are excluded.**
- **Template members are excluded.** No template instantiation will be performed.
- Members must be accessible under the given `std::meta::access_context`.
- Duplicated keys are not allowed (inherited from the [underlying fixed map](./fixed_map.md#components)).
- The generated lookup table inherits the same `operator[]`, `find()`, and `size()` interface as the [underlying fixed map](./fixed_map.md#components), including the default-value fallback for missing keys.

### Overload Groups

Each `make_*_fixed_map` function has three overloads. See section [“Components”](#components) for the complete overload set.

#### Overload (1): Pattern Only

```
make_*_fixed_map(T, pattern, options = {}, ctx = CURRENT)
make_*_fixed_map(ns, pattern, options = {})
```

- `T` or `ns`: `^^T` for a class, `^^ns` for a namespace.
- `pattern`: A `std::string_view` of form `"prefix*suffix"`. See [below](#pattern-format) for format details.
- `options`: Optional `string_key_fixed_map_options` for the underlying string-key fixed map. Default options are used if not provided.

The matched part (prefix and suffix stripped) is used as the string key.

#### Overload (2): Pattern + Transform Function

```
make_*_fixed_map(T, pattern, transform_fn, options = {}, ctx = CURRENT)
make_*_fixed_map(ns, pattern, transform_fn, options = {})
```

Same as (1), but the matched part is passed through `transform_fn` before becoming the key. The transform function has one of these call signatures:

1. `transform_fn(std::string_view matched_part) -> K` — key is `K` (integral, enum, or string-like).
2. `transform_fn(std::string_view matched_part) -> std::optional<K>` — the member is included only if a non-nullopt value is returned.

For each member visited, if it is anonymous or non-addressable, or its identifier does not match the given pattern, then `transform_fn` is not invoked for this member.

The `options` parameter matches the key type: `integral_key_fixed_map_options` for integral/enum keys, `string_key_fixed_map_options` for string keys.

#### Overload (3): Transform Function Only (No Pattern)

```
make_*_fixed_map(T, transform_fn, options = {}, ctx = CURRENT)
make_*_fixed_map(ns, transform_fn, options = {})
```

No pattern filtering — the transform function receives each candidate member's reflection (`std::meta::info`) directly. The transform function has one of these call signatures:

1. `transform_fn(std::meta::info member) -> K`
2. `transform_fn(std::meta::info member) -> std::optional<K>` — included only if non-nullopt.

For each member visited, if it is non-addressable, then `transform_fn` is not invoked for this member.

### Pattern Format

The `pattern` string must follow the format `"prefix*suffix"` where `*` appears exactly once. For a member to match:

- It must have an identifier.
- The identifier must start with `prefix` and end with `suffix`.
- The matched part (with prefix and suffix removed) is used as the key (or passed to transform function).

Examples:

- `"value_*"` matches `value_x`, `value_y`, `value_z` → keys `"x"`, `"y"`, `"z"`.
- `"get_*_squared"` matches `get_x_squared`, `get_y_squared` → keys `"x"`, `"y"`.
- `"*"` (empty prefix and suffix) tries to match every named member → key is the full identifier.
- `"get_*"` matches `get_x`, `get_y`, but does _not_ match `get` (no suffix after prefix) or `getter_x` (missing the literal `_` separator).

### meta_variant Values

When matched members have heterogeneous pointer types, the value type becomes `meta_variant<Ptr1, Ptr2, ...>`. Access the value via `.visit()`:

```cpp
constexpr auto table = RBOX_STATIC_DATA_MEMBER_FIXED_MAP(MyClass, "s_*");
// value_type = meta_variant<const int*, double*>

// Usage pattern #1:
auto visitor_1 = [](auto ptr) -> double {
    if (ptr == nullptr) return -1.0;
    return static_cast<double>(*ptr);
};
// table[key] -> value_type
// If key does not exist in the table, then table[key] is value_type{},
// which is static_cast<const int*>(nullptr), i.e. value-initialized as its 1st alternative.
auto result_1 = table["alpha"].visit(visitor_1);

// Usage pattern #2:
auto visitor_2 = [](auto ptr) -> double {
    assert(ptr != nullptr);  // The pointer (to member) can never be nullptr as long as it's found.
    return static_cast<double>(*ptr);
};
// table.find(key) -> std::optional<const value_type&>
auto result_2a = std::optional<double>{};
if (auto ptr = table.find("beta")) {
    result_2a = ptr->visit(visitor_2);
}
// Use monadic operation of std::optional to simplify the code.
auto result_2b = table.find("beta").transform([&](auto variant) {
    return variant.visit(visitor_2);
});
```

## Components

All the components shown below are defined in header `<rbox/lookup.hpp>`.

### Non-Static Data Members (Class)

Only public members can be queried (GCC 16.1 limitation).

```cpp
namespace rbox {

// (1) Pattern only
consteval auto make_public_nonstatic_data_member_fixed_map(
    std::meta::info T, std::string_view pattern, const string_key_fixed_map_options& options = {})
    -> std::meta::info;

// (2.1) Pattern + transform_fn → integral/enum key
template <ikey_pattern_transform_fn TransformFn>
consteval auto make_public_nonstatic_data_member_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (2.2) Pattern + transform_fn → string key
template <skey_pattern_transform_fn TransformFn>
consteval auto make_public_nonstatic_data_member_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.1) Transform function only (meta::info) → integral/enum key
template <ikey_member_transform_fn TransformFn>
consteval auto make_public_nonstatic_data_member_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.2) Transform function only (meta::info) → string key
template <skey_member_transform_fn TransformFn>
consteval auto make_public_nonstatic_data_member_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

}  // namespace rbox

#define RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(T, ...) \
    [:rbox::make_public_nonstatic_data_member_fixed_map(^^T, ##__VA_ARGS__):]

```
### Non-Static Member Functions (Class)

```cpp
namespace rbox {

// (1) Pattern only
consteval auto make_nonstatic_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

consteval auto make_public_nonstatic_member_function_fixed_map(
    std::meta::info T, std::string_view pattern, const string_key_fixed_map_options& options = {})
    -> std::meta::info;

// (2.1) Pattern + transform_fn → integral/enum key
template <ikey_pattern_transform_fn TransformFn>
consteval auto make_nonstatic_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <ikey_pattern_transform_fn TransformFn>
consteval auto make_public_nonstatic_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (2.2) Pattern + transform_fn → string key
template <skey_pattern_transform_fn TransformFn>
consteval auto make_nonstatic_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <skey_pattern_transform_fn TransformFn>
consteval auto make_public_nonstatic_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.1) Transform function only (meta::info) → integral/enum key
template <ikey_member_transform_fn TransformFn>
consteval auto make_nonstatic_member_function_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <ikey_member_transform_fn TransformFn>
consteval auto make_public_nonstatic_member_function_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.2) Transform function only (meta::info) → string key
template <skey_member_transform_fn TransformFn>
consteval auto make_nonstatic_member_function_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <skey_member_transform_fn TransformFn>
consteval auto make_public_nonstatic_member_function_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

}  // namespace rbox

#define RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...) \
    [:rbox::make_nonstatic_member_function_fixed_map(^^T, ##__VA_ARGS__):]

#define RBOX_PUBLIC_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...) \
    [:rbox::make_public_nonstatic_member_function_fixed_map(^^T, ##__VA_ARGS__):]

```
### Static Data Members (Class)

```cpp
namespace rbox {

// (1) Pattern only
consteval auto make_static_data_member_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

consteval auto make_public_static_data_member_fixed_map(
    std::meta::info T, std::string_view pattern, const string_key_fixed_map_options& options = {})
    -> std::meta::info;

// (2.1) Pattern + transform_fn → integral/enum key
template <ikey_pattern_transform_fn TransformFn>
consteval auto make_static_data_member_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <ikey_pattern_transform_fn TransformFn>
consteval auto make_public_static_data_member_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (2.2) Pattern + transform_fn → string key
template <skey_pattern_transform_fn TransformFn>
consteval auto make_static_data_member_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <skey_pattern_transform_fn TransformFn>
consteval auto make_public_static_data_member_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.1) Transform function only (meta::info) → integral/enum key
template <ikey_member_transform_fn TransformFn>
consteval auto make_static_data_member_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <ikey_member_transform_fn TransformFn>
consteval auto make_public_static_data_member_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.2) Transform function only (meta::info) → string key
template <skey_member_transform_fn TransformFn>
consteval auto make_static_data_member_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <skey_member_transform_fn TransformFn>
consteval auto make_public_static_data_member_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

}  // namespace rbox

#define RBOX_STATIC_DATA_MEMBER_FIXED_MAP(T, ...) \
    [:rbox::make_static_data_member_fixed_map(^^T, ##__VA_ARGS__):]

#define RBOX_PUBLIC_STATIC_DATA_MEMBER_FIXED_MAP(T, ...) \
    [:rbox::make_public_static_data_member_fixed_map(^^T, ##__VA_ARGS__):]

```
### Static Member Functions (Class)

```cpp
namespace rbox {

// (1) Pattern only
consteval auto make_static_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

consteval auto make_public_static_member_function_fixed_map(
    std::meta::info T, std::string_view pattern, const string_key_fixed_map_options& options = {})
    -> std::meta::info;

// (2.1) Pattern + transform_fn → integral/enum key
template <ikey_pattern_transform_fn TransformFn>
consteval auto make_static_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <ikey_pattern_transform_fn TransformFn>
consteval auto make_public_static_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (2.2) Pattern + transform_fn → string key
template <skey_pattern_transform_fn TransformFn>
consteval auto make_static_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <skey_pattern_transform_fn TransformFn>
consteval auto make_public_static_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.1) Transform function only (meta::info) → integral/enum key
template <ikey_member_transform_fn TransformFn>
consteval auto make_static_member_function_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <ikey_member_transform_fn TransformFn>
consteval auto make_public_static_member_function_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.2) Transform function only (meta::info) → string key
template <skey_member_transform_fn TransformFn>
consteval auto make_static_member_function_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info;

template <skey_member_transform_fn TransformFn>
consteval auto make_public_static_member_function_fixed_map(
    std::meta::info T,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

}  // namespace rbox

#define RBOX_STATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...) \
    [:rbox::make_static_member_function_fixed_map(^^T, ##__VA_ARGS__):]

#define RBOX_PUBLIC_STATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...) \
    [:rbox::make_public_static_member_function_fixed_map(^^T, ##__VA_ARGS__):]

```
### Namespace Members

Two functions cover all namespace member categories:

```cpp
namespace rbox {

// ===== Variables =====

// (1) Pattern only
consteval auto make_variable_fixed_map(
    std::meta::info ns, std::string_view pattern, const string_key_fixed_map_options& options = {})
    -> std::meta::info;

// (2.1) Pattern + transform_fn → integral/enum key
template <ikey_pattern_transform_fn TransformFn>
consteval auto make_variable_fixed_map(
    std::meta::info ns,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (2.2) Pattern + transform_fn → string key
template <skey_pattern_transform_fn TransformFn>
consteval auto make_variable_fixed_map(
    std::meta::info ns,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.1) Transform function only (meta::info) → integral/enum key
template <ikey_member_transform_fn TransformFn>
consteval auto make_variable_fixed_map(
    std::meta::info ns,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.2) Transform function only (meta::info) → string key
template <skey_member_transform_fn TransformFn>
consteval auto make_variable_fixed_map(
    std::meta::info ns,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

// ===== Functions =====

// (1) Pattern only
consteval auto make_function_fixed_map(
    std::meta::info ns, std::string_view pattern, const string_key_fixed_map_options& options = {})
    -> std::meta::info;

// (2.1) Pattern + transform_fn → integral/enum key
template <ikey_pattern_transform_fn TransformFn>
consteval auto make_function_fixed_map(
    std::meta::info ns,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (2.2) Pattern + transform_fn → string key
template <skey_pattern_transform_fn TransformFn>
consteval auto make_function_fixed_map(
    std::meta::info ns,
    std::string_view pattern,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.1) Transform function only (meta::info) → integral/enum key
template <ikey_member_transform_fn TransformFn>
consteval auto make_function_fixed_map(
    std::meta::info ns,
    const TransformFn& transform_fn,
    const integral_key_fixed_map_options& options = {}) -> std::meta::info;

// (3.2) Transform function only (meta::info) → string key
template <skey_member_transform_fn TransformFn>
consteval auto make_function_fixed_map(
    std::meta::info ns,
    const TransformFn& transform_fn,
    const string_key_fixed_map_options& options = {}) -> std::meta::info;

}  // namespace rbox

#define RBOX_VARIABLE_FIXED_MAP(ns, ...) [:rbox::make_variable_fixed_map(^^ns, ##__VA_ARGS__):]

#define RBOX_FUNCTION_FIXED_MAP(ns, ...) [:rbox::make_function_fixed_map(^^ns, ##__VA_ARGS__):]

```
