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

#include <rbox/lookup.hpp>

#include "tests/lookup/lookup_test_options.hpp"

namespace test_ns {
constexpr int v_alpha = 1;
constexpr int v_beta = 2;
constexpr int v_gamma = 3;
double v_delta = 4.0;
double v_epsilon = 5.0;

constexpr int f_sum(int x, int y)
{
    return x + y;
}
constexpr int f_mul(int x, int y)
{
    return x * y;
}
double f_avg(double x, double y)
{
    return (x + y) / 2.0;
}
}  // namespace test_ns

// ==========================================================================
// VARIABLE_FIXED_MAP
// ==========================================================================

// Overload (1): (ns, pattern)
TEST(NamespaceLookupTableByName, Overload1_Variables)
{
    constexpr auto table = RBOX_VARIABLE_FIXED_MAP(test_ns, "v_*");
    // Matches all 5: 3x constexpr int + 2x double → meta_variant
    static_assert(table.size() == 5);

    auto visitor = [](auto ptr) -> std::string {
        if (ptr == nullptr) return "<null>";
        return std::to_string(*ptr);
    };
    EXPECT_EQ("1", table["alpha"].visit(visitor));
    EXPECT_EQ("2", table["beta"].visit(visitor));
    EXPECT_EQ("3", table["gamma"].visit(visitor));
    EXPECT_EQ("4", table["delta"].visit(visitor));
    EXPECT_EQ("5", table["epsilon"].visit(visitor));
    EXPECT_EQ("<null>", table["v_alpha"].visit(visitor));
    EXPECT_EQ("<null>", table[""].visit(visitor));
}

// Overload (2.2): (ns, pattern, transform_fn) where transform_fn: string_view -> optional<string>
TEST(NamespaceLookupTableByName, Overload22_Variables)
{
    constexpr auto options = rbox::string_key_fixed_map_options{};
    constexpr auto table = RBOX_VARIABLE_FIXED_MAP(
        test_ns,
        "v_*",
        [](std::string_view name) -> std::optional<std::string> {
            if (name == "delta" || name == "epsilon") return std::nullopt;
            return std::string{name};
        },
        options);
    static_assert(std::is_same_v<const int*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    CHECK_VARIABLE(1, table["alpha"]);
    CHECK_VARIABLE(2, table["beta"]);
    CHECK_VARIABLE(3, table["gamma"]);
    EXPECT_EQ(nullptr, table["delta"]);
    EXPECT_EQ(nullptr, table["epsilon"]);
}

// Overload (3.2): (ns, transform_fn) where transform_fn: meta::info -> optional<string>
TEST(NamespaceLookupTableByName, Overload32_Variables)
{
    constexpr auto table =
        RBOX_VARIABLE_FIXED_MAP(test_ns, [](std::meta::info member) -> std::optional<std::string> {
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (!id.starts_with("v_")) return std::nullopt;
            // Only keep alpha, beta, gamma (exclude delta, epsilon)
            auto suffix = id.substr(2);
            if (suffix == "alpha" || suffix == "beta" || suffix == "gamma") {
                return std::string{suffix};
            }
            return std::nullopt;
        });
    static_assert(std::is_same_v<const int*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    CHECK_VARIABLE(1, table["alpha"]);
    CHECK_VARIABLE(2, table["beta"]);
    CHECK_VARIABLE(3, table["gamma"]);
    EXPECT_EQ(nullptr, table["delta"]);
    EXPECT_EQ(nullptr, table["epsilon"]);
}

// ==========================================================================
// FUNCTION_FIXED_MAP
// ==========================================================================

// Overload (1): (ns, pattern)
TEST(NamespaceLookupTableByName, Overload1_Functions)
{
    constexpr auto table = RBOX_FUNCTION_FIXED_MAP(test_ns, "f_*");
    static_assert(table.size() == 3);

    auto visitor = [](auto fptr) -> double {
        if (fptr == nullptr) return -999.0;
        return fptr(3, 4);
    };
    EXPECT_EQ(7.0, table["sum"].visit(visitor));
    EXPECT_EQ(12.0, table["mul"].visit(visitor));
    EXPECT_EQ(3.5, table["avg"].visit(visitor));
    EXPECT_EQ(-999.0, table["alpha"].visit(visitor));
    EXPECT_EQ(-999.0, table[""].visit(visitor));
}

// Overload (2.2): (ns, pattern, transform_fn) where transform_fn: string_view -> optional<string>
TEST(NamespaceLookupTableByName, Overload22_Functions)
{
    constexpr auto options = rbox::string_key_fixed_map_options{};
    constexpr auto table = RBOX_FUNCTION_FIXED_MAP(
        test_ns,
        "f_*",
        [](std::string_view name) -> std::optional<std::string> {
            if (name == "avg") return std::nullopt;
            return std::string{name};
        },
        options);
    static_assert(std::is_same_v<int (*)(int, int), decltype(table)::value_type>);
    static_assert(table.size() == 2);

    CHECK_FUNCTION(7, table["sum"], 3, 4);
    CHECK_FUNCTION(12, table["mul"], 3, 4);
    EXPECT_EQ(nullptr, table["avg"]);
    EXPECT_EQ(nullptr, table[""]);
}

// Overload (3.2): (ns, transform_fn) where transform_fn: meta::info -> optional<string>
TEST(NamespaceLookupTableByName, Overload32_Functions)
{
    constexpr auto table =
        RBOX_FUNCTION_FIXED_MAP(test_ns, [](std::meta::info member) -> std::optional<std::string> {
            if (!is_function(member)) return std::nullopt;
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (id.starts_with("f_")) {
                return std::string{id};
            }
            return std::nullopt;
        });
    static_assert(table.size() == 3);

    auto visitor = [](auto fptr) -> double {
        if (fptr == nullptr) return -999.0;
        return fptr(3, 4);
    };
    EXPECT_EQ(7.0, table["f_sum"].visit(visitor));
    EXPECT_EQ(12.0, table["f_mul"].visit(visitor));
    EXPECT_EQ(3.5, table["f_avg"].visit(visitor));
    EXPECT_EQ(-999.0, table["sum"].visit(visitor));
    EXPECT_EQ(-999.0, table[""].visit(visitor));
}
