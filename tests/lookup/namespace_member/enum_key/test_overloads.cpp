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

#include <rbox/enum/enum_cast.hpp>
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

enum test_var_key {
    var_enum_alpha = -1,
    var_enum_beta = 1,
    var_enum_gamma = 3,
};

enum test_fn_key {
    fn_enum_sum = 7,
    fn_enum_mul = 11,
    fn_enum_avg = 13,
};

// ==========================================================================
// VARIABLE_FIXED_MAP
// ==========================================================================

// Overload (2.1): (ns, pattern, transform_fn) where transform_fn: string_view -> optional<E>
TEST(NamespaceLookupTableByEnum, Overload21_Variables)
{
    constexpr auto table = RBOX_VARIABLE_FIXED_MAP(
        test_ns, "v_*", [](std::string_view name) -> std::optional<test_var_key> {
            if (name == "alpha") return var_enum_alpha;
            if (name == "beta") return var_enum_beta;
            if (name == "gamma") return var_enum_gamma;
            return std::nullopt;
        });
    static_assert(std::is_same_v<const int*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    CHECK_VARIABLE(1, table[var_enum_alpha]);
    CHECK_VARIABLE(2, table[var_enum_beta]);
    CHECK_VARIABLE(3, table[var_enum_gamma]);
    EXPECT_EQ_STATIC(nullptr, table[static_cast<test_var_key>(5)]);
    EXPECT_EQ_STATIC(nullptr, table[static_cast<test_var_key>(-2)]);
}

// Overload (3.1): (ns, transform_fn) where transform_fn: meta::info -> optional<E>
TEST(NamespaceLookupTableByEnum, Overload31_Variables)
{
    constexpr auto table =
        RBOX_VARIABLE_FIXED_MAP(test_ns, [](std::meta::info member) -> std::optional<test_var_key> {
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (!id.starts_with("v_")) return std::nullopt;
            auto suffix = id.substr(2);
            if (suffix == "alpha") return var_enum_alpha;
            if (suffix == "beta") return var_enum_beta;
            if (suffix == "gamma") return var_enum_gamma;
            return std::nullopt;
        });
    static_assert(std::is_same_v<const int*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    CHECK_VARIABLE(1, table[var_enum_alpha]);
    CHECK_VARIABLE(2, table[var_enum_beta]);
    CHECK_VARIABLE(3, table[var_enum_gamma]);
    EXPECT_EQ_STATIC(nullptr, table[static_cast<test_var_key>(99)]);
}

// ==========================================================================
// FUNCTION_FIXED_MAP
// ==========================================================================

// Overload (2.1): (ns, pattern, transform_fn) where transform_fn: string_view -> optional<E>
TEST(NamespaceLookupTableByEnum, Overload21_Functions)
{
    constexpr auto table = RBOX_FUNCTION_FIXED_MAP(
        test_ns, "f_*", [](std::string_view name) -> std::optional<test_fn_key> {
            if (name == "sum") return fn_enum_sum;
            if (name == "mul") return fn_enum_mul;
            if (name == "avg") return fn_enum_avg;
            return std::nullopt;
        });
    static_assert(table.size() == 3);

    auto visitor = [](auto fptr) -> double {
        if (fptr == nullptr) return -999.0;
        return fptr(3, 4);
    };
    EXPECT_EQ(7.0, table[fn_enum_sum].visit(visitor));
    EXPECT_EQ(12.0, table[fn_enum_mul].visit(visitor));
    EXPECT_EQ(3.5, table[fn_enum_avg].visit(visitor));
    EXPECT_EQ(-999.0, table[static_cast<test_fn_key>(0)].visit(visitor));
}

// Overload (3.1): (ns, transform_fn) where transform_fn: meta::info -> optional<E>
TEST(NamespaceLookupTableByEnum, Overload31_Functions)
{
    constexpr auto table =
        RBOX_FUNCTION_FIXED_MAP(test_ns, [](std::meta::info member) -> std::optional<test_fn_key> {
            if (!is_function(member)) return std::nullopt;
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (id == "f_sum") return fn_enum_sum;
            if (id == "f_mul") return fn_enum_mul;
            if (id == "f_avg") return fn_enum_avg;
            return std::nullopt;
        });
    static_assert(table.size() == 3);

    auto visitor = [](auto fptr) -> double {
        if (fptr == nullptr) return -999.0;
        return fptr(3, 4);
    };
    EXPECT_EQ(7.0, table[fn_enum_sum].visit(visitor));
    EXPECT_EQ(12.0, table[fn_enum_mul].visit(visitor));
    EXPECT_EQ(3.5, table[fn_enum_avg].visit(visitor));
    EXPECT_EQ(-999.0, table[static_cast<test_fn_key>(0)].visit(visitor));
}
