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

struct foo_1_t {
    // Nested classes are expected to be filtered out.
    struct value_nested_squared_plus_a {};

    int value_x;
    long value_y;
    long value_z;
    double value_u;
    double value_v;
    float value_w;
    int size;

    constexpr int get_x_squared_plus_a(int a) const
    {
        return value_x * value_x + a;
    }
    constexpr long get_y_squared_plus_a(long a) const
    {
        return value_y * value_y + a;
    }
    constexpr int get_z_plus_a(int a) const
    {
        return value_z + a;
    }
    constexpr double get_u_plus_a(double a) const
    {
        return value_u + a;
    }
    constexpr double get_v_squared_plus_a(double a) const
    {
        return value_v * value_v + a;
    }
    constexpr float get_w_squared_plus_a(float a) const
    {
        return value_w * value_w + a;
    }
};

TEST(ClassLookupTableByName, VariantNSDM)
{
    constexpr auto table_value = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(foo_1_t, "value_*");

    using value_type = decltype(table_value)::value_type;
    constexpr auto value_type_name = display_string_of(dealias(^^value_type));
    EXPECT_THAT(value_type_name, testing::HasSubstr("meta_variant"));
    EXPECT_THAT(value_type_name, testing::HasSubstr("int"));
    EXPECT_THAT(value_type_name, testing::HasSubstr("long"));
    EXPECT_THAT(value_type_name, testing::HasSubstr("float"));
    EXPECT_THAT(value_type_name, testing::HasSubstr("double"));

    static_assert(table_value.size() == 6);

    auto foo = foo_1_t{1, 2, 3, -1.5, -2.5, -3.5, 4};
    auto visitor = [&foo](auto mptr) {
        if (mptr == nullptr) {
            return std::string{"(nullptr)"};
        } else {
            auto value = foo.*mptr;
            constexpr auto type_name = display_string_of(^^decltype(value));
            return std::format("({}){}", type_name, value);
        }
    };

    EXPECT_EQ("(int)1", table_value["x"].visit(visitor));
    EXPECT_THAT(table_value["y"].visit(visitor), testing::AnyOf("(long)2", "(long int)2"));
    EXPECT_THAT(table_value["z"].visit(visitor), testing::AnyOf("(long)3", "(long int)3"));
    EXPECT_EQ("(double)-1.5", table_value["u"].visit(visitor));
    EXPECT_EQ("(double)-2.5", table_value["v"].visit(visitor));
    EXPECT_EQ("(float)-3.5", table_value["w"].visit(visitor));

    EXPECT_EQ_STATIC(std::nullopt, table_value.find("a"));
    EXPECT_EQ_STATIC(std::nullopt, table_value.find("size"));
    EXPECT_EQ_STATIC(std::nullopt, table_value.find("value_x"));
    EXPECT_EQ("(nullptr)", table_value[""].visit(visitor));
}

TEST(ClassLookupTableByName, VariantMethod)
{
    constexpr auto table_fn =
        RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(foo_1_t, "get_*_squared_plus_a");
    static_assert(table_fn.size() == 4);

    static constexpr auto foo = foo_1_t{1, 2, 3, -1.5, -2.5, -3.5, 4};
    constexpr auto visitor = [](auto mptr) constexpr -> double {
        if (mptr == nullptr) {
            return -1234.5;
        } else {
            return std::invoke(mptr, foo, 100);
        }
    };

    EXPECT_EQ_STATIC(101.0, table_fn["x"].visit(visitor));
    EXPECT_EQ_STATIC(104.0, table_fn["y"].visit(visitor));
    EXPECT_EQ_STATIC(106.25, table_fn["v"].visit(visitor));
    EXPECT_EQ_STATIC(112.25, table_fn["w"].visit(visitor));

    EXPECT_EQ_STATIC(std::nullopt, table_fn.find("z"));
    EXPECT_EQ_STATIC(std::nullopt, table_fn.find("get_x_squared_plus_a"));
    EXPECT_EQ_STATIC(std::nullopt, table_fn.find("get_z_plus_a"));
    EXPECT_EQ_STATIC(-1234.5, table_fn[""].visit(visitor));
}
