## Introduction

`to_static_storage()` is a standalone component provided by `rbox` which promotes input to static storage — via
[`rbox::define_static_array()`](utils/define_static_array.md), [`rbox::define_static_string()`](utils/define_static_string.md), etc. — *recursively*.

Defined in header `<rbox/to_static_storage.hpp>`.

```cpp
namespace rbox {

template <class T>
concept promotable_to_static_storage = /* see below */;

struct to_static_storage_t {
    template <promotable_to_static_storage T>
    static consteval auto operator()(const T& value)
        -> to_static_storage_result_t<T>;

    template <promotable_to_static_storage T>
    static consteval auto operator()(std::initializer_list<T> values)
        -> to_static_storage_result_t<T>;
};

constexpr auto to_static_storage = to_static_storage_t{};

template <promotable_to_static_storage T>
using to_static_storage_result_t = decltype(to_static_storage(std::declval<T>()));

template <flattenable_class T>
struct structural_mirror {
    // A structural type whose members mirror the flattened non-static data members of T,
    // with each member type recursively promoted via to_static_storage_result_t.
    class type;
};

template <flattenable_class T>
using structural_mirror_t = typename structural_mirror<T>::type;

}  // namespace rbox
```

The function object `to_static_storage` converts input to a compile-time static constant with
[structural](https://en.cppreference.com/w/cpp/language/template_parameters) storage type. For ranges, strings, tuples and variants, rbox provides [structural alternative types](./utils.md#structural-alternative-types)
which are applied in the conversion result. For flattenable class types whose non-static data members can all be promoted to static storage **recursively**, rbox provides `structural_mirror_t` as the memberwise promotion result.

A type `T` satisfies the concept `promotable_to_static_storage` if and only if `to_static_storage` accepts values of
type `T`. The conversion rules (tested in the same order) are as follows:

1. **Ranges**: If `T` satisfies `std::ranges::input_range` with value type `V`:
   - If `V` is a character type ([`char_type<V>`](utils/concepts.md)), the result is `meta_basic_string_view<V>`;
   - If `V` is a scalar type other than pointer, the result is `meta_span<V>`;
   - Otherwise, each element is individually promoted, and the result is
     `meta_span<to_static_storage_result_t<V>>`.

2. **Tuple-like types**: If `T` satisfies [`tuple_like<T>`](type_traits/tuple_like.md) with element types `Vs...`,
   the result is `MetaTuple<to_static_storage_result_t<Vs>...>` where `MetaTuple` is one of
   [`meta_pair`](utils/meta_pair.md) (size 2), [`meta_triple`](utils/meta_triple.md) (size 3) or
   [`meta_tuple`](utils/meta_tuple.md) (otherwise).

3. **Variant-like types**: If `T` satisfies `variant_like<T>` with alternative types `As...`,
   the result is [`meta_variant`](utils/meta_variant.md)`<to_static_storage_result_t<As>...>`.
   Each promoted alternative type must satisfy [`trivial_type`](type_traits/trivial_types.md)
   (required by `meta_variant`).

4. **Pointers**: If `T` is a pointer type:
   - If the pointed-to type is a character type, the result is
     `meta_basic_string_view<std::remove_pointer_t<T>>` (the pointer is assumed to point to a
     null-terminated string);
   - If `T` is `std::nullptr_t`, the result is `std::nullptr_t`;
   - Otherwise, the result is `T` (identity conversion).

5. **Function types**: If `T` is a function type, the result is `std::add_pointer_t<T>` (i.e., a
   function pointer type).

6. **Structural types**: For any other type `T`, if `T` itself is a structural type (scalar types, lvalue reference types, arrays of
   structural types, and class types with all public non-static data members of structural types),
   the result is `T` (identity conversion).

7. **Flattenable classes**: For any other type `T`, if `T` satisfies [`flattenable_class<T>`](./type_traits/flattenable.md),
   the result is `structural_mirror_t<T>` — a structural mirror type whose members correspond to the
   flattened non-static data members of `T`, with each member type recursively promoted via
   `to_static_storage_result_t`.

> `structural_mirror_t<T>` flattens the inheritance hierarchy: members of all base classes appear as
> direct members of the mirror type. References and bit-fields are preserved as-is.
> If a member is itself a reference type, it appears as a reference in the mirror (no promotion);
> otherwise each member is individually promoted by `to_static_storage()`.
>
> **Limitation**: Anonymous union members are not supported. GCC does not provide a standards-conforming
> copy constructor for unnamed union types (possibly a GCC bug, see [[class.copy.ctor]/6](https://eel.is/c++draft/class.copy.ctor#6)),
> making it impossible to copy them in a `consteval` context. A class type is not promotable if it
> contains anonymous union members.

If a type does not match any of the above rules, a compile error occurs.

The type alias `to_static_storage_result_t<T>` obtains the result type of `to_static_storage` for a
given input type `T`.

## Example

```cpp
// (1) Range (including initializer list) to meta_span
constexpr auto span_1 = rbox::to_static_storage(std::vector<int>{1, 2, 3, 4, 5});
static_assert(std::is_same_v<std::remove_const_t<decltype(span_1)>,
                             rbox::meta_span<int>>);

constexpr auto span_2 = rbox::to_static_storage({"Cat", "Dog", "Rabbit"});
static_assert(std::is_same_v<decltype(span_2),
                             const rbox::meta_span<rbox::meta_string_view>>);

// (2) Tuple-like to structural tuple alternative (meta_pair, meta_triple, meta_tuple)
constexpr auto meta_tup =
    rbox::to_static_storage(std::tuple<int, double, std::u16string>{42, 3.14, u"Hello"});
static_assert(std::is_same_v<
              std::remove_const_t<decltype(meta_tup)>,
              rbox::meta_triple<int, double, rbox::meta_u16string_view>>);

// (3) Const char pointer to meta_string_view
constexpr auto cstr = rbox::to_static_storage("world");
static_assert(std::is_same_v<std::remove_const_t<decltype(cstr)>, rbox::meta_string_view>);

// (4) Identity (structural type)
struct point_t {
    int x;
    int y;
};
constexpr auto val = rbox::to_static_storage(point_t{.x = 12, .y = 34});
static_assert(std::is_same_v<decltype(val), const point_t>);

// (5) Function to function pointer
using F = void(int, double);
static_assert(std::is_same_v<
    rbox::to_static_storage_result_t<F>,
    void (*)(int, double)>);

// (6) Pointer-to-member (identity)
constexpr auto mem_ptr = rbox::to_static_storage(&point_t::x);
static_assert(std::is_same_v<decltype(mem_ptr), int point_t::* const>);

// (7) Variant-like to meta_variant
constexpr auto var_val = rbox::to_static_storage(std::variant<int, std::string>(42));
static_assert(std::is_same_v<
              std::remove_const_t<decltype(var_val)>,
              rbox::meta_variant<int, rbox::meta_string_view>>);

// (8) Flattenable class to structural_mirror_t
struct widget_t {
    std::string name;
    std::vector<int> sizes;
};
constexpr auto mirrored = rbox::to_static_storage(widget_t{"gear", {3, 5, 7}});
static_assert(std::is_same_v<
              rbox::meta_string_view,               // std::string → meta_string_view
              decltype(mirrored.name)>);
static_assert(std::is_same_v<
              rbox::meta_span<int>,                 // std::vector<int> → meta_span<int>
              decltype(mirrored.sizes)>);

// (9) Using to_static_storage_result_t
static_assert(std::is_same_v<
    rbox::to_static_storage_result_t<std::vector<int>>,
    rbox::meta_span<int>>);
```

See [unit test](../tests/test_to_static_storage.cpp) for more examples.
