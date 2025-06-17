## Pointer-to-Member Utilities

Defined in header `<rbox/type_traits/pointer_to_member.hpp>`.

```cpp
namespace rbox {

template <class Mem, class T>
using pointer_to_member_t = Mem T::*;

template <class T>
using member_type_t = /* ... */;

template <class T>
using member_parent_t = /* ... */;

}  // namespace rbox
```

### Type Aliases

- `pointer_to_member_t<Mem, T>` — Convenience alias for a pointer-to-member type `Mem T::*`.

- `member_type_t<T>` — Extracts the member type from a pointer-to-member type `T`. For `T = Mem Class::*`, yields `Mem`. Produces a compile error if `T` is not a pointer-to-member type.

- `member_parent_t<T>` — Extracts the parent class type from a pointer-to-member type `T`. For `T = Mem Class::*`, yields `Class`. Produces a compile error if `T` is not a pointer-to-member type.

### Example

```cpp
struct Foo {
    int bar;
    double baz;
};

using BarMem = rbox::pointer_to_member_t<int, Foo>;  // int Foo::*

static_assert(std::is_same_v<rbox::member_type_t<int Foo::*>, int>);
static_assert(std::is_same_v<rbox::member_parent_t<int Foo::*>, Foo>);
```
