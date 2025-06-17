## Enum Switch-Case Support

Defined in header `<rbox/enum/enum_switch.hpp>`.

```cpp
namespace rbox {

// (1)
template <enum_type E, class Func>
constexpr auto enum_switch(Func&& func, E e) -> void;
// (2)
template <enum_type E, class Func, class FallbackFunc>
constexpr auto enum_switch(Func&& func, FallbackFunc&& fallback, E e) -> void;
// (3)
template <non_reference_type T, enum_type E, class Func>
constexpr auto enum_switch(Func&& func, E e) -> std::optional<T>;
// (4)
template <class T, enum_type E, class Func>
constexpr auto enum_switch(Func&& func, E e, T&& default_value) -> std::decay_t<T>;

}  // namespace rbox
```

- (1) `enum_switch(func, e)` invokes `func(std::cw<e>) -> void`, or no-op if `e` does not exist in enum type definition;
- (2) `enum_switch(func, fallback, e)` invokes `func(std::cw<e>) -> void`, or invokes `fallback(e)` if `e` does not exist in enum type definition;
- (3) `enum_switch<T>(func, e)` invokes `func(std::cw<e>) -> T'`, or returns `std::nullopt` if `e` does not exist in enum type definition. Return type `T'` of `func` will be converted to destination type `T`;
- (4) `enum_switch(func, e, default_value)` invokes `func(std::cw<e>) -> T'`, or returns `default_value` if `e` does not exist in enum type definition. Keep an eye on the type `T` of `default_value` as the return type `T'` of `func` will be converted to destination type `T`.

The function object should be invocable with a single argument of type `std::constant_wrapper<e>` for each `e` defined in the enum type.

For GCC and Clang, always-inline attributes are used to ensure the same performance as hand-written switch-case sequence.

Example:

```cpp
enum class opcode : uint8_t {
    nop,
    mov,
    add_i32,
    sub_i32,
    mul_i32,
    div_i32,
    mod_i32,
    add_f32,
    sub_f32,
    mul_f32,
    div_f32,
    mod_f32,
    load_i32,
    load_f32,
    load_obj,
    store_i32,
    store_f32,
    store_obj,
    return_void,
};

enum class opcode_category : uint8_t {
    nullary,
    unary_arithmetic,
    binary_arithmetic,
    load_store,
};

template <opcode Op>
constexpr auto opcode_category_v = /* Compilation error for invalid Op */;
// Specializations for each opcode
template <>
constexpr auto opcode_category_v<opcode::add_i32> = opcode_category::binary_arithmetic;
// ...

void run_interpreter(const uint8_t* bytecode, context_t* ctx)
{
    bool done = false;
    while (!done) {
        // decltype(opc) is in [std::constant_wrapper<e> for e in enum class opcode],
        // i.e. rbox::enum_count_v<opcode> overloads will be instantiated.
        rbox::enum_switch(
            [&done, &bytecode, ctx](auto opc) {
                if constexpr (opc == opcode::return_void) {
                    done = true;
                } else if constexpr (opcode_category_v<opc> == opcode_category::binary_arithmetic) {
                    bytecode = handle_binary_arithmetic(bytecode, ctx);
                } else if constexpr (opcode_category_v<opc> == opcode_category::load_store) {
                    bytecode = handle_load_store(bytecode, ctx);
                } else { /* ... */
                }
            },
            [](opcode invalid_opcode) {
                auto msg = std::format(
                    "Invalid opcode {}. Please verify the input bytecode",
                    std::to_underlying(opcode));
                FATAL_ERROR(std::move(msg));
            },
            static_cast<opcode>(*bytecode));
    }
}
```
