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
#include <rbox/utils/meta_triple.hpp>

#include "tests/test_options.hpp"

using Triple = rbox::meta_triple<int, double, char>;

TEST(UtilsMetaTriple, Members)
{
    constexpr auto t = Triple{1, 3.14, 'x'};
    static_assert(t.first == 1);
    static_assert(t.second == 3.14);
    static_assert(t.third == 'x');
}

TEST(UtilsMetaTriple, GetLvalueRef)
{
    auto t = Triple{1, 2.0, 'a'};
    auto& r0 = get<0>(t);
    auto& r1 = get<1>(t);
    auto& r2 = get<2>(t);
    EXPECT_EQ(1, r0);
    EXPECT_DOUBLE_EQ(2.0, r1);
    EXPECT_EQ('a', r2);

    r0 = 10;
    r1 = 20.0;
    r2 = 'z';
    EXPECT_EQ(10, t.first);
    EXPECT_DOUBLE_EQ(20.0, t.second);
    EXPECT_EQ('z', t.third);
}

TEST(UtilsMetaTriple, GetConstLvalueRef)
{
    const auto t = Triple{3, 4.5, 'b'};
    const auto& r0 = get<0>(t);
    const auto& r1 = get<1>(t);
    const auto& r2 = get<2>(t);
    EXPECT_EQ(3, r0);
    EXPECT_DOUBLE_EQ(4.5, r1);
    EXPECT_EQ('b', r2);
}

TEST(UtilsMetaTriple, GetRvalueRef)
{
    auto v0 = get<0>(Triple{5, 6.0, 'c'});
    auto v1 = get<1>(Triple{5, 6.0, 'c'});
    auto v2 = get<2>(Triple{5, 6.0, 'c'});
    EXPECT_EQ(5, v0);
    EXPECT_DOUBLE_EQ(6.0, v1);
    EXPECT_EQ('c', v2);
}

TEST(UtilsMetaTriple, StructuredBinding)
{
    auto t = Triple{100, 200.5, 'q'};
    auto& [x, y, z] = t;
    EXPECT_EQ(100, x);
    EXPECT_DOUBLE_EQ(200.5, y);
    EXPECT_EQ('q', z);

    x = -1;
    y = -2.0;
    z = '!';
    EXPECT_EQ(-1, t.first);
    EXPECT_DOUBLE_EQ(-2.0, t.second);
    EXPECT_EQ('!', t.third);
}

TEST(UtilsMetaTriple, StructuredBindingConst)
{
    const auto t = Triple{7, 8.5, 'k'};
    auto& [x, y, z] = t;
    static_assert(std::is_same_v<const int, std::remove_reference_t<decltype(x)>>);
    static_assert(std::is_same_v<const double, std::remove_reference_t<decltype(y)>>);
    static_assert(std::is_same_v<const char, std::remove_reference_t<decltype(z)>>);
    EXPECT_EQ(7, x);
    EXPECT_DOUBLE_EQ(8.5, y);
    EXPECT_EQ('k', z);
}

TEST(UtilsMetaTriple, CompareEqualUnequal)
{
    constexpr auto a = Triple{1, 2.0, 'a'};
    constexpr auto b = Triple{1, 2.0, 'a'};
    constexpr auto c = Triple{1, 2.0, 'b'};
    constexpr auto d = Triple{1, 3.0, 'a'};

    static_assert(a == b);
    static_assert(a != c);
    static_assert(a != d);
}

TEST(UtilsMetaTriple, CompareLess)
{
    static_assert(Triple{1, 2.0, 'a'} < Triple{1, 2.0, 'b'});
    static_assert(Triple{1, 2.0, 'a'} < Triple{1, 3.0, 'a'});
    static_assert(Triple{1, 2.0, 'a'} < Triple{2, 1.0, 'a'});
}

TEST(UtilsMetaTriple, CompareLessEqual)
{
    constexpr auto a = Triple{1, 2.0, 'a'};
    constexpr auto b = Triple{1, 2.0, 'a'};
    constexpr auto c = Triple{1, 2.0, 'b'};

    static_assert(a <= b);
    static_assert(a <= c);
}

TEST(UtilsMetaTriple, CompareGreater)
{
    static_assert(Triple{2, 1.0, 'a'} > Triple{1, 3.0, 'a'});
}

TEST(UtilsMetaTriple, CompareGreaterEqual)
{
    constexpr auto a = Triple{2, 1.0, 'a'};
    constexpr auto b = Triple{1, 3.0, 'a'};
    constexpr auto c = Triple{2, 1.0, 'a'};

    static_assert(a >= b);
    static_assert(a >= c);
}

TEST(UtilsMetaTriple, CompareWithStdTuple)
{
    constexpr auto a = Triple{1, 2.0, 'a'};
    constexpr auto b = std::tuple{1, 2.0, 'a'};
    constexpr auto c = std::tuple{1, 2.0, 'b'};

    static_assert(a == b);
    static_assert(a != c);
    static_assert(a < c);
}

TEST(UtilsMetaTriple, CompareThreeWay)
{
    static_assert((Triple{1, 2.0, 'a'} <=> Triple{1, 2.0, 'a'}) == 0);
    static_assert((Triple{1, 2.0, 'a'} <=> Triple{1, 2.0, 'b'}) < 0);
    static_assert((Triple{2, 1.0, 'a'} <=> Triple{1, 3.0, 'a'}) > 0);
}

TEST(UtilsMetaTriple, CompareThreeWayResultType)
{
    // meta_triple<int, int, int> → strong_ordering
    using IntTriple = rbox::meta_triple<int, int, int>;
    static_assert(std::is_same_v<std::strong_ordering, decltype(IntTriple{} <=> IntTriple{})>);

    // meta_triple<int, double, char> → partial_ordering
    static_assert(std::is_same_v<std::partial_ordering, decltype(Triple{} <=> Triple{})>);
}

TEST(UtilsMetaTriple, CompareWithNaN)
{
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    constexpr auto t_finite = Triple{1, 2.0, 'a'};
    constexpr auto t_nan = Triple{1, nan, 'a'};

    EXPECT_FALSE(t_nan == t_nan);
    EXPECT_TRUE(t_nan != t_nan);

    EXPECT_FALSE(t_nan < t_finite);
    EXPECT_FALSE(t_nan > t_finite);
    EXPECT_FALSE(t_nan <= t_finite);
    EXPECT_FALSE(t_nan >= t_finite);

    EXPECT_EQ(std::partial_ordering::unordered, t_nan <=> t_finite);
    EXPECT_EQ(std::partial_ordering::unordered, t_nan <=> t_nan);
}
