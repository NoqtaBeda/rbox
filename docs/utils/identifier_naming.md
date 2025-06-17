## Converting Identifier Naming

Defined in header `<rbox/utils/identifier_naming.hpp>`.

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

// (1) Identifier validation check
constexpr bool is_valid_identifier(std::string_view identifier);

// (2) Identifier conversion returning std::string
//
// Each to_*_case_t provides the same three operator() overloads:
//   - operator()(std::string_view)            — default tag (non_alpha_as_lower)
//   - operator()(non_alpha_as_lower_tag_t, std::string_view)
//   - operator()(non_alpha_as_upper_tag_t, std::string_view)
//
// The example below uses to_snake_case_t as representative.
struct to_snake_case_t {
    static constexpr auto operator()(std::string_view identifier)
        -> std::string;
    static constexpr auto operator()(non_alpha_as_lower_tag_t tag, std::string_view identifier)
        -> std::string;
    static constexpr auto operator()(non_alpha_as_upper_tag_t tag, std::string_view identifier)
        -> std::string;
};

struct to_all_caps_snake_case_t { /* same interface as above */ };
struct to_kebab_case_t { /* same interface as above */ };
struct to_all_caps_kebab_case_t { /* same interface as above */ };
struct to_lower_camel_case_t { /* same interface as above */ };
struct to_upper_camel_case_t { /* same interface as above */ };
struct to_lower_camel_snake_case_t { /* same interface as above */ };
struct to_upper_camel_snake_case_t { /* same interface as above */ };
struct to_http_header_case_t { /* same interface as above */ };

// (3) Identifier conversion returning std::optional<std::string>
struct to_snake_case_opt_t {
    static constexpr auto operator()(std::string_view identifier)
        -> std::optional<std::string>;
    static constexpr auto operator()(non_alpha_as_lower_tag_t tag, std::string_view identifier)
        -> std::optional<std::string>;
    static constexpr auto operator()(non_alpha_as_upper_tag_t tag, std::string_view identifier)
        -> std::optional<std::string>;
};

struct to_all_caps_snake_case_opt_t { /* same interface as above */ };
struct to_kebab_case_opt_t { /* same interface as above */ };
struct to_all_caps_kebab_case_opt_t { /* same interface as above */ };
struct to_lower_camel_case_opt_t { /* same interface as above */ };
struct to_upper_camel_case_opt_t { /* same interface as above */ };
struct to_lower_camel_snake_case_opt_t { /* same interface as above */ };
struct to_upper_camel_snake_case_opt_t { /* same interface as above */ };
struct to_http_header_case_opt_t { /* same interface as above */ };

// Variable templates (2)
inline constexpr auto to_snake_case = to_snake_case_t{};
inline constexpr auto to_all_caps_snake_case = to_all_caps_snake_case_t{};
inline constexpr auto to_kebab_case = to_kebab_case_t{};
inline constexpr auto to_all_caps_kebab_case = to_all_caps_kebab_case_t{};
inline constexpr auto to_lower_camel_case = to_lower_camel_case_t{};
inline constexpr auto to_upper_camel_case = to_upper_camel_case_t{};
inline constexpr auto to_lower_camel_snake_case = to_lower_camel_snake_case_t{};
inline constexpr auto to_upper_camel_snake_case = to_upper_camel_snake_case_t{};
inline constexpr auto to_http_header_case = to_http_header_case_t{};

// Variable templates (3)
inline constexpr auto to_snake_case_opt = to_snake_case_opt_t{};
inline constexpr auto to_all_caps_snake_case_opt = to_all_caps_snake_case_opt_t{};
inline constexpr auto to_kebab_case_opt = to_kebab_case_opt_t{};
inline constexpr auto to_all_caps_kebab_case_opt = to_all_caps_kebab_case_opt_t{};
inline constexpr auto to_lower_camel_case_opt = to_lower_camel_case_opt_t{};
inline constexpr auto to_upper_camel_case_opt = to_upper_camel_case_opt_t{};
inline constexpr auto to_lower_camel_snake_case_opt = to_lower_camel_snake_case_opt_t{};
inline constexpr auto to_upper_camel_snake_case_opt = to_upper_camel_snake_case_opt_t{};
inline constexpr auto to_http_header_case_opt = to_http_header_case_opt_t{};

// (4) Runtime dispatcher
struct convert_identifier_t {
    static constexpr auto operator()(std::string_view identifier, identifier_naming_rule to_rule)
        -> std::string;
};

struct convert_identifier_opt_t {
    static constexpr auto operator()(std::string_view identifier, identifier_naming_rule to_rule)
        -> std::optional<std::string>;
};

inline constexpr auto convert_identifier = convert_identifier_t{};
inline constexpr auto convert_identifier_opt = convert_identifier_opt_t{};

}  // namespace rbox
```

### Description

(1) `is_valid_identifier(std::string_view identifier)` checks whether `identifier` is valid by the following rules:

- `identifier` should be non-empty;
- Each character in `identifier` should be one of the following ASCII characters:
  - Letters: `[A-Za-z]`;
  - Digits: `[0-9]`;
  - Dollar, underscore or hyphen: `[$_-]`.
- The first character is not a digit.

(2) The function `to_*_case` converts the input identifier to specified form. If input identifier is invalid (checked by `is_valid_identifier()`), then an empty string is returned.

(3) The function `to_*_case_opt` converts the input identifier to specified form. If input identifier is invalid (checked by `is_valid_identifier()`), then `std::nullopt` is returned.

All supported identifier cases:

1. `to_snake_case` and `to_snake_case_opt`: Converts to `snake_case`;
2. `to_all_caps_snake_case` and `to_all_caps_snake_case_opt`: Converts to `ALL_CAPS_SNAKE_CASE`;
3. `to_kebab_case` and `to_kebab_case_opt`: Converts to `kebab-case`;
4. `to_all_caps_kebab_case` and `to_all_caps_kebab_case_opt`: Converts to `ALL-CAPS-KEBAB-CASE`;
5. `to_lower_camel_case` and `to_lower_camel_case_opt`: Converts to `lowerCamelCase`;
6. `to_upper_camel_case` and `to_upper_camel_case_opt`: Converts to `UpperCamelCase`;
7. `to_lower_camel_snake_case` and `to_lower_camel_snake_case_opt`: Converts to `lower_Camel_Snake_Case`;
8. `to_upper_camel_snake_case` and `to_upper_camel_snake_case_opt`: Converts to `Upper_Camel_Snake_Case`;
9. `to_http_header_case` and `to_http_header_case_opt`: Converts to `Http-Header-Case`.

For each valid identifier, conversion is done by the following steps:

1. Split the input identifier into word segments:
   1.1. Split input identifier by `'-'` and `'_'`;
   1.2. For each segment obtained from step 1.1, split again by letter case.
2. Convert each segment by the specified destination case;
3. Concatenate the converted segments.

### Example

1. Let input identifier be `"exampleInput_ParseJSONDocument_TestCase1"`;
2. After step 1.1: `["exampleInput", "ParseJSONDocument", "TestCase1"]`;
3. After step 1.2:

- `"exampleInput" -> ["example", "Input"]`;
- `"ParseJSONDocument" -> ["Parse", "JSON", "Document"]`. Note that for $N+1$ consecutive uppercase letters, the first $N$ form a single word, and the last one starts the next word;
- `"TestCase1" -> ["Test", "Case1"]` for overload (2.1) and (3.1) which handles non-letter characters (i.e. digits or `'$'`) as lowercase letters, or
- `"TestCase1" -> ["Test", "Case", "1"]` for overload (2.2) and (3.2) which handles non-letter characters as uppercase ones.

4. Assume we are converting to upper camel case with overload (2.1). For each word obtained in step 1.2, we convert the first character to uppercase and all following letters to lowercase. Finally, words are converted to `"Example", "Input", "Parse", "Json", "Document", "Test", "Case1"` (Note that `JSON` is converted to `Json`: The uniform conversion rule is applied to each word regardless of its input form);
5. After concatenation: `"ExampleInputParseJsonDocumentTestCase1"`.
