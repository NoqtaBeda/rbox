## Flattenable Type

Defined in header `<rbox/type_traits/class_types/flattened_nsdm.hpp>`.

```cpp
namespace rbox {

// (1)
template <class T>
concept partially_flattenable_class = /* ... */;
// (2)
template <class T>
concept partially_flattenable = /* ... */;
// (3)
template <class T>
concept flattenable_class = /* ... */;
// (4)
template <class T>
concept flattenable = /* ... */;
// (5)
template <class T>
concept flattenable_aggregate_class = /* ... */;
// (6)
template <class T>
concept flattenable_aggregate = /* ... */;

}  // namespace rbox
```

These concepts test whether a type can be "flattened", i.e. whether we can construct an aggregate type which contains all the non-static data members (or public ones only) without ambiguity or loss of information. Flattening can be helpful during serialization or deserialization of C++ types.

- **(1)** The concept `partially_flattenable_class` tests whether `std::remove_cv_t<T>` is a non-union class type that is partially flattenable. A class type is partially flattenable if it satisfies all the following:
  - No virtual base classes;
  - No ambiguous base classes;
  - Every direct base class is also partially flattenable, recursively.
- **(2)** The concept `partially_flattenable` tests whether `std::remove_cv_t<T>` is a partially flattenable type. A type is partially flattenable if it can be classified as one of the following:
  - **Scalar types**: arithmetic types, enum types, pointers (including `std::nullptr_t` and function pointers), pointers to members, etc.;
  - **C-style array types**: including bounded `U[N]`, unbounded `U[]`, multi-dimension `U[N][K]`, etc.;
  - **Partially flattenable class types**: as described above.
- **(3)** The concept `flattenable_class` tests whether `std::remove_cv_t<T>` is a non-union class type that is flattenable. A class type is flattenable if it is a partially flattenable class type that additionally satisfies the following conditions:
  - No private or protected non-static data members;
  - No private or protected direct base classes that are not empty;
  - Every direct base class is also flattenable, recursively.
- **(4)** The concept `flattenable` tests whether `std::remove_cv_t<T>` is a flattenable type, which is either scalar type, C-style array type, or flattenable class type as described above.
- **(5)** The concept `flattenable_aggregate_class` tests whether `std::remove_cv_t<T>` is a non-union class type that is a flattenable aggregate. A class type is a flattenable aggregate if it is a flattenable class type that additionally satisfies the following constraints:
  - The class is an aggregate;
  - Every direct base class is also a flattenable aggregate, recursively.
- **(6)** The concept `flattenable_aggregate` tests whether `std::remove_cv_t<T>` is a flattenable aggregate type, which is one of the following:
  - **C-style array types**: including bounded `U[N]`, unbounded `U[]`, multi-dimension `U[N][K]`, etc.;
  - **Flattenable aggregate class types**: as described above.

Note that references, unions, and classes with virtual inheritance are never flattenable. Additionally, classes with private or protected non-static data members (either defined directly or inherited from non-public base classes) are never `flattenable` but may be `partially_flattenable`.

Example:

```cpp
struct foo_t {
    std::string first_name;
    std::string last_name;
};
struct bar_t : foo_t {
    int age;
};
// Flattened as: {
//   first_name: std::string
//   last_name: std::string
//   age: int
// }
static_assert(rbox::flattenable_aggregate_class<bar_t>);

struct baz_t : foo_t {
    virtual void introduce_myself() {
        std::println("Hello. I am {} {}.", this->first_name, this->last_name);
    }
};
static_assert(rbox::flattenable_class<baz_t>);
static_assert(!rbox::flattenable_aggregate_class<baz_t>);

class qux_t : foo_t {
private:
    uint64_t last_visit_timestamp_;
};

// Partially flattenable (we can still try to flatten its public members): {
//   first_name: std::string
//   last_name: std::string
//   /* non-public data members are filtered out */
// }
static_assert(rbox::partially_flattenable_class<qux_t>);
// Not flattenable (due to existence of private non-static data members)
static_assert(!rbox::flattenable_class<qux_t>);
// Not flattenable aggregate (stronger constraints than flattenable concept)
static_assert(!rbox::flattenable_aggregate_class<qux_t>);

// Hint: partially_flattenable has loose constraints that most class types in practice can satisfy.
// A class type can be partially flattenable even if there's nothing public to be flattened.
static_assert(rbox::partially_flattenable_class<std::string>);
static_assert(rbox::partially_flattenable_class<std::vector<std::string>>);
```

See [unit test](../../tests/type_traits/class_types/test_flattenable_types.cpp) for more examples and details.

## Details of Flattened Class Types

Defined in header `<rbox/type_traits/class_types/flattened_nsdm.hpp>`.

```cpp
namespace rbox {

struct flattened_data_member_info {
    std::meta::info info;
    std::meta::member_offset actual_offset;

    consteval auto type() const -> std::meta::info;
    consteval auto direct_parent() const -> std::meta::info;
    consteval auto offset_in_parent() const -> std::meta::member_offset;
    consteval auto offset_bits_in_parent() const -> size_t;
    consteval auto offset_bytes_in_parent() const -> size_t;
    consteval auto actual_offset_bits() const -> size_t;
    consteval auto actual_offset_bytes() const -> size_t;
};

template <partially_flattenable_class T>
constexpr meta_span<flattened_data_member_info> all_flattened_nonstatic_data_members_v;

template <partially_flattenable_class T>
constexpr meta_span<flattened_data_member_info> public_flattened_nonstatic_data_members_v;

consteval auto all_flattened_nonstatic_data_members_of(std::meta::info T)
    -> meta_span<flattened_data_member_info>;

consteval auto public_flattened_nonstatic_data_members_of(std::meta::info T)
    -> meta_span<flattened_data_member_info>;

}  // namespace rbox
```

The struct `flattened_data_member_info` holds information about a non-static data member of class type `T`, which may be inherited from base classes of `T`. It contains the following members:

- `info` - Reflection to a non-static data member of `T`, either defined by `T` directly or inherited from some base class;
- `actual_offset` - Offset of the member relative to `T`;
- `type()` - Returns the reflection to the member's type;
- `direct_parent()` - Returns the reflection to the class that the member is directly defined in;
- `offset_in_parent()` - Returns the offset of the member in its direct parent class;
- `offset_bits_in_parent()` - Returns the offset in bits of the member in its direct parent class;
- `offset_bytes_in_parent()` - Returns the offset in bytes of the member in its direct parent class (compile error if member is a bit-field);
- `actual_offset_bits()` - Returns the actual offset in bits of the member relative to `T`;
- `actual_offset_bytes()` - Returns the actual offset in bytes of the member relative to `T` (compile error if member is a bit-field).

The following example shows the difference between "offset in parent" and "actual offset". Actual offsets are always relative to `DemoStructC`.

```cpp
struct DemoStructA {
    int64_t a1;
    int32_t a2;
};
struct DemoStructB : DemoStructA {
    int64_t b1;
    std::array<int32_t, 4> b2;
};
struct DemoStructC : DemoStructB {
private:
    int32_t c1;

public:
    int32_t c2;
};
// Layout of class DemoStructC (each '*' represents one byte):
// 0               8               16              24              32              40
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// |---- a1 -----| |-a2--|         |---- b1 -----| |------------ b2 -------------| |-c1--| |-c2--|
```

|    Data Member    | Size  | `actual_offset_bytes()` | `offset_bytes_in_parent()` |
| :---------------: | :---: | :---------------------: | :------------------------: |
| `DemoStructA::a1` |   8   |            0            |             0              |
| `DemoStructA::a2` |   4   |            8            |             8              |
| `DemoStructB::b1` |   8   |           16            |             0              |
| `DemoStructB::b2` |  16   |           24            |             8              |
| `DemoStructC::c1` |   4   |           40            |             0              |
| `DemoStructC::c2` |   4   |           44            |             4              |

The variable template `all_flattened_nonstatic_data_members_v` gets a full list of non-static data members of non-union class `T`, including direct and inherited ones. Members are sorted in ascending order by actual offset relative to `T`.

The variable template `public_flattened_nonstatic_data_members_v` gets a full list of non-static data members with public access of non-union class `T` (i.e. each member in the list can be accessed globally via class `T`), including direct and inherited ones. Members are sorted in ascending order by actual offset relative to `T`.

The function `all_flattened_nonstatic_data_members_of` takes a reflection `T` of a class type and returns a `meta_span<flattened_data_member_info>` containing all non-static data members (including inherited ones) of that class.  `T` must reflect a type that satisfies the `partially_flattenable_class` concept.

The function `public_flattened_nonstatic_data_members_of` is the public-only variant.

Example:

```cpp


// | `DemoStructA::a1` |  8 |  0 | 0 |
// | `DemoStructA::a2` |  4 |  8 | 8 |
// | `DemoStructB::b1` |  8 | 16 | 0 |
// | `DemoStructB::b2` | 16 | 24 | 8 |
// | `DemoStructC::c1` |  4 | 40 | 0 |
// | `DemoStructC::c2` |  4 | 44 | 4 |
template for (constexpr auto M : rbox::all_flattened_nonstatic_data_members_v<DemoStructC>)
{
    std::println(
        "| `{}::{}` | {:2d} | {:2d} | {} |",
        identifier_of(M.direct_parent()),
        identifier_of(M.info),
        size_of(M.info),
        M.actual_offset_bytes(),
        M.offset_bytes_in_parent());
}
```
