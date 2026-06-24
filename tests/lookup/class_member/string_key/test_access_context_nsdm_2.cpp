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
// Non-Static Data Members — Explicit ctx args
// ==========================================================================

struct nsdm_explicit_ctx_t {
    double public_val = 100;

private:
    double private_val = 200;

protected:
    int protected_val = 300;

public:
    constexpr nsdm_explicit_ctx_t() = default;
    constexpr nsdm_explicit_ctx_t(double pub, double priv, int prot)
        : public_val(pub), private_val(priv), protected_val(prot)
    {
    }

    static consteval auto build_priv_table()
    {
        return rbox::make_nonstatic_data_member_fixed_map(^^nsdm_explicit_ctx_t, "*");
    }

    static consteval auto build_unchecked_table()
    {
        constexpr auto options = rbox::string_key_fixed_map_options{};
        return rbox::make_nonstatic_data_member_fixed_map(
            ^^nsdm_explicit_ctx_t, "*", options, rbox::unchecked_context());
    }
};

constexpr auto nsdm_public = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(nsdm_explicit_ctx_t, "*");
constexpr auto nsdm_priv = [:nsdm_explicit_ctx_t::build_priv_table():];
constexpr auto nsdm_unchecked = [:nsdm_explicit_ctx_t::build_unchecked_table():];

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersExplicitCtxSizes)
{
    // public: public_val
    static_assert(nsdm_public.size() == 1);
    // priv:   public_val, private_val, protected_val
    static_assert(nsdm_priv.size() == 3);
    // unchecked: public_val, private_val, protected_val  (same as priv)
    static_assert(nsdm_unchecked.size() == 3);
}

TEST(ClassLookupTableByAccessContext, NonStaticDataMembersExplicitCtxCheckValues)
{
    constexpr auto obj = nsdm_explicit_ctx_t(10, 20, 30);
    auto visitor = [&obj](auto mptr) -> double {
        if (mptr == nullptr) return -1;
        return obj.*mptr;
    };

    // public
    CHECK_MEMBER_VARIABLE(10, obj, nsdm_public["public_val"]);
    EXPECT_EQ(nullptr, nsdm_public["private_val"]);
    EXPECT_EQ(nullptr, nsdm_public["protected_val"]);

    // priv
    EXPECT_EQ(10, nsdm_priv["public_val"].visit(visitor));
    EXPECT_EQ(20, nsdm_priv["private_val"].visit(visitor));
    EXPECT_EQ(30, nsdm_priv["protected_val"].visit(visitor));

    // unchecked
    EXPECT_EQ(10, nsdm_unchecked["public_val"].visit(visitor));
    EXPECT_EQ(20, nsdm_unchecked["private_val"].visit(visitor));
    EXPECT_EQ(30, nsdm_unchecked["protected_val"].visit(visitor));
}
