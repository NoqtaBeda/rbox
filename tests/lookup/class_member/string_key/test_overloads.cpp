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

struct test_t {
    // ---- Static data members ----
    static constexpr int sdm_a = 10;
    static constexpr int sdm_b = 20;
    static constexpr int sdm_c = 30;
    static inline double sdm_pi = 3.14;
    static inline double sdm_euler = 2.718;

    // ---- Non-static data members ----
    int nsdm_a;
    int nsdm_b;
    int nsdm_c;
    double nsdm_d;
    double nsdm_e;

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
        return nsdm_a + nsdm_b + nsdm_c;
    }
    int nsmf_prod() const
    {
        return nsdm_a * nsdm_b * nsdm_c;
    }
    double nsmf_avg() const
    {
        return (nsdm_d + nsdm_e) / 2.0;
    }
};

// ==========================================================================
// Non-Static Data Members
// ==========================================================================

TEST(ClassLookupTableByName, Overload1_NonStaticDataMembers)
{
    constexpr auto table = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(test_t, "nsdm_*");
    static_assert(table.size() == 5);

    constexpr auto obj =
        test_t{.nsdm_a = 1, .nsdm_b = 2, .nsdm_c = 4, .nsdm_d = 7.5, .nsdm_e = 16.5};
    auto visitor = [&obj](auto mptr) -> std::string {
        if (mptr == nullptr) {
            return "<null>";
        }
        return std::to_string(obj.*mptr);
    };
    EXPECT_EQ("1", table["a"].visit(visitor));
    EXPECT_EQ("2", table["b"].visit(visitor));
    EXPECT_EQ("4", table["c"].visit(visitor));
    EXPECT_EQ("7.5", table["d"].visit(visitor));
    EXPECT_EQ("16.5", table["e"].visit(visitor));

    EXPECT_EQ("<null>", table["nsdm_a"].visit(visitor));
    EXPECT_EQ("<null>", table[""].visit(visitor));
}

TEST(ClassLookupTableByName, Overload22_NonStaticDataMembers)
{
    constexpr auto options = rbox::string_key_fixed_map_options{};
    constexpr auto table = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(
        test_t,
        "nsdm_*",
        [](std::string_view name) -> std::optional<std::string> {
            if (name == "d" || name == "e") return std::nullopt;
            return std::string{name};
        },
        options);
    static_assert(std::is_same_v<int test_t::*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    constexpr auto obj = test_t{.nsdm_a = 1, .nsdm_b = 2, .nsdm_c = 4, .nsdm_d = 8, .nsdm_e = 16};
    CHECK_MEMBER_VARIABLE_STATIC(1, obj, table["a"]);
    CHECK_MEMBER_VARIABLE_STATIC(2, obj, table["b"]);
    CHECK_MEMBER_VARIABLE_STATIC(4, obj, table["c"]);
    EXPECT_EQ_STATIC(nullptr, table["d"]);
    EXPECT_EQ_STATIC(nullptr, table["e"]);
}

TEST(ClassLookupTableByName, Overload32_NonStaticDataMembers)
{
    constexpr auto table = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(
        test_t, [](std::meta::info member) -> std::optional<std::string> {
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (!id.starts_with("nsdm_")) return std::nullopt;
            auto suffix = id.substr(5);
            if (suffix.length() == 1 && 'a' <= suffix[0] && suffix[0] <= 'c') {
                return std::string{suffix};
            }
            return std::nullopt;
        });
    static_assert(std::is_same_v<int test_t::*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    constexpr auto obj = test_t{.nsdm_a = 1, .nsdm_b = 2, .nsdm_c = 4, .nsdm_d = 8, .nsdm_e = 16};
    CHECK_MEMBER_VARIABLE_STATIC(1, obj, table["a"]);
    CHECK_MEMBER_VARIABLE_STATIC(2, obj, table["b"]);
    CHECK_MEMBER_VARIABLE_STATIC(4, obj, table["c"]);
    EXPECT_EQ_STATIC(nullptr, table["d"]);
    EXPECT_EQ_STATIC(nullptr, table["e"]);
}

// ==========================================================================
// Non-Static Member Functions
// ==========================================================================

TEST(ClassLookupTableByName, Overload1_NonStaticMemberFunctions)
{
    constexpr auto table = RBOX_PUBLIC_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(test_t, "nsmf_*");
    static_assert(table.size() == 3);

    constexpr auto obj = test_t{.nsdm_a = 3, .nsdm_b = 5, .nsdm_c = 7, .nsdm_d = 10, .nsdm_e = 20};
    auto visitor = [&obj](auto mptr) -> double {
        if (mptr == nullptr) return -1.0;
        return (obj.*mptr)();
    };
    EXPECT_EQ(15.0, table["sum"].visit(visitor));
    EXPECT_EQ(105.0, table["prod"].visit(visitor));
    EXPECT_EQ(15.0, table["avg"].visit(visitor));

    EXPECT_EQ(-1.0, table["a"].visit(visitor));
    EXPECT_EQ(-1.0, table["NSMF_SUM"].visit(visitor));
}

TEST(ClassLookupTableByName, Overload22_NonStaticMemberFunctions)
{
    constexpr auto options = rbox::string_key_fixed_map_options{};
    constexpr auto table = RBOX_PUBLIC_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(
        test_t,
        "nsmf_*",
        [](std::string_view name) -> std::optional<std::string> {
            if (name == "avg") return std::nullopt;
            return std::string{name};
        },
        options);
    static_assert(std::is_same_v<int (test_t::*)() const, decltype(table)::value_type>);
    static_assert(table.size() == 2);

    constexpr auto obj = test_t{.nsdm_a = 3, .nsdm_b = 5, .nsdm_c = 7, .nsdm_d = 10, .nsdm_e = 20};
    CHECK_MEMBER_FUNCTION(15, obj, table["sum"]);
    CHECK_MEMBER_FUNCTION(105, obj, table["prod"]);
    EXPECT_EQ_STATIC(nullptr, table["avg"]);
    EXPECT_EQ_STATIC(nullptr, table["e"]);
}

TEST(ClassLookupTableByName, Overload32_NonStaticMemberFunctions)
{
    constexpr auto table = RBOX_PUBLIC_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(
        test_t, [](std::meta::info member) -> std::optional<std::string> {
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (id.starts_with("nsmf_")) {
                return std::string{id};
            }
            return std::nullopt;
        });
    static_assert(table.size() == 3);

    constexpr auto obj = test_t{.nsdm_a = 3, .nsdm_b = 5, .nsdm_c = 7, .nsdm_d = 10, .nsdm_e = 20};
    auto visitor = [&obj](auto mptr) -> double {
        if (mptr == nullptr) return -1.0;
        return (obj.*mptr)();
    };
    EXPECT_EQ(15.0, table["nsmf_sum"].visit(visitor));
    EXPECT_EQ(105.0, table["nsmf_prod"].visit(visitor));
    EXPECT_EQ(15.0, table["nsmf_avg"].visit(visitor));

    EXPECT_EQ(-1.0, table["sum"].visit(visitor));
    EXPECT_EQ(-1.0, table["nsmf_x"].visit(visitor));
}

// ==========================================================================
// Static Data Members
// ==========================================================================

TEST(ClassLookupTableByName, Overload1_StaticDataMembers)
{
    constexpr auto table = RBOX_STATIC_DATA_MEMBER_FIXED_MAP(test_t, "sdm_*");
    static_assert(table.size() == 5);

    auto visitor = [](auto ptr) -> std::string {
        if (ptr == nullptr) return "<null>";
        return std::to_string(*ptr);
    };
    EXPECT_EQ("10", table["a"].visit(visitor));
    EXPECT_EQ("20", table["b"].visit(visitor));
    EXPECT_EQ("30", table["c"].visit(visitor));
    EXPECT_EQ("3.14", table["pi"].visit(visitor));
    EXPECT_EQ("2.718", table["euler"].visit(visitor));

    EXPECT_EQ("<null>", table["sdm_a"].visit(visitor));
    EXPECT_EQ("<null>", table[""].visit(visitor));
}

TEST(ClassLookupTableByName, Overload22_StaticDataMembers)
{
    constexpr auto options = rbox::string_key_fixed_map_options{};
    constexpr auto table = RBOX_STATIC_DATA_MEMBER_FIXED_MAP(
        test_t,
        "sdm_*",
        [](std::string_view name) -> std::optional<std::string> {
            if (name.length() == 1) return std::string{name};  // "a", "b", "c"
            return std::nullopt;                               // skip multi-letter names
        },
        options);
    static_assert(std::is_same_v<const int*, decltype(table)::value_type>);
    static_assert(table.size() == 3);

    CHECK_VARIABLE(10, table["a"]);
    CHECK_VARIABLE(20, table["b"]);
    CHECK_VARIABLE(30, table["c"]);
    EXPECT_EQ_STATIC(nullptr, table["pi"]);
    EXPECT_EQ_STATIC(nullptr, table["euler"]);
}

TEST(ClassLookupTableByName, Overload32_StaticDataMembers)
{
    constexpr auto table = RBOX_STATIC_DATA_MEMBER_FIXED_MAP(
        test_t, [](std::meta::info member) -> std::optional<std::string> {
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (!id.starts_with("sdm_")) return std::nullopt;
            return std::string{id.substr(4)};
        });
    static_assert(table.size() == 5);

    auto visitor = [](auto ptr) -> std::string {
        if (ptr == nullptr) return "<null>";
        return std::to_string(*ptr);
    };
    EXPECT_EQ("10", table["a"].visit(visitor));
    EXPECT_EQ("20", table["b"].visit(visitor));
    EXPECT_EQ("30", table["c"].visit(visitor));
    EXPECT_EQ("3.14", table["pi"].visit(visitor));
    EXPECT_EQ("2.718", table["euler"].visit(visitor));

    EXPECT_EQ("<null>", table["x"].visit(visitor));
}

// ==========================================================================
// Static Member Functions
// ==========================================================================

TEST(ClassLookupTableByName, Overload1_StaticMemberFunctions)
{
    constexpr auto table = RBOX_STATIC_MEMBER_FUNCTION_FIXED_MAP(test_t, "smf_*");
    static_assert(table.size() == 3);

    auto visitor = [](auto fptr) -> double {
        if (fptr == nullptr) return -1.0;
        return fptr(3, 4);
    };
    EXPECT_EQ(7.0, table["add"].visit(visitor));
    EXPECT_EQ(12.0, table["mul"].visit(visitor));
    EXPECT_EQ(0.75, table["div"].visit(visitor));

    EXPECT_EQ(-1.0, table["smf_add"].visit(visitor));
    EXPECT_EQ(-1.0, table[""].visit(visitor));
}

TEST(ClassLookupTableByName, Overload22_StaticMemberFunctions)
{
    constexpr auto options = rbox::string_key_fixed_map_options{};
    constexpr auto table = RBOX_STATIC_MEMBER_FUNCTION_FIXED_MAP(
        test_t,
        "smf_*",
        [](std::string_view name) -> std::optional<std::string> {
            if (name == "div") return std::nullopt;
            return std::string{name};
        },
        options);
    static_assert(std::is_same_v<int (*)(int, int), decltype(table)::value_type>);
    static_assert(table.size() == 2);

    CHECK_FUNCTION(7, table["add"], 3, 4);
    CHECK_FUNCTION(12, table["mul"], 3, 4);
    EXPECT_EQ_STATIC(nullptr, table["div"]);
}

TEST(ClassLookupTableByName, Overload32_StaticMemberFunctions)
{
    constexpr auto table = RBOX_STATIC_MEMBER_FUNCTION_FIXED_MAP(
        test_t, [](std::meta::info member) -> std::optional<std::string> {
            if (!has_identifier(member)) return std::nullopt;
            auto id = std::meta::identifier_of(member);
            if (id.starts_with("smf_")) {
                return std::string{id.substr(4)};  // strip prefix
            }
            return std::nullopt;
        });
    static_assert(table.size() == 3);

    auto visitor = [](auto fptr) -> double {
        if (fptr == nullptr) return -1.0;
        return fptr(3, 4);
    };
    EXPECT_EQ(7.0, table["add"].visit(visitor));
    EXPECT_EQ(12.0, table["mul"].visit(visitor));
    EXPECT_EQ(0.75, table["div"].visit(visitor));

    EXPECT_EQ(-1.0, table[""].visit(visitor));
}
