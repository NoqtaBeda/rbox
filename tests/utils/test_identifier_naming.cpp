/**
 * Copyright (c) 2026 NoqtaBeda (noqtabeda@163.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#include <rbox/utils/identifier_naming.hpp>

#include "tests/test_options.hpp"

using namespace std::literals;

constexpr auto INVALID = "";

#define TEST_INVALID_IDENTIFIERS(func)                                               \
    EXPECT_EQ(INVALID, rbox::func(""));                                              \
    EXPECT_EQ(std::nullopt, rbox::func##_opt(""));                                   \
    EXPECT_EQ(INVALID, rbox::func("1"));                                             \
    EXPECT_EQ(std::nullopt, rbox::func##_opt("1"));                                  \
    EXPECT_EQ(INVALID, rbox::func("1xy"));                                           \
    EXPECT_EQ(std::nullopt, rbox::func##_opt("1xy"));                                \
    EXPECT_EQ(INVALID, rbox::func("printf()"));                                      \
    EXPECT_EQ(std::nullopt, rbox::func##_opt("printf()"));                           \
    /* '#' and '@' are not supported. */                                             \
    EXPECT_EQ(INVALID, rbox::func("#values"));                                       \
    EXPECT_EQ(std::nullopt, rbox::func##_opt("#values"));                            \
    EXPECT_EQ(INVALID, rbox::func("@values"));                                       \
    EXPECT_EQ(std::nullopt, rbox::func##_opt("@values"));                            \
                                                                                     \
    EXPECT_EQ(INVALID, rbox::func(rbox::non_alpha_as_upper, ""));                    \
    EXPECT_EQ(std::nullopt, rbox::func##_opt(rbox::non_alpha_as_upper, ""));         \
    EXPECT_EQ(INVALID, rbox::func(rbox::non_alpha_as_upper, "1"));                   \
    EXPECT_EQ(std::nullopt, rbox::func##_opt(rbox::non_alpha_as_upper, "1"));        \
    EXPECT_EQ(INVALID, rbox::func(rbox::non_alpha_as_upper, "1xy"));                 \
    EXPECT_EQ(std::nullopt, rbox::func##_opt(rbox::non_alpha_as_upper, "1xy"));      \
    EXPECT_EQ(INVALID, rbox::func(rbox::non_alpha_as_upper, "printf()"));            \
    EXPECT_EQ(std::nullopt, rbox::func##_opt(rbox::non_alpha_as_upper, "printf()")); \
    /* '#' and '@' are not supported. */                                             \
    EXPECT_EQ(INVALID, rbox::func(rbox::non_alpha_as_upper, "#values"));             \
    EXPECT_EQ(std::nullopt, rbox::func##_opt(rbox::non_alpha_as_upper, "#values"));  \
    EXPECT_EQ(INVALID, rbox::func(rbox::non_alpha_as_upper, "@values"));             \
    EXPECT_EQ(std::nullopt, rbox::func##_opt(rbox::non_alpha_as_upper, "@values"))

TEST(UtilsIdentifierNaming, ToSnakeCase)
{
    EXPECT_EQ_STATIC("get_name", rbox::to_snake_case("getName"));
    EXPECT_EQ_STATIC("set_json_file_path", rbox::to_snake_case("setJsonFilePath"));
    EXPECT_EQ_STATIC("set_json_file_path", rbox::to_snake_case("setJSONFilePath"));
    EXPECT_EQ_STATIC("value_offset", rbox::to_snake_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC("yaml_parser", rbox::to_snake_case("YamlParser"));
    EXPECT_EQ_STATIC("yaml_parser", rbox::to_snake_case("YAMLParser"));
    EXPECT_EQ_STATIC("num_gc_threads", rbox::to_snake_case("num-gcThreads"));
    EXPECT_EQ_STATIC("a", rbox::to_snake_case("a"));
    EXPECT_EQ_STATIC("a", rbox::to_snake_case("A"));
    EXPECT_EQ_STATIC("$", rbox::to_snake_case("$"));

    EXPECT_EQ_STATIC("snake_case", rbox::to_snake_case("snake_case"));
    EXPECT_EQ_STATIC("all_caps_snake_case", rbox::to_snake_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("kebab_case", rbox::to_snake_case("kebab-case"));
    EXPECT_EQ_STATIC("all_caps_kebab_case", rbox::to_snake_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("lower_camel_case", rbox::to_snake_case("lowerCamelCase"));
    EXPECT_EQ_STATIC("upper_camel_case", rbox::to_snake_case("UpperCamelCase"));
    EXPECT_EQ_STATIC("lower_camel_snake_case", rbox::to_snake_case("lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("upper_camel_snake_case", rbox::to_snake_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("http_header_case", rbox::to_snake_case("HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("get_x1_and_y1", rbox::to_snake_case("getX1AndY1"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_snake_case("x1y2"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_snake_case("X1y2"));
    EXPECT_EQ_STATIC("x1_y2", rbox::to_snake_case("x1Y2"));
    EXPECT_EQ_STATIC("x1_y2", rbox::to_snake_case("X1Y2"));
    EXPECT_EQ_STATIC("$num_args", rbox::to_snake_case("$numArgs"));
    EXPECT_EQ_STATIC("$_num_args", rbox::to_snake_case("$NumArgs"));
    EXPECT_EQ_STATIC("$_n_args", rbox::to_snake_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC("get_x1_and_y1", rbox::to_snake_case(rbox::non_alpha_as_upper, "getX1AndY1"));
    EXPECT_EQ_STATIC("x_1y_2", rbox::to_snake_case(rbox::non_alpha_as_upper, "x1y2"));
    EXPECT_EQ_STATIC("x_1y_2", rbox::to_snake_case(rbox::non_alpha_as_upper, "X1y2"));
    EXPECT_EQ_STATIC("x_1y2", rbox::to_snake_case(rbox::non_alpha_as_upper, "x1Y2"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_snake_case(rbox::non_alpha_as_upper, "X1Y2"));
    EXPECT_EQ_STATIC("$num_args", rbox::to_snake_case(rbox::non_alpha_as_upper, "$numArgs"));
    EXPECT_EQ_STATIC("$_num_args", rbox::to_snake_case(rbox::non_alpha_as_upper, "$NumArgs"));
    EXPECT_EQ_STATIC("$n_args", rbox::to_snake_case(rbox::non_alpha_as_upper, "$NArgs"));

    EXPECT_EQ_STATIC("matrix_4x4", rbox::to_snake_case(rbox::non_alpha_as_lower, "MATRIX_4x4"));
    EXPECT_EQ_STATIC("matrix_4x4", rbox::to_snake_case(rbox::non_alpha_as_upper, "MATRIX_4X4"));
    EXPECT_EQ_STATIC(
        "is_ieee754_float", rbox::to_snake_case(rbox::non_alpha_as_upper, "IsIEEE754Float"));

    EXPECT_EQ_STATIC("num_args_", rbox::to_snake_case("numArgs_"));
    EXPECT_EQ_STATIC("num_args____", rbox::to_snake_case("numArgs_-_-"));
    EXPECT_EQ_STATIC("_noreturn", rbox::to_snake_case("_Noreturn"));
    EXPECT_EQ_STATIC("__no_return", rbox::to_snake_case("__NoReturn"));
    EXPECT_EQ_STATIC("__x__y__", rbox::to_snake_case("__X__Y__"));
    EXPECT_EQ_STATIC("___ab_c__d_ef___", rbox::to_snake_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC("__num_processes", rbox::to_snake_case("--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("_", rbox::to_snake_case("_"));
    EXPECT_EQ_STATIC("_", rbox::to_snake_case("-"));
    EXPECT_EQ_STATIC("____", rbox::to_snake_case("____"));
    EXPECT_EQ_STATIC("____", rbox::to_snake_case("----"));
    EXPECT_EQ_STATIC("________", rbox::to_snake_case("_--__--_"));

    // Invalid identifiers
    TEST_INVALID_IDENTIFIERS(to_snake_case);
}

TEST(UtilsIdentifierNaming, ToAllCapsSnakeCase)
{
    EXPECT_EQ_STATIC("GET_NAME", rbox::to_all_caps_snake_case("getName"));
    EXPECT_EQ_STATIC("SET_JSON_FILE_PATH", rbox::to_all_caps_snake_case("setJsonFilePath"));
    EXPECT_EQ_STATIC("SET_JSON_FILE_PATH", rbox::to_all_caps_snake_case("setJSONFilePath"));
    EXPECT_EQ_STATIC("VALUE_OFFSET", rbox::to_all_caps_snake_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC("YAML_PARSER", rbox::to_all_caps_snake_case("YamlParser"));
    EXPECT_EQ_STATIC("YAML_PARSER", rbox::to_all_caps_snake_case("YAMLParser"));
    EXPECT_EQ_STATIC("NUM_GC_THREADS", rbox::to_all_caps_snake_case("num-gcThreads"));
    EXPECT_EQ_STATIC("A", rbox::to_all_caps_snake_case("a"));
    EXPECT_EQ_STATIC("A", rbox::to_all_caps_snake_case("A"));
    EXPECT_EQ_STATIC("$", rbox::to_all_caps_snake_case("$"));

    EXPECT_EQ_STATIC("SNAKE_CASE", rbox::to_all_caps_snake_case("snake_case"));
    EXPECT_EQ_STATIC("ALL_CAPS_SNAKE_CASE", rbox::to_all_caps_snake_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("KEBAB_CASE", rbox::to_all_caps_snake_case("kebab-case"));
    EXPECT_EQ_STATIC("ALL_CAPS_KEBAB_CASE", rbox::to_all_caps_snake_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("LOWER_CAMEL_CASE", rbox::to_all_caps_snake_case("lowerCamelCase"));
    EXPECT_EQ_STATIC("UPPER_CAMEL_CASE", rbox::to_all_caps_snake_case("UpperCamelCase"));
    EXPECT_EQ_STATIC(
        "LOWER_CAMEL_SNAKE_CASE", rbox::to_all_caps_snake_case("lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(
        "UPPER_CAMEL_SNAKE_CASE", rbox::to_all_caps_snake_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("HTTP_HEADER_CASE", rbox::to_all_caps_snake_case("HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("GET_X1_AND_Y1", rbox::to_all_caps_snake_case("getX1AndY1"));
    EXPECT_EQ_STATIC("X1Y2", rbox::to_all_caps_snake_case("x1y2"));
    EXPECT_EQ_STATIC("X1Y2", rbox::to_all_caps_snake_case("X1y2"));
    EXPECT_EQ_STATIC("X1_Y2", rbox::to_all_caps_snake_case("x1Y2"));
    EXPECT_EQ_STATIC("X1_Y2", rbox::to_all_caps_snake_case("X1Y2"));
    EXPECT_EQ_STATIC("$NUM_ARGS", rbox::to_all_caps_snake_case("$numArgs"));
    EXPECT_EQ_STATIC("$_NUM_ARGS", rbox::to_all_caps_snake_case("$NumArgs"));
    EXPECT_EQ_STATIC("$_N_ARGS", rbox::to_all_caps_snake_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(
        "GET_X1_AND_Y1", rbox::to_all_caps_snake_case(rbox::non_alpha_as_upper, "getX1AndY1"));
    EXPECT_EQ_STATIC("X_1Y_2", rbox::to_all_caps_snake_case(rbox::non_alpha_as_upper, "x1y2"));
    EXPECT_EQ_STATIC("X_1Y_2", rbox::to_all_caps_snake_case(rbox::non_alpha_as_upper, "X1y2"));
    EXPECT_EQ_STATIC("X_1Y2", rbox::to_all_caps_snake_case(rbox::non_alpha_as_upper, "x1Y2"));
    EXPECT_EQ_STATIC("X1Y2", rbox::to_all_caps_snake_case(rbox::non_alpha_as_upper, "X1Y2"));
    EXPECT_EQ_STATIC(
        "$NUM_ARGS", rbox::to_all_caps_snake_case(rbox::non_alpha_as_upper, "$numArgs"));
    EXPECT_EQ_STATIC(
        "$_NUM_ARGS", rbox::to_all_caps_snake_case(rbox::non_alpha_as_upper, "$NumArgs"));
    EXPECT_EQ_STATIC("$N_ARGS", rbox::to_all_caps_snake_case(rbox::non_alpha_as_upper, "$NArgs"));

    EXPECT_EQ_STATIC(
        "MATRIX_4X4", rbox::to_all_caps_snake_case(rbox::non_alpha_as_lower, "MATRIX_4x4"));
    EXPECT_EQ_STATIC(
        "MATRIX_4X4", rbox::to_all_caps_snake_case(rbox::non_alpha_as_upper, "MATRIX_4X4"));
    EXPECT_EQ_STATIC(
        "IS_IEEE754_FLOAT",
        rbox::to_all_caps_snake_case(rbox::non_alpha_as_upper, "IsIEEE754Float"));

    EXPECT_EQ_STATIC("NUM_ARGS_", rbox::to_all_caps_snake_case("numArgs_"));
    EXPECT_EQ_STATIC("_NORETURN", rbox::to_all_caps_snake_case("_Noreturn"));
    EXPECT_EQ_STATIC("__NO_RETURN", rbox::to_all_caps_snake_case("__NoReturn"));
    EXPECT_EQ_STATIC("__X__Y__", rbox::to_all_caps_snake_case("__X__Y__"));
    EXPECT_EQ_STATIC("___AB_C__D_EF___", rbox::to_all_caps_snake_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC("__NUM_PROCESSES", rbox::to_all_caps_snake_case("--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("_", rbox::to_all_caps_snake_case("_"));
    EXPECT_EQ_STATIC("_", rbox::to_all_caps_snake_case("-"));
    EXPECT_EQ_STATIC("____", rbox::to_all_caps_snake_case("____"));
    EXPECT_EQ_STATIC("____", rbox::to_all_caps_snake_case("----"));
    EXPECT_EQ_STATIC("________", rbox::to_all_caps_snake_case("_--__--_"));

    // Invalid identifiers
    TEST_INVALID_IDENTIFIERS(to_all_caps_snake_case);
}

TEST(UtilsIdentifierNaming, ToKebabCase)
{
    EXPECT_EQ_STATIC("get-name", rbox::to_kebab_case("getName"));
    EXPECT_EQ_STATIC("set-json-file-path", rbox::to_kebab_case("setJsonFilePath"));
    EXPECT_EQ_STATIC("set-json-file-path", rbox::to_kebab_case("setJSONFilePath"));
    EXPECT_EQ_STATIC("value-offset", rbox::to_kebab_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC("yaml-parser", rbox::to_kebab_case("YamlParser"));
    EXPECT_EQ_STATIC("yaml-parser", rbox::to_kebab_case("YAMLParser"));
    EXPECT_EQ_STATIC("num-gc-threads", rbox::to_kebab_case("num-gcThreads"));
    EXPECT_EQ_STATIC("a", rbox::to_kebab_case("a"));
    EXPECT_EQ_STATIC("a", rbox::to_kebab_case("A"));
    EXPECT_EQ_STATIC("$", rbox::to_kebab_case("$"));

    EXPECT_EQ_STATIC("snake-case", rbox::to_kebab_case("snake_case"));
    EXPECT_EQ_STATIC("all-caps-snake-case", rbox::to_kebab_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("kebab-case", rbox::to_kebab_case("kebab-case"));
    EXPECT_EQ_STATIC("all-caps-kebab-case", rbox::to_kebab_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("lower-camel-case", rbox::to_kebab_case("lowerCamelCase"));
    EXPECT_EQ_STATIC("upper-camel-case", rbox::to_kebab_case("UpperCamelCase"));
    EXPECT_EQ_STATIC("lower-camel-snake-case", rbox::to_kebab_case("lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("upper-camel-snake-case", rbox::to_kebab_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("http-header-case", rbox::to_kebab_case("HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("get-x1-and-y1", rbox::to_kebab_case("getX1AndY1"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_kebab_case("x1y2"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_kebab_case("X1y2"));
    EXPECT_EQ_STATIC("x1-y2", rbox::to_kebab_case("x1Y2"));
    EXPECT_EQ_STATIC("x1-y2", rbox::to_kebab_case("X1Y2"));
    EXPECT_EQ_STATIC("$num-args", rbox::to_kebab_case("$numArgs"));
    EXPECT_EQ_STATIC("$-num-args", rbox::to_kebab_case("$NumArgs"));
    EXPECT_EQ_STATIC("$-n-args", rbox::to_kebab_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC("get-x1-and-y1", rbox::to_kebab_case(rbox::non_alpha_as_upper, "getX1AndY1"));
    EXPECT_EQ_STATIC("x-1y-2", rbox::to_kebab_case(rbox::non_alpha_as_upper, "x1y2"));
    EXPECT_EQ_STATIC("x-1y-2", rbox::to_kebab_case(rbox::non_alpha_as_upper, "X1y2"));
    EXPECT_EQ_STATIC("x-1y2", rbox::to_kebab_case(rbox::non_alpha_as_upper, "x1Y2"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_kebab_case(rbox::non_alpha_as_upper, "X1Y2"));
    EXPECT_EQ_STATIC("$num-args", rbox::to_kebab_case(rbox::non_alpha_as_upper, "$numArgs"));
    EXPECT_EQ_STATIC("$-num-args", rbox::to_kebab_case(rbox::non_alpha_as_upper, "$NumArgs"));
    EXPECT_EQ_STATIC("$n-args", rbox::to_kebab_case(rbox::non_alpha_as_upper, "$NArgs"));

    EXPECT_EQ_STATIC("matrix-4x4", rbox::to_kebab_case(rbox::non_alpha_as_lower, "MATRIX_4x4"));
    EXPECT_EQ_STATIC("matrix-4x4", rbox::to_kebab_case(rbox::non_alpha_as_upper, "MATRIX_4X4"));
    EXPECT_EQ_STATIC(
        "is-ieee754-float", rbox::to_kebab_case(rbox::non_alpha_as_upper, "IsIEEE754Float"));

    EXPECT_EQ_STATIC("num-args-", rbox::to_kebab_case("numArgs_"));
    EXPECT_EQ_STATIC("-noreturn", rbox::to_kebab_case("_Noreturn"));
    EXPECT_EQ_STATIC("--no-return", rbox::to_kebab_case("__NoReturn"));
    EXPECT_EQ_STATIC("--x--y--", rbox::to_kebab_case("__X__Y__"));
    EXPECT_EQ_STATIC("---ab-c--d-ef---", rbox::to_kebab_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC("--num-processes", rbox::to_kebab_case("--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("-", rbox::to_kebab_case("_"));
    EXPECT_EQ_STATIC("-", rbox::to_kebab_case("-"));
    EXPECT_EQ_STATIC("----", rbox::to_kebab_case("____"));
    EXPECT_EQ_STATIC("----", rbox::to_kebab_case("----"));
    EXPECT_EQ_STATIC("--------", rbox::to_kebab_case("_--__--_"));

    // Invalid identifiers
    TEST_INVALID_IDENTIFIERS(to_kebab_case);
}

TEST(UtilsIdentifierNaming, ToAllCapsKebabCase)
{
    EXPECT_EQ_STATIC("GET-NAME", rbox::to_all_caps_kebab_case("getName"));
    EXPECT_EQ_STATIC("SET-JSON-FILE-PATH", rbox::to_all_caps_kebab_case("setJsonFilePath"));
    EXPECT_EQ_STATIC("SET-JSON-FILE-PATH", rbox::to_all_caps_kebab_case("setJSONFilePath"));
    EXPECT_EQ_STATIC("VALUE-OFFSET", rbox::to_all_caps_kebab_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC("YAML-PARSER", rbox::to_all_caps_kebab_case("YamlParser"));
    EXPECT_EQ_STATIC("YAML-PARSER", rbox::to_all_caps_kebab_case("YAMLParser"));
    EXPECT_EQ_STATIC("NUM-GC-THREADS", rbox::to_all_caps_kebab_case("num-gcThreads"));
    EXPECT_EQ_STATIC("A", rbox::to_all_caps_kebab_case("a"));
    EXPECT_EQ_STATIC("A", rbox::to_all_caps_kebab_case("A"));
    EXPECT_EQ_STATIC("$", rbox::to_all_caps_kebab_case("$"));

    EXPECT_EQ_STATIC("SNAKE-CASE", rbox::to_all_caps_kebab_case("snake_case"));
    EXPECT_EQ_STATIC("ALL-CAPS-SNAKE-CASE", rbox::to_all_caps_kebab_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("KEBAB-CASE", rbox::to_all_caps_kebab_case("kebab-case"));
    EXPECT_EQ_STATIC("ALL-CAPS-KEBAB-CASE", rbox::to_all_caps_kebab_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("LOWER-CAMEL-CASE", rbox::to_all_caps_kebab_case("lowerCamelCase"));
    EXPECT_EQ_STATIC("UPPER-CAMEL-CASE", rbox::to_all_caps_kebab_case("UpperCamelCase"));
    EXPECT_EQ_STATIC(
        "LOWER-CAMEL-SNAKE-CASE", rbox::to_all_caps_kebab_case("lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(
        "UPPER-CAMEL-SNAKE-CASE", rbox::to_all_caps_kebab_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("HTTP-HEADER-CASE", rbox::to_all_caps_kebab_case("HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("GET-X1-AND-Y1", rbox::to_all_caps_kebab_case("getX1AndY1"));
    EXPECT_EQ_STATIC("X1Y2", rbox::to_all_caps_kebab_case("x1y2"));
    EXPECT_EQ_STATIC("X1Y2", rbox::to_all_caps_kebab_case("X1y2"));
    EXPECT_EQ_STATIC("X1-Y2", rbox::to_all_caps_kebab_case("x1Y2"));
    EXPECT_EQ_STATIC("X1-Y2", rbox::to_all_caps_kebab_case("X1Y2"));
    EXPECT_EQ_STATIC("$NUM-ARGS", rbox::to_all_caps_kebab_case("$numArgs"));
    EXPECT_EQ_STATIC("$-NUM-ARGS", rbox::to_all_caps_kebab_case("$NumArgs"));
    EXPECT_EQ_STATIC("$-N-ARGS", rbox::to_all_caps_kebab_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(
        "GET-X1-AND-Y1", rbox::to_all_caps_kebab_case(rbox::non_alpha_as_upper, "getX1AndY1"));
    EXPECT_EQ_STATIC("X-1Y-2", rbox::to_all_caps_kebab_case(rbox::non_alpha_as_upper, "x1y2"));
    EXPECT_EQ_STATIC("X-1Y-2", rbox::to_all_caps_kebab_case(rbox::non_alpha_as_upper, "X1y2"));
    EXPECT_EQ_STATIC("X-1Y2", rbox::to_all_caps_kebab_case(rbox::non_alpha_as_upper, "x1Y2"));
    EXPECT_EQ_STATIC("X1Y2", rbox::to_all_caps_kebab_case(rbox::non_alpha_as_upper, "X1Y2"));
    EXPECT_EQ_STATIC(
        "$NUM-ARGS", rbox::to_all_caps_kebab_case(rbox::non_alpha_as_upper, "$numArgs"));
    EXPECT_EQ_STATIC(
        "$-NUM-ARGS", rbox::to_all_caps_kebab_case(rbox::non_alpha_as_upper, "$NumArgs"));
    EXPECT_EQ_STATIC("$N-ARGS", rbox::to_all_caps_kebab_case(rbox::non_alpha_as_upper, "$NArgs"));

    EXPECT_EQ_STATIC(
        "MATRIX-4X4", rbox::to_all_caps_kebab_case(rbox::non_alpha_as_lower, "MATRIX_4x4"));
    EXPECT_EQ_STATIC(
        "MATRIX-4X4", rbox::to_all_caps_kebab_case(rbox::non_alpha_as_upper, "MATRIX_4X4"));
    EXPECT_EQ_STATIC(
        "IS-IEEE754-FLOAT",
        rbox::to_all_caps_kebab_case(rbox::non_alpha_as_upper, "IsIEEE754Float"));

    EXPECT_EQ_STATIC("NUM-ARGS-", rbox::to_all_caps_kebab_case("numArgs_"));
    EXPECT_EQ_STATIC("-NORETURN", rbox::to_all_caps_kebab_case("_Noreturn"));
    EXPECT_EQ_STATIC("--NO-RETURN", rbox::to_all_caps_kebab_case("__NoReturn"));
    EXPECT_EQ_STATIC("--X--Y--", rbox::to_all_caps_kebab_case("__X__Y__"));
    EXPECT_EQ_STATIC("---AB-C--D-EF---", rbox::to_all_caps_kebab_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC("--NUM-PROCESSES", rbox::to_all_caps_kebab_case("--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("-", rbox::to_all_caps_kebab_case("_"));
    EXPECT_EQ_STATIC("-", rbox::to_all_caps_kebab_case("-"));
    EXPECT_EQ_STATIC("----", rbox::to_all_caps_kebab_case("____"));
    EXPECT_EQ_STATIC("----", rbox::to_all_caps_kebab_case("----"));
    EXPECT_EQ_STATIC("--------", rbox::to_all_caps_kebab_case("_--__--_"));

    // Invalid identifiers
    TEST_INVALID_IDENTIFIERS(to_all_caps_kebab_case);
}

TEST(UtilsIdentifierNaming, ToLowerCamelCase)
{
    EXPECT_EQ_STATIC("getName", rbox::to_lower_camel_case("getName"));
    EXPECT_EQ_STATIC("setJsonFilePath", rbox::to_lower_camel_case("setJsonFilePath"));
    // Note: irreversible convertion
    EXPECT_EQ_STATIC("setJsonFilePath", rbox::to_lower_camel_case("setJSONFilePath"));
    EXPECT_EQ_STATIC("valueOffset", rbox::to_lower_camel_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC("yamlParser", rbox::to_lower_camel_case("YamlParser"));
    EXPECT_EQ_STATIC("yamlParser", rbox::to_lower_camel_case("YAMLParser"));
    EXPECT_EQ_STATIC("numGcThreads", rbox::to_lower_camel_case("num-gcThreads"));
    EXPECT_EQ_STATIC("a", rbox::to_lower_camel_case("a"));
    EXPECT_EQ_STATIC("a", rbox::to_lower_camel_case("A"));
    EXPECT_EQ_STATIC("$", rbox::to_lower_camel_case("$"));

    EXPECT_EQ_STATIC("snakeCase", rbox::to_lower_camel_case("snake_case"));
    EXPECT_EQ_STATIC("allCapsSnakeCase", rbox::to_lower_camel_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("kebabCase", rbox::to_lower_camel_case("kebab-case"));
    EXPECT_EQ_STATIC("allCapsKebabCase", rbox::to_lower_camel_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("lowerCamelCase", rbox::to_lower_camel_case("lowerCamelCase"));
    EXPECT_EQ_STATIC("upperCamelCase", rbox::to_lower_camel_case("UpperCamelCase"));
    EXPECT_EQ_STATIC("lowerCamelSnakeCase", rbox::to_lower_camel_case("lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("upperCamelSnakeCase", rbox::to_lower_camel_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("httpHeaderCase", rbox::to_lower_camel_case("HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("getX1AndY1", rbox::to_lower_camel_case("getX1AndY1"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_lower_camel_case("x1y2"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_lower_camel_case("X1y2"));
    EXPECT_EQ_STATIC("x1Y2", rbox::to_lower_camel_case("x1Y2"));
    EXPECT_EQ_STATIC("x1Y2", rbox::to_lower_camel_case("X1Y2"));
    EXPECT_EQ_STATIC("$numArgs", rbox::to_lower_camel_case("$numArgs"));
    EXPECT_EQ_STATIC("$NumArgs", rbox::to_lower_camel_case("$NumArgs"));
    EXPECT_EQ_STATIC("$NArgs", rbox::to_lower_camel_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(
        "getX1AndY1", rbox::to_lower_camel_case(rbox::non_alpha_as_upper, "getX1AndY1"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_lower_camel_case(rbox::non_alpha_as_upper, "x1y2"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_lower_camel_case(rbox::non_alpha_as_upper, "X1y2"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_lower_camel_case(rbox::non_alpha_as_upper, "x1Y2"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_lower_camel_case(rbox::non_alpha_as_upper, "X1Y2"));
    EXPECT_EQ_STATIC("$numArgs", rbox::to_lower_camel_case(rbox::non_alpha_as_upper, "$numArgs"));
    EXPECT_EQ_STATIC("$NumArgs", rbox::to_lower_camel_case(rbox::non_alpha_as_upper, "$NumArgs"));
    EXPECT_EQ_STATIC("$nArgs", rbox::to_lower_camel_case(rbox::non_alpha_as_upper, "$NArgs"));

    EXPECT_EQ_STATIC(
        "matrix4x4", rbox::to_lower_camel_case(rbox::non_alpha_as_lower, "MATRIX_4x4"));
    EXPECT_EQ_STATIC(
        "matrix4x4", rbox::to_lower_camel_case(rbox::non_alpha_as_upper, "MATRIX_4X4"));
    // Note: irreversible
    EXPECT_EQ_STATIC(
        "isIeee754Float", rbox::to_lower_camel_case(rbox::non_alpha_as_upper, "IsIEEE754Float"));

    EXPECT_EQ_STATIC("numArgs_", rbox::to_lower_camel_case("numArgs_"));
    EXPECT_EQ_STATIC("_noreturn", rbox::to_lower_camel_case("_Noreturn"));
    EXPECT_EQ_STATIC("__noReturn", rbox::to_lower_camel_case("__NoReturn"));
    EXPECT_EQ_STATIC("__x_Y__", rbox::to_lower_camel_case("__X__Y__"));
    EXPECT_EQ_STATIC("___abC_DEf___", rbox::to_lower_camel_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC("__numProcesses", rbox::to_lower_camel_case("--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("_", rbox::to_lower_camel_case("_"));
    EXPECT_EQ_STATIC("_", rbox::to_lower_camel_case("-"));
    EXPECT_EQ_STATIC("____", rbox::to_lower_camel_case("____"));
    EXPECT_EQ_STATIC("____", rbox::to_lower_camel_case("----"));
    EXPECT_EQ_STATIC("________", rbox::to_lower_camel_case("_--__--_"));

    // Invalid identifiers
    TEST_INVALID_IDENTIFIERS(to_lower_camel_case);
}

TEST(UtilsIdentifierNaming, ToUpperCamelCase)
{
    EXPECT_EQ_STATIC("GetName", rbox::to_upper_camel_case("getName"));
    EXPECT_EQ_STATIC("SetJsonFilePath", rbox::to_upper_camel_case("setJsonFilePath"));
    // Note: irreversible convertion
    EXPECT_EQ_STATIC("SetJsonFilePath", rbox::to_upper_camel_case("setJSONFilePath"));
    EXPECT_EQ_STATIC("ValueOffset", rbox::to_upper_camel_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC("YamlParser", rbox::to_upper_camel_case("YamlParser"));
    EXPECT_EQ_STATIC("YamlParser", rbox::to_upper_camel_case("YAMLParser"));
    EXPECT_EQ_STATIC("NumGcThreads", rbox::to_upper_camel_case("num-gcThreads"));
    EXPECT_EQ_STATIC("A", rbox::to_upper_camel_case("a"));
    EXPECT_EQ_STATIC("A", rbox::to_upper_camel_case("A"));
    EXPECT_EQ_STATIC("$", rbox::to_upper_camel_case("$"));

    EXPECT_EQ_STATIC("SnakeCase", rbox::to_upper_camel_case("snake_case"));
    EXPECT_EQ_STATIC("AllCapsSnakeCase", rbox::to_upper_camel_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("KebabCase", rbox::to_upper_camel_case("kebab-case"));
    EXPECT_EQ_STATIC("AllCapsKebabCase", rbox::to_upper_camel_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("LowerCamelCase", rbox::to_upper_camel_case("lowerCamelCase"));
    EXPECT_EQ_STATIC("UpperCamelCase", rbox::to_upper_camel_case("UpperCamelCase"));
    EXPECT_EQ_STATIC("LowerCamelSnakeCase", rbox::to_upper_camel_case("lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("UpperCamelSnakeCase", rbox::to_upper_camel_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("HttpHeaderCase", rbox::to_upper_camel_case("HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("GetX1AndY1", rbox::to_upper_camel_case("getX1AndY1"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_upper_camel_case("x1y2"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_upper_camel_case("X1y2"));
    EXPECT_EQ_STATIC("X1Y2", rbox::to_upper_camel_case("x1Y2"));
    EXPECT_EQ_STATIC("X1Y2", rbox::to_upper_camel_case("X1Y2"));
    EXPECT_EQ_STATIC("$numArgs", rbox::to_upper_camel_case("$numArgs"));
    EXPECT_EQ_STATIC("$NumArgs", rbox::to_upper_camel_case("$NumArgs"));
    EXPECT_EQ_STATIC("$NArgs", rbox::to_upper_camel_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(
        "GetX1AndY1", rbox::to_upper_camel_case(rbox::non_alpha_as_upper, "getX1AndY1"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_upper_camel_case(rbox::non_alpha_as_upper, "x1y2"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_upper_camel_case(rbox::non_alpha_as_upper, "X1y2"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_upper_camel_case(rbox::non_alpha_as_upper, "x1Y2"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_upper_camel_case(rbox::non_alpha_as_upper, "X1Y2"));
    EXPECT_EQ_STATIC("$numArgs", rbox::to_upper_camel_case(rbox::non_alpha_as_upper, "$numArgs"));
    EXPECT_EQ_STATIC("$NumArgs", rbox::to_upper_camel_case(rbox::non_alpha_as_upper, "$NumArgs"));
    EXPECT_EQ_STATIC("$nArgs", rbox::to_upper_camel_case(rbox::non_alpha_as_upper, "$NArgs"));

    EXPECT_EQ_STATIC(
        "Matrix4x4", rbox::to_upper_camel_case(rbox::non_alpha_as_lower, "MATRIX_4x4"));
    EXPECT_EQ_STATIC(
        "Matrix4x4", rbox::to_upper_camel_case(rbox::non_alpha_as_upper, "MATRIX_4X4"));
    // Note: irreversible
    EXPECT_EQ_STATIC(
        "IsIeee754Float", rbox::to_upper_camel_case(rbox::non_alpha_as_upper, "IsIEEE754Float"));

    EXPECT_EQ_STATIC("NumArgs_", rbox::to_upper_camel_case("numArgs_"));
    EXPECT_EQ_STATIC("_Noreturn", rbox::to_upper_camel_case("_Noreturn"));
    EXPECT_EQ_STATIC("__NoReturn", rbox::to_upper_camel_case("__NoReturn"));
    EXPECT_EQ_STATIC("__X_Y__", rbox::to_upper_camel_case("__X__Y__"));
    EXPECT_EQ_STATIC("___AbC_DEf___", rbox::to_upper_camel_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC("__NumProcesses", rbox::to_upper_camel_case("--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("_", rbox::to_upper_camel_case("_"));
    EXPECT_EQ_STATIC("_", rbox::to_upper_camel_case("-"));
    EXPECT_EQ_STATIC("____", rbox::to_upper_camel_case("____"));
    EXPECT_EQ_STATIC("____", rbox::to_upper_camel_case("----"));
    EXPECT_EQ_STATIC("________", rbox::to_upper_camel_case("_--__--_"));

    // Invalid identifiers
    TEST_INVALID_IDENTIFIERS(to_upper_camel_case);
}

TEST(UtilsIdentifierNaming, ToLowerCamelSnakeCase)
{
    EXPECT_EQ_STATIC("get_Name", rbox::to_lower_camel_snake_case("getName"));
    EXPECT_EQ_STATIC("set_Json_File_Path", rbox::to_lower_camel_snake_case("setJsonFilePath"));
    // Note: irreversible convertion
    EXPECT_EQ_STATIC("set_Json_File_Path", rbox::to_lower_camel_snake_case("setJSONFilePath"));
    EXPECT_EQ_STATIC("value_Offset", rbox::to_lower_camel_snake_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC("yaml_Parser", rbox::to_lower_camel_snake_case("YamlParser"));
    EXPECT_EQ_STATIC("yaml_Parser", rbox::to_lower_camel_snake_case("YAMLParser"));
    EXPECT_EQ_STATIC("num_Gc_Threads", rbox::to_lower_camel_snake_case("num-gcThreads"));
    EXPECT_EQ_STATIC("a", rbox::to_lower_camel_snake_case("a"));
    EXPECT_EQ_STATIC("a", rbox::to_lower_camel_snake_case("A"));
    EXPECT_EQ_STATIC("$", rbox::to_lower_camel_snake_case("$"));

    EXPECT_EQ_STATIC("snake_Case", rbox::to_lower_camel_snake_case("snake_case"));
    EXPECT_EQ_STATIC("all_Caps_Snake_Case", rbox::to_lower_camel_snake_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("kebab_Case", rbox::to_lower_camel_snake_case("kebab-case"));
    EXPECT_EQ_STATIC("all_Caps_Kebab_Case", rbox::to_lower_camel_snake_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("lower_Camel_Case", rbox::to_lower_camel_snake_case("lowerCamelCase"));
    EXPECT_EQ_STATIC("upper_Camel_Case", rbox::to_lower_camel_snake_case("UpperCamelCase"));
    EXPECT_EQ_STATIC(
        "lower_Camel_Snake_Case", rbox::to_lower_camel_snake_case("lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(
        "upper_Camel_Snake_Case", rbox::to_lower_camel_snake_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("http_Header_Case", rbox::to_lower_camel_snake_case("HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("get_X1_And_Y1", rbox::to_lower_camel_snake_case("getX1AndY1"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_lower_camel_snake_case("x1y2"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_lower_camel_snake_case("X1y2"));
    EXPECT_EQ_STATIC("x1_Y2", rbox::to_lower_camel_snake_case("x1Y2"));
    EXPECT_EQ_STATIC("x1_Y2", rbox::to_lower_camel_snake_case("X1Y2"));
    EXPECT_EQ_STATIC("$num_Args", rbox::to_lower_camel_snake_case("$numArgs"));
    EXPECT_EQ_STATIC("$_Num_Args", rbox::to_lower_camel_snake_case("$NumArgs"));
    EXPECT_EQ_STATIC("$_N_Args", rbox::to_lower_camel_snake_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(
        "get_X1_And_Y1", rbox::to_lower_camel_snake_case(rbox::non_alpha_as_upper, "getX1AndY1"));
    EXPECT_EQ_STATIC("x_1y_2", rbox::to_lower_camel_snake_case(rbox::non_alpha_as_upper, "x1y2"));
    EXPECT_EQ_STATIC("x_1y_2", rbox::to_lower_camel_snake_case(rbox::non_alpha_as_upper, "X1y2"));
    EXPECT_EQ_STATIC("x_1y2", rbox::to_lower_camel_snake_case(rbox::non_alpha_as_upper, "x1Y2"));
    EXPECT_EQ_STATIC("x1y2", rbox::to_lower_camel_snake_case(rbox::non_alpha_as_upper, "X1Y2"));
    EXPECT_EQ_STATIC(
        "$num_Args", rbox::to_lower_camel_snake_case(rbox::non_alpha_as_upper, "$numArgs"));
    EXPECT_EQ_STATIC(
        "$_Num_Args", rbox::to_lower_camel_snake_case(rbox::non_alpha_as_upper, "$NumArgs"));
    EXPECT_EQ_STATIC(
        "$n_Args", rbox::to_lower_camel_snake_case(rbox::non_alpha_as_upper, "$NArgs"));

    EXPECT_EQ_STATIC(
        "matrix_4x4", rbox::to_lower_camel_snake_case(rbox::non_alpha_as_lower, "MATRIX_4x4"));
    EXPECT_EQ_STATIC(
        "matrix_4x4", rbox::to_lower_camel_snake_case(rbox::non_alpha_as_upper, "MATRIX_4X4"));
    // Note: irreversible
    EXPECT_EQ_STATIC(
        "is_Ieee754_Float",
        rbox::to_lower_camel_snake_case(rbox::non_alpha_as_upper, "IsIEEE754Float"));

    EXPECT_EQ_STATIC("num_Args_", rbox::to_lower_camel_snake_case("numArgs_"));
    EXPECT_EQ_STATIC("_noreturn", rbox::to_lower_camel_snake_case("_Noreturn"));
    EXPECT_EQ_STATIC("__no_Return", rbox::to_lower_camel_snake_case("__NoReturn"));
    EXPECT_EQ_STATIC("__x__Y__", rbox::to_lower_camel_snake_case("__X__Y__"));
    EXPECT_EQ_STATIC("___ab_C__D_Ef___", rbox::to_lower_camel_snake_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC("__num_Processes", rbox::to_lower_camel_snake_case("--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("_", rbox::to_lower_camel_snake_case("_"));
    EXPECT_EQ_STATIC("_", rbox::to_lower_camel_snake_case("-"));
    EXPECT_EQ_STATIC("____", rbox::to_lower_camel_snake_case("____"));
    EXPECT_EQ_STATIC("____", rbox::to_lower_camel_snake_case("----"));
    EXPECT_EQ_STATIC("________", rbox::to_lower_camel_snake_case("_--__--_"));

    // Invalid identifiers
    TEST_INVALID_IDENTIFIERS(to_lower_camel_snake_case);
}

TEST(UtilsIdentifierNaming, ToUpperCamelSnakeCase)
{
    EXPECT_EQ_STATIC("Get_Name", rbox::to_upper_camel_snake_case("getName"));
    EXPECT_EQ_STATIC("Set_Json_File_Path", rbox::to_upper_camel_snake_case("setJsonFilePath"));
    // Note: irreversible convertion
    EXPECT_EQ_STATIC("Set_Json_File_Path", rbox::to_upper_camel_snake_case("setJSONFilePath"));
    EXPECT_EQ_STATIC("Value_Offset", rbox::to_upper_camel_snake_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC("Yaml_Parser", rbox::to_upper_camel_snake_case("YamlParser"));
    EXPECT_EQ_STATIC("Yaml_Parser", rbox::to_upper_camel_snake_case("YAMLParser"));
    EXPECT_EQ_STATIC("Num_Gc_Threads", rbox::to_upper_camel_snake_case("num-gcThreads"));
    EXPECT_EQ_STATIC("A", rbox::to_upper_camel_snake_case("a"));
    EXPECT_EQ_STATIC("A", rbox::to_upper_camel_snake_case("A"));
    EXPECT_EQ_STATIC("$", rbox::to_upper_camel_snake_case("$"));

    EXPECT_EQ_STATIC("Snake_Case", rbox::to_upper_camel_snake_case("snake_case"));
    EXPECT_EQ_STATIC("All_Caps_Snake_Case", rbox::to_upper_camel_snake_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("Kebab_Case", rbox::to_upper_camel_snake_case("kebab-case"));
    EXPECT_EQ_STATIC("All_Caps_Kebab_Case", rbox::to_upper_camel_snake_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("Lower_Camel_Case", rbox::to_upper_camel_snake_case("lowerCamelCase"));
    EXPECT_EQ_STATIC("Upper_Camel_Case", rbox::to_upper_camel_snake_case("UpperCamelCase"));
    EXPECT_EQ_STATIC(
        "Lower_Camel_Snake_Case", rbox::to_upper_camel_snake_case("lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(
        "Upper_Camel_Snake_Case", rbox::to_upper_camel_snake_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("Http_Header_Case", rbox::to_upper_camel_snake_case("HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("Get_X1_And_Y1", rbox::to_upper_camel_snake_case("getX1AndY1"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_upper_camel_snake_case("x1y2"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_upper_camel_snake_case("X1y2"));
    EXPECT_EQ_STATIC("X1_Y2", rbox::to_upper_camel_snake_case("x1Y2"));
    EXPECT_EQ_STATIC("X1_Y2", rbox::to_upper_camel_snake_case("X1Y2"));
    EXPECT_EQ_STATIC("$num_Args", rbox::to_upper_camel_snake_case("$numArgs"));
    EXPECT_EQ_STATIC("$_Num_Args", rbox::to_upper_camel_snake_case("$NumArgs"));
    EXPECT_EQ_STATIC("$_N_Args", rbox::to_upper_camel_snake_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(
        "Get_X1_And_Y1", rbox::to_upper_camel_snake_case(rbox::non_alpha_as_upper, "getX1AndY1"));
    EXPECT_EQ_STATIC("X_1y_2", rbox::to_upper_camel_snake_case(rbox::non_alpha_as_upper, "x1y2"));
    EXPECT_EQ_STATIC("X_1y_2", rbox::to_upper_camel_snake_case(rbox::non_alpha_as_upper, "X1y2"));
    EXPECT_EQ_STATIC("X_1y2", rbox::to_upper_camel_snake_case(rbox::non_alpha_as_upper, "x1Y2"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_upper_camel_snake_case(rbox::non_alpha_as_upper, "X1Y2"));
    EXPECT_EQ_STATIC(
        "$num_Args", rbox::to_upper_camel_snake_case(rbox::non_alpha_as_upper, "$numArgs"));
    EXPECT_EQ_STATIC(
        "$_Num_Args", rbox::to_upper_camel_snake_case(rbox::non_alpha_as_upper, "$NumArgs"));
    EXPECT_EQ_STATIC(
        "$n_Args", rbox::to_upper_camel_snake_case(rbox::non_alpha_as_upper, "$NArgs"));

    EXPECT_EQ_STATIC(
        "Matrix_4x4", rbox::to_upper_camel_snake_case(rbox::non_alpha_as_lower, "MATRIX_4x4"));
    EXPECT_EQ_STATIC(
        "Matrix_4x4", rbox::to_upper_camel_snake_case(rbox::non_alpha_as_upper, "MATRIX_4X4"));
    // Note: irreversible
    EXPECT_EQ_STATIC(
        "Is_Ieee754_Float",
        rbox::to_upper_camel_snake_case(rbox::non_alpha_as_upper, "IsIEEE754Float"));

    EXPECT_EQ_STATIC("Num_Args_", rbox::to_upper_camel_snake_case("numArgs_"));
    EXPECT_EQ_STATIC("_Noreturn", rbox::to_upper_camel_snake_case("_Noreturn"));
    EXPECT_EQ_STATIC("__No_Return", rbox::to_upper_camel_snake_case("__NoReturn"));
    EXPECT_EQ_STATIC("__X__Y__", rbox::to_upper_camel_snake_case("__X__Y__"));
    EXPECT_EQ_STATIC("___Ab_C__D_Ef___", rbox::to_upper_camel_snake_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC("__Num_Processes", rbox::to_upper_camel_snake_case("--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("_", rbox::to_upper_camel_snake_case("_"));
    EXPECT_EQ_STATIC("_", rbox::to_upper_camel_snake_case("-"));
    EXPECT_EQ_STATIC("____", rbox::to_upper_camel_snake_case("____"));
    EXPECT_EQ_STATIC("____", rbox::to_upper_camel_snake_case("----"));
    EXPECT_EQ_STATIC("________", rbox::to_upper_camel_snake_case("_--__--_"));

    // Invalid identifiers
    TEST_INVALID_IDENTIFIERS(to_upper_camel_snake_case);
}

TEST(UtilsIdentifierNaming, ToHttpHeaderCase)
{
    EXPECT_EQ_STATIC("Get-Name", rbox::to_http_header_case("getName"));
    EXPECT_EQ_STATIC("Set-Json-File-Path", rbox::to_http_header_case("setJsonFilePath"));
    // Note: irreversible convertion
    EXPECT_EQ_STATIC("Set-Json-File-Path", rbox::to_http_header_case("setJSONFilePath"));
    EXPECT_EQ_STATIC("Value-Offset", rbox::to_http_header_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC("Yaml-Parser", rbox::to_http_header_case("YamlParser"));
    EXPECT_EQ_STATIC("Yaml-Parser", rbox::to_http_header_case("YAMLParser"));
    EXPECT_EQ_STATIC("Num-Gc-Threads", rbox::to_http_header_case("num-gcThreads"));
    EXPECT_EQ_STATIC("A", rbox::to_http_header_case("a"));
    EXPECT_EQ_STATIC("A", rbox::to_http_header_case("A"));
    EXPECT_EQ_STATIC("$", rbox::to_http_header_case("$"));

    EXPECT_EQ_STATIC("Snake-Case", rbox::to_http_header_case("snake_case"));
    EXPECT_EQ_STATIC("All-Caps-Snake-Case", rbox::to_http_header_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("Kebab-Case", rbox::to_http_header_case("kebab-case"));
    EXPECT_EQ_STATIC("All-Caps-Kebab-Case", rbox::to_http_header_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("Lower-Camel-Case", rbox::to_http_header_case("lowerCamelCase"));
    EXPECT_EQ_STATIC("Upper-Camel-Case", rbox::to_http_header_case("UpperCamelCase"));
    EXPECT_EQ_STATIC("Lower-Camel-Snake-Case", rbox::to_http_header_case("lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("Upper-Camel-Snake-Case", rbox::to_http_header_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("Http-Header-Case", rbox::to_http_header_case("HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("Get-X1-And-Y1", rbox::to_http_header_case("getX1AndY1"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_http_header_case("x1y2"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_http_header_case("X1y2"));
    EXPECT_EQ_STATIC("X1-Y2", rbox::to_http_header_case("x1Y2"));
    EXPECT_EQ_STATIC("X1-Y2", rbox::to_http_header_case("X1Y2"));
    EXPECT_EQ_STATIC("$num-Args", rbox::to_http_header_case("$numArgs"));
    EXPECT_EQ_STATIC("$-Num-Args", rbox::to_http_header_case("$NumArgs"));
    EXPECT_EQ_STATIC("$-N-Args", rbox::to_http_header_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(
        "Get-X1-And-Y1", rbox::to_http_header_case(rbox::non_alpha_as_upper, "getX1AndY1"));
    EXPECT_EQ_STATIC("X-1y-2", rbox::to_http_header_case(rbox::non_alpha_as_upper, "x1y2"));
    EXPECT_EQ_STATIC("X-1y-2", rbox::to_http_header_case(rbox::non_alpha_as_upper, "X1y2"));
    EXPECT_EQ_STATIC("X-1y2", rbox::to_http_header_case(rbox::non_alpha_as_upper, "x1Y2"));
    EXPECT_EQ_STATIC("X1y2", rbox::to_http_header_case(rbox::non_alpha_as_upper, "X1Y2"));
    EXPECT_EQ_STATIC("$num-Args", rbox::to_http_header_case(rbox::non_alpha_as_upper, "$numArgs"));
    EXPECT_EQ_STATIC("$-Num-Args", rbox::to_http_header_case(rbox::non_alpha_as_upper, "$NumArgs"));
    EXPECT_EQ_STATIC("$n-Args", rbox::to_http_header_case(rbox::non_alpha_as_upper, "$NArgs"));

    EXPECT_EQ_STATIC(
        "Matrix-4x4", rbox::to_http_header_case(rbox::non_alpha_as_lower, "MATRIX_4x4"));
    EXPECT_EQ_STATIC(
        "Matrix-4x4", rbox::to_http_header_case(rbox::non_alpha_as_upper, "MATRIX_4X4"));
    // Note: irreversible
    EXPECT_EQ_STATIC(
        "Is-Ieee754-Float", rbox::to_http_header_case(rbox::non_alpha_as_upper, "IsIEEE754Float"));

    EXPECT_EQ_STATIC("Num-Args-", rbox::to_http_header_case("numArgs_"));
    EXPECT_EQ_STATIC("-Noreturn", rbox::to_http_header_case("_Noreturn"));
    EXPECT_EQ_STATIC("--No-Return", rbox::to_http_header_case("__NoReturn"));
    EXPECT_EQ_STATIC("--X--Y--", rbox::to_http_header_case("__X__Y__"));
    EXPECT_EQ_STATIC("---Ab-C--D-Ef---", rbox::to_http_header_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC("--Num-Processes", rbox::to_http_header_case("--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("-", rbox::to_http_header_case("_"));
    EXPECT_EQ_STATIC("-", rbox::to_http_header_case("-"));
    EXPECT_EQ_STATIC("----", rbox::to_http_header_case("____"));
    EXPECT_EQ_STATIC("----", rbox::to_http_header_case("----"));
    EXPECT_EQ_STATIC("--------", rbox::to_http_header_case("_--__--_"));

    // Invalid identifiers
    TEST_INVALID_IDENTIFIERS(to_http_header_case);
}

TEST(UtilsIdentifierNaming, ConvertIdentifier)
{
    using rules = rbox::identifier_naming_rule;
    EXPECT_EQ_STATIC("num_args", rbox::convert_identifier("numArgs", rules::snake_case));
    EXPECT_EQ_STATIC("NUM_ARGS", rbox::convert_identifier("numArgs", rules::all_caps_snake_case));
    EXPECT_EQ_STATIC("num-args", rbox::convert_identifier("numArgs", rules::kebab_case));
    EXPECT_EQ_STATIC("NUM-ARGS", rbox::convert_identifier("numArgs", rules::all_caps_kebab_case));
    EXPECT_EQ_STATIC("numArgs", rbox::convert_identifier("num-args", rules::lower_camel_case));
    EXPECT_EQ_STATIC("NumArgs", rbox::convert_identifier("num-args", rules::upper_camel_case));
    EXPECT_EQ_STATIC(
        "num_Args", rbox::convert_identifier("num-args", rules::lower_camel_snake_case));
    EXPECT_EQ_STATIC(
        "Num_Args", rbox::convert_identifier("num-args", rules::upper_camel_snake_case));
    EXPECT_EQ_STATIC("Num-Args", rbox::convert_identifier("num-args", rules::http_header_case));
    // Fallback on invalid naming rule
    EXPECT_EQ("", rbox::convert_identifier("numArgs", static_cast<rules>(123)));
}

TEST(UtilsIdentifierNaming, ConvertIdentifierOpt)
{
    using rules = rbox::identifier_naming_rule;
    EXPECT_EQ_STATIC("num_args", rbox::convert_identifier_opt("numArgs", rules::snake_case));
    EXPECT_EQ_STATIC(
        "NUM_ARGS", rbox::convert_identifier_opt("numArgs", rules::all_caps_snake_case));
    EXPECT_EQ_STATIC("num-args", rbox::convert_identifier_opt("numArgs", rules::kebab_case));
    EXPECT_EQ_STATIC(
        "NUM-ARGS", rbox::convert_identifier_opt("numArgs", rules::all_caps_kebab_case));
    EXPECT_EQ_STATIC("numArgs", rbox::convert_identifier_opt("num-args", rules::lower_camel_case));
    EXPECT_EQ_STATIC("NumArgs", rbox::convert_identifier_opt("num-args", rules::upper_camel_case));
    EXPECT_EQ_STATIC(
        "num_Args", rbox::convert_identifier_opt("num-args", rules::lower_camel_snake_case));
    EXPECT_EQ_STATIC(
        "Num_Args", rbox::convert_identifier_opt("num-args", rules::upper_camel_snake_case));
    EXPECT_EQ_STATIC("Num-Args", rbox::convert_identifier_opt("num-args", rules::http_header_case));
    // Fallback on invalid naming rule
    EXPECT_EQ(std::nullopt, rbox::convert_identifier_opt("numArgs", static_cast<rules>(123)));
}
