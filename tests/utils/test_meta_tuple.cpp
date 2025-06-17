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
#include <ranges>
#include <rbox/utils/functional.hpp>
#include <rbox/utils/meta_tuple.hpp>

#include "tests/test_options.hpp"

TEST(UtilsMetaTuple, Small)
{
    using tuple_type = rbox::meta_tuple<char, int, double>;
    static_assert(rbox::tuple_like<tuple_type>);
    static_assert(std::tuple_size_v<tuple_type> == 3);
    static_assert(std::is_same_v<std::tuple_element_t<0, tuple_type>, char>);
    static_assert(std::is_same_v<std::tuple_element_t<1, tuple_type>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<2, tuple_type>, double>);

    // CTAD
    auto t = rbox::meta_tuple{'0', 123, 2.5};
    static_assert(std::is_same_v<decltype(t), tuple_type>);
    // Free get()
    EXPECT_EQ('0', get<0>(t));
    EXPECT_EQ(123, get<1>(t));
    EXPECT_EQ(2.5, get<2>(t));
    // Direct access via field name
    EXPECT_EQ('0', t.elements.v1);
    EXPECT_EQ(123, t.elements.v2);
    EXPECT_EQ(2.5, t.elements.v3);

    get<0>(t) += 5;
    EXPECT_EQ('5', t.elements.v1);

    // Structured binding
    auto [x, y, z] = t;
    EXPECT_EQ('5', x);
    EXPECT_EQ(123, y);
    EXPECT_EQ(2.5, z);
}

TEST(UtilsMetaTuple, Large)
{
    using tuple_type = rbox::meta_tuple<bool, char, int, float, long, double>;
    static_assert(rbox::tuple_like<tuple_type>);
    static_assert(std::tuple_size_v<tuple_type> == 6);
    static_assert(std::is_same_v<std::tuple_element_t<0, tuple_type>, bool>);
    static_assert(std::is_same_v<std::tuple_element_t<1, tuple_type>, char>);
    static_assert(std::is_same_v<std::tuple_element_t<2, tuple_type>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<3, tuple_type>, float>);
    static_assert(std::is_same_v<std::tuple_element_t<4, tuple_type>, long>);
    static_assert(std::is_same_v<std::tuple_element_t<5, tuple_type>, double>);

    // CTAD
    auto t = tuple_type{true, 'A', 4, 1.25f, 10L, 3.25};
    static_assert(std::is_same_v<decltype(t), tuple_type>);
    // Free get()
    EXPECT_TRUE(get<0>(t));
    EXPECT_EQ('A', get<1>(t));
    EXPECT_EQ(4, get<2>(t));
    EXPECT_EQ(1.25f, get<3>(t));
    EXPECT_EQ(10L, get<4>(t));
    EXPECT_EQ(3.25, get<5>(t));

    get<1>(t) += 7;
    EXPECT_EQ('H', t.elements.v2);

    // Structured binding
    auto [b, c, i, f, l, d] = t;
    EXPECT_TRUE(b);
    EXPECT_EQ('H', c);
    EXPECT_EQ(4, i);
    EXPECT_EQ(1.25f, f);
    EXPECT_EQ(10L, l);
    EXPECT_EQ(3.25, d);
}

TEST(UtilsMetaTuple, RangeAdaptors)
{
    // Create a range of meta_tuple
    std::vector<rbox::meta_tuple<int, int>> tuples = {{1, 2}, {3, 4}, {4, 6}, {7, 8}, {8, 10}};

    // Test views::filter
    auto filtered = tuples | std::views::filter([](const auto& t) {
                        return get<0>(t) % 2 != 0;
                    });
    EXPECT_EQ(std::ranges::distance(filtered), 3);
    // Verify filtered elements
    std::vector<rbox::meta_tuple<int, int>> filtered_result;
    std::ranges::copy(filtered, std::back_inserter(filtered_result));
    EXPECT_EQ(filtered_result[0], tuples[0]);
    EXPECT_EQ(filtered_result[1], tuples[1]);
    EXPECT_EQ(filtered_result[2], tuples[3]);

    // Test views::transform
    auto transformed = tuples | std::views::transform([](const auto& t) {
                           return rbox::meta_tuple{get<0>(t) * 2, get<1>(t) * 2};
                       });
    std::vector<rbox::meta_tuple<int, int>> transformed_expected = {
        {2, 4}, {6, 8}, {8, 12}, {14, 16}, {16, 20}};
    EXPECT_EQ(std::ranges::distance(transformed), 5);
    // Verify transformed elements
    std::vector<rbox::meta_tuple<int, int>> transformed_result;
    std::ranges::copy(transformed, std::back_inserter(transformed_result));
    EXPECT_EQ(transformed_result, transformed_expected);

    // Test views::take
    auto taken = tuples | std::views::take(3);
    EXPECT_EQ(std::ranges::distance(taken), 3);
    // Verify taken elements
    std::vector<rbox::meta_tuple<int, int>> taken_result;
    std::ranges::copy(taken, std::back_inserter(taken_result));
    EXPECT_EQ(taken_result[0], tuples[0]);
    EXPECT_EQ(taken_result[1], tuples[1]);
    EXPECT_EQ(taken_result[2], tuples[2]);

    // Test views::drop
    auto dropped = tuples | std::views::drop(2);
    EXPECT_EQ(std::ranges::distance(dropped), 3);
    // Verify dropped elements
    std::vector<rbox::meta_tuple<int, int>> dropped_result;
    std::ranges::copy(dropped, std::back_inserter(dropped_result));
    EXPECT_EQ(dropped_result[0], tuples[2]);
    EXPECT_EQ(dropped_result[1], tuples[3]);
    EXPECT_EQ(dropped_result[2], tuples[4]);

    // Test views::reverse
    auto reversed = tuples | std::views::reverse;
    EXPECT_EQ(std::ranges::distance(reversed), 5);
    // Verify reversed elements
    std::vector<rbox::meta_tuple<int, int>> reversed_result;
    std::ranges::copy(reversed, std::back_inserter(reversed_result));
    EXPECT_EQ(reversed_result[0], tuples[4]);
    EXPECT_EQ(reversed_result[1], tuples[3]);
    EXPECT_EQ(reversed_result[2], tuples[2]);
    EXPECT_EQ(reversed_result[3], tuples[1]);
    EXPECT_EQ(reversed_result[4], tuples[0]);

    // Test views::zip with another range
    std::vector<int> values = {10, 20, 30, 40, 50};
    auto zipped = std::views::zip(tuples, values);
    EXPECT_EQ(std::ranges::distance(zipped), 5);
    // Verify zipped elements
    int i = 0;
    for (const auto& [t, v] : zipped) {
        EXPECT_EQ(t, tuples[i]);
        EXPECT_EQ(v, values[i]);
        ++i;
    }

    // Test views::enumerate
    auto enumerated = tuples | std::views::enumerate;
    EXPECT_EQ(std::ranges::distance(enumerated), 5);
    // Verify enumerated elements
    i = 0;
    for (const auto& [idx, t] : enumerated) {
        EXPECT_EQ(idx, i);
        EXPECT_EQ(t, tuples[i]);
        ++i;
    }
}

TEST(UtilsMetaTuple, RangeAdaptorsWithDifferentTypes)
{
    // Create a range of meta_tuple with different types
    std::vector<rbox::meta_tuple<int, double, std::string>> tuples = {
        {1, 1.1, "one"}, {2, 2.2, "two"}, {3, 3.3, "three"}};

    // Test views::filter with different types
    auto filtered = tuples | std::views::filter([](const auto& t) {
                        return get<0>(t) > 1;
                    });
    EXPECT_EQ(std::ranges::distance(filtered), 2);
    // Verify filtered elements
    std::vector<rbox::meta_tuple<int, double, std::string>> filtered_result;
    std::ranges::copy(filtered, std::back_inserter(filtered_result));
    EXPECT_EQ(filtered_result[0], tuples[1]);
    EXPECT_EQ(filtered_result[1], tuples[2]);

    // Test views::transform with type conversion
    auto transformed =
        tuples | std::views::transform([](const auto& t) {
            return rbox::meta_tuple{get<0>(t), get<1>(t) + 1.0, get<2>(t) + "_transformed"};
        });
    EXPECT_EQ(std::ranges::distance(transformed), 3);
    // Verify transformed elements
    std::vector<rbox::meta_tuple<int, double, std::string>> transformed_result;
    std::ranges::copy(transformed, std::back_inserter(transformed_result));
    EXPECT_EQ(transformed_result[0], (rbox::meta_tuple{1, 2.1, "one_transformed"}));
    EXPECT_EQ(transformed_result[1], (rbox::meta_tuple{2, 3.2, "two_transformed"}));
    EXPECT_EQ(transformed_result[2], (rbox::meta_tuple{3, 4.3, "three_transformed"}));
}

// ==========================================================================
// meta_tuple — comparison
// ==========================================================================

using Cmp2 = rbox::meta_tuple<int, double>;
using Cmp3 = rbox::meta_tuple<int, double, char>;

TEST(UtilsMetaTuple, CompareEqualUnequal)
{
    EXPECT_TRUE((Cmp2{1, 2.0} == Cmp2{1, 2.0}));
    EXPECT_FALSE((Cmp2{1, 2.0} == Cmp2{1, 3.0}));
    EXPECT_FALSE((Cmp2{1, 2.0} == Cmp2{2, 2.0}));
    EXPECT_TRUE((Cmp2{1, 2.0} != Cmp2{1, 3.0}));
    EXPECT_TRUE((Cmp2{1, 2.0} != Cmp2{2, 2.0}));
    EXPECT_FALSE((Cmp2{1, 2.0} != Cmp2{1, 2.0}));
}

TEST(UtilsMetaTuple, CompareLess)
{
    EXPECT_TRUE((Cmp2{1, 2.0} < Cmp2{1, 3.0}));
    EXPECT_TRUE((Cmp2{1, 2.0} < Cmp2{2, 1.0}));
    EXPECT_FALSE((Cmp2{1, 3.0} < Cmp2{1, 2.0}));
    EXPECT_FALSE((Cmp2{2, 1.0} < Cmp2{1, 2.0}));
    EXPECT_FALSE((Cmp2{1, 2.0} < Cmp2{1, 2.0}));
}

TEST(UtilsMetaTuple, CompareLessEqual)
{
    EXPECT_TRUE((Cmp2{1, 2.0} <= Cmp2{1, 2.0}));
    EXPECT_TRUE((Cmp2{1, 2.0} <= Cmp2{1, 3.0}));
    EXPECT_FALSE((Cmp2{1, 3.0} <= Cmp2{1, 2.0}));
}

TEST(UtilsMetaTuple, CompareGreater)
{
    EXPECT_TRUE((Cmp2{2, 1.0} > Cmp2{1, 3.0}));
    EXPECT_TRUE((Cmp2{1, 3.0} > Cmp2{1, 2.0}));
    EXPECT_FALSE((Cmp2{1, 2.0} > Cmp2{1, 3.0}));
    EXPECT_FALSE((Cmp2{1, 2.0} > Cmp2{1, 2.0}));
}

TEST(UtilsMetaTuple, CompareGreaterEqual)
{
    EXPECT_TRUE((Cmp2{2, 1.0} >= Cmp2{1, 3.0}));
    EXPECT_TRUE((Cmp2{1, 2.0} >= Cmp2{1, 2.0}));
    EXPECT_FALSE((Cmp2{1, 2.0} >= Cmp2{1, 3.0}));
}

TEST(UtilsMetaTuple, CompareThreeWay)
{
    EXPECT_EQ((Cmp2{1, 2.0} <=> Cmp2{1, 2.0}), std::partial_ordering::equivalent);
    EXPECT_EQ((Cmp2{1, 2.0} <=> Cmp2{1, 3.0}), std::partial_ordering::less);
    EXPECT_EQ((Cmp2{2, 1.0} <=> Cmp2{1, 3.0}), std::partial_ordering::greater);
}

TEST(UtilsMetaTuple, CompareThreeWayResultType)
{
    // meta_tuple<int, int> → all elements have strong_ordering → result is strong_ordering
    using Int2 = rbox::meta_tuple<int, int>;
    static_assert(std::is_same_v<std::strong_ordering, decltype(Int2{1, 2} <=> Int2{1, 2})>);

    // meta_tuple<int, double> → double weakens to partial_ordering → partial_ordering
    static_assert(std::is_same_v<std::partial_ordering, decltype(Cmp2{1, 2.0} <=> Cmp2{1, 3.0})>);
}

TEST(UtilsMetaTuple, CompareWithStdTuple)
{
    EXPECT_TRUE((Cmp2{1, 2.0} == std::tuple{1, 2.0}));
    EXPECT_FALSE((Cmp2{1, 2.0} == std::tuple{1, 3.0}));
    EXPECT_TRUE((Cmp2{1, 2.0} != std::tuple{1, 3.0}));
    EXPECT_TRUE((Cmp2{1, 2.0} < std::tuple{1, 3.0}));
    EXPECT_TRUE((Cmp2{2, 1.0} > std::tuple{1, 3.0}));
}

TEST(UtilsMetaTuple, CompareWithNaN)
{
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    auto t_finite = Cmp2{1, 2.0};
    auto t_nan_first = Cmp2{1, nan};
    auto t_nan_second = Cmp2{1, nan};

    EXPECT_FALSE(t_nan_first == t_nan_second);
    EXPECT_TRUE(t_nan_first != t_nan_second);

    EXPECT_FALSE(t_nan_first < t_finite);
    EXPECT_FALSE(t_nan_first > t_finite);
    EXPECT_FALSE(t_nan_first <= t_finite);
    EXPECT_FALSE(t_nan_first >= t_finite);
    EXPECT_FALSE(t_finite < t_nan_first);
    EXPECT_FALSE(t_finite > t_nan_first);

    EXPECT_EQ(std::partial_ordering::unordered, t_nan_first <=> t_finite);
    EXPECT_EQ(std::partial_ordering::unordered, t_nan_first <=> t_nan_second);
}

TEST(UtilsMetaTuple, CompareThreeElements)
{
    EXPECT_TRUE((Cmp3{1, 2.0, 'a'} == Cmp3{1, 2.0, 'a'}));
    EXPECT_TRUE((Cmp3{1, 2.0, 'a'} != Cmp3{1, 2.0, 'b'}));
    EXPECT_TRUE((Cmp3{1, 2.0, 'a'} < Cmp3{1, 2.0, 'b'}));
    EXPECT_TRUE((Cmp3{1, 2.0, 'a'} < Cmp3{1, 3.0, 'a'}));
    EXPECT_TRUE((Cmp3{1, 2.0, 'a'} < Cmp3{2, 1.0, 'a'}));
    EXPECT_TRUE((Cmp3{2, 1.0, 'a'} > Cmp3{1, 3.0, 'a'}));

    EXPECT_EQ((Cmp3{1, 2.0, 'a'} <=> Cmp3{1, 2.0, 'a'}), std::partial_ordering::equivalent);
    EXPECT_EQ((Cmp3{1, 2.0, 'a'} <=> Cmp3{1, 2.0, 'b'}), std::partial_ordering::less);
    EXPECT_EQ((Cmp3{2, 1.0, 'a'} <=> Cmp3{1, 3.0, 'a'}), std::partial_ordering::greater);
}
