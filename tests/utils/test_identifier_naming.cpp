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

using namespace rbox;
using namespace std::literals;

constexpr auto U = non_alpha_as_upper;
constexpr auto L = non_alpha_as_lower;

TEST(UtilsIdentifierNaming, ToSnakeCase)
{
    EXPECT_EQ_STATIC("get_name", to_snake_case("getName"));
    EXPECT_EQ_STATIC(U"set_json_file_path", to_snake_case(U"setJsonFilePath"));
    EXPECT_EQ_STATIC(U"set_json_file_path", to_snake_case(U"setJSONFilePath"));
    EXPECT_EQ_STATIC(u"value_offset", to_snake_case(u"VALUE_OFFSET"));
    EXPECT_EQ_STATIC(u8"yaml_parser", to_snake_case(u8"YamlParser"));
    EXPECT_EQ_STATIC(u8"yaml_parser", to_snake_case(u8"YAMLParser"));
    EXPECT_EQ_STATIC(u"num_gc_threads", to_snake_case(u"num-gcThreads"));
    EXPECT_EQ_STATIC(U"a", to_snake_case(U"a"));
    EXPECT_EQ_STATIC("a", to_snake_case("A"));
    EXPECT_EQ_STATIC("$", to_snake_case("$"));

    EXPECT_EQ_STATIC(u8"snake_case", to_snake_case(u8"snake_case"));
    EXPECT_EQ_STATIC(U"all_caps_snake_case", to_snake_case(U"ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC(L"kebab_case", to_snake_case(L"kebab-case"));
    EXPECT_EQ_STATIC(u8"all_caps_kebab_case", to_snake_case(u8"ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("lower_camel_case", to_snake_case("lowerCamelCase"));
    EXPECT_EQ_STATIC("upper_camel_case", to_snake_case("UpperCamelCase"));
    EXPECT_EQ_STATIC(u8"lower_camel_snake_case", to_snake_case(u8"lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(U"upper_camel_snake_case", to_snake_case(U"Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(u"http_header_case", to_snake_case(u"HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC(U"get_x1_and_y1", to_snake_case(U"getX1AndY1"));
    EXPECT_EQ_STATIC(u8"x1y2", to_snake_case(u8"x1y2"));
    EXPECT_EQ_STATIC(u8"x1y2", to_snake_case(u8"X1y2"));
    EXPECT_EQ_STATIC("x1_y2", to_snake_case("x1Y2"));
    EXPECT_EQ_STATIC(u8"x1_y2", to_snake_case(u8"X1Y2"));
    EXPECT_EQ_STATIC(u8"$num_args", to_snake_case(u8"$numArgs"));
    EXPECT_EQ_STATIC(u"$_num_args", to_snake_case(u"$NumArgs"));
    EXPECT_EQ_STATIC(L"$_n_args", to_snake_case(L"$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC("get_x1_and_y1", to_snake_case(U, "getX1AndY1"));
    EXPECT_EQ_STATIC(L"x_1y_2", to_snake_case(U, L"x1y2"));
    EXPECT_EQ_STATIC(L"x_1y_2", to_snake_case(U, L"X1y2"));
    EXPECT_EQ_STATIC(u8"x_1y2", to_snake_case(U, u8"x1Y2"));
    EXPECT_EQ_STATIC(u8"x1y2", to_snake_case(U, u8"X1Y2"));
    EXPECT_EQ_STATIC(u"$num_args", to_snake_case(U, u"$numArgs"));
    EXPECT_EQ_STATIC(L"$_num_args", to_snake_case(U, L"$NumArgs"));
    EXPECT_EQ_STATIC(L"$n_args", to_snake_case(U, L"$NArgs"));

    EXPECT_EQ_STATIC(u"matrix_4x4", to_snake_case(L, u"MATRIX_4x4"));
    EXPECT_EQ_STATIC("matrix_4x4", to_snake_case(U, "MATRIX_4X4"));
    EXPECT_EQ_STATIC(u"is_ieee754_float", to_snake_case(U, u"IsIEEE754Float"));

    EXPECT_EQ_STATIC("num_args_", to_snake_case("numArgs_"));
    EXPECT_EQ_STATIC(L"num_args____", to_snake_case(L"numArgs_-_-"));
    EXPECT_EQ_STATIC(u8"_noreturn", to_snake_case(u8"_Noreturn"));
    EXPECT_EQ_STATIC(L"__no_return", to_snake_case(L"__NoReturn"));
    EXPECT_EQ_STATIC(u"__x__y__", to_snake_case(u"__X__Y__"));
    EXPECT_EQ_STATIC(u8"___ab_c__d_ef___", to_snake_case(u8"___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC(L"__num_processes", to_snake_case(L"--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("_", to_snake_case("_"));
    EXPECT_EQ_STATIC(u"_", to_snake_case(u"-"));
    EXPECT_EQ_STATIC(L"____", to_snake_case(L"____"));
    EXPECT_EQ_STATIC(L"____", to_snake_case(L"----"));
    EXPECT_EQ_STATIC(L"________", to_snake_case(L"_--__--_"));
}

TEST(UtilsIdentifierNaming, ToAllCapsSnakeCase)
{
    EXPECT_EQ_STATIC(L"GET_NAME", to_all_caps_snake_case(L"getName"));
    EXPECT_EQ_STATIC(U"SET_JSON_FILE_PATH", to_all_caps_snake_case(U"setJsonFilePath"));
    EXPECT_EQ_STATIC(u"SET_JSON_FILE_PATH", to_all_caps_snake_case(u"setJSONFilePath"));
    EXPECT_EQ_STATIC(U"VALUE_OFFSET", to_all_caps_snake_case(U"VALUE_OFFSET"));
    EXPECT_EQ_STATIC(u8"YAML_PARSER", to_all_caps_snake_case(u8"YamlParser"));
    EXPECT_EQ_STATIC("YAML_PARSER", to_all_caps_snake_case("YAMLParser"));
    EXPECT_EQ_STATIC(L"NUM_GC_THREADS", to_all_caps_snake_case(L"num-gcThreads"));
    EXPECT_EQ_STATIC(L"A", to_all_caps_snake_case(L"a"));
    EXPECT_EQ_STATIC(u8"A", to_all_caps_snake_case(u8"A"));
    EXPECT_EQ_STATIC("$", to_all_caps_snake_case("$"));

    EXPECT_EQ_STATIC(u8"SNAKE_CASE", to_all_caps_snake_case(u8"snake_case"));
    EXPECT_EQ_STATIC(u8"ALL_CAPS_SNAKE_CASE", to_all_caps_snake_case(u8"ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("KEBAB_CASE", to_all_caps_snake_case("kebab-case"));
    EXPECT_EQ_STATIC(L"ALL_CAPS_KEBAB_CASE", to_all_caps_snake_case(L"ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC(u"LOWER_CAMEL_CASE", to_all_caps_snake_case(u"lowerCamelCase"));
    EXPECT_EQ_STATIC(L"UPPER_CAMEL_CASE", to_all_caps_snake_case(L"UpperCamelCase"));
    EXPECT_EQ_STATIC(U"LOWER_CAMEL_SNAKE_CASE", to_all_caps_snake_case(U"lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(U"UPPER_CAMEL_SNAKE_CASE", to_all_caps_snake_case(U"Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(u8"HTTP_HEADER_CASE", to_all_caps_snake_case(u8"HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("GET_X1_AND_Y1", to_all_caps_snake_case("getX1AndY1"));
    EXPECT_EQ_STATIC(U"X1Y2", to_all_caps_snake_case(U"x1y2"));
    EXPECT_EQ_STATIC(L"X1Y2", to_all_caps_snake_case(L"X1y2"));
    EXPECT_EQ_STATIC("X1_Y2", to_all_caps_snake_case("x1Y2"));
    EXPECT_EQ_STATIC("X1_Y2", to_all_caps_snake_case("X1Y2"));
    EXPECT_EQ_STATIC("$NUM_ARGS", to_all_caps_snake_case("$numArgs"));
    EXPECT_EQ_STATIC(U"$_NUM_ARGS", to_all_caps_snake_case(U"$NumArgs"));
    EXPECT_EQ_STATIC(u"$_N_ARGS", to_all_caps_snake_case(u"$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(U"GET_X1_AND_Y1", to_all_caps_snake_case(U, U"getX1AndY1"));
    EXPECT_EQ_STATIC(u"X_1Y_2", to_all_caps_snake_case(U, u"x1y2"));
    EXPECT_EQ_STATIC(L"X_1Y_2", to_all_caps_snake_case(U, L"X1y2"));
    EXPECT_EQ_STATIC(u8"X_1Y2", to_all_caps_snake_case(U, u8"x1Y2"));
    EXPECT_EQ_STATIC(L"X1Y2", to_all_caps_snake_case(U, L"X1Y2"));
    EXPECT_EQ_STATIC(u8"$NUM_ARGS", to_all_caps_snake_case(U, u8"$numArgs"));
    EXPECT_EQ_STATIC("$_NUM_ARGS", to_all_caps_snake_case(U, "$NumArgs"));
    EXPECT_EQ_STATIC("$N_ARGS", to_all_caps_snake_case(U, "$NArgs"));

    EXPECT_EQ_STATIC(u8"MATRIX_4X4", to_all_caps_snake_case(L, u8"MATRIX_4x4"));
    EXPECT_EQ_STATIC(u"MATRIX_4X4", to_all_caps_snake_case(U, u"MATRIX_4X4"));
    EXPECT_EQ_STATIC("IS_IEEE754_FLOAT", to_all_caps_snake_case(U, "IsIEEE754Float"));

    EXPECT_EQ_STATIC("NUM_ARGS_", to_all_caps_snake_case("numArgs_"));
    EXPECT_EQ_STATIC(u8"_NORETURN", to_all_caps_snake_case(u8"_Noreturn"));
    EXPECT_EQ_STATIC(u"__NO_RETURN", to_all_caps_snake_case(u"__NoReturn"));
    EXPECT_EQ_STATIC(U"__X__Y__", to_all_caps_snake_case(U"__X__Y__"));
    EXPECT_EQ_STATIC(u8"___AB_C__D_EF___", to_all_caps_snake_case(u8"___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC(L"__NUM_PROCESSES", to_all_caps_snake_case(L"--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC(u8"_", to_all_caps_snake_case(u8"_"));
    EXPECT_EQ_STATIC(L"_", to_all_caps_snake_case(L"-"));
    EXPECT_EQ_STATIC(U"____", to_all_caps_snake_case(U"____"));
    EXPECT_EQ_STATIC(u8"____", to_all_caps_snake_case(u8"----"));
    EXPECT_EQ_STATIC(u8"________", to_all_caps_snake_case(u8"_--__--_"));
}

TEST(UtilsIdentifierNaming, ToKebabCase)
{
    EXPECT_EQ_STATIC(L"get-name", to_kebab_case(L"getName"));
    EXPECT_EQ_STATIC(U"set-json-file-path", to_kebab_case(U"setJsonFilePath"));
    EXPECT_EQ_STATIC("set-json-file-path", to_kebab_case("setJSONFilePath"));
    EXPECT_EQ_STATIC("value-offset", to_kebab_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC(u8"yaml-parser", to_kebab_case(u8"YamlParser"));
    EXPECT_EQ_STATIC(L"yaml-parser", to_kebab_case(L"YAMLParser"));
    EXPECT_EQ_STATIC("num-gc-threads", to_kebab_case("num-gcThreads"));
    EXPECT_EQ_STATIC(u8"a", to_kebab_case(u8"a"));
    EXPECT_EQ_STATIC(U"a", to_kebab_case(U"A"));
    EXPECT_EQ_STATIC("$", to_kebab_case("$"));

    EXPECT_EQ_STATIC(U"snake-case", to_kebab_case(U"snake_case"));
    EXPECT_EQ_STATIC(U"all-caps-snake-case", to_kebab_case(U"ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC(u8"kebab-case", to_kebab_case(u8"kebab-case"));
    EXPECT_EQ_STATIC(U"all-caps-kebab-case", to_kebab_case(U"ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC(u"lower-camel-case", to_kebab_case(u"lowerCamelCase"));
    EXPECT_EQ_STATIC(L"upper-camel-case", to_kebab_case(L"UpperCamelCase"));
    EXPECT_EQ_STATIC(u8"lower-camel-snake-case", to_kebab_case(u8"lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(u8"upper-camel-snake-case", to_kebab_case(u8"Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(L"http-header-case", to_kebab_case(L"HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("get-x1-and-y1", to_kebab_case("getX1AndY1"));
    EXPECT_EQ_STATIC(u8"x1y2", to_kebab_case(u8"x1y2"));
    EXPECT_EQ_STATIC(L"x1y2", to_kebab_case(L"X1y2"));
    EXPECT_EQ_STATIC(u8"x1-y2", to_kebab_case(u8"x1Y2"));
    EXPECT_EQ_STATIC(U"x1-y2", to_kebab_case(U"X1Y2"));
    EXPECT_EQ_STATIC(L"$num-args", to_kebab_case(L"$numArgs"));
    EXPECT_EQ_STATIC(U"$-num-args", to_kebab_case(U"$NumArgs"));
    EXPECT_EQ_STATIC("$-n-args", to_kebab_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC("get-x1-and-y1", to_kebab_case(U, "getX1AndY1"));
    EXPECT_EQ_STATIC(U"x-1y-2", to_kebab_case(U, U"x1y2"));
    EXPECT_EQ_STATIC(U"x-1y-2", to_kebab_case(U, U"X1y2"));
    EXPECT_EQ_STATIC(u"x-1y2", to_kebab_case(U, u"x1Y2"));
    EXPECT_EQ_STATIC("x1y2", to_kebab_case(U, "X1Y2"));
    EXPECT_EQ_STATIC("$num-args", to_kebab_case(U, "$numArgs"));
    EXPECT_EQ_STATIC(u8"$-num-args", to_kebab_case(U, u8"$NumArgs"));
    EXPECT_EQ_STATIC(u"$n-args", to_kebab_case(U, u"$NArgs"));

    EXPECT_EQ_STATIC(u8"matrix-4x4", to_kebab_case(L, u8"MATRIX_4x4"));
    EXPECT_EQ_STATIC(L"matrix-4x4", to_kebab_case(U, L"MATRIX_4X4"));
    EXPECT_EQ_STATIC(u"is-ieee754-float", to_kebab_case(U, u"IsIEEE754Float"));

    EXPECT_EQ_STATIC(u8"num-args-", to_kebab_case(u8"numArgs_"));
    EXPECT_EQ_STATIC(L"-noreturn", to_kebab_case(L"_Noreturn"));
    EXPECT_EQ_STATIC(u8"--no-return", to_kebab_case(u8"__NoReturn"));
    EXPECT_EQ_STATIC(L"--x--y--", to_kebab_case(L"__X__Y__"));
    EXPECT_EQ_STATIC(u8"---ab-c--d-ef---", to_kebab_case(u8"___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC(u8"--num-processes", to_kebab_case(u8"--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC(U"-", to_kebab_case(U"_"));
    EXPECT_EQ_STATIC(U"-", to_kebab_case(U"-"));
    EXPECT_EQ_STATIC(U"----", to_kebab_case(U"____"));
    EXPECT_EQ_STATIC(u"----", to_kebab_case(u"----"));
    EXPECT_EQ_STATIC("--------", to_kebab_case("_--__--_"));
}

TEST(UtilsIdentifierNaming, ToAllCapsKebabCase)
{
    EXPECT_EQ_STATIC(U"GET-NAME", to_all_caps_kebab_case(U"getName"));
    EXPECT_EQ_STATIC("SET-JSON-FILE-PATH", to_all_caps_kebab_case("setJsonFilePath"));
    EXPECT_EQ_STATIC(U"SET-JSON-FILE-PATH", to_all_caps_kebab_case(U"setJSONFilePath"));
    EXPECT_EQ_STATIC(U"VALUE-OFFSET", to_all_caps_kebab_case(U"VALUE_OFFSET"));
    EXPECT_EQ_STATIC(L"YAML-PARSER", to_all_caps_kebab_case(L"YamlParser"));
    EXPECT_EQ_STATIC(U"YAML-PARSER", to_all_caps_kebab_case(U"YAMLParser"));
    EXPECT_EQ_STATIC("NUM-GC-THREADS", to_all_caps_kebab_case("num-gcThreads"));
    EXPECT_EQ_STATIC(u"A", to_all_caps_kebab_case(u"a"));
    EXPECT_EQ_STATIC(L"A", to_all_caps_kebab_case(L"A"));
    EXPECT_EQ_STATIC("$", to_all_caps_kebab_case("$"));

    EXPECT_EQ_STATIC(U"SNAKE-CASE", to_all_caps_kebab_case(U"snake_case"));
    EXPECT_EQ_STATIC("ALL-CAPS-SNAKE-CASE", to_all_caps_kebab_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC(L"KEBAB-CASE", to_all_caps_kebab_case(L"kebab-case"));
    EXPECT_EQ_STATIC("ALL-CAPS-KEBAB-CASE", to_all_caps_kebab_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC(U"LOWER-CAMEL-CASE", to_all_caps_kebab_case(U"lowerCamelCase"));
    EXPECT_EQ_STATIC("UPPER-CAMEL-CASE", to_all_caps_kebab_case("UpperCamelCase"));
    EXPECT_EQ_STATIC(L"LOWER-CAMEL-SNAKE-CASE", to_all_caps_kebab_case(L"lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(U"UPPER-CAMEL-SNAKE-CASE", to_all_caps_kebab_case(U"Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(U"HTTP-HEADER-CASE", to_all_caps_kebab_case(U"HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC(u"GET-X1-AND-Y1", to_all_caps_kebab_case(u"getX1AndY1"));
    EXPECT_EQ_STATIC(U"X1Y2", to_all_caps_kebab_case(U"x1y2"));
    EXPECT_EQ_STATIC("X1Y2", to_all_caps_kebab_case("X1y2"));
    EXPECT_EQ_STATIC("X1-Y2", to_all_caps_kebab_case("x1Y2"));
    EXPECT_EQ_STATIC(u8"X1-Y2", to_all_caps_kebab_case(u8"X1Y2"));
    EXPECT_EQ_STATIC("$NUM-ARGS", to_all_caps_kebab_case("$numArgs"));
    EXPECT_EQ_STATIC(U"$-NUM-ARGS", to_all_caps_kebab_case(U"$NumArgs"));
    EXPECT_EQ_STATIC(U"$-N-ARGS", to_all_caps_kebab_case(U"$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC("GET-X1-AND-Y1", to_all_caps_kebab_case(U, "getX1AndY1"));
    EXPECT_EQ_STATIC(L"X-1Y-2", to_all_caps_kebab_case(U, L"x1y2"));
    EXPECT_EQ_STATIC(u8"X-1Y-2", to_all_caps_kebab_case(U, u8"X1y2"));
    EXPECT_EQ_STATIC(L"X-1Y2", to_all_caps_kebab_case(U, L"x1Y2"));
    EXPECT_EQ_STATIC(L"X1Y2", to_all_caps_kebab_case(U, L"X1Y2"));
    EXPECT_EQ_STATIC(u"$NUM-ARGS", to_all_caps_kebab_case(U, u"$numArgs"));
    EXPECT_EQ_STATIC(U"$-NUM-ARGS", to_all_caps_kebab_case(U, U"$NumArgs"));
    EXPECT_EQ_STATIC(u"$N-ARGS", to_all_caps_kebab_case(U, u"$NArgs"));

    EXPECT_EQ_STATIC(u8"MATRIX-4X4", to_all_caps_kebab_case(L, u8"MATRIX_4x4"));
    EXPECT_EQ_STATIC(u"MATRIX-4X4", to_all_caps_kebab_case(U, u"MATRIX_4X4"));
    EXPECT_EQ_STATIC(u8"IS-IEEE754-FLOAT", to_all_caps_kebab_case(U, u8"IsIEEE754Float"));

    EXPECT_EQ_STATIC(u"NUM-ARGS-", to_all_caps_kebab_case(u"numArgs_"));
    EXPECT_EQ_STATIC("-NORETURN", to_all_caps_kebab_case("_Noreturn"));
    EXPECT_EQ_STATIC(u"--NO-RETURN", to_all_caps_kebab_case(u"__NoReturn"));
    EXPECT_EQ_STATIC(L"--X--Y--", to_all_caps_kebab_case(L"__X__Y__"));
    EXPECT_EQ_STATIC("---AB-C--D-EF---", to_all_caps_kebab_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC(u"--NUM-PROCESSES", to_all_caps_kebab_case(u"--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC(L"-", to_all_caps_kebab_case(L"_"));
    EXPECT_EQ_STATIC(u"-", to_all_caps_kebab_case(u"-"));
    EXPECT_EQ_STATIC(u"----", to_all_caps_kebab_case(u"____"));
    EXPECT_EQ_STATIC(u"----", to_all_caps_kebab_case(u"----"));
    EXPECT_EQ_STATIC(L"--------", to_all_caps_kebab_case(L"_--__--_"));
}

TEST(UtilsIdentifierNaming, ToLowerCamelCase)
{
    EXPECT_EQ_STATIC(L"getName", to_lower_camel_case(L"getName"));
    EXPECT_EQ_STATIC("setJsonFilePath", to_lower_camel_case("setJsonFilePath"));
    // Note: irreversible convertion
    EXPECT_EQ_STATIC("setJsonFilePath", to_lower_camel_case("setJSONFilePath"));
    EXPECT_EQ_STATIC(U"valueOffset", to_lower_camel_case(U"VALUE_OFFSET"));
    EXPECT_EQ_STATIC("yamlParser", to_lower_camel_case("YamlParser"));
    EXPECT_EQ_STATIC("yamlParser", to_lower_camel_case("YAMLParser"));
    EXPECT_EQ_STATIC(U"numGcThreads", to_lower_camel_case(U"num-gcThreads"));
    EXPECT_EQ_STATIC(u"a", to_lower_camel_case(u"a"));
    EXPECT_EQ_STATIC("a", to_lower_camel_case("A"));
    EXPECT_EQ_STATIC(L"$", to_lower_camel_case(L"$"));

    EXPECT_EQ_STATIC(u"snakeCase", to_lower_camel_case(u"snake_case"));
    EXPECT_EQ_STATIC(u8"allCapsSnakeCase", to_lower_camel_case(u8"ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC(L"kebabCase", to_lower_camel_case(L"kebab-case"));
    EXPECT_EQ_STATIC(u"allCapsKebabCase", to_lower_camel_case(u"ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC(L"lowerCamelCase", to_lower_camel_case(L"lowerCamelCase"));
    EXPECT_EQ_STATIC(u"upperCamelCase", to_lower_camel_case(u"UpperCamelCase"));
    EXPECT_EQ_STATIC(U"lowerCamelSnakeCase", to_lower_camel_case(U"lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(L"upperCamelSnakeCase", to_lower_camel_case(L"Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(L"httpHeaderCase", to_lower_camel_case(L"HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC(L"getX1AndY1", to_lower_camel_case(L"getX1AndY1"));
    EXPECT_EQ_STATIC("x1y2", to_lower_camel_case("x1y2"));
    EXPECT_EQ_STATIC(u8"x1y2", to_lower_camel_case(u8"X1y2"));
    EXPECT_EQ_STATIC(u"x1Y2", to_lower_camel_case(u"x1Y2"));
    EXPECT_EQ_STATIC("x1Y2", to_lower_camel_case("X1Y2"));
    EXPECT_EQ_STATIC("$numArgs", to_lower_camel_case("$numArgs"));
    EXPECT_EQ_STATIC("$NumArgs", to_lower_camel_case("$NumArgs"));
    EXPECT_EQ_STATIC(u"$NArgs", to_lower_camel_case(u"$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(u"getX1AndY1", to_lower_camel_case(U, u"getX1AndY1"));
    EXPECT_EQ_STATIC(u"x1y2", to_lower_camel_case(U, u"x1y2"));
    EXPECT_EQ_STATIC(u8"x1y2", to_lower_camel_case(U, u8"X1y2"));
    EXPECT_EQ_STATIC(u"x1y2", to_lower_camel_case(U, u"x1Y2"));
    EXPECT_EQ_STATIC("x1y2", to_lower_camel_case(U, "X1Y2"));
    EXPECT_EQ_STATIC("$numArgs", to_lower_camel_case(U, "$numArgs"));
    EXPECT_EQ_STATIC(L"$NumArgs", to_lower_camel_case(U, L"$NumArgs"));
    EXPECT_EQ_STATIC(u"$nArgs", to_lower_camel_case(U, u"$NArgs"));

    EXPECT_EQ_STATIC(u"matrix4x4", to_lower_camel_case(L, u"MATRIX_4x4"));
    EXPECT_EQ_STATIC(L"matrix4x4", to_lower_camel_case(U, L"MATRIX_4X4"));
    // Note: irreversible
    EXPECT_EQ_STATIC(u"isIeee754Float", to_lower_camel_case(U, u"IsIEEE754Float"));

    EXPECT_EQ_STATIC(U"numArgs_", to_lower_camel_case(U"numArgs_"));
    EXPECT_EQ_STATIC(U"_noreturn", to_lower_camel_case(U"_Noreturn"));
    EXPECT_EQ_STATIC(L"__noReturn", to_lower_camel_case(L"__NoReturn"));
    EXPECT_EQ_STATIC(U"__x_Y__", to_lower_camel_case(U"__X__Y__"));
    EXPECT_EQ_STATIC(L"___abC_DEf___", to_lower_camel_case(L"___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC(u"__numProcesses", to_lower_camel_case(u"--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC(u8"_", to_lower_camel_case(u8"_"));
    EXPECT_EQ_STATIC(L"_", to_lower_camel_case(L"-"));
    EXPECT_EQ_STATIC(U"____", to_lower_camel_case(U"____"));
    EXPECT_EQ_STATIC("____", to_lower_camel_case("----"));
    EXPECT_EQ_STATIC(u8"________", to_lower_camel_case(u8"_--__--_"));
}

TEST(UtilsIdentifierNaming, ToUpperCamelCase)
{
    EXPECT_EQ_STATIC("GetName", to_upper_camel_case("getName"));
    EXPECT_EQ_STATIC("SetJsonFilePath", to_upper_camel_case("setJsonFilePath"));
    // Note: irreversible convertion
    EXPECT_EQ_STATIC(L"SetJsonFilePath", to_upper_camel_case(L"setJSONFilePath"));
    EXPECT_EQ_STATIC("ValueOffset", to_upper_camel_case("VALUE_OFFSET"));
    EXPECT_EQ_STATIC("YamlParser", to_upper_camel_case("YamlParser"));
    EXPECT_EQ_STATIC("YamlParser", to_upper_camel_case("YAMLParser"));
    EXPECT_EQ_STATIC(L"NumGcThreads", to_upper_camel_case(L"num-gcThreads"));
    EXPECT_EQ_STATIC(U"A", to_upper_camel_case(U"a"));
    EXPECT_EQ_STATIC(L"A", to_upper_camel_case(L"A"));
    EXPECT_EQ_STATIC(U"$", to_upper_camel_case(U"$"));

    EXPECT_EQ_STATIC(U"SnakeCase", to_upper_camel_case(U"snake_case"));
    EXPECT_EQ_STATIC("AllCapsSnakeCase", to_upper_camel_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC(u8"KebabCase", to_upper_camel_case(u8"kebab-case"));
    EXPECT_EQ_STATIC("AllCapsKebabCase", to_upper_camel_case("ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC(L"LowerCamelCase", to_upper_camel_case(L"lowerCamelCase"));
    EXPECT_EQ_STATIC(L"UpperCamelCase", to_upper_camel_case(L"UpperCamelCase"));
    EXPECT_EQ_STATIC("LowerCamelSnakeCase", to_upper_camel_case("lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(u"UpperCamelSnakeCase", to_upper_camel_case(u"Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(U"HttpHeaderCase", to_upper_camel_case(U"HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC(u"GetX1AndY1", to_upper_camel_case(u"getX1AndY1"));
    EXPECT_EQ_STATIC(u8"X1y2", to_upper_camel_case(u8"x1y2"));
    EXPECT_EQ_STATIC(u"X1y2", to_upper_camel_case(u"X1y2"));
    EXPECT_EQ_STATIC(u8"X1Y2", to_upper_camel_case(u8"x1Y2"));
    EXPECT_EQ_STATIC(L"X1Y2", to_upper_camel_case(L"X1Y2"));
    EXPECT_EQ_STATIC(U"$numArgs", to_upper_camel_case(U"$numArgs"));
    EXPECT_EQ_STATIC(L"$NumArgs", to_upper_camel_case(L"$NumArgs"));
    EXPECT_EQ_STATIC(u"$NArgs", to_upper_camel_case(u"$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(U"GetX1AndY1", to_upper_camel_case(U, U"getX1AndY1"));
    EXPECT_EQ_STATIC(U"X1y2", to_upper_camel_case(U, U"x1y2"));
    EXPECT_EQ_STATIC(u8"X1y2", to_upper_camel_case(U, u8"X1y2"));
    EXPECT_EQ_STATIC(L"X1y2", to_upper_camel_case(U, L"x1Y2"));
    EXPECT_EQ_STATIC(U"X1y2", to_upper_camel_case(U, U"X1Y2"));
    EXPECT_EQ_STATIC(u"$numArgs", to_upper_camel_case(U, u"$numArgs"));
    EXPECT_EQ_STATIC(L"$NumArgs", to_upper_camel_case(U, L"$NumArgs"));
    EXPECT_EQ_STATIC(L"$nArgs", to_upper_camel_case(U, L"$NArgs"));

    EXPECT_EQ_STATIC(U"Matrix4x4", to_upper_camel_case(L, U"MATRIX_4x4"));
    EXPECT_EQ_STATIC("Matrix4x4", to_upper_camel_case(U, "MATRIX_4X4"));
    // Note: irreversible
    EXPECT_EQ_STATIC(u"IsIeee754Float", to_upper_camel_case(U, u"IsIEEE754Float"));

    EXPECT_EQ_STATIC("NumArgs_", to_upper_camel_case("numArgs_"));
    EXPECT_EQ_STATIC("_Noreturn", to_upper_camel_case("_Noreturn"));
    EXPECT_EQ_STATIC(U"__NoReturn", to_upper_camel_case(U"__NoReturn"));
    EXPECT_EQ_STATIC(L"__X_Y__", to_upper_camel_case(L"__X__Y__"));
    EXPECT_EQ_STATIC("___AbC_DEf___", to_upper_camel_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC(L"__NumProcesses", to_upper_camel_case(L"--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC(u8"_", to_upper_camel_case(u8"_"));
    EXPECT_EQ_STATIC(u8"_", to_upper_camel_case(u8"-"));
    EXPECT_EQ_STATIC("____", to_upper_camel_case("____"));
    EXPECT_EQ_STATIC("____", to_upper_camel_case("----"));
    EXPECT_EQ_STATIC(u"________", to_upper_camel_case(u"_--__--_"));
}

TEST(UtilsIdentifierNaming, ToLowerCamelSnakeCase)
{
    EXPECT_EQ_STATIC(u8"get_Name", to_lower_camel_snake_case(u8"getName"));
    EXPECT_EQ_STATIC(U"set_Json_File_Path", to_lower_camel_snake_case(U"setJsonFilePath"));
    // Note: irreversible convertion
    EXPECT_EQ_STATIC("set_Json_File_Path", to_lower_camel_snake_case("setJSONFilePath"));
    EXPECT_EQ_STATIC(U"value_Offset", to_lower_camel_snake_case(U"VALUE_OFFSET"));
    EXPECT_EQ_STATIC(U"yaml_Parser", to_lower_camel_snake_case(U"YamlParser"));
    EXPECT_EQ_STATIC(L"yaml_Parser", to_lower_camel_snake_case(L"YAMLParser"));
    EXPECT_EQ_STATIC(u8"num_Gc_Threads", to_lower_camel_snake_case(u8"num-gcThreads"));
    EXPECT_EQ_STATIC(L"a", to_lower_camel_snake_case(L"a"));
    EXPECT_EQ_STATIC(L"a", to_lower_camel_snake_case(L"A"));
    EXPECT_EQ_STATIC(U"$", to_lower_camel_snake_case(U"$"));

    EXPECT_EQ_STATIC(U"snake_Case", to_lower_camel_snake_case(U"snake_case"));
    EXPECT_EQ_STATIC(u8"all_Caps_Snake_Case", to_lower_camel_snake_case(u8"ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC("kebab_Case", to_lower_camel_snake_case("kebab-case"));
    EXPECT_EQ_STATIC(U"all_Caps_Kebab_Case", to_lower_camel_snake_case(U"ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC(L"lower_Camel_Case", to_lower_camel_snake_case(L"lowerCamelCase"));
    EXPECT_EQ_STATIC(U"upper_Camel_Case", to_lower_camel_snake_case(U"UpperCamelCase"));
    EXPECT_EQ_STATIC(
        u8"lower_Camel_Snake_Case", to_lower_camel_snake_case(u8"lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("upper_Camel_Snake_Case", to_lower_camel_snake_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(u"http_Header_Case", to_lower_camel_snake_case(u"HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC(L"get_X1_And_Y1", to_lower_camel_snake_case(L"getX1AndY1"));
    EXPECT_EQ_STATIC(u"x1y2", to_lower_camel_snake_case(u"x1y2"));
    EXPECT_EQ_STATIC(u8"x1y2", to_lower_camel_snake_case(u8"X1y2"));
    EXPECT_EQ_STATIC("x1_Y2", to_lower_camel_snake_case("x1Y2"));
    EXPECT_EQ_STATIC(u8"x1_Y2", to_lower_camel_snake_case(u8"X1Y2"));
    EXPECT_EQ_STATIC(u8"$num_Args", to_lower_camel_snake_case(u8"$numArgs"));
    EXPECT_EQ_STATIC(U"$_Num_Args", to_lower_camel_snake_case(U"$NumArgs"));
    EXPECT_EQ_STATIC("$_N_Args", to_lower_camel_snake_case("$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC("get_X1_And_Y1", to_lower_camel_snake_case(U, "getX1AndY1"));
    EXPECT_EQ_STATIC(U"x_1y_2", to_lower_camel_snake_case(U, U"x1y2"));
    EXPECT_EQ_STATIC(u8"x_1y_2", to_lower_camel_snake_case(U, u8"X1y2"));
    EXPECT_EQ_STATIC(U"x_1y2", to_lower_camel_snake_case(U, U"x1Y2"));
    EXPECT_EQ_STATIC(u"x1y2", to_lower_camel_snake_case(U, u"X1Y2"));
    EXPECT_EQ_STATIC(U"$num_Args", to_lower_camel_snake_case(U, U"$numArgs"));
    EXPECT_EQ_STATIC(u"$_Num_Args", to_lower_camel_snake_case(U, u"$NumArgs"));
    EXPECT_EQ_STATIC(u"$n_Args", to_lower_camel_snake_case(U, u"$NArgs"));

    EXPECT_EQ_STATIC("matrix_4x4", to_lower_camel_snake_case(L, "MATRIX_4x4"));
    EXPECT_EQ_STATIC(U"matrix_4x4", to_lower_camel_snake_case(U, U"MATRIX_4X4"));
    // Note: irreversible
    EXPECT_EQ_STATIC(u"is_Ieee754_Float", to_lower_camel_snake_case(U, u"IsIEEE754Float"));

    EXPECT_EQ_STATIC(L"num_Args_", to_lower_camel_snake_case(L"numArgs_"));
    EXPECT_EQ_STATIC(L"_noreturn", to_lower_camel_snake_case(L"_Noreturn"));
    EXPECT_EQ_STATIC("__no_Return", to_lower_camel_snake_case("__NoReturn"));
    EXPECT_EQ_STATIC("__x__Y__", to_lower_camel_snake_case("__X__Y__"));
    EXPECT_EQ_STATIC("___ab_C__D_Ef___", to_lower_camel_snake_case("___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC(u"__num_Processes", to_lower_camel_snake_case(u"--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC(U"_", to_lower_camel_snake_case(U"_"));
    EXPECT_EQ_STATIC(L"_", to_lower_camel_snake_case(L"-"));
    EXPECT_EQ_STATIC(L"____", to_lower_camel_snake_case(L"____"));
    EXPECT_EQ_STATIC(u8"____", to_lower_camel_snake_case(u8"----"));
    EXPECT_EQ_STATIC(u"________", to_lower_camel_snake_case(u"_--__--_"));
}

TEST(UtilsIdentifierNaming, ToUpperCamelSnakeCase)
{
    EXPECT_EQ_STATIC(U"Get_Name", to_upper_camel_snake_case(U"getName"));
    EXPECT_EQ_STATIC(u"Set_Json_File_Path", to_upper_camel_snake_case(u"setJsonFilePath"));
    // Note: irreversible convertion
    EXPECT_EQ_STATIC(U"Set_Json_File_Path", to_upper_camel_snake_case(U"setJSONFilePath"));
    EXPECT_EQ_STATIC(u8"Value_Offset", to_upper_camel_snake_case(u8"VALUE_OFFSET"));
    EXPECT_EQ_STATIC(U"Yaml_Parser", to_upper_camel_snake_case(U"YamlParser"));
    EXPECT_EQ_STATIC(U"Yaml_Parser", to_upper_camel_snake_case(U"YAMLParser"));
    EXPECT_EQ_STATIC(L"Num_Gc_Threads", to_upper_camel_snake_case(L"num-gcThreads"));
    EXPECT_EQ_STATIC(L"A", to_upper_camel_snake_case(L"a"));
    EXPECT_EQ_STATIC(u8"A", to_upper_camel_snake_case(u8"A"));
    EXPECT_EQ_STATIC(L"$", to_upper_camel_snake_case(L"$"));

    EXPECT_EQ_STATIC(u"Snake_Case", to_upper_camel_snake_case(u"snake_case"));
    EXPECT_EQ_STATIC(u"All_Caps_Snake_Case", to_upper_camel_snake_case(u"ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC(u"Kebab_Case", to_upper_camel_snake_case(u"kebab-case"));
    EXPECT_EQ_STATIC(L"All_Caps_Kebab_Case", to_upper_camel_snake_case(L"ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC(u8"Lower_Camel_Case", to_upper_camel_snake_case(u8"lowerCamelCase"));
    EXPECT_EQ_STATIC(U"Upper_Camel_Case", to_upper_camel_snake_case(U"UpperCamelCase"));
    EXPECT_EQ_STATIC(
        L"Lower_Camel_Snake_Case", to_upper_camel_snake_case(L"lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC("Upper_Camel_Snake_Case", to_upper_camel_snake_case("Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(u8"Http_Header_Case", to_upper_camel_snake_case(u8"HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC(u"Get_X1_And_Y1", to_upper_camel_snake_case(u"getX1AndY1"));
    EXPECT_EQ_STATIC(u8"X1y2", to_upper_camel_snake_case(u8"x1y2"));
    EXPECT_EQ_STATIC("X1y2", to_upper_camel_snake_case("X1y2"));
    EXPECT_EQ_STATIC(L"X1_Y2", to_upper_camel_snake_case(L"x1Y2"));
    EXPECT_EQ_STATIC("X1_Y2", to_upper_camel_snake_case("X1Y2"));
    EXPECT_EQ_STATIC(u8"$num_Args", to_upper_camel_snake_case(u8"$numArgs"));
    EXPECT_EQ_STATIC(L"$_Num_Args", to_upper_camel_snake_case(L"$NumArgs"));
    EXPECT_EQ_STATIC(U"$_N_Args", to_upper_camel_snake_case(U"$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(u8"Get_X1_And_Y1", to_upper_camel_snake_case(U, u8"getX1AndY1"));
    EXPECT_EQ_STATIC(u"X_1y_2", to_upper_camel_snake_case(U, u"x1y2"));
    EXPECT_EQ_STATIC(u"X_1y_2", to_upper_camel_snake_case(U, u"X1y2"));
    EXPECT_EQ_STATIC(L"X_1y2", to_upper_camel_snake_case(U, L"x1Y2"));
    EXPECT_EQ_STATIC(L"X1y2", to_upper_camel_snake_case(U, L"X1Y2"));
    EXPECT_EQ_STATIC(U"$num_Args", to_upper_camel_snake_case(U, U"$numArgs"));
    EXPECT_EQ_STATIC(L"$_Num_Args", to_upper_camel_snake_case(U, L"$NumArgs"));
    EXPECT_EQ_STATIC(U"$n_Args", to_upper_camel_snake_case(U, U"$NArgs"));

    EXPECT_EQ_STATIC("Matrix_4x4", to_upper_camel_snake_case(L, "MATRIX_4x4"));
    EXPECT_EQ_STATIC(u8"Matrix_4x4", to_upper_camel_snake_case(U, u8"MATRIX_4X4"));
    // Note: irreversible
    EXPECT_EQ_STATIC(L"Is_Ieee754_Float", to_upper_camel_snake_case(U, L"IsIEEE754Float"));

    EXPECT_EQ_STATIC(u8"Num_Args_", to_upper_camel_snake_case(u8"numArgs_"));
    EXPECT_EQ_STATIC(u8"_Noreturn", to_upper_camel_snake_case(u8"_Noreturn"));
    EXPECT_EQ_STATIC(L"__No_Return", to_upper_camel_snake_case(L"__NoReturn"));
    EXPECT_EQ_STATIC(u8"__X__Y__", to_upper_camel_snake_case(u8"__X__Y__"));
    EXPECT_EQ_STATIC(u"___Ab_C__D_Ef___", to_upper_camel_snake_case(u"___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC(u8"__Num_Processes", to_upper_camel_snake_case(u8"--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC(u"_", to_upper_camel_snake_case(u"_"));
    EXPECT_EQ_STATIC("_", to_upper_camel_snake_case("-"));
    EXPECT_EQ_STATIC(L"____", to_upper_camel_snake_case(L"____"));
    EXPECT_EQ_STATIC("____", to_upper_camel_snake_case("----"));
    EXPECT_EQ_STATIC(u8"________", to_upper_camel_snake_case(u8"_--__--_"));
}

TEST(UtilsIdentifierNaming, ToHttpHeaderCase)
{
    EXPECT_EQ_STATIC(L"Get-Name", to_http_header_case(L"getName"));
    EXPECT_EQ_STATIC("Set-Json-File-Path", to_http_header_case("setJsonFilePath"));
    // Note: irreversible convertion
    EXPECT_EQ_STATIC("Set-Json-File-Path", to_http_header_case("setJSONFilePath"));
    EXPECT_EQ_STATIC(L"Value-Offset", to_http_header_case(L"VALUE_OFFSET"));
    EXPECT_EQ_STATIC(U"Yaml-Parser", to_http_header_case(U"YamlParser"));
    EXPECT_EQ_STATIC(u8"Yaml-Parser", to_http_header_case(u8"YAMLParser"));
    EXPECT_EQ_STATIC(U"Num-Gc-Threads", to_http_header_case(U"num-gcThreads"));
    EXPECT_EQ_STATIC(u8"A", to_http_header_case(u8"a"));
    EXPECT_EQ_STATIC(U"A", to_http_header_case(U"A"));
    EXPECT_EQ_STATIC("$", to_http_header_case("$"));

    EXPECT_EQ_STATIC(u"Snake-Case", to_http_header_case(u"snake_case"));
    EXPECT_EQ_STATIC("All-Caps-Snake-Case", to_http_header_case("ALL_CAPS_SNAKE_CASE"));
    EXPECT_EQ_STATIC(U"Kebab-Case", to_http_header_case(U"kebab-case"));
    EXPECT_EQ_STATIC(u"All-Caps-Kebab-Case", to_http_header_case(u"ALL-CAPS-KEBAB-CASE"));
    EXPECT_EQ_STATIC("Lower-Camel-Case", to_http_header_case("lowerCamelCase"));
    EXPECT_EQ_STATIC(u8"Upper-Camel-Case", to_http_header_case(u8"UpperCamelCase"));
    EXPECT_EQ_STATIC(U"Lower-Camel-Snake-Case", to_http_header_case(U"lower_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(u8"Upper-Camel-Snake-Case", to_http_header_case(u8"Upper_Camel_Snake_Case"));
    EXPECT_EQ_STATIC(u8"Http-Header-Case", to_http_header_case(u8"HTTP-Header-Case"));

    // Non-alpha characters as lower case
    EXPECT_EQ_STATIC("Get-X1-And-Y1", to_http_header_case("getX1AndY1"));
    EXPECT_EQ_STATIC(u"X1y2", to_http_header_case(u"x1y2"));
    EXPECT_EQ_STATIC("X1y2", to_http_header_case("X1y2"));
    EXPECT_EQ_STATIC("X1-Y2", to_http_header_case("x1Y2"));
    EXPECT_EQ_STATIC(u8"X1-Y2", to_http_header_case(u8"X1Y2"));
    EXPECT_EQ_STATIC(L"$num-Args", to_http_header_case(L"$numArgs"));
    EXPECT_EQ_STATIC(L"$-Num-Args", to_http_header_case(L"$NumArgs"));
    EXPECT_EQ_STATIC(u8"$-N-Args", to_http_header_case(u8"$NArgs"));

    // Non-alpha characters as upper case
    EXPECT_EQ_STATIC(u8"Get-X1-And-Y1", to_http_header_case(U, u8"getX1AndY1"));
    EXPECT_EQ_STATIC(u"X-1y-2", to_http_header_case(U, u"x1y2"));
    EXPECT_EQ_STATIC(L"X-1y-2", to_http_header_case(U, L"X1y2"));
    EXPECT_EQ_STATIC("X-1y2", to_http_header_case(U, "x1Y2"));
    EXPECT_EQ_STATIC(u8"X1y2", to_http_header_case(U, u8"X1Y2"));
    EXPECT_EQ_STATIC(U"$num-Args", to_http_header_case(U, U"$numArgs"));
    EXPECT_EQ_STATIC(u"$-Num-Args", to_http_header_case(U, u"$NumArgs"));
    EXPECT_EQ_STATIC(L"$n-Args", to_http_header_case(U, L"$NArgs"));

    EXPECT_EQ_STATIC(u"Matrix-4x4", to_http_header_case(L, u"MATRIX_4x4"));
    EXPECT_EQ_STATIC(U"Matrix-4x4", to_http_header_case(U, U"MATRIX_4X4"));
    // Note: irreversible
    EXPECT_EQ_STATIC(L"Is-Ieee754-Float", to_http_header_case(U, L"IsIEEE754Float"));

    EXPECT_EQ_STATIC(U"Num-Args-", to_http_header_case(U"numArgs_"));
    EXPECT_EQ_STATIC("-Noreturn", to_http_header_case("_Noreturn"));
    EXPECT_EQ_STATIC(U"--No-Return", to_http_header_case(U"__NoReturn"));
    EXPECT_EQ_STATIC(U"--X--Y--", to_http_header_case(U"__X__Y__"));
    EXPECT_EQ_STATIC(L"---Ab-C--D-Ef---", to_http_header_case(L"___AbC__DEf___"));
    // When you forget to remove the "--" prefix:
    EXPECT_EQ_STATIC(L"--Num-Processes", to_http_header_case(L"--num-processes"));

    // Delimiters only
    EXPECT_EQ_STATIC("-", to_http_header_case("_"));
    EXPECT_EQ_STATIC("-", to_http_header_case("-"));
    EXPECT_EQ_STATIC("----", to_http_header_case("____"));
    EXPECT_EQ_STATIC(u"----", to_http_header_case(u"----"));
    EXPECT_EQ_STATIC(U"--------", to_http_header_case(U"_--__--_"));
}

TEST(UtilsIdentifierNaming, ConvertIdentifier)
{
    using enum identifier_naming_rule;

    EXPECT_EQ_STATIC("num_args", convert_identifier("numArgs", snake_case));
    EXPECT_EQ_STATIC(U"NUM_ARGS", convert_identifier(U"numArgs", all_caps_snake_case));
    EXPECT_EQ_STATIC("num-args", convert_identifier("numArgs", kebab_case));
    EXPECT_EQ_STATIC(L"NUM-ARGS", convert_identifier(L"numArgs", all_caps_kebab_case));
    EXPECT_EQ_STATIC(L"numArgs", convert_identifier(L"num-args", lower_camel_case));
    EXPECT_EQ_STATIC(L"NumArgs", convert_identifier(L"num-args", upper_camel_case));
    EXPECT_EQ_STATIC(u8"num_Args", convert_identifier(u8"num-args", lower_camel_snake_case));
    EXPECT_EQ_STATIC("Num_Args", convert_identifier("num-args", upper_camel_snake_case));
    EXPECT_EQ_STATIC("Num-Args", convert_identifier("num-args", http_header_case));
    // Fallback on invalid naming rule
    EXPECT_EQ("", convert_identifier("numArgs", static_cast<identifier_naming_rule>(123)));
}
