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

#include <rbox/utils/concepts.hpp>
#include <rbox/utils/meta_variant.hpp>

#include "tests/test_options.hpp"

template <class T, class U>
    requires (std::is_same_v<T, U>)
constexpr auto equals_as_same_type(const T& t, const U& u)
{
    return t == u;
}

TEST(UtilsMetaVariant, Basic)
{
    static constexpr auto x = 42;
    static constexpr auto y = 1.25;
    static constexpr auto z = 42L;

    constexpr auto u = rbox::meta_variant<int, double>(x);
    static_assert(u.index() == 0);
    static_assert(u.holds_alternative<int>());
    static_assert(!u.holds_alternative<double>());
    static_assert(u.get<int>() == 42);
    static_assert(*u.get_if<int>() == 42);
    static_assert(u.get<0>() == 42);
    static_assert(*u.get_if<0>() == 42);

    static_assert(holds_alternative<int>(u));
    static_assert(!holds_alternative<double>(u));
    static_assert(get<int>(u) == 42);
    static_assert(get<0>(u) == 42);
    static_assert(*get_if<int>(u) == 42);
    static_assert(*get_if<0>(u) == 42);

    constexpr auto v = rbox::meta_variant<int, double>(y);
    static_assert(v.index() == 1);
    static_assert(!v.holds_alternative<int>());
    static_assert(v.holds_alternative<double>());
    static_assert(v.get<double>() == 1.25);
    static_assert(*v.get_if<double>() == 1.25);
    static_assert(v.get<1>() == 1.25);
    static_assert(*v.get_if<1>() == 1.25);

    static_assert(holds_alternative<double>(v));
    static_assert(!holds_alternative<int>(v));
    static_assert(get<double>(v) == 1.25);
    static_assert(*get_if<double>(v) == 1.25);
    static_assert(get<1>(v) == 1.25);
    static_assert(*get_if<1>(v) == 1.25);

    constexpr auto w = rbox::meta_variant<int, long>(z);

    constexpr auto visitor = [](rbox::arithmetic_type auto value) {
        return static_cast<int>(value + 1.75);
    };
    static_assert(u.visit(visitor) == 43);
    static_assert(v.visit(visitor) == 3);
    static_assert(visit(visitor, u) == 43);
    static_assert(visit(visitor, v) == 3);

    static_assert(u != v);
    static_assert(equals_as_same_type((u <=> v), std::partial_ordering::less));

    static_assert(u == u);
    static_assert(equals_as_same_type((u <=> u), std::partial_ordering::equivalent));

    static_assert(v == v);
    static_assert(equals_as_same_type((v <=> v), std::partial_ordering::equivalent));

    static_assert(w == w);
    static_assert(equals_as_same_type((w <=> w), std::strong_ordering::equal));
}

TEST(UtilsMetaVariant, DefaultConstruction)
{
    constexpr auto v = rbox::meta_variant<int, double, long>{};
    static_assert(v.index() == 0);
    static_assert(v.holds_alternative<int>());
    static_assert(v.get<int>() == 0);
    static_assert(v.get<0>() == 0);
}

TEST(UtilsMetaVariant, MoreAlternatives)
{
    constexpr auto v = rbox::meta_variant<int, double, long, char, bool>(true);
    static_assert(v.index() == 4);
    static_assert(v.holds_alternative<bool>());
    static_assert(v.get<bool>() == true);
    static_assert(v.get<4>() == true);

    static_assert(holds_alternative<bool>(v));
    static_assert(!holds_alternative<int>(v));
}

TEST(UtilsMetaVariant, GetIfWrongAlternative)
{
    constexpr auto v = rbox::meta_variant<int, double>(3.14);
    static_assert(v.get_if<double>() != nullptr);
    static_assert(*v.get_if<double>() == 3.14);
    static_assert(v.get_if<int>() == nullptr);
    static_assert(v.get_if<0>() == nullptr);
    static_assert(v.get_if<1>() != nullptr);
    static_assert(*v.get_if<1>() == 3.14);
}

TEST(UtilsMetaVariant, CopyConstruction)
{
    constexpr auto v1 = rbox::meta_variant<int, double, long>(42L);
    constexpr auto v2 = v1;
    static_assert(v2.index() == 2);
    static_assert(v2.get<long>() == 42L);
    static_assert(v1 == v2);
    static_assert(equals_as_same_type((v1 <=> v2), std::partial_ordering::equivalent));
}

TEST(UtilsMetaVariant, Assignment)
{
    constexpr auto make = []() constexpr {
        auto v = rbox::meta_variant<int, double, long>(42);
        v = rbox::meta_variant<int, double, long>(3.14);
        return v;
    };
    constexpr auto v = make();
    static_assert(v.index() == 1);
    static_assert(v.get<double>() == 3.14);
}

// int converts to long (exact match absent → single conversion match)
struct convertible_to_double {
    int value;
    constexpr operator double() const
    {
        return static_cast<double>(value);
    }
    constexpr operator int() const = delete;
};

static_assert(!std::is_convertible_v<convertible_to_double, int>);
static_assert(std::is_convertible_v<convertible_to_double, double>);

TEST(UtilsMetaVariant, ConversionMatch)
{
    // long literal: exact match for long
    constexpr auto v1 = rbox::meta_variant<long, double>(42L);
    static_assert(v1.index() == 0);
    static_assert(v1.get<long>() == 42);

    constexpr auto v2 = rbox::meta_variant<int, double>(convertible_to_double{42});
    static_assert(v2.index() == 1);
    static_assert(v2.get<double>() == 42.0);
}

TEST(UtilsMetaVariant, VisitorReturnDifferentTypes)
{
    constexpr auto v1 = rbox::meta_variant<int, double>(42);
    constexpr auto v2 = rbox::meta_variant<int, double>(3.14);

    auto to_string = [](const auto& x) -> std::string {
        return std::to_string(x);
    };
    EXPECT_EQ("42", v1.visit(to_string));
    EXPECT_EQ("3.14", v2.visit(to_string));
}

TEST(UtilsMetaVariant, VisitorWithMutations)
{
    auto v = rbox::meta_variant<int, double>(10);

    auto doubler = [](auto& x) {
        x *= 2;
    };
    v.visit(doubler);
    EXPECT_EQ(20, v.get<int>());

    // Switch to double; double it.
    v = rbox::meta_variant<int, double>(3.5);
    v.visit(doubler);
    EXPECT_EQ(7.0, v.get<double>());
}

TEST(UtilsMetaVariant, ConstCorrectness)
{
    constexpr auto v = rbox::meta_variant<int, double>(42);
    static_assert(std::is_same_v<const int&, decltype(v.get<int>())>);
    static_assert(std::is_same_v<const int*, decltype(v.get_if<int>())>);
    static_assert(std::is_same_v<const double*, decltype(v.get_if<double>())>);
}

TEST(UtilsMetaVariant, SingleAlternative)
{
    constexpr auto v = rbox::meta_variant<int>(42);
    static_assert(v.index() == 0);
    static_assert(v.holds_alternative<int>());
    static_assert(v.get<int>() == 42);
    static_assert(v.get<0>() == 42);
    static_assert(v.get_if<int>() != nullptr);
    static_assert(*v.get_if<int>() == 42);

    constexpr auto vd = rbox::meta_variant<int>{};
    static_assert(vd.index() == 0);
    static_assert(vd.get<int>() == 0);

    constexpr auto visitor = [](const auto& x) {
        return x + 1;
    };
    static_assert(v.visit(visitor) == 43);
}

TEST(UtilsMetaVariant, ComparisonDifferentAlternatives)
{
    constexpr auto v_int = rbox::meta_variant<int, double, long>(42);
    constexpr auto v_dbl = rbox::meta_variant<int, double, long>(3.14);
    constexpr auto v_lng = rbox::meta_variant<int, double, long>(42L);

    static_assert(v_int != v_dbl);
    static_assert(v_int != v_lng);
    static_assert(v_dbl != v_lng);

    // int(index 0) < double(index 1) < long(index 2)
    static_assert(v_int < v_dbl);
    static_assert(v_dbl < v_lng);
    static_assert(v_int < v_lng);
}

TEST(UtilsMetaVariant, ComparisonSameAlternativeDifferentValues)
{
    constexpr auto v1 = rbox::meta_variant<int, double>(10);
    constexpr auto v2 = rbox::meta_variant<int, double>(20);
    constexpr auto v3 = rbox::meta_variant<int, double>(10);

    static_assert(v1 == v3);
    static_assert(v1 != v2);
    static_assert(v1 < v2);
    static_assert(v2 > v1);
}

TEST(UtilsMetaVariant, FreeFunctionGetIf)
{
    constexpr auto v = rbox::meta_variant<int, double, long>(3.14);

    // Free functions
    static_assert(get_if<int>(v) == nullptr);
    const auto* p = get_if<double>(v);
    ASSERT_NE(nullptr, p);
    EXPECT_EQ(3.14, *p);

    static_assert(get_if<0>(v) == nullptr);
    const auto* p1 = get_if<1>(v);
    ASSERT_NE(nullptr, p1);
    EXPECT_EQ(3.14, *p1);
}

TEST(UtilsMetaVariant, Emplace)
{
    auto v = rbox::meta_variant<int, double>(42);
    EXPECT_EQ(0, v.index());

    // Emplace via assignment
    v = rbox::meta_variant<int, double>(3.14);
    EXPECT_EQ(1, v.index());
    EXPECT_EQ(3.14, v.get<double>());
}

TEST(UtilsMetaVariant, OperatorAssignment)
{
    auto v = rbox::meta_variant<int, double, long>(42);
    EXPECT_EQ(0, v.index());
    EXPECT_EQ(42, v.get<int>());

    // Assign a variant of same alternative → copy value
    auto v_dbl = rbox::meta_variant<int, double, long>(3.14);
    v = v_dbl;
    EXPECT_EQ(1, v.index());
    EXPECT_EQ(3.14, v.get<double>());

    // Self-assignment
    v = v;
    EXPECT_EQ(1, v.index());
    EXPECT_EQ(3.14, v.get<double>());

    // Assign with third alternative
    v = rbox::meta_variant<int, double, long>(42L);
    EXPECT_EQ(2, v.index());
    EXPECT_EQ(42L, v.get<long>());

    // Chain assignment
    auto v2 = rbox::meta_variant<int, double, long>(100);
    v2 = v = rbox::meta_variant<int, double, long>(3.14);
    EXPECT_EQ(1, v2.index());
    EXPECT_EQ(3.14, v2.get<double>());
}

TEST(UtilsMetaVariant, RepeatedEmplace)
{
    auto v = rbox::meta_variant<int, double, long, char>(42);
    EXPECT_EQ(0, v.index());

    v = rbox::meta_variant<int, double, long, char>(3.14);
    EXPECT_EQ(1, v.index());

    v = rbox::meta_variant<int, double, long, char>(42L);
    EXPECT_EQ(2, v.index());

    v = rbox::meta_variant<int, double, long, char>('X');
    EXPECT_EQ(3, v.index());
    EXPECT_EQ('X', v.get<char>());

    // Back to first
    v = rbox::meta_variant<int, double, long, char>(-1);
    EXPECT_EQ(0, v.index());
    EXPECT_EQ(-1, v.get<int>());
}

TEST(UtilsMetaVariant, AssignSameAlternativeDifferentValue)
{
    auto v = rbox::meta_variant<int, double>(10);
    EXPECT_EQ(10, v.get<int>());

    // Assign another int variant → overwrites
    v = rbox::meta_variant<int, double>(20);
    EXPECT_EQ(0, v.index());
    EXPECT_EQ(20, v.get<int>());

    v = rbox::meta_variant<int, double>(30);
    EXPECT_EQ(0, v.index());
    EXPECT_EQ(30, v.get<int>());
}

TEST(UtilsMetaVariant, EmplaceWithGetter)
{
    constexpr auto make = []() constexpr {
        auto v = rbox::meta_variant<int, double>(42);
        v = rbox::meta_variant<int, double>(3.14);
        return v;
    };
    constexpr auto v = make();
    static_assert(v.index() == 1);
    static_assert(v.get<double>() == 3.14);
    static_assert(v.get<1>() == 3.14);
}

TEST(UtilsMetaVariant, CopyAssignmentPreservesValue)
{
    auto v1 = rbox::meta_variant<int, double>(42);
    auto v2 = rbox::meta_variant<int, double>(0);

    v2 = v1;
    EXPECT_EQ(0, v2.index());
    EXPECT_EQ(42, v2.get<int>());
    EXPECT_EQ(v1, v2);

    // Verify v1 is untouched
    EXPECT_EQ(0, v1.index());
    EXPECT_EQ(42, v1.get<int>());
}

TEST(UtilsMetaVariant, HardValueTypeSemantics)
{
    struct hard_value {
        int data;
        explicit hard_value(int d) : data(d) {}
        hard_value() = default;
        hard_value(const hard_value&) = default;
        hard_value(hard_value&&) = default;
        hard_value& operator=(const hard_value&) = default;
        hard_value& operator=(hard_value&&) = default;
        auto operator==(const hard_value&) const -> bool = default;
        auto operator<=>(const hard_value&) const = default;
    };

    // Default construction
    auto v1 = rbox::meta_variant<hard_value, int>{};
    EXPECT_EQ(0, v1.index());
    EXPECT_EQ(0, v1.get<hard_value>().data);

    // Construction
    auto v2 = rbox::meta_variant<hard_value, int>(hard_value{42});
    EXPECT_EQ(0, v2.index());
    EXPECT_EQ(42, v2.get<hard_value>().data);

    // Assignment
    v1 = rbox::meta_variant<hard_value, int>(hard_value{99});
    EXPECT_EQ(0, v1.index());
    EXPECT_EQ(99, v1.get<hard_value>().data);

    // Copy assignment
    v1 = v2;
    EXPECT_EQ(0, v1.index());
    EXPECT_EQ(42, v1.get<hard_value>().data);

    // Switch type
    v1 = rbox::meta_variant<hard_value, int>(100);
    EXPECT_EQ(1, v1.index());
    EXPECT_EQ(100, v1.get<int>());
}
