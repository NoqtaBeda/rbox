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
// Static Data Members — Protected & Private Inheritance
// ==========================================================================

struct sdm_grand_base_t {
    static inline int g_pub = 10;

private:
    static inline int g_priv = 20;

protected:
    static constexpr double g_prot = 1.5;

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_data_member_fixed_map(^^sdm_grand_base_t, "*");
    }
    static consteval auto build_unchecked_table()
    {
        constexpr auto options = rbox::string_key_fixed_map_options{};
        return rbox::make_static_data_member_fixed_map(
            ^^sdm_grand_base_t, "*", options, rbox::unchecked_context());
    }
};

struct sdm_prot_derived_t : protected sdm_grand_base_t {
    static inline int d_pub = 30;

private:
    static inline int d_priv = 40;

protected:
    static constexpr double d_prot = 2.5;

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_data_member_fixed_map(^^sdm_prot_derived_t, "*");
    }
    static consteval auto build_unchecked_table()
    {
        constexpr auto options = rbox::string_key_fixed_map_options{};
        return rbox::make_static_data_member_fixed_map(
            ^^sdm_prot_derived_t, "*", options, rbox::unchecked_context());
    }
};

struct sdm_priv_derived_t : private sdm_grand_base_t {
    static inline int d_pub = 50;

private:
    static inline int d_priv = 60;

protected:
    static constexpr double d_prot = 3.5;

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_data_member_fixed_map(^^sdm_priv_derived_t, "*");
    }
    static consteval auto build_unchecked_table()
    {
        constexpr auto options = rbox::string_key_fixed_map_options{};
        return rbox::make_static_data_member_fixed_map(
            ^^sdm_priv_derived_t, "*", options, rbox::unchecked_context());
    }
};

constexpr auto sdm_grand_public = RBOX_PUBLIC_STATIC_DATA_MEMBER_FIXED_MAP(sdm_grand_base_t, "*");
constexpr auto sdm_grand_priv = [:sdm_grand_base_t::build_priv_table():];
constexpr auto sdm_grand_unchecked = [:sdm_grand_base_t::build_unchecked_table():];

constexpr auto sdm_prot_public = RBOX_PUBLIC_STATIC_DATA_MEMBER_FIXED_MAP(sdm_prot_derived_t, "*");
constexpr auto sdm_prot_priv = [:sdm_prot_derived_t::build_priv_table():];
constexpr auto sdm_prot_unchecked = [:sdm_prot_derived_t::build_unchecked_table():];

constexpr auto sdm_priv_public = RBOX_PUBLIC_STATIC_DATA_MEMBER_FIXED_MAP(sdm_priv_derived_t, "*");
constexpr auto sdm_priv_priv = [:sdm_priv_derived_t::build_priv_table():];
constexpr auto sdm_priv_unchecked = [:sdm_priv_derived_t::build_unchecked_table():];

TEST(ClassLookupTableByAccessContext, StaticDataMembersGrandBase)
{
    // priv:   g_pub, g_priv, g_prot  (self: sees all)
    static_assert(sdm_grand_priv.size() == 3);
    // public: g_pub
    static_assert(sdm_grand_public.size() == 1);
    // unchecked: g_pub, g_priv, g_prot  (same as priv)
    static_assert(sdm_grand_unchecked.size() == 3);
}

TEST(ClassLookupTableByAccessContext, StaticDataMembersProtectedInheritance)
{
    // public: d_pub  (g_pub is now protected via protected inheritance)
    static_assert(sdm_prot_public.size() == 1);
    // priv:   g_pub(inherited→prot), g_prot, d_pub, d_priv, d_prot
    static_assert(sdm_prot_priv.size() == 5);
    // unchecked: g_pub, g_priv, g_prot, d_pub, d_priv, d_prot  (all 6)
    static_assert(sdm_prot_unchecked.size() == 6);
}

TEST(ClassLookupTableByAccessContext, StaticDataMembersPrivateInheritance)
{
    // public: d_pub  (inherited members are now private via private inheritance)
    static_assert(sdm_priv_public.size() == 1);
    // priv:   g_pub(inherited→priv), g_prot(inherited→priv), d_pub, d_priv, d_prot
    static_assert(sdm_priv_priv.size() == 5);
    // unchecked: g_pub, g_priv, g_prot, d_pub, d_priv, d_prot  (all 6)
    static_assert(sdm_priv_unchecked.size() == 6);
}

TEST(ClassLookupTableByAccessContext, StaticDataMembersProtectedInheritanceCheckValues)
{
    auto visitor = [](auto ptr) -> double {
        if (ptr == nullptr) return -1;
        return *ptr;
    };

    CHECK_VARIABLE(30, sdm_prot_public["d_pub"]);
    EXPECT_EQ(nullptr, sdm_prot_public["g_pub"]);

    EXPECT_EQ(10, sdm_prot_priv["g_pub"].visit(visitor));
    EXPECT_EQ(1.5, sdm_prot_priv["g_prot"].visit(visitor));
    EXPECT_EQ(30, sdm_prot_priv["d_pub"].visit(visitor));
    EXPECT_EQ(40, sdm_prot_priv["d_priv"].visit(visitor));
    EXPECT_EQ(2.5, sdm_prot_priv["d_prot"].visit(visitor));
    EXPECT_EQ(-1, sdm_prot_priv["g_priv"].visit(visitor));

    EXPECT_EQ(10, sdm_prot_unchecked["g_pub"].visit(visitor));
    EXPECT_EQ(20, sdm_prot_unchecked["g_priv"].visit(visitor));
    EXPECT_EQ(1.5, sdm_prot_unchecked["g_prot"].visit(visitor));
    EXPECT_EQ(30, sdm_prot_unchecked["d_pub"].visit(visitor));
    EXPECT_EQ(40, sdm_prot_unchecked["d_priv"].visit(visitor));
    EXPECT_EQ(2.5, sdm_prot_unchecked["d_prot"].visit(visitor));
}

TEST(ClassLookupTableByAccessContext, StaticDataMembersPrivateInheritanceCheckValues)
{
    auto visitor = [](auto ptr) -> double {
        if (ptr == nullptr) return -1;
        return *ptr;
    };

    CHECK_VARIABLE(50, sdm_priv_public["d_pub"]);
    EXPECT_EQ(nullptr, sdm_priv_public["g_pub"]);

    EXPECT_EQ(10, sdm_priv_priv["g_pub"].visit(visitor));
    EXPECT_EQ(1.5, sdm_priv_priv["g_prot"].visit(visitor));
    EXPECT_EQ(50, sdm_priv_priv["d_pub"].visit(visitor));
    EXPECT_EQ(60, sdm_priv_priv["d_priv"].visit(visitor));
    EXPECT_EQ(3.5, sdm_priv_priv["d_prot"].visit(visitor));
    EXPECT_EQ(-1, sdm_priv_priv["g_priv"].visit(visitor));

    EXPECT_EQ(10, sdm_priv_unchecked["g_pub"].visit(visitor));
    EXPECT_EQ(20, sdm_priv_unchecked["g_priv"].visit(visitor));
    EXPECT_EQ(1.5, sdm_priv_unchecked["g_prot"].visit(visitor));
    EXPECT_EQ(50, sdm_priv_unchecked["d_pub"].visit(visitor));
    EXPECT_EQ(60, sdm_priv_unchecked["d_priv"].visit(visitor));
    EXPECT_EQ(3.5, sdm_priv_unchecked["d_prot"].visit(visitor));
}

// ==========================================================================
// Static Member Functions — Protected Inheritance, with unchecked ctx
// ==========================================================================

struct smf_grand_base_t {
    static int g_pub_add(int x, int y)
    {
        return x + y;
    }

private:
    static int g_priv_mul(int x, int y)
    {
        return x * y;
    }

protected:
    static int g_prot_sub(int x, int y)
    {
        return x - y;
    }

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_member_function_fixed_map(
            ^^smf_grand_base_t, [](std::meta::info member) -> std::optional<std::string> {
              if (identifier_of(member) == "build_priv_table"
                  || identifier_of(member) == "build_unchecked_table")
                  return std::nullopt;
              return std::string{identifier_of(member)};
            });
    }

    static consteval auto build_unchecked_table()
    {
        return rbox::make_static_member_function_fixed_map(
            ^^smf_grand_base_t,
            [](std::meta::info member) -> std::optional<std::string> {
              if (identifier_of(member) == "build_priv_table"
                  || identifier_of(member) == "build_unchecked_table")
                  return std::nullopt;
              return std::string{identifier_of(member)};
            },
            {},
            rbox::unchecked_context());
    }
};

struct smf_prot_derived_t : protected smf_grand_base_t {
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
            ^^smf_prot_derived_t, [](std::meta::info member) -> std::optional<std::string> {
              if (identifier_of(member) == "build_priv_table"
                  || identifier_of(member) == "build_unchecked_table")
                  return std::nullopt;
              return std::string{identifier_of(member)};
            });
    }

    static consteval auto build_unchecked_table()
    {
        return rbox::make_static_member_function_fixed_map(
            ^^smf_prot_derived_t,
            [](std::meta::info member) -> std::optional<std::string> {
              if (identifier_of(member) == "build_priv_table"
                  || identifier_of(member) == "build_unchecked_table")
                  return std::nullopt;
              return std::string{identifier_of(member)};
            },
            {},
            rbox::unchecked_context());
    }
};

constexpr auto smf_filter = [](std::meta::info member) -> std::optional<std::string> {
    if (identifier_of(member) == "build_priv_table"
        || identifier_of(member) == "build_unchecked_table")
        return std::nullopt;
    return std::string{identifier_of(member)};
};

constexpr auto smf_grand_public =
    RBOX_PUBLIC_STATIC_MEMBER_FUNCTION_FIXED_MAP(smf_grand_base_t, smf_filter);
constexpr auto smf_grand_priv = [:smf_grand_base_t::build_priv_table():];
constexpr auto smf_grand_unchecked = [:smf_grand_base_t::build_unchecked_table():];

constexpr auto smf_prot_public =
    RBOX_PUBLIC_STATIC_MEMBER_FUNCTION_FIXED_MAP(smf_prot_derived_t, smf_filter);
constexpr auto smf_prot_priv = [:smf_prot_derived_t::build_priv_table():];
constexpr auto smf_prot_unchecked = [:smf_prot_derived_t::build_unchecked_table():];

TEST(ClassLookupTableByAccessContext, StaticMemberFunctionsProtectedInheritance)
{
    // priv:     g_pub_add, g_priv_mul, g_prot_sub  (self: sees all 3)
    static_assert(smf_grand_priv.size() == 3);
    // public:   g_pub_add
    static_assert(smf_grand_public.size() == 1);

    // public:   d_pub_pow  (g_pub_add now protected via protected inheritance)
    static_assert(smf_prot_public.size() == 1);
    // priv:     g_pub_add(inherited→prot), g_prot_sub, d_pub_pow, d_priv_max, d_prot_min
    static_assert(smf_prot_priv.size() == 5);
    // unchecked: g_pub_add, g_priv_mul, g_prot_sub, d_pub_pow, d_priv_max, d_prot_min (all 6)
    static_assert(smf_prot_unchecked.size() == 6);
}

TEST(ClassLookupTableByAccessContext, StaticMemberFunctionsProtectedInheritanceCheckValues)
{
    CHECK_FUNCTION(8, smf_prot_public["d_pub_pow"], 2, 3);
    EXPECT_EQ(nullptr, smf_prot_public["g_pub_add"]);

    CHECK_FUNCTION(5, smf_prot_priv["g_pub_add"], 2, 3);
    CHECK_FUNCTION(-1, smf_prot_priv["g_prot_sub"], 2, 3);
    CHECK_FUNCTION(8, smf_prot_priv["d_pub_pow"], 2, 3);
    CHECK_FUNCTION(3, smf_prot_priv["d_priv_max"], 2, 3);
    CHECK_FUNCTION(2, smf_prot_priv["d_prot_min"], 2, 3);
    EXPECT_EQ(nullptr, smf_prot_priv["g_priv_mul"]);

    CHECK_FUNCTION(5, smf_prot_unchecked["g_pub_add"], 2, 3);
    CHECK_FUNCTION(6, smf_prot_unchecked["g_priv_mul"], 2, 3);
    CHECK_FUNCTION(-1, smf_prot_unchecked["g_prot_sub"], 2, 3);
    CHECK_FUNCTION(8, smf_prot_unchecked["d_pub_pow"], 2, 3);
    CHECK_FUNCTION(3, smf_prot_unchecked["d_priv_max"], 2, 3);
    CHECK_FUNCTION(2, smf_prot_unchecked["d_prot_min"], 2, 3);
}

// ==========================================================================
// Explicit ctx args — unprivileged_context() vs unchecked_context()
// (privileged tables built inside classes, extracted via [::...:])
// ==========================================================================

// --- Non-static member functions with explicit ctx ---

struct nsmf_explicit_ctx_t {
    int value = 0;

public:
    constexpr int public_get() const
    {
        return value;
    }

private:
    constexpr int private_get() const
    {
        return value * 10;
    }

protected:
    constexpr int protected_get() const
    {
        return value * 100;
    }

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_nonstatic_member_function_fixed_map(^^nsmf_explicit_ctx_t, "*");
    }
    static consteval auto build_unchecked_table()
    {
        constexpr auto options = rbox::string_key_fixed_map_options{};
        return rbox::make_nonstatic_member_function_fixed_map(
            ^^nsmf_explicit_ctx_t, "*", options, rbox::unchecked_context());
    }
};

constexpr auto nsmf_public =
    RBOX_PUBLIC_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(nsmf_explicit_ctx_t, "*");
constexpr auto nsmf_priv = [:nsmf_explicit_ctx_t::build_priv_table():];
constexpr auto nsmf_unchecked = [:nsmf_explicit_ctx_t::build_unchecked_table():];

TEST(ClassLookupTableByAccessContext, NonStaticMemberFunctionsExplicitCtxSizes)
{
    // public: public_get
    static_assert(nsmf_public.size() == 1);
    // priv:   public_get, private_get, protected_get
    static_assert(nsmf_priv.size() == 3);
    // unchecked: public_get, private_get, protected_get  (same as priv)
    static_assert(nsmf_unchecked.size() == 3);
}

TEST(ClassLookupTableByAccessContext, NonStaticMemberFunctionsExplicitCtxCheckValues)
{
    constexpr auto obj = nsmf_explicit_ctx_t{.value = 7};

    CHECK_MEMBER_FUNCTION(7, obj, nsmf_public["public_get"]);
    EXPECT_EQ(nullptr, nsmf_public["private_get"]);
    EXPECT_EQ(nullptr, nsmf_public["protected_get"]);

    CHECK_MEMBER_FUNCTION(7, obj, nsmf_priv["public_get"]);
    CHECK_MEMBER_FUNCTION(70, obj, nsmf_priv["private_get"]);
    CHECK_MEMBER_FUNCTION(700, obj, nsmf_priv["protected_get"]);

    CHECK_MEMBER_FUNCTION(7, obj, nsmf_unchecked["public_get"]);
    CHECK_MEMBER_FUNCTION(70, obj, nsmf_unchecked["private_get"]);
    CHECK_MEMBER_FUNCTION(700, obj, nsmf_unchecked["protected_get"]);
}

// --- Static data members with explicit ctx ---

struct sdm_explicit_ctx_t {
    static inline int public_val = 100;

private:
    static inline int private_val = 200;

protected:
    static constexpr double protected_val = 3.14;

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_data_member_fixed_map(^^sdm_explicit_ctx_t, "*");
    }
    static consteval auto build_unchecked_table()
    {
        constexpr auto options = rbox::string_key_fixed_map_options{};
        return rbox::make_static_data_member_fixed_map(
            ^^sdm_explicit_ctx_t, "*", options, rbox::unchecked_context());
    }
};

constexpr auto sdm_public = RBOX_PUBLIC_STATIC_DATA_MEMBER_FIXED_MAP(sdm_explicit_ctx_t, "*");
constexpr auto sdm_priv = [:sdm_explicit_ctx_t::build_priv_table():];
constexpr auto sdm_unchecked = [:sdm_explicit_ctx_t::build_unchecked_table():];

TEST(ClassLookupTableByAccessContext, StaticDataMembersExplicitCtxSizes)
{
    // public: public_val
    static_assert(sdm_public.size() == 1);
    // priv:   public_val, private_val, protected_val
    static_assert(sdm_priv.size() == 3);
    // unchecked: public_val, private_val, protected_val  (same as priv)
    static_assert(sdm_unchecked.size() == 3);
}

TEST(ClassLookupTableByAccessContext, StaticDataMembersExplicitCtxCheckValues)
{
    auto visitor = [](auto ptr) -> double {
        if (ptr == nullptr) return -1;
        return *ptr;
    };

    CHECK_VARIABLE(100, sdm_public["public_val"]);
    EXPECT_EQ(nullptr, sdm_public["private_val"]);

    EXPECT_EQ(100, sdm_priv["public_val"].visit(visitor));
    EXPECT_EQ(200, sdm_priv["private_val"].visit(visitor));
    EXPECT_EQ(3.14, sdm_priv["protected_val"].visit(visitor));

    EXPECT_EQ(100, sdm_unchecked["public_val"].visit(visitor));
    EXPECT_EQ(200, sdm_unchecked["private_val"].visit(visitor));
    EXPECT_EQ(3.14, sdm_unchecked["protected_val"].visit(visitor));
}

// --- Static member functions with explicit ctx ---

struct smf_explicit_ctx_t {
    static int pub_fn(int x, int y)
    {
        return x + y;
    }

private:
    static int priv_fn(int x, int y)
    {
        return x * y;
    }

protected:
    static int prot_fn(int x, int y)
    {
        return x - y;
    }

public:
    static consteval auto build_priv_table()
    {
        return rbox::make_static_member_function_fixed_map(
            ^^smf_explicit_ctx_t, [](std::meta::info member) -> std::optional<std::string> {
              if (identifier_of(member) == "build_priv_table"
                  || identifier_of(member) == "build_unchecked_table")
                  return std::nullopt;
              return std::string{identifier_of(member)};
            });
    }

    static consteval auto build_unchecked_table()
    {
        return rbox::make_static_member_function_fixed_map(
            ^^smf_explicit_ctx_t,
            [](std::meta::info member) -> std::optional<std::string> {
              if (identifier_of(member) == "build_priv_table"
                  || identifier_of(member) == "build_unchecked_table")
                  return std::nullopt;
              return std::string{identifier_of(member)};
            },
            {},
            rbox::unchecked_context());
    }
};

constexpr auto smf_public = RBOX_PUBLIC_STATIC_MEMBER_FUNCTION_FIXED_MAP(
    smf_explicit_ctx_t, [](std::meta::info member) -> std::optional<std::string> {
        if (identifier_of(member) == "build_priv_table"
            || identifier_of(member) == "build_unchecked_table")
            return std::nullopt;
        return std::string{identifier_of(member)};
    });
constexpr auto smf_priv = [:smf_explicit_ctx_t::build_priv_table():];
constexpr auto smf_unchecked = [:smf_explicit_ctx_t::build_unchecked_table():];

TEST(ClassLookupTableByAccessContext, StaticMemberFunctionsExplicitCtxSizes)
{
    // public: pub_fn
    static_assert(smf_public.size() == 1);
    // priv:   pub_fn, priv_fn, prot_fn
    static_assert(smf_priv.size() == 3);
    // unchecked: pub_fn, priv_fn, prot_fn  (same as priv)
    static_assert(smf_unchecked.size() == 3);
}

TEST(ClassLookupTableByAccessContext, StaticMemberFunctionsExplicitCtxCheckValues)
{
    CHECK_FUNCTION(5, smf_public["pub_fn"], 2, 3);
    EXPECT_EQ(nullptr, smf_public["priv_fn"]);
    EXPECT_EQ(nullptr, smf_public["prot_fn"]);

    CHECK_FUNCTION(5, smf_priv["pub_fn"], 2, 3);
    CHECK_FUNCTION(6, smf_priv["priv_fn"], 2, 3);
    CHECK_FUNCTION(-1, smf_priv["prot_fn"], 2, 3);

    CHECK_FUNCTION(5, smf_unchecked["pub_fn"], 2, 3);
    CHECK_FUNCTION(6, smf_unchecked["priv_fn"], 2, 3);
    CHECK_FUNCTION(-1, smf_unchecked["prot_fn"], 2, 3);
}

// ==========================================================================
// Non-Static Data Members — Protected Inheritance (public only)
// ==========================================================================

struct nsdm_grand_base_t {
    double g_pub = 1.0;

private:
    double g_priv = 3.0;

protected:
    int g_prot = 7;

public:
    constexpr nsdm_grand_base_t() = default;
    constexpr nsdm_grand_base_t(double pub, double priv, int prot)
        : g_pub(pub), g_priv(priv), g_prot(prot)
    {
    }
};

struct nsdm_prot_derived_t : protected nsdm_grand_base_t {
    double d_pub = 2.0;

private:
    double d_priv = 4.0;

protected:
    int d_prot = 8;

public:
    constexpr nsdm_prot_derived_t() = default;
    constexpr nsdm_prot_derived_t(
        double gpub, double gpriv, int gprot, double dpub, double dpriv, int dprot)
        : nsdm_grand_base_t(gpub, gpriv, gprot), d_pub(dpub), d_priv(dpriv), d_prot(dprot)
    {
    }
};

constexpr auto nsdm_grand_public =
    RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(nsdm_grand_base_t, "*");
constexpr auto nsdm_prot_public =
    RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(nsdm_prot_derived_t, "*");

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersProtectedInheritanceSizes)
{
    // base public:   g_pub
    static_assert(nsdm_grand_public.size() == 1);
    // prot-derived public: d_pub  (g_pub now protected via protected inheritance)
    static_assert(nsdm_prot_public.size() == 1);
}

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersProtectedInheritanceCheckValues)
{
    constexpr auto obj = nsdm_prot_derived_t(10, 30, 70, 20, 40, 80);
    CHECK_MEMBER_VARIABLE(20, obj, nsdm_prot_public["d_pub"]);
    EXPECT_EQ(nullptr, nsdm_prot_public["g_pub"]);
    EXPECT_EQ(nullptr, nsdm_prot_public["g_priv"]);
    EXPECT_EQ(nullptr, nsdm_prot_public["g_prot"]);
}
