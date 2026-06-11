# `rbox`: Reflection Toolbox for C++26

## Introduction

**rbox** is a header-only library that leverages the C++26 Reflection facilities ([P2996](https://wg21.link/p2996)) to provide compile-time utilities for C++ development. It aims to eliminate boilerplate code for common scenarios: enum to/from string, constant dispatch table, serialization and debug dump, etc. — all of which are driven by compile-time reflection.

Key features:

- **Compile-time fixed map** — Builds a constant key-value lookup table at compile time with **zero boilerplate code** and **competitive performance**. `rbox` picks the optimal underlying data structure adaptively according to your input data — dense array, binary search, or hash table.

- **Member dispatch table** — Built on top of the fixed map. Collects class or namespace members whose identifiers match a given wildcard pattern (e.g., `"visit_*"`, `"get_*_of"`) and builds a dispatch table whose values are pointers (or member pointers) to the matched members. `rbox` provides a clean and effective alternative for dispatching idiom, compared to traditional if-else chains or X-macros.

- **Enum operations** — Converts enums to/from strings or integers (including enum flag types), with `std::format` support, bitwise/comparison operator macros, and compile-time metadata accessors. **Signedness-safe integer comparison** is supported.

- **Serialization** — Serializes a `struct` to JSON with one function call, recursing into nested objects, arrays, tuples, and variants. Additionally, `dump_to_json_like` produces more readable debug output. Both components are fully `constexpr`-compatible with UTF support.

Auxiliary features:

- **Type traits**: Provides a mechanism to "flatten" a class type — get a full list of data members, including those inherited from direct or indirect bases. Many other useful components for type classification, invocability testing, etc. are included.

- **Utility**: A collection of frequently used components, including:
  - **Recursive data promotion to static storage** — `to_static_storage()` recursively converts nested ranges, tuples, and variants into structural types, going beyond what `std::define_static_array()` and `std::define_static_string()` offer;
  - **`constexpr` UTF conversion** — Converts strings between UTF-8, UTF-16, and UTF-32 encodings;
  - **`constexpr` string builder** — Builds strings with an exponentially growing buffer;
  - **`constexpr` identifier style converter** — Converts between snake_case, CamelCase, Http-Header-Case, and more;
  - Structural container types (`meta_span`, `meta_variant`, etc.) that complement the standard library.

Detailed documentation for each component is available in the [`docs/`](docs/) directory:

| Component       | Document                                                                                      |
| --------------- | --------------------------------------------------------------------------------------------- |
| Enum operations | [`docs/enum.md`](docs/enum.md)                                                                |
| Fixed map       | [`docs/fixed_map.md`](docs/fixed_map.md)                                                      |
| Dispatch table  | [`docs/lookup.md`](docs/lookup.md)                                                            |
| Serialization   | [`docs/serializer.md`](docs/serializer.md)                                                    |
| Type traits     | [`docs/type_traits.md`](docs/type_traits.md)                                                  |
| Utilities       | [`docs/utils.md`](docs/utils.md) and [`docs/to_static_storage.md`](docs/to_static_storage.md) |

## Quick Start

### Compile-time Fixed Map

```cpp
#include <rbox/fixed_map.hpp>

int main() {
    // Integral-key map — Auto-selected as O(1) lookup:
    // if key < 1 or key > 6:
    //     return std::nullopt
    // return underlying_array[key - 1]
    constexpr auto entries = [] {
        return std::vector<std::pair<int, std::string>>{
            {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}, {6, "six"}};
    };
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(entries());
    static_assert(map[2] == "two");
    static_assert(map.find(-1) == std::nullopt);

    // String-key map — Possibly auto-selected as O(L) + O(1) hash-table lookup,
    // where L = max length of keys in the fixed map
    constexpr auto str_entries = [] {
        return std::vector<std::pair<std::string, int>>{
            {"apple", 1}, {"banana", 2}, {"cherry", 3}, {"orange", 5}, {"pineapple", 8}};
    };
    constexpr auto str_map = RBOX_STRING_KEY_FIXED_MAP(str_entries());
    static_assert(str_map["cherry"] == 3);

    // Case-insensitive string-key map
    constexpr auto ci_map = RBOX_STRING_KEY_FIXED_MAP(str_entries(), {
        .ascii_case_insensitive = true,
    });
    static_assert(ci_map["CHERRY"] == 3);
}
```

### Compile-time Member Dispatch Table

#### Pattern Match

```cpp
#include <print>
#include <rbox/lookup.hpp>

struct point_t {
    double x;
    double y;
    double z;

    double get_x() const { return x; }
    double get_y() const { return y; }
    double get_z() const { return z; }
};

// Pattern-based namespace member lookup
namespace stats {
int g_call_count = 0;
int g_gc_count = 0;
int g_throw_count = 0;

bool increment(std::string_view event, int n = 1) {
    // Builds a fixed map: {"call" -> &g_call_count, "gc" -> &g_gc_count, ...}
    constexpr auto table = RBOX_VARIABLE_FIXED_MAP(stats, "g_*_count");
    if (int* p = table[event]) {
        *p += n;
        return true;
    }
    return false;
}
}  // namespace stats

std::optional<double> get_coord(const point_t& pt, std::string_view which) {
    // Pattern-based class member lookup
    constexpr auto accessors = RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(point_t, "get_*");
    // Contents: {
    //     "x" -> &point_t::get_x,
    //     "y" -> &point_t::get_y,
    //     "z" -> &point_t::get_z
    // }
    if (auto mfp = accessors[which]) {
        return (pt.*mfp)();
    }
    return std::nullopt;
}
```

#### Pattern Match + Transformation

```cpp
#include <print>
#include <rbox/enum/enum_cast.hpp>
#include <rbox/lookup.hpp>

enum class filter_t { blur, sharpen, grayscale, invert };

struct image_processor {
    void apply_blur()    { std::println("blurring..."); }
    void apply_sharpen() { std::println("sharpening..."); }
    void apply_grayscale() { std::println("grayscaling..."); }
    void apply_invert()  { std::println("inverting..."); }

    void process(filter_t filter) {
        // Pattern "apply_*" matches apply_blur, apply_sharpen, etc.
        // The 3rd argument (transformer) converts the matched part to an enum key:
        //   "blur" → filter_t::blur, "sharpen" → filter_t::sharpen, ...
        constexpr auto table = RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(
            image_processor,           // class type
            "apply_*",                 // name pattern
            rbox::enum_cast<filter_t>  // transform matched part → enum key
        );
        // Contents: { blur→&apply_blur, sharpen→&apply_sharpen, ... }
        if (auto mfp = table[filter]) {
            (this->*mfp)();
        }
    }
};
```

#### Custom Member Transformer

```cpp
#include <print>
#include <rbox/enum/enum_cast.hpp>
#include <rbox/lookup.hpp>
#include <rbox/utils/ctype/case_conversion.hpp>  // ascii_toupper

enum class opcode_t { ADD, SUB, MUL, DIV };

struct calculator {
    int add(int a, int b) { return a + b; }
    int sub(int a, int b) { return a - b; }
    int mul(int a, int b) { return a * b; }
    int div(int a, int b) { return a / b; }

    int compute(opcode_t op, int a, int b) {
        // Access to std::meta::info of each member directly.
        constexpr auto table = RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(
            calculator,
            // Custom transformer: (std::meta::info) → std::optional<opcode_t>
            [](std::meta::info member) -> std::optional<opcode_t> {
                using call_signature = int(int, int);
                if (!is_same_type(type_of(member), ^^call_signature)) {
                    return std::nullopt;
                }
                // Convert identifier to uppercase and try to match against opcode_t entries
                return rbox::enum_cast<opcode_t>(
                    rbox::ascii_toupper(identifier_of(member)));
            });
        // Contents: { add→&calculator::ADD, sub→&calculator::SUB, ... }
        if (auto mfp = table[op]) {
            return (this->*mfp)(a, b);
        }
        return 0;
    }
};
```

#### Variant Member Types

```cpp
#include <print>
#include <rbox/lookup.hpp>

struct painter {
    void draw_circle(int radius) { std::println("circle r={}", radius); }
    void draw_rect(int w, int h) { std::println("rect {}x{}", w, h); }
    void draw_line(int len)      { std::println("line len={}", len); }
    void draw_point()            { std::println("point"); }
    void draw_clear()            { std::println("clear"); }
    // Not matched: different prefix
    void erase_all() {}

    void render(std::string_view shape, int a = 0, int b = 0) {
        constexpr auto table = RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(painter, "draw_*");
        // Members draw_circle, draw_rect, ... have different signatures,
        // so value_type = meta_variant<
        //   void (painter::*)(int),
        //   void (painter::*)(int, int),
        //   void (painter::*)()>

        table[shape].visit([&](auto mfp) {
            if (mfp == nullptr) return;
            // Use if constexpr + requires to select the right call signature
            if constexpr (requires { (this->*mfp)(a, b); }) {
                (this->*mfp)(a, b);
            } else if constexpr (requires { (this->*mfp)(a); }) {
                (this->*mfp)(a);
            } else {
                (this->*mfp)();
            }
        });
    }
};
```

### Enum Operations

```cpp
#include <print>
#include <rbox/enum.hpp>

enum class color_t { red, green, blue };

enum class permissions_t : int {
    read = 1,
    write = 2,
    execute = 4,
};
// operators: |, |=, &, &=, ^, ^=, ~
RBOX_DEFINE_ENUM_BITWISE_OPERATORS(permissions_t)

int main() {
    // Enum value → name
    std::println("color: {}", rbox::enum_name(color_t::red));  // "red"

    // Name → enum value
    auto c = rbox::enum_cast<color_t>("blue");     // std::optional, *c == color_t::blue
    auto ci = *rbox::ascii_ci_enum_cast<color_t>("BLUE");  // color_t::blue

    // Enum flag value → string
    auto rw = permissions_t::read | permissions_t::write;
    std::println("perms: {}", *rbox::enum_flags_name(rw));  // "read|write" (or "write|read")

    // String → enum flag value
    auto rwx = *rbox::enum_flags_cast<permissions_t>("read | write | execute");
    // rwx == permissions_t::read | permissions_t::write | permissions_t::execute

    // std::format support (requires <rbox/enum/enum_format.hpp>)
    std::println("{}", color_t::green);  // "green"
    std::println("{:F}", rwx);           // "read|write|execute" ('F' for enum flag)
    std::println("{:F | }", rwx);        // "read | write | execute" ('F' + delimiter)
}
```

### Serialization

```cpp
#include <print>
#include <rbox/serializer.hpp>

enum class job_t {
    developer = 0,
    manager = 1,
    accountant = 2,
};

struct person_t {
    std::string name;
    int age;
    job_t job;
};

int main() {
    // JSON serialization
    person_t p{"Alice", 30, job_t::developer};
    std::println("{}", rbox::serialize_to_json(p));
    // {"name":"Alice","age":30,"job":0}

    // With indent
    std::println("{}", rbox::serialize_to_json(p, 2, ' '));
    // {
    //   "name": "Alice",
    //   "age": 30,
    //   "job": 0
    // }

    // Debug-friendly dump (unquoted field names, single-quoted chars)
    std::println("{}", rbox::dump_to_json_like(p));
    // {name:"Alice",age:30,job:developer}

    // Enum-to-string serialization (with char8_t output)
    constexpr auto opts = rbox::serialize_options{
        .enum_to_string = true,
    };
    auto u8json = rbox::serialize_to_json<char8_t, opts>(p);
    // u8string: {"name":"Alice","age":30,"job":"developer"}
}
```

## Prerequisites

- **GCC 16.1** or later (with `-freflection` flag)

## Using rbox in Your CMake Project

### Via `add_subdirectory` (recommended for git submodule)

```shell
git submodule add https://github.com/noqtabeda/rbox.git third_party/rbox
```

```cmake
add_subdirectory(third_party/rbox)

target_link_libraries(myapp PRIVATE rbox::rbox)          # header-only
```

### Via `FetchContent` (no pre-installation needed)

```cmake
include(FetchContent)
FetchContent_Declare(
    rbox
    GIT_REPOSITORY https://github.com/noqtabeda/rbox.git
    GIT_TAG        main   # or a specific release tag, with format "release-vX.Y.Z"
)
FetchContent_MakeAvailable(rbox)

target_link_libraries(myapp PRIVATE rbox::rbox)
```

## Build & Run Test Cases

### With XMake

```shell
# Configure (GCC 16)
xmake f -m debug \
        --sdk=<gcc-root> \
        --cxxflags="-freflection" \
        --toolchain=gcc

# Build and run all tests
xmake run --group=tests/**

# Run a specific test
xmake run tests-utils-test_utility
```

Available CMake options:

| Option        | Default | Description                                |
| ------------- | ------- | ------------------------------------------ |
| `static-test` | `false` | Enable compile-time static assertion tests |

### With CMake

```shell
# Configure
cmake -B build -G Ninja \
      -DCMAKE_C_COMPILER=<gcc-root>/bin/gcc \
      -DCMAKE_CXX_COMPILER=<gcc-root>/bin/g++ \
      -DCMAKE_BUILD_TYPE=Debug

# Build all targets
cmake --build build -j$(nproc)

# Run all tests
ctest --test-dir build -j$(nproc)

# Run a specific test
ctest --test-dir build -R tests-utils-test_utility
```

Available CMake options:

| Option             | Default | Description                                |
| ------------------ | ------- | ------------------------------------------ |
| `RBOX_STATIC_TEST` | `OFF`   | Enable compile-time static assertion tests |

## License

MIT License. See [`LICENSE`](LICENSE) for details.

## Acknowledgements

Greatest respect to every author of P2996 and [experimental Clang fork](https://github.com/bloomberg/clang-p2996).

Special thanks to author of [`magic_enum`](https://github.com/Neargye/magic_enum), the C++17 enum reflection library. API design of enum operations in `rbox` are inspired by `magic_enum` library.

## Future Roadmap

- `fixed_set`
- Serialization
  - to YAML
- Deserialization
  - to JSON
  - to YAML
