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

struct test_t {
    // ---- Static data members ----
    static constexpr int sdm_alpha = 10;
    static constexpr int sdm_beta = 20;
    static constexpr int sdm_gamma = 30;
    static inline double sdm_pi = 3.14;
    static inline double sdm_e = 2.718;

    // ---- Non-static data members ----
    int nsdm_alpha;
    int nsdm_beta;
    int nsdm_gamma;
    double nsdm_delta;
    double nsdm_epsilon;

    // ---- Static member functions ----
    static int smf_add(int x, int y)
    {
        return x + y;
    }
    static int smf_mul(int x, int y)
    {
        return x * y;
    }
    static double smf_div(double x, double y)
    {
        return x / y;
    }

    // ---- Non-static member functions ----
    int nsmf_sum() const
    {
        return nsdm_alpha + nsdm_beta + nsdm_gamma;
    }
    int nsmf_prod() const
    {
        return nsdm_alpha * nsdm_beta * nsdm_gamma;
    }
    double nsmf_avg() const
    {
        return (nsdm_delta + nsdm_epsilon) / 2.0;
    }
};

enum test_nsdm_key {
    nsdm_enum_alpha = 1,
    nsdm_enum_beta = 3,
    nsdm_enum_gamma = 6,
};

enum test_nsmf_key {
    nsmf_enum_sum = 2,
    nsmf_enum_prod = 4,
    nsmf_enum_avg = 7,
};

enum test_sdm_key {
    sdm_enum_alpha = 8,
    sdm_enum_beta = 9,
    sdm_enum_gamma = 10,
};

enum test_smf_key {
    smf_enum_add = 11,
    smf_enum_mul = 13,
    smf_enum_div = 17,
};

// ==========================================================================
// Non-Static Data Members
// ==========================================================================

// Overload (2.1): (T, pattern, transform_fn) where transform_fn: string_view -> optional<E>
TEST(ClassLookupTableByEnum, Overload21_NonStaticDataMembers)
{
    constexpr auto table = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(
        test_t, "nsdm_*", [](std::string_view name) -> std::optional<test_nsdm_key> {
            if (name == "alpha") return test_nsdm_key::nsdm_enum_alpha;
            if (name == "beta") return test_nsdm_key::nsdm_enum_beta;
            if (name == "gamma") return test_nsdm_key::nsdm_enum_gamma;
            return std::nullopt;
        });
    static_assert(std::is_same_v<int test_t::*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    constexpr auto obj = test_t{
        .nsdm_alpha = 1, .nsdm_beta = 2, .nsdm_gamma = 4, .nsdm_delta = 8, .nsdm_epsilon = 16};
    CHECK_MEMBER_VARIABLE_STATIC(1, obj, table[nsdm_enum_alpha]);
    CHECK_MEMBER_VARIABLE_STATIC(2, obj, table[nsdm_enum_beta]);
    CHECK_MEMBER_VARIABLE_STATIC(4, obj, table[nsdm_enum_gamma]);
    EXPECT_EQ_STATIC(nullptr, table[static_cast<test_nsdm_key>(2)]);
    EXPECT_EQ_STATIC(nullptr, table[static_cast<test_nsdm_key>(0)]);
}

// Overload (3.1): (T, transform_fn) where transform_fn: meta::info -> optional<E>
constexpr auto nsdm_key_from_name = [](std::meta::info member) -> std::optional<test_nsdm_key> {
    if (!has_identifier(member)) return std::nullopt;
    auto id = std::meta::identifier_of(member);
    if (!id.starts_with("nsdm_")) return std::nullopt;
    auto suffix = id.substr(5);
    if (suffix == "alpha") return nsdm_enum_alpha;
    if (suffix == "beta") return nsdm_enum_beta;
    if (suffix == "gamma") return nsdm_enum_gamma;
    return std::nullopt;
};

TEST(ClassLookupTableByEnum, Overload31_NonStaticDataMembers)
{
    constexpr auto table = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(test_t, nsdm_key_from_name);
    static_assert(std::is_same_v<int test_t::*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    constexpr auto obj = test_t{
        .nsdm_alpha = 1, .nsdm_beta = 2, .nsdm_gamma = 4, .nsdm_delta = 8, .nsdm_epsilon = 16};
    CHECK_MEMBER_VARIABLE_STATIC(1, obj, table[nsdm_enum_alpha]);
    CHECK_MEMBER_VARIABLE_STATIC(2, obj, table[nsdm_enum_beta]);
    CHECK_MEMBER_VARIABLE_STATIC(4, obj, table[nsdm_enum_gamma]);
    EXPECT_EQ_STATIC(nullptr, table[static_cast<test_nsdm_key>(99)]);
}

// ==========================================================================
// Non-Static Member Functions
// ==========================================================================

// Overload (2.1)
TEST(ClassLookupTableByEnum, Overload21_NonStaticMemberFunctions)
{
    constexpr auto table = RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(
        test_t, "nsmf_*", [](std::string_view name) -> std::optional<test_nsmf_key> {
            if (name == "sum") return test_nsmf_key::nsmf_enum_sum;
            if (name == "prod") return test_nsmf_key::nsmf_enum_prod;
            if (name == "avg") return test_nsmf_key::nsmf_enum_avg;
            return std::nullopt;
        });
    // Mixed return types (int, int, double) → meta_variant
    static_assert(table.size() == 3);

    constexpr auto obj = test_t{
        .nsdm_alpha = 3, .nsdm_beta = 5, .nsdm_gamma = 7, .nsdm_delta = 10, .nsdm_epsilon = 20};
    auto visitor = [&obj](auto mptr) -> double {
        if (mptr == nullptr) return -1.0;
        return (obj.*mptr)();
    };
    EXPECT_EQ(15.0, table[nsmf_enum_sum].visit(visitor));
    EXPECT_EQ(105.0, table[nsmf_enum_prod].visit(visitor));
    EXPECT_EQ(15.0, table[nsmf_enum_avg].visit(visitor));
    EXPECT_EQ(-1.0, table[static_cast<test_nsmf_key>(0)].visit(visitor));
}

// Overload (3.1)
TEST(ClassLookupTableByEnum, Overload31_NonStaticMemberFunctions)
{
    constexpr auto table = RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(
        test_t, [](std::meta::info member) -> std::optional<test_nsmf_key> {
            if (!is_function(member) || is_static_member(member)) return std::nullopt;
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (id == "nsmf_sum") return nsmf_enum_sum;
            if (id == "nsmf_prod") return nsmf_enum_prod;
            if (id == "nsmf_avg") return nsmf_enum_avg;
            return std::nullopt;
        });
    static_assert(table.size() == 3);

    constexpr auto obj = test_t{
        .nsdm_alpha = 3, .nsdm_beta = 5, .nsdm_gamma = 7, .nsdm_delta = 10, .nsdm_epsilon = 20};
    auto visitor = [&obj](auto mptr) -> double {
        if (mptr == nullptr) return -1.0;
        return (obj.*mptr)();
    };
    EXPECT_EQ(15.0, table[nsmf_enum_sum].visit(visitor));
    EXPECT_EQ(105.0, table[nsmf_enum_prod].visit(visitor));
    EXPECT_EQ(15.0, table[nsmf_enum_avg].visit(visitor));
    EXPECT_EQ(-1.0, table[static_cast<test_nsmf_key>(0)].visit(visitor));
}

// ==========================================================================
// Static Data Members
// ==========================================================================

// Overload (2.1)
TEST(ClassLookupTableByEnum, Overload21_StaticDataMembers)
{
    constexpr auto table = RBOX_STATIC_DATA_MEMBER_FIXED_MAP(
        test_t, "sdm_*", [](std::string_view name) -> std::optional<test_sdm_key> {
            if (name == "alpha") return sdm_enum_alpha;
            if (name == "beta") return sdm_enum_beta;
            if (name == "gamma") return sdm_enum_gamma;
            return std::nullopt;
        });
    static_assert(std::is_same_v<const int*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    CHECK_VARIABLE(10, table[sdm_enum_alpha]);
    CHECK_VARIABLE(20, table[sdm_enum_beta]);
    CHECK_VARIABLE(30, table[sdm_enum_gamma]);
    EXPECT_EQ_STATIC(nullptr, table[static_cast<test_sdm_key>(0)]);
    EXPECT_EQ_STATIC(nullptr, table[static_cast<test_sdm_key>(5)]);
}

// Overload (3.1)
TEST(ClassLookupTableByEnum, Overload31_StaticDataMembers)
{
    constexpr auto table = RBOX_STATIC_DATA_MEMBER_FIXED_MAP(
        test_t, [](std::meta::info member) -> std::optional<test_sdm_key> {
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (!id.starts_with("sdm_")) return std::nullopt;
            auto suffix = id.substr(4);
            if (suffix == "alpha") return sdm_enum_alpha;
            if (suffix == "beta") return sdm_enum_beta;
            if (suffix == "gamma") return sdm_enum_gamma;
            return std::nullopt;
        });
    static_assert(std::is_same_v<const int*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    CHECK_VARIABLE(10, table[sdm_enum_alpha]);
    CHECK_VARIABLE(20, table[sdm_enum_beta]);
    CHECK_VARIABLE(30, table[sdm_enum_gamma]);
    EXPECT_EQ_STATIC(nullptr, table[static_cast<test_sdm_key>(0)]);
    EXPECT_EQ_STATIC(nullptr, table[static_cast<test_sdm_key>(99)]);
}

// ==========================================================================
// Static Member Functions
// ==========================================================================

// Overload (2.1)
TEST(ClassLookupTableByEnum, Overload21_StaticMemberFunctions)
{
    constexpr auto table = RBOX_STATIC_MEMBER_FUNCTION_FIXED_MAP(
        test_t, "smf_*", [](std::string_view name) -> std::optional<test_smf_key> {
            if (name == "add") return smf_enum_add;
            if (name == "mul") return smf_enum_mul;
            if (name == "div") return smf_enum_div;
            return std::nullopt;
        });
    // Mixed return types (int, int, double) → meta_variant
    static_assert(table.size() == 3);

    auto visitor = [](auto fptr) -> double {
        if (fptr == nullptr) return -999.0;
        return fptr(3, 4);
    };
    EXPECT_EQ(7.0, table[smf_enum_add].visit(visitor));
    EXPECT_EQ(12.0, table[smf_enum_mul].visit(visitor));
    EXPECT_EQ(0.75, table[smf_enum_div].visit(visitor));
    EXPECT_EQ(-999.0, table[static_cast<test_smf_key>(0)].visit(visitor));
}

// Overload (3.1)
TEST(ClassLookupTableByEnum, Overload31_StaticMemberFunctions)
{
    constexpr auto table = RBOX_STATIC_MEMBER_FUNCTION_FIXED_MAP(
        test_t, [](std::meta::info member) -> std::optional<test_smf_key> {
            if (!is_static_member(member) || !is_function(member)) return std::nullopt;
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (id == "smf_add") return smf_enum_add;
            if (id == "smf_mul") return smf_enum_mul;
            if (id == "smf_div") return smf_enum_div;
            return std::nullopt;
        });
    static_assert(table.size() == 3);

    auto visitor = [](auto fptr) -> double {
        if (fptr == nullptr) return -999.0;
        return fptr(3, 4);
    };
    EXPECT_EQ(7.0, table[smf_enum_add].visit(visitor));
    EXPECT_EQ(12.0, table[smf_enum_mul].visit(visitor));
    EXPECT_EQ(0.75, table[smf_enum_div].visit(visitor));
    EXPECT_EQ(-999.0, table[static_cast<test_smf_key>(0)].visit(visitor));
}
