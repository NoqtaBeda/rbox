## Enum Type Hash

Defined in header `<rbox/enum/enum_hash.hpp>`.

```cpp
namespace rbox {

template <enum_type E>
constexpr uint64_t enum_hash_v;

}  // namespace rbox
```

`enum_hash_v<E>` gets the 64-bit hash value of given enum type.

Any change to the entry set (including insertion, deletion and modification to entry name or value) will change the hash value. But only changing entry order or name of the enum type itself will not.

Enum hash is useful to guard an enum type from unexpected updates with:

- new entries added, until the developer finishes work with this new entry;
- existing entries deleted, until the developer finishes cleanup work with the deleted entry;
- entry name change, to enforce semantic consistency;
- entry value change.

Example:

```cpp
enum class foo_0 {
    one = 1,
    two = 2,
    three = 3
};
enum class foo_1 {
    one = 1,
    two = 2,
    three = 3
};
enum class foo_2 {
    one = 1,
    three = 3,
    two = 2
};
enum class foo_3 {
    one = 1,
    two = 2,
    three = 3,
    four = 4
};
enum class foo_4 {
    two = 2,
    three = 3
};
enum class foo_5 {
    one = 1,
    two = 2,
    three = -3
};
enum class foo_6 {
    one = 1,
    zwei = 2,
    three = 3
};

// Same hash
static_assert(rbox::enum_hash_v<foo_0> == rbox::enum_hash_v<foo_1>);  // Identical entries
static_assert(rbox::enum_hash_v<foo_0> == rbox::enum_hash_v<foo_2>);  // Order change only
// Different hash
static_assert(rbox::enum_hash_v<foo_0> != rbox::enum_hash_v<foo_3>);  // Adds new entry
static_assert(rbox::enum_hash_v<foo_0> != rbox::enum_hash_v<foo_4>);  // Deletes entry
static_assert(rbox::enum_hash_v<foo_0> != rbox::enum_hash_v<foo_5>);  // Value change
static_assert(rbox::enum_hash_v<foo_0> != rbox::enum_hash_v<foo_6>);  // Name change
```
