## Converting Identifier Naming

Defined in header `<rbox/utils/identifier_naming.hpp>`.

> **⚠️ Warning:** This component does **not** perform any input validation. It assumes the input
> is a well-formed identifier with ASCII characters only
> and does not check for invalid characters or malformed patterns.
> The only delimiters recognized are `'_'` and `'-'`; any other splitting or validation logic
> (e.g. handling whitespace, dots, or custom separators) is the caller's responsibility.
> Passing arbitrary or unsanitized strings may produce unexpected results
> (e.g. whitespaces or non-ASCII characters are passed through unchanged).

```cpp
namespace rbox {

enum class identifier_naming_rule {
    snake_case,
    all_caps_snake_case,
    kebab_case,
    all_caps_kebab_case,
    lower_camel_case,
    upper_camel_case,
    lower_camel_snake_case,
    upper_camel_snake_case,
    http_header_case,
};

struct non_alpha_as_lower_tag_t {};
struct non_alpha_as_upper_tag_t {};

constexpr auto non_alpha_as_lower = non_alpha_as_lower_tag_t{};
constexpr auto non_alpha_as_upper = non_alpha_as_upper_tag_t{};

// (1) Identifier conversion returning std::basic_string<CharT>
struct to_snake_case_t {
    // (1.1) Convert, treating non-alpha characters as lowercase
    template <string_like StringLike>
    static constexpr auto operator()(const StringLike& identifier)
        /* -> std::basic_string<CharT> */;

    // (1.2) Convert with explicit non-alpha treatment tag (lower)
    template <string_like StringLike>
    static constexpr auto operator()(
        non_alpha_as_lower_tag_t tag, const StringLike& identifier)
        /* -> std::basic_string<CharT> */;

    // (1.3) Convert with explicit non-alpha treatment tag (upper)
    template <string_like StringLike>
    static constexpr auto operator()(
        non_alpha_as_upper_tag_t tag, const StringLike& identifier)
        /* -> std::basic_string<CharT> */;

    // (1.4) Convert from pointer + length, treating non-alpha as lowercase
    template <char_type CharT>
    static constexpr auto operator()(const CharT* identifier, const CharT* identifier_end)
        -> std::basic_string<CharT>;

    // (1.5) Convert from pointer + length with explicit tag
    template <char_type CharT>
    static constexpr auto operator()(
        non_alpha_as_lower_tag_t tag,
        const CharT* identifier,
        const CharT* identifier_end) -> std::basic_string<CharT>;

    // (1.6) Convert from pointer + length with explicit tag
    template <char_type CharT>
    static constexpr auto operator()(
        non_alpha_as_upper_tag_t tag,
        const CharT* identifier,
        const CharT* identifier_end) -> std::basic_string<CharT>;
};

struct to_all_caps_snake_case_t { /* same interface as above */ };
struct to_kebab_case_t { /* same interface as above */ };
struct to_all_caps_kebab_case_t { /* same interface as above */ };
struct to_lower_camel_case_t { /* same interface as above */ };
struct to_upper_camel_case_t { /* same interface as above */ };
struct to_lower_camel_snake_case_t { /* same interface as above */ };
struct to_upper_camel_snake_case_t { /* same interface as above */ };
struct to_http_header_case_t { /* same interface as above */ };

// Variable templates
inline constexpr auto to_snake_case = to_snake_case_t{};
inline constexpr auto to_all_caps_snake_case = to_all_caps_snake_case_t{};
inline constexpr auto to_kebab_case = to_kebab_case_t{};
inline constexpr auto to_all_caps_kebab_case = to_all_caps_kebab_case_t{};
inline constexpr auto to_lower_camel_case = to_lower_camel_case_t{};
inline constexpr auto to_upper_camel_case = to_upper_camel_case_t{};
inline constexpr auto to_lower_camel_snake_case = to_lower_camel_snake_case_t{};
inline constexpr auto to_upper_camel_snake_case = to_upper_camel_snake_case_t{};
inline constexpr auto to_http_header_case = to_http_header_case_t{};

// (2) Runtime dispatcher
struct convert_identifier_t {
    // (2.1) Convert with explicit tag, pointer + length, and naming rule
    template <char_type CharT>
    static constexpr auto operator()(
        non_alpha_as_lower_tag_t tag,
        const CharT* identifier,
        const CharT* identifier_end,
        identifier_naming_rule naming_rule) -> std::basic_string<CharT>;

    // (2.2) Convert string-like with explicit tag and naming rule
    template <string_like StringLike>
    static constexpr auto operator()(
        non_alpha_as_lower_tag_t tag,
        const StringLike& identifier,
        identifier_naming_rule naming_rule) /* -> std::basic_string_view<CharT> */;

    // (2.3) Convert pointer + length with naming rule (default tag)
    template <char_type CharT>
    static constexpr auto operator()(
        const CharT* identifier,
        const CharT* identifier_end,
        identifier_naming_rule naming_rule) -> std::basic_string<CharT>;

    // (2.4) Convert string-like with naming rule (default tag)
    template <string_like StringLike>
    static constexpr auto operator()(
        const StringLike& identifier,
        identifier_naming_rule naming_rule) /* -> std::basic_string_view<CharT> */;
};

constexpr auto convert_identifier = convert_identifier_t{};

}  // namespace rbox
```

### Description

The `to_*_case` function objects convert an input identifier to the specified naming convention.
They support all character types (`char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`) and
return `std::basic_string<CharT>` matching the input character type. Input can be any
[*string-like*](../type_traits/string_like_types.md) type (e.g. `std::string`, `std::string_view`,
`const char*`, or a character array).

> **⚠️ Warning:** Although multiple character types are supported, case conversion only handles
> **ASCII letters** (`'a'`–`'z'`, `'A'`–`'Z'`). All non-ASCII characters are passed through
> unchanged. Since compile-time evaluation has no access to the system locale,
> locale-aware case conversion (e.g. `'é'` → `'É'`, `'ß'` → `'SS'`) is not feasible in
> a `constexpr` context.

All supported identifier cases:

1. `to_snake_case`: Converts to `snake_case`;
2. `to_all_caps_snake_case`: Converts to `ALL_CAPS_SNAKE_CASE`;
3. `to_kebab_case`: Converts to `kebab-case`;
4. `to_all_caps_kebab_case`: Converts to `ALL-CAPS-KEBAB-CASE`;
5. `to_lower_camel_case`: Converts to `lowerCamelCase`;
6. `to_upper_camel_case`: Converts to `UpperCamelCase`;
7. `to_lower_camel_snake_case`: Converts to `lower_Camel_Snake_Case`;
8. `to_upper_camel_snake_case`: Converts to `Upper_Camel_Snake_Case`;
9. `to_http_header_case`: Converts to `Http-Header-Case`.

#### Non-alpha character handling

The overloads accepting a `non_alpha_as_lower_tag_t` or `non_alpha_as_upper_tag_t` tag control how
non-alphabetic characters (digits, `'$'`, etc.) are treated during word segmentation:

- **`non_alpha_as_lower`** (default): Non-alpha characters behave like lowercase letters —
  they do not start new words. E.g. `"TestCase1"` is split into `["Test", "Case1"]`.
- **`non_alpha_as_upper`**: Non-alpha characters behave like uppercase letters — each
  non-alpha character can start a new word. E.g. `"TestCase1"` is split into `["Test", "Case", "1"]`.

#### Conversion algorithm

For each identifier, conversion is done by the following steps:

1. Split the input identifier into word segments:
   1.1. Split input identifier by `'-'` and `'_'`;
   1.2. For each segment obtained from step 1.1, split again by letter case.
2. Convert each segment by the specified destination case;
3. Concatenate the converted segments.

#### Conversion is not round-trippable

Note that the conversion is **uniform**: each word is formatted according to the destination case
rule regardless of its original form. This means round-tripping is generally not guaranteed.
For example, `"JSON"` becomes `"Json"` in camel case — converting back to all-caps would yield
`"JSON"`, but the original `"JSON"` → `"Json"` step is lossy with respect to the upper/lower
distinction of letters following the first one.

### Examples

#### Basic usage

```cpp
#include <rbox/utils/identifier_naming.hpp>

// plain char
std::string s1 = rbox::to_snake_case("lowerCamelCase");       // "lower_camel_case"
std::string s2 = rbox::to_upper_camel_case("snake_case");     // "SnakeCase"
std::string s3 = rbox::to_kebab_case("UpperCamelCase");       // "upper-camel-case"

// wide characters
std::wstring ws = rbox::to_snake_case(L"lowerCamelCase");     // L"lower_camel_case"

// UTF-8 (char8_t)
std::u8string u8s = rbox::to_all_caps_snake_case(u8"kebab-case"); // u8"KEBAB_CASE"

// UTF-16 (char16_t)
std::u16string u16s = rbox::to_kebab_case(u"UpperCamelCase"); // u"upper-camel-case"

// UTF-32 (char32_t)
std::u32string u32s = rbox::to_http_header_case(U"snake_case"); // U"Snake-Case"
```

#### Controlling non-alpha character treatment

```cpp
using namespace rbox;

// Default: digits treated as lowercase (do not start new words)
auto a = to_snake_case("getX1AndY1");              // "get_x1_and_y1"

// Explicit: digits treated as lowercase
auto b = to_snake_case(non_alpha_as_lower, "getX1AndY1");  // "get_x1_and_y1"

// Digits treated as uppercase (start new words)
auto c = to_snake_case(non_alpha_as_upper, "getX1AndY1");  // "get_x_1_and_y_1"
```

#### Using the runtime dispatcher

```cpp
using namespace rbox;

auto s1 = convert_identifier("lowerCamelCase", identifier_naming_rule::snake_case);
// "lower_camel_case"

auto s2 = convert_identifier("snake_case", identifier_naming_rule::upper_camel_case);
// "SnakeCase"

// With explicit tag
auto s3 = convert_identifier(
    non_alpha_as_upper, u"getX1AndY1", identifier_naming_rule::snake_case);
// u"get_x_1_and_y_1"
```

#### Step-by-step example

1. Let input identifier be `"exampleInput_ParseJSONDocument_TestCase1"`;
2. After step 1.1 (split by delimiters): `["exampleInput", "ParseJSONDocument", "TestCase1"]`;
3. After step 1.2 (split by letter case):

- `"exampleInput"` → `["example", "Input"]`;
- `"ParseJSONDocument"` → `["Parse", "JSON", "Document"]`. Note that for $N+1$ consecutive uppercase letters, the first $N$ form a single word, and the last one starts the next word;
- `"TestCase1"` → `["Test", "Case1"]` with `non_alpha_as_lower` (the default), or
- `"TestCase1"` → `["Test", "Case", "1"]` with `non_alpha_as_upper`.

4. Assume we are converting to upper camel case. For each word we convert the first character to uppercase and all following letters to lowercase. Words become `"Example", "Input", "Parse", "Json", "Document", "Test", "Case1"` (note that `"JSON"` is converted to `"Json"`: the uniform conversion rule is applied to each word regardless of its original form);
5. After concatenation: `"ExampleInputParseJsonDocumentTestCase1"`.
