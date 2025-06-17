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

// ==========================================================================
// Non-Static Member Functions
// ==========================================================================

struct nsmf_test_t {
    double value_x = 0;
    double value_y = 0;
    double value_z = 0;

public:
    constexpr double pub_squared_sum() const
    {
        return value_x * value_x + value_y * value_y;
    }

private:
    constexpr double priv_sum() const
    {
        return value_x + value_y + value_z;
    }

protected:
    constexpr double prot_mul() const
    {
        return value_x * value_y * value_z;
    }

public:
    constexpr double pub_div() const
    {
        return value_y == 0 ? 0 : value_x / value_y;
    }

private:
    constexpr double priv_min() const
    {
        return value_x < value_y ? value_x : value_y;
    }

protected:
    constexpr double prot_div() const
    {
        return value_y == 0 ? 0 : value_x / value_y;
    }

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_nonstatic_member_function_fixed_map(^^nsmf_test_t, "*");
    }
};

constexpr auto nsmf_table_public =
    RBOX_PUBLIC_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(nsmf_test_t, "*");
constexpr auto nsmf_table_priv = [:nsmf_test_t::build_priv_table():];

TEST(ClassLookupTableByAccessContext, NonStaticMemberFunctionsSizes)
{
    static_assert(nsmf_table_priv.size() == 6);
    static_assert(nsmf_table_public.size() == 2);
}

TEST(ClassLookupTableByAccessContext, NonStaticMemberFunctionsCheckValues)
{
    constexpr auto obj = nsmf_test_t{.value_x = 3, .value_y = 4, .value_z = 5};
    CHECK_MEMBER_FUNCTION_STATIC(25.0, obj, nsmf_table_public["pub_squared_sum"]);
    CHECK_MEMBER_FUNCTION_STATIC(0.75, obj, nsmf_table_public["pub_div"]);
    EXPECT_EQ_STATIC(nullptr, nsmf_table_public["priv_sum"]);
    EXPECT_EQ_STATIC(nullptr, nsmf_table_public["prot_mul"]);

    CHECK_MEMBER_FUNCTION_STATIC(25.0, obj, nsmf_table_priv["pub_squared_sum"]);
    CHECK_MEMBER_FUNCTION_STATIC(0.75, obj, nsmf_table_priv["pub_div"]);
    CHECK_MEMBER_FUNCTION_STATIC(12.0, obj, nsmf_table_priv["priv_sum"]);
    CHECK_MEMBER_FUNCTION_STATIC(60.0, obj, nsmf_table_priv["prot_mul"]);
    CHECK_MEMBER_FUNCTION_STATIC(3.0, obj, nsmf_table_priv["priv_min"]);
    CHECK_MEMBER_FUNCTION_STATIC(0.75, obj, nsmf_table_priv["prot_div"]);
}

// ==========================================================================
// Static Data Members
// ==========================================================================

struct sdm_test_t {
    static inline int s_a = 1;

private:
    static inline int s_b = 2;

protected:
    static constexpr double s_c = 3.14;

public:
    static inline int s_d = 4;

private:
    static inline int s_e = 5;

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_data_member_fixed_map(^^sdm_test_t, "*");
    }
};

constexpr auto sdm_table_public = RBOX_PUBLIC_STATIC_DATA_MEMBER_FIXED_MAP(sdm_test_t, "*");
constexpr auto sdm_table_priv = [:sdm_test_t::build_priv_table():];

TEST(ClassLookupTableByAccessContext, StaticDataMembersSizes)
{
    static_assert(sdm_table_priv.size() == 5);
    static_assert(sdm_table_public.size() == 2);
}

TEST(ClassLookupTableByAccessContext, StaticDataMembersCheckValues)
{
    auto visitor = [](auto ptr) -> double {
        if (ptr == nullptr) return -1.0;
        return *ptr;
    };
    CHECK_VARIABLE(1, sdm_table_public["s_a"]);
    CHECK_VARIABLE(4, sdm_table_public["s_d"]);
    EXPECT_EQ(nullptr, sdm_table_public["s_b"]);
    EXPECT_EQ(nullptr, sdm_table_public["s_e"]);

    EXPECT_EQ(1.0, sdm_table_priv["s_a"].visit(visitor));
    EXPECT_EQ(2.0, sdm_table_priv["s_b"].visit(visitor));
    EXPECT_EQ(3.14, sdm_table_priv["s_c"].visit(visitor));
    EXPECT_EQ(4.0, sdm_table_priv["s_d"].visit(visitor));
    EXPECT_EQ(5.0, sdm_table_priv["s_e"].visit(visitor));
}

// ==========================================================================
// Static Member Functions
// ==========================================================================

struct smf_test_t {
    static int pub_add(int x, int y)
    {
        return x + y;
    }

private:
    static int priv_mul(int x, int y)
    {
        return x * y;
    }

protected:
    static int prot_sub(int x, int y)
    {
        return x - y;
    }

public:
    static double pub_div(double x, double y)
    {
        return x / y;
    }

    static consteval auto build_priv_table()
    {
        return rbox::make_static_member_function_fixed_map(
            ^^smf_test_t, [](std::meta::info member) -> std::optional<std::string> {
              if (member != ^^smf_test_t::build_priv_table) {
                  return std::string{identifier_of(member)};
              } else {
                  return std::nullopt;
              }
            });
    }
};

constexpr auto smf_table_public = RBOX_PUBLIC_STATIC_MEMBER_FUNCTION_FIXED_MAP(
    smf_test_t, [](std::meta::info member) -> std::optional<std::string> {
        if (member != ^^smf_test_t::build_priv_table) {
            return std::string{identifier_of(member)};
        } else {
            return std::nullopt;
        }
    });
constexpr auto smf_table_priv = [:smf_test_t::build_priv_table():];

TEST(ClassLookupTableByAccessContext, StaticMemberFunctionsSizes)
{
    static_assert(smf_table_priv.size() == 4);
    static_assert(smf_table_public.size() == 2);
}

TEST(ClassLookupTableByAccessContext, StaticMemberFunctionsCheckValues)
{
    auto visitor_int = [](auto fptr) -> int {
        if (fptr == nullptr) return -999;
        return fptr(10, 3);
    };
    auto visitor_dbl = [](auto fptr) -> double {
        if (fptr == nullptr) return -999.0;
        return fptr(10.0, 4.0);
    };

    EXPECT_EQ(13, smf_table_public["pub_add"].visit(visitor_int));
    EXPECT_EQ(2.5, smf_table_public["pub_div"].visit(visitor_dbl));
    EXPECT_EQ(-999, smf_table_public["priv_mul"].visit(visitor_int));
    EXPECT_EQ(-999, smf_table_public["prot_sub"].visit(visitor_int));

    EXPECT_EQ(13, smf_table_priv["pub_add"].visit(visitor_int));
    EXPECT_EQ(30, smf_table_priv["priv_mul"].visit(visitor_int));
    EXPECT_EQ(7, smf_table_priv["prot_sub"].visit(visitor_int));
    EXPECT_EQ(2.5, smf_table_priv["pub_div"].visit(visitor_dbl));
}

// ==========================================================================
// Non-Static Data Members — only PUBLIC API available (GCC bug workaround)
// Verify that PUBLIC only sees public members
// ==========================================================================

struct nsdm_test_t {
    double pub_x = 0;

private:
    double priv_y = 0;

protected:
    int prot_z = 0;

public:
    int pub_w = 0;

    constexpr nsdm_test_t() = default;
    constexpr nsdm_test_t(double px, double py, int pz, int pw)
        : pub_x(px), priv_y(py), prot_z(pz), pub_w(pw)
    {
    }
};

constexpr auto nsdm_table_public = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(nsdm_test_t, "*");

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersSizes)
{
    static_assert(nsdm_table_public.size() == 2);
}

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersCheckValues)
{
    constexpr auto obj = nsdm_test_t(1.5, 2.5, 3, 4);

    auto visitor = [&obj](auto mptr) -> double {
        if (mptr == nullptr) return -1.0;
        return obj.*mptr;
    };
    EXPECT_EQ(1.5, nsdm_table_public["pub_x"].visit(visitor));
    EXPECT_EQ(4.0, nsdm_table_public["pub_w"].visit(visitor));
    EXPECT_EQ(-1.0, nsdm_table_public["priv_y"].visit(visitor));
    EXPECT_EQ(-1.0, nsdm_table_public["prot_z"].visit(visitor));
}
