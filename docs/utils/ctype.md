## ASCII Character Type Utilities

rbox provides a set of `constexpr` ASCII character classification, testing, and case conversion utilities. They operate on ASCII characters (0–127) only — non-ASCII inputs always yield `false` (for predicates) or are returned unchanged (for conversions). These utilities are locale-independent and fully `constexpr`, making them suitable for compile-time string processing.

Defined in header `<rbox/utils/ctype.hpp>`.

### Components

* [ascii_test](ctype/ascii_test.md): ASCII character/string check
* [case_conversion](ctype/case_conversion.md): ASCII case conversion (`ascii_tolower`, `ascii_toupper`)
* [char_categories](ctype/char_categories.md): ASCII character classification predicates
