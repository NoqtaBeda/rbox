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
// Non-Static Member Functions — Inheritance
// ==========================================================================

struct base_nsmf_t {
    int b_val = 0;

public:
    constexpr int b_pub_get() const
    {
        return b_val;
    }

private:
    constexpr int b_priv_get() const
    {
        return b_val * 2;
    }

protected:
    constexpr int b_prot_get() const
    {
        return b_val * 3;
    }

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_nonstatic_member_function_fixed_map(^^base_nsmf_t, "*");
    }
};

struct derived_nsmf_t : base_nsmf_t {
    int d_val = 0;

    constexpr derived_nsmf_t() = default;
    constexpr derived_nsmf_t(int bv, int dv) : base_nsmf_t{bv}, d_val(dv) {}

public:
    constexpr int d_pub_get() const
    {
        return b_val + d_val;
    }

private:
    constexpr int d_priv_get() const
    {
        return d_val * 10;
    }

protected:
    constexpr int d_prot_get() const
    {
        return d_val * 100;
    }

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_nonstatic_member_function_fixed_map(^^derived_nsmf_t, "*");
    }
};

constexpr auto base_nsmf_public = RBOX_PUBLIC_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(base_nsmf_t, "*");
constexpr auto base_nsmf_priv = [:base_nsmf_t::build_priv_table():];

constexpr auto derived_nsmf_public =
    RBOX_PUBLIC_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(derived_nsmf_t, "*");
constexpr auto derived_nsmf_priv = [:derived_nsmf_t::build_priv_table():];

TEST(ClassLookupTableByAccessContext, NonStaticMemberFunctionsBase)
{
    static_assert(base_nsmf_priv.size() == 3);
    static_assert(base_nsmf_public.size() == 1);
}

TEST(ClassLookupTableByAccessContext, NonStaticMemberFunctionsBaseCheckValues)
{
    constexpr auto obj = base_nsmf_t{.b_val = 10};
    CHECK_MEMBER_FUNCTION_STATIC(10, obj, base_nsmf_public["b_pub_get"]);
    EXPECT_EQ(nullptr, base_nsmf_public["b_priv_get"]);
    EXPECT_EQ(nullptr, base_nsmf_public["b_prot_get"]);

    CHECK_MEMBER_FUNCTION_STATIC(10, obj, base_nsmf_priv["b_pub_get"]);
    CHECK_MEMBER_FUNCTION_STATIC(20, obj, base_nsmf_priv["b_priv_get"]);
    CHECK_MEMBER_FUNCTION_STATIC(30, obj, base_nsmf_priv["b_prot_get"]);
}

TEST(ClassLookupTableByAccessContext, NonStaticMemberFunctionsDerived)
{
    static_assert(derived_nsmf_public.size() == 2);

    // Privileged: b_pub_get + b_prot_get + d_pub_get + d_priv_get + d_prot_get = 5
    static_assert(derived_nsmf_priv.size() == 5);
}

TEST(ClassLookupTableByAccessContext, NonStaticMemberFunctionsDerivedCheckValues)
{
    constexpr auto obj = derived_nsmf_t(10, 5);

    CHECK_MEMBER_FUNCTION_STATIC(10, obj, derived_nsmf_public["b_pub_get"]);
    CHECK_MEMBER_FUNCTION_STATIC(15, obj, derived_nsmf_public["d_pub_get"]);
    EXPECT_EQ(nullptr, derived_nsmf_public["b_prot_get"]);
    EXPECT_EQ(nullptr, derived_nsmf_public["d_priv_get"]);

    CHECK_MEMBER_FUNCTION_STATIC(10, obj, derived_nsmf_priv["b_pub_get"]);
    CHECK_MEMBER_FUNCTION_STATIC(30, obj, derived_nsmf_priv["b_prot_get"]);
    CHECK_MEMBER_FUNCTION_STATIC(15, obj, derived_nsmf_priv["d_pub_get"]);
    CHECK_MEMBER_FUNCTION_STATIC(50, obj, derived_nsmf_priv["d_priv_get"]);
    CHECK_MEMBER_FUNCTION_STATIC(500, obj, derived_nsmf_priv["d_prot_get"]);
    EXPECT_EQ(nullptr, derived_nsmf_priv["b_priv_get"]);
}

// ==========================================================================
// Static Data Members — Inheritance
// ==========================================================================

struct base_sdm_t {
    static inline int b_pub = 100;

private:
    static inline int b_priv = 200;

protected:
    static constexpr double b_prot = 3.14;

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_data_member_fixed_map(^^base_sdm_t, "*");
    }
};

struct derived_sdm_t : base_sdm_t {
    static inline int d_pub = 300;

private:
    static inline int d_priv = 400;

protected:
    static constexpr double d_prot = 6.28;

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_data_member_fixed_map(^^derived_sdm_t, "*");
    }
};

constexpr auto base_sdm_public = RBOX_PUBLIC_STATIC_DATA_MEMBER_FIXED_MAP(base_sdm_t, "*");
constexpr auto base_sdm_priv = [:base_sdm_t::build_priv_table():];
constexpr auto derived_sdm_public = RBOX_PUBLIC_STATIC_DATA_MEMBER_FIXED_MAP(derived_sdm_t, "*");
constexpr auto derived_sdm_priv = [:derived_sdm_t::build_priv_table():];

TEST(ClassLookupTableByAccessContext, StaticDataMembersBase)
{
    static_assert(base_sdm_priv.size() == 3);
    static_assert(base_sdm_public.size() == 1);
}

TEST(ClassLookupTableByAccessContext, StaticDataMembersBaseCheckValues)
{
    auto visitor = [](auto ptr) -> double {
        if (ptr == nullptr) return -1;
        return *ptr;
    };
    CHECK_VARIABLE(100, base_sdm_public["b_pub"]);
    EXPECT_EQ(nullptr, base_sdm_public["b_priv"]);
    EXPECT_EQ(nullptr, base_sdm_public["b_prot"]);

    EXPECT_EQ(100, base_sdm_priv["b_pub"].visit(visitor));
    EXPECT_EQ(200, base_sdm_priv["b_priv"].visit(visitor));
    EXPECT_EQ(3.14, base_sdm_priv["b_prot"].visit(visitor));
}

TEST(ClassLookupTableByAccessContext, StaticDataMembersDerived)
{
    // Privileged: b_pub + b_prot + d_pub + d_priv + d_prot = 5 (b_priv not visible from derived)
    static_assert(derived_sdm_priv.size() == 5);
    // Public: b_pub + d_pub = 2
    static_assert(derived_sdm_public.size() == 2);
}

TEST(ClassLookupTableByAccessContext, StaticDataMembersDerivedCheckValues)
{
    auto visitor = [](auto ptr) -> double {
        if (ptr == nullptr) return -1;
        return *ptr;
    };
    CHECK_VARIABLE(100, derived_sdm_public["b_pub"]);
    CHECK_VARIABLE(300, derived_sdm_public["d_pub"]);
    EXPECT_EQ(nullptr, derived_sdm_public["b_prot"]);
    EXPECT_EQ(nullptr, derived_sdm_public["d_priv"]);

    EXPECT_EQ(100, derived_sdm_priv["b_pub"].visit(visitor));
    EXPECT_EQ(3.14, derived_sdm_priv["b_prot"].visit(visitor));
    EXPECT_EQ(300, derived_sdm_priv["d_pub"].visit(visitor));
    EXPECT_EQ(400, derived_sdm_priv["d_priv"].visit(visitor));
    EXPECT_EQ(6.28, derived_sdm_priv["d_prot"].visit(visitor));
    EXPECT_EQ(-1, derived_sdm_priv["b_priv"].visit(visitor));
}

// ==========================================================================
// Static Member Functions — Inheritance
// ==========================================================================

struct base_smf_t {
    static int b_pub_add(int x, int y)
    {
        return x + y;
    }

private:
    static int b_priv_mul(int x, int y)
    {
        return x * y;
    }

protected:
    static int b_prot_sub(int x, int y)
    {
        return x - y;
    }

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_member_function_fixed_map(
            ^^base_smf_t, [](std::meta::info member) -> std::optional<std::string> {
              if (identifier_of(member) == "build_priv_table") return std::nullopt;
              return std::string{identifier_of(member)};
            });
    }
};

struct derived_smf_t : base_smf_t {
    static int d_pub_pow(int x, int y)
    {
        int r = 1;
        while (y--) r *= x;
        return r;
    }

private:
    static int d_priv_max(int x, int y)
    {
        return x > y ? x : y;
    }

protected:
    static int d_prot_min(int x, int y)
    {
        return x < y ? x : y;
    }

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_member_function_fixed_map(
            ^^derived_smf_t, [](std::meta::info member) -> std::optional<std::string> {
              if (identifier_of(member) == "build_priv_table") return std::nullopt;
              return std::string{identifier_of(member)};
            });
    }
};

constexpr auto base_smf_public = RBOX_PUBLIC_STATIC_MEMBER_FUNCTION_FIXED_MAP(
    base_smf_t, [](std::meta::info member) -> std::optional<std::string> {
        if (identifier_of(member) == "build_priv_table") return std::nullopt;
        return std::string{identifier_of(member)};
    });
constexpr auto base_smf_priv = [:base_smf_t::build_priv_table():];

constexpr auto derived_smf_public = RBOX_PUBLIC_STATIC_MEMBER_FUNCTION_FIXED_MAP(
    derived_smf_t, [](std::meta::info member) -> std::optional<std::string> {
        if (identifier_of(member) == "build_priv_table") return std::nullopt;
        return std::string{identifier_of(member)};
    });
constexpr auto derived_smf_priv = [:derived_smf_t::build_priv_table():];

TEST(ClassLookupTableByAccessContext, StaticMemberFunctionsBase)
{
    static_assert(base_smf_priv.size() == 3);
    static_assert(base_smf_public.size() == 1);
}

TEST(ClassLookupTableByAccessContext, StaticMemberFunctionsBaseCheckValues)
{
    CHECK_FUNCTION(9, base_smf_public["b_pub_add"], 6, 3);
    EXPECT_EQ(nullptr, base_smf_public["b_priv_mul"]);
    EXPECT_EQ(nullptr, base_smf_public["b_prot_sub"]);

    CHECK_FUNCTION(9, base_smf_priv["b_pub_add"], 6, 3);
    CHECK_FUNCTION(18, base_smf_priv["b_priv_mul"], 6, 3);
    CHECK_FUNCTION(3, base_smf_priv["b_prot_sub"], 6, 3);
}

TEST(ClassLookupTableByAccessContext, StaticMemberFunctionsDerived)
{
    // Privileged: b_pub_add + b_prot_sub + d_pub_pow + d_priv_max + d_prot_min = 5
    static_assert(derived_smf_priv.size() == 5);
    // Public: b_pub_add + d_pub_pow = 2
    static_assert(derived_smf_public.size() == 2);
}

TEST(ClassLookupTableByAccessContext, StaticMemberFunctionsDerivedCheckValues)
{
    CHECK_FUNCTION(5, derived_smf_public["b_pub_add"], 2, 3);
    CHECK_FUNCTION(8, derived_smf_public["d_pub_pow"], 2, 3);
    EXPECT_EQ(nullptr, derived_smf_public["b_prot_sub"]);
    EXPECT_EQ(nullptr, derived_smf_public["d_priv_max"]);

    CHECK_FUNCTION(5, derived_smf_priv["b_pub_add"], 2, 3);
    CHECK_FUNCTION(-1, derived_smf_priv["b_prot_sub"], 2, 3);
    CHECK_FUNCTION(8, derived_smf_priv["d_pub_pow"], 2, 3);
    CHECK_FUNCTION(3, derived_smf_priv["d_priv_max"], 2, 3);
    CHECK_FUNCTION(2, derived_smf_priv["d_prot_min"], 2, 3);
    EXPECT_EQ(nullptr, derived_smf_priv["b_priv_mul"]);
}

// ==========================================================================
// Non-Static Data Members — Inheritance (public API only)
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
};

constexpr auto base_nsdm_public = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(base_nsdm_t, "*");
constexpr auto derived_nsdm_public =
    RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(derived_nsdm_t, "*");

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersBase)
{
    static_assert(base_nsdm_public.size() == 1);
}

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersDerived)
{
    static_assert(derived_nsdm_public.size() == 2);
}

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersDerivedCheckValues)
{
    constexpr auto obj = derived_nsdm_t(10, 20, 30, 40, 50, 60);
    CHECK_MEMBER_VARIABLE(10, obj, derived_nsdm_public["b_pub"]);
    CHECK_MEMBER_VARIABLE(40, obj, derived_nsdm_public["d_pub"]);
    EXPECT_EQ(nullptr, derived_nsdm_public["b_priv"]);
    EXPECT_EQ(nullptr, derived_nsdm_public["b_prot"]);
    EXPECT_EQ(nullptr, derived_nsdm_public["d_priv"]);
    EXPECT_EQ(nullptr, derived_nsdm_public["d_prot"]);
}
