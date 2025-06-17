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

#include <limits>
#include <rbox/utils/meta_pair.hpp>

#include "tests/test_options.hpp"

using Pair = rbox::meta_pair<int, double>;

TEST(UtilsMetaPair, Members)
{
    constexpr auto p = Pair{1, 3.14};
    static_assert(p.first == 1);
    static_assert(p.second == 3.14);
}

TEST(UtilsMetaPair, GetLvalueRef)
{
    auto p = Pair{10, 20.0};
    auto& r0 = get<0>(p);
    auto& r1 = get<1>(p);
    EXPECT_EQ(10, r0);
    EXPECT_EQ(20.0, r1);

    r0 = 42;
    r1 = 99.9;
    EXPECT_EQ(42, p.first);
    EXPECT_EQ(99.9, p.second);
}

TEST(UtilsMetaPair, GetConstLvalueRef)
{
    const auto p = Pair{5, 6.25};
    const auto& r0 = get<0>(p);
    const auto& r1 = get<1>(p);
    EXPECT_EQ(5, r0);
    EXPECT_EQ(6.25, r1);
}

TEST(UtilsMetaPair, GetRvalueRef)
{
    auto first = get<0>(Pair{7, 8.5});
    auto second = get<1>(Pair{7, 8.5});
    EXPECT_EQ(7, first);
    EXPECT_EQ(8.5, second);
}

TEST(UtilsMetaPair, StructuredBinding)
{
    auto p = Pair{100, 200.5};
    auto& [x, y] = p;
    EXPECT_EQ(100, x);
    EXPECT_DOUBLE_EQ(200.5, y);

    x = -1;
    y = -2.0;
    EXPECT_EQ(-1, p.first);
    EXPECT_DOUBLE_EQ(-2.0, p.second);
}

TEST(UtilsMetaPair, StructuredBindingConst)
{
    const auto p = Pair{33, 66.0};
    const auto& [x, y] = p;
    static_assert(std::is_same_v<const int, std::remove_reference_t<decltype(x)>>);
    static_assert(std::is_same_v<const double, std::remove_reference_t<decltype(y)>>);
    EXPECT_EQ(33, x);
    EXPECT_DOUBLE_EQ(66.0, y);
}

TEST(UtilsMetaPair, CompareEqualUnequal)
{
    constexpr auto a = Pair{1, 2.0};
    constexpr auto b = Pair{1, 2.0};
    constexpr auto c = Pair{1, 3.0};
    constexpr auto d = Pair{2, 1.0};

    static_assert(a == b);
    static_assert(a != c);
    static_assert(a != d);
}

TEST(UtilsMetaPair, CompareLess)
{
    static_assert(Pair{1, 2.0} < Pair{1, 3.0});
    static_assert(Pair{1, 2.0} < Pair{2, 1.0});
}

TEST(UtilsMetaPair, CompareLessEqual)
{
    constexpr auto a = Pair{1, 2.0};
    constexpr auto b = Pair{1, 2.0};
    constexpr auto c = Pair{1, 3.0};

    static_assert(a <= b);
    static_assert(a <= c);
}

TEST(UtilsMetaPair, CompareGreater)
{
    static_assert(Pair{2, 1.0} > Pair{1, 3.0});
}

TEST(UtilsMetaPair, CompareGreaterEqual)
{
    constexpr auto a = Pair{2, 1.0};
    constexpr auto b = Pair{1, 3.0};
    constexpr auto c = Pair{2, 1.0};

    static_assert(a >= b);
    static_assert(a >= c);
}

TEST(UtilsMetaPair, CompareWithStdPair)
{
    constexpr auto a = Pair{1, 2.0};
    constexpr auto b = std::pair{1, 2.0};
    constexpr auto c = std::pair{1, 3.0};

    static_assert(a == b);
    static_assert(a != c);
    static_assert(a < c);
}

TEST(UtilsMetaPair, CompareWithStdTuple)
{
    constexpr auto a = Pair{1, 2.0};
    constexpr auto b = std::tuple{1, 2.0};
    constexpr auto c = std::tuple{1, 3.0};

    static_assert(a == b);
    static_assert(a != c);
    static_assert(a < c);
}

TEST(UtilsMetaPair, CompareThreeWay)
{
    static_assert((Pair{1, 2.0} <=> Pair{1, 2.0}) == 0);
    static_assert((Pair{1, 2.0} <=> Pair{1, 3.0}) < 0);
    static_assert((Pair{2, 1.0} <=> Pair{1, 3.0}) > 0);
}

TEST(UtilsMetaPair, CompareThreeWayResultType)
{
    // meta_pair<int, int> → strong_ordering
    using IntPair = rbox::meta_pair<int, int>;
    static_assert(std::is_same_v<std::strong_ordering, decltype(IntPair{} <=> IntPair{})>);

    // meta_pair<int, double> → double weakens to partial_ordering → partial_ordering
    static_assert(std::is_same_v<std::partial_ordering, decltype(Pair{} <=> Pair{})>);

    // meta_pair<double, double> → partial_ordering
    using DoublePair = rbox::meta_pair<double, double>;
    static_assert(std::is_same_v<std::partial_ordering, decltype(DoublePair{} <=> DoublePair{})>);
}

TEST(UtilsMetaPair, CompareWithNaN)
{
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    constexpr auto p_finite = Pair{1, 2.0};
    constexpr auto p_nan_first = Pair{1, nan};
    constexpr auto p_nan_second = Pair{1, nan};

    // NaN != NaN in any position
    EXPECT_FALSE(p_nan_first == p_nan_second);
    EXPECT_TRUE(p_nan_first != p_nan_second);

    // Comparisons with NaN yield false for all relational operators
    EXPECT_FALSE(p_nan_first < p_finite);
    EXPECT_FALSE(p_nan_first > p_finite);
    EXPECT_FALSE(p_nan_first <= p_finite);
    EXPECT_FALSE(p_nan_first >= p_finite);
    EXPECT_FALSE(p_finite < p_nan_first);
    EXPECT_FALSE(p_finite > p_nan_first);

    // <=> with NaN yields unordered
    EXPECT_EQ(std::partial_ordering::unordered, p_nan_first <=> p_finite);
    EXPECT_EQ(std::partial_ordering::unordered, p_nan_first <=> p_nan_second);
}
