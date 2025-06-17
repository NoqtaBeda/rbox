## Testing Addressable Members

Defined in header `<rbox/utils/addressable_member.hpp>`.

```cpp
namespace rbox {

consteval bool is_addressable_class_member(std::meta::info member);
consteval bool is_addressable_non_class_member(std::meta::info member);

}  // namespace rbox
```

`is_addressable_class_member(member)` checks whether `member` is some _class_ member which is addressable, i.e. `&[:member:]` is a valid constant expression. Addressable class member is one of the following:

- Non-static data member which is:
  - not template;
  - neither reference nor bit-field;
- Static data member (can be reference) which is not template;
- Non-static member function which is:
  - not template;
  - neither of constructor, destructor, or deleted;
- Static member function which is:
  - not template;
  - not deleted.

`is_addressable_non_class_member(member)` checks whether `member` is some _non-class_ member which is addressable, i.e. `&[:member:]` is a valid constant expression. Addressable non-class member is one of the following:

- Variable (may be reference) which is not template;
- Function which is:
  - not template;
  - not deleted.

Detailed examples are shown in [unit test cases](../tests/utils/test_addressable_member.cpp).
