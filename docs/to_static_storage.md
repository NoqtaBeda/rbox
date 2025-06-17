## Introduction

`to_static_storage()` is a standalone component provided by `rbox` which promotes input to static storage via `std::define_static_array()`, `std::define_static_string()`, etc. *recursively*.

Defined in header `<rbox/to_static_storage.hpp>`.

```cpp
namespace rbox {

template <class T>
concept promotable_to_static_storage = /* see below */;

struct to_static_storage_t {
    template <promotable_to_static_storage T>
    static consteval auto operator()(const T& value)
        /* -> to_static_storage_result_t<T> */;

    template <promotable_to_static_storage T>
    static consteval auto operator()(std::initializer_list<T> values)
        /* -> to_static_storage_result_t<T> */;
};

constexpr auto to_static_storage = to_static_storage_t{};

template <promotable_to_static_storage T>
using to_static_storage_result_t = decltype(to_static_storage(std::declval<T>()));

}  // namespace rbox
```

The function object `to_static_storage` converts input to a compile-time static constant with [structural](https://en.cppreference.com/w/cpp/language/template_parameters) storage type. For ranges, strings, tuples and variants, rbox provides [structural alternative types](./utils.md#structural-alternative-types) which are applied in the conversion result.

A type `T` satisfies the concept `promotable_to_static_storage` if and only if `to_static_storage` accepts values of type `T`. The conversion rules are as follows:

1. **Ranges**: If `T` satisfies `std::ranges::input_range` with value type `V`:
   - If `V` is a character type, the result is `meta_basic_string_view<V>`;
   - Otherwise, the result is `meta_span<to_static_storage_result_t<V>>`.
2. **Tuple-like types**: If `T` satisfies `tuple_like` with element types `Vs...`, the result is `MetaTuple<to_static_storage_result_t<Vs>...>` where `MetaTuple` is one of `meta_pair` (when tuple size is 2), `meta_triple` (when tuple size is 3) or `meta_tuple` (otherwise).
3. **Variant-like types**: If `T` satisfies `variant_like` with alternative types `As...`, the result is `meta_variant<to_static_storage_result_t<As>...>`. Each element type `to_static_storage_result_t<A>` must be a trivial type (required by `meta_variant`).
4. **Pointers**: If `T` is a pointer type:
   - If `T` is `std::nullptr_t`, the result is `std::nullptr_t`;
   - Otherwise, if the pointed-to type is a character type, the result is `meta_basic_string_view<std::remove_pointer_t<T>>` (assumes the pointer points to a null-terminated string);
   - Otherwise, the result is `T` (identity conversion).
5. **Functions**: If `T` is a function type, the result is `std::add_pointer_t<T>` (i.e., a function pointer type).
6. **Other types**: For any other type `T`, if `T` is a structural type, the result is `T` (identity conversion). Otherwise, a compile error occurs.

The type alias `to_static_storage_result_t<T>` obtains the result type of `to_static_storage` for a given input type `T`.

## Example

```cpp
// (1) Range (including initializer list) to meta_span
constexpr auto span_1 = rbox::to_static_storage(std::vector<int>{1, 2, 3, 4, 5});
static_assert(std::is_same_v<std::remove_const_t<decltype(span_1)>,
                             rbox::meta_span<int>>);

constexpr auto span_2 = rbox::to_static_storage({"Cat", "Dog", "Rabbit"});
static_assert(std::is_same_v<std::remove_const_t<decltype(span_2)>,
                             rbox::meta_span<rbox::meta_string_view>>);

// (2) Tuple-like to meta-tuple family (meta_pair, meta_triple or meta_tuple)
constexpr auto meta_tup =
    rbox::to_static_storage(std::tuple<int, double, std::u16string>{42, 3.14, u"Hello"});
static_assert(std::is_same_v<
              std::remove_const_t<decltype(meta_tup)>,
              rbox::meta_triple<int, double, rbox::meta_u16string_view>>);

// (3) Const char pointer to meta_string_view
constexpr auto cstr = rbox::to_static_storage("world");
static_assert(std::is_same_v<std::remove_const_t<decltype(cstr)>, rbox::meta_string_view>);

// (4) Identity
struct point_t {
    int x;
    int y;
};
constexpr auto val = rbox::to_static_storage(point_t{.x = 12, .y = 34});
static_assert(std::is_same_v<decltype(val), const point_t>);
```

See [unit test](../tests/test_to_static_storage.cpp) for more examples and details.
