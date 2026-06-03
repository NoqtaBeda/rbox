## Class Type Property Testing

Defined in header `<rbox/type_traits/class_types.hpp>`, or the following subcomponent headers respectively:

1. `<rbox/type_traits/class_types/ambiguous_inheritance.hpp>`
2. `<rbox/type_traits/class_types/non_public_nsdm.hpp>`
3. `<rbox/type_traits/class_types/virtual_inheritance.hpp>`

```cpp
namespace rbox {

// 1.
template <class T>
concept class_without_ambiguous_inheritance = /* ... */;
// 2.
template <class T>
concept class_without_non_public_nonstatic_data_members = /* ... */;
// 3.
template <class T>
concept class_without_virtual_inheritance = /* ... */;

}  // namespace rbox
```

The concept `class_without_ambiguous_inheritance` tests whether `std::remove_cv_t<T>` is a non-union class type without ambiguous base, i.e. no non-virtual base class `B` is inherited more than once, either directly or indirectly.

- Empty base classes (typically tag types in practice) may cause ambiguity as well even if they have no effect on class layout.
- Always evaluates to `false` for non-class types (including references).

The concept `class_without_non_public_nonstatic_data_members` tests whether `std::remove_cv_t<T>` is a non-union class without inaccessible non-static data members, i.e.:

- No direct protected or private data members;
- No protected or private direct base classes that are not empty;
- No protected or private data members from public base classes, recursively.
- Always evaluates to `false` for non-class types: scalar types, references, arrays, unions, etc.

The concept `class_without_virtual_inheritance` checks whether `std::remove_cv_t<T>` is a non-union class without virtual base classes (either directly or indirectly).

- Always evaluates to `false` for non-class types: scalar types, references, arrays, unions, etc.

Example:

```cpp
struct A { int x; };
struct B : A { double y; };
struct C : A, B {};  // Ambiguous: A appears twice

struct D { int x; virtual void f() {} };
struct E : virtual D {};

class F { int private_x; };

static_assert(rbox::class_without_ambiguous_inheritance<A>);
static_assert(rbox::class_without_ambiguous_inheritance<B>);
static_assert(!rbox::class_without_ambiguous_inheritance<C>);

static_assert(rbox::class_without_virtual_inheritance<A>);
static_assert(!rbox::class_without_virtual_inheritance<E>);

static_assert(rbox::class_without_non_public_nonstatic_data_members<A>);
static_assert(!rbox::class_without_non_public_nonstatic_data_members<F>);
```
