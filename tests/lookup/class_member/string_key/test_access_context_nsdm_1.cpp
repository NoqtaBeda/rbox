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
// Non-Static Data Members — Inheritance (priv + unchecked)
// ==========================================================================

struct base_nsdm_t {
    double b_pub = 1.0;

private:
    double b_priv = 2.0;

protected:
    int b_prot = 3;

public:
    constexpr base_nsdm_t() = default;
    constexpr base_nsdm_t(double pub, double priv, int prot)
        : b_pub(pub), b_priv(priv), b_prot(prot)
    {
    }

    static consteval auto build_priv_table()
    {
        return rbox::make_nonstatic_data_member_fixed_map(^^base_nsdm_t, "*");
    }

    static consteval auto build_unchecked_table()
    {
        constexpr auto options = rbox::string_key_fixed_map_options{};
        return rbox::make_nonstatic_data_member_fixed_map(
            ^^base_nsdm_t, "*", options, rbox::unchecked_context());
    }
};

struct derived_nsdm_t : base_nsdm_t {
    double d_pub = 4.0;

private:
    double d_priv = 5.0;

protected:
    int d_prot = 6;

public:
    constexpr derived_nsdm_t() = default;
    constexpr derived_nsdm_t(
        double bpub, double bpriv, int bprot, double dpub, double dpriv, int dprot)
        : base_nsdm_t(bpub, bpriv, bprot), d_pub(dpub), d_priv(dpriv), d_prot(dprot)
    {
    }

    static consteval auto build_priv_table()
    {
        return rbox::make_nonstatic_data_member_fixed_map(^^derived_nsdm_t, "*");
    }

    static consteval auto build_unchecked_table()
    {
        constexpr auto options = rbox::string_key_fixed_map_options{};
        return rbox::make_nonstatic_data_member_fixed_map(
            ^^derived_nsdm_t, "*", options, rbox::unchecked_context());
    }
};

constexpr auto base_nsdm_public = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(base_nsdm_t, "*");
constexpr auto base_nsdm_priv = [:base_nsdm_t::build_priv_table():];
constexpr auto base_nsdm_unchecked = [:base_nsdm_t::build_unchecked_table():];

constexpr auto derived_nsdm_public =
    RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(derived_nsdm_t, "*");
constexpr auto derived_nsdm_priv = [:derived_nsdm_t::build_priv_table():];
constexpr auto derived_nsdm_unchecked = [:derived_nsdm_t::build_unchecked_table():];

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersBasePriv)
{
    // public: b_pub
    static_assert(base_nsdm_public.size() == 1);
    // priv:   b_pub, b_priv, b_prot
    static_assert(base_nsdm_priv.size() == 3);
    // unchecked: b_pub, b_priv, b_prot  (same as priv)
    static_assert(base_nsdm_unchecked.size() == 3);
}

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersBaseCheckValuesPriv)
{
    constexpr auto obj = base_nsdm_t(10, 20, 30);
    auto visitor = [&obj](auto mptr) -> double {
        if (mptr == nullptr) return -1;
        return obj.*mptr;
    };

    // public
    CHECK_MEMBER_VARIABLE(10, obj, base_nsdm_public["b_pub"]);
    EXPECT_EQ(nullptr, base_nsdm_public["b_priv"]);
    EXPECT_EQ(nullptr, base_nsdm_public["b_prot"]);

    // priv
    EXPECT_EQ(10, base_nsdm_priv["b_pub"].visit(visitor));
    EXPECT_EQ(20, base_nsdm_priv["b_priv"].visit(visitor));
    EXPECT_EQ(30, base_nsdm_priv["b_prot"].visit(visitor));

    // unchecked
    EXPECT_EQ(10, base_nsdm_unchecked["b_pub"].visit(visitor));
    EXPECT_EQ(20, base_nsdm_unchecked["b_priv"].visit(visitor));
    EXPECT_EQ(30, base_nsdm_unchecked["b_prot"].visit(visitor));
}

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersDerivedPriv)
{
    // Privileged: b_pub + b_prot + d_pub + d_priv + d_prot = 5 (b_priv not visible from derived)
    static_assert(derived_nsdm_priv.size() == 5);
    // Public: b_pub + d_pub = 2
    static_assert(derived_nsdm_public.size() == 2);
    // unchecked: b_pub, b_priv, b_prot, d_pub, d_priv, d_prot  (all 6)
    static_assert(derived_nsdm_unchecked.size() == 6);
}

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersDerivedCheckValuesPriv)
{
    constexpr auto obj = derived_nsdm_t(10, 20, 30, 40, 50, 60);
    auto visitor = [&obj](auto mptr) -> double {
        if (mptr == nullptr) return -1;
        return obj.*mptr;
    };

    // public
    CHECK_MEMBER_VARIABLE(10, obj, derived_nsdm_public["b_pub"]);
    CHECK_MEMBER_VARIABLE(40, obj, derived_nsdm_public["d_pub"]);
    EXPECT_EQ(nullptr, derived_nsdm_public["b_priv"]);
    EXPECT_EQ(nullptr, derived_nsdm_public["b_prot"]);
    EXPECT_EQ(nullptr, derived_nsdm_public["d_priv"]);
    EXPECT_EQ(nullptr, derived_nsdm_public["d_prot"]);

    // priv
    EXPECT_EQ(10, derived_nsdm_priv["b_pub"].visit(visitor));
    EXPECT_EQ(30, derived_nsdm_priv["b_prot"].visit(visitor));
    EXPECT_EQ(40, derived_nsdm_priv["d_pub"].visit(visitor));
    EXPECT_EQ(50, derived_nsdm_priv["d_priv"].visit(visitor));
    EXPECT_EQ(60, derived_nsdm_priv["d_prot"].visit(visitor));
    EXPECT_EQ(-1, derived_nsdm_priv["b_priv"].visit(visitor));

    // unchecked
    EXPECT_EQ(10, derived_nsdm_unchecked["b_pub"].visit(visitor));
    EXPECT_EQ(20, derived_nsdm_unchecked["b_priv"].visit(visitor));
    EXPECT_EQ(30, derived_nsdm_unchecked["b_prot"].visit(visitor));
    EXPECT_EQ(40, derived_nsdm_unchecked["d_pub"].visit(visitor));
    EXPECT_EQ(50, derived_nsdm_unchecked["d_priv"].visit(visitor));
    EXPECT_EQ(60, derived_nsdm_unchecked["d_prot"].visit(visitor));
}
