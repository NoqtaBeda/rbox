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

#include <format>
#include <rbox/type_traits/trivial_types.hpp>
#include <rbox/utils/format.hpp>
#include <rbox/utils/meta_pair.hpp>
#include <rbox/utils/meta_span.hpp>
#include <rbox/utils/meta_string_view.hpp>
#include <rbox/utils/meta_triple.hpp>
#include <rbox/utils/meta_tuple.hpp>
#include <rbox/utils/meta_variant.hpp>
#include <string>

#include "tests/test_options.hpp"

TEST(UtilsMetaAlternatives, IsTrivial)
{
    static_assert(rbox::trivial_type<rbox::meta_span<int>>);
    static_assert(rbox::trivial_type<rbox::meta_string_view>);
    static_assert(rbox::trivial_type<rbox::meta_pair<int, int*>>);
    static_assert(rbox::trivial_type<rbox::meta_triple<int, int*, int**>>);
    static_assert(rbox::trivial_type<rbox::meta_tuple<int, int*, int**, int***>>);
    static_assert(rbox::trivial_type<rbox::meta_variant<int, int*, int**, int***>>);
}

TEST(UtilsMetaAlternatives, FormatMetaStringViewChar)
{
    auto sv = rbox::meta_string_view("hello");
    auto result = std::format("{}", sv);
    EXPECT_EQ(result, "hello");
}

TEST(UtilsMetaAlternatives, FormatMetaStringViewCharFillAlign)
{
    auto sv = rbox::meta_string_view("hi");
    auto result = std::format("{:>6}", sv);
    EXPECT_EQ(result, "    hi");
}

TEST(UtilsMetaAlternatives, FormatMetaStringViewWchar)
{
    auto sv = rbox::meta_basic_string_view<wchar_t>(L"world");
    auto result = std::format(L"{}", sv);
    EXPECT_EQ(result, L"world");
}

TEST(UtilsMetaAlternatives, FormatMetaStringViewWcharFillAlign)
{
    auto sv = rbox::meta_basic_string_view<wchar_t>(L"x");
    auto result = std::format(L"{:*<4}", sv);
    EXPECT_EQ(result, L"x***");
}
