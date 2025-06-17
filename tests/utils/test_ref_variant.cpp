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
#include <rbox/utils/ref_variant.hpp>

#include "tests/test_options.hpp"

template <class T, class U>
    requires (std::is_same_v<T, U>)
constexpr auto equals_as_same_type(const T& t, const U& u)
{
    return t == u;
}

TEST(UtilsRefVariant, Basic)
{
    static constexpr auto x = 42;
    static constexpr auto y = 1.25;
    static constexpr auto z1 = 42L;
    static constexpr auto z2 = 42L;

    constexpr auto u = rbox::ref_variant<const int, const double>(x);
    static_assert(u.index() == 0);
    static_assert(u.holds_alternative<int>());
    static_assert(!u.holds_alternative<double>());
    static_assert(u.get<int>() == 42);
    static_assert(*u.get_if<const int>() == 42);
    static_assert(u.get<0>() == 42);
    static_assert(*u.get_if<0>() == 42);

    static_assert(holds_alternative<int>(u));
    static_assert(!holds_alternative<double>(u));
    static_assert(get<int>(u) == 42);
    static_assert(get<0>(u) == 42);
    static_assert(*get_if<const int>(u) == 42);
    static_assert(*get_if<0>(u) == 42);

    constexpr auto v = rbox::ref_variant<const int, const double>(y);
    static_assert(v.index() == 1);
    static_assert(!v.holds_alternative<int>());
    static_assert(v.holds_alternative<double>());
    static_assert(v.get<const double>() == 1.25);
    static_assert(*v.get_if<double>() == 1.25);
    static_assert(v.get<1>() == 1.25);
    static_assert(*v.get_if<1>() == 1.25);

    static_assert(holds_alternative<double>(v));
    static_assert(!holds_alternative<int>(v));
    static_assert(get<double>(v) == 1.25);
    static_assert(*get_if<double>(v) == 1.25);
    static_assert(get<1>(v) == 1.25);
    static_assert(*get_if<1>(v) == 1.25);

    constexpr auto visitor = [](rbox::arithmetic_type auto value) {
        return static_cast<int>(value + 1.75);
    };
    static_assert(u.visit(visitor) == 43);
    static_assert(v.visit(visitor) == 3);
    static_assert(visit(visitor, u) == 43);
    static_assert(visit(visitor, v) == 3);

    constexpr auto w1 = rbox::cref_variant<int, long>(z1);
    constexpr auto w2 = rbox::cref_variant<int, long>(z2);

    static_assert(u != v);
    static_assert(equals_as_same_type((u <=> v), std::partial_ordering::less));

    static_assert(u == u);
    static_assert(equals_as_same_type((u <=> u), std::partial_ordering::equivalent));

    static_assert(v == v);
    static_assert(equals_as_same_type((v <=> v), std::partial_ordering::equivalent));

    static_assert(w1 == w1);
    static_assert(equals_as_same_type((w1 <=> w1), std::strong_ordering::equal));
    // Same referenced value (despite of different memory address)
    static_assert(w1 == w2);
    static_assert(equals_as_same_type((w1 <=> w1), std::strong_ordering::equal));
}

TEST(UtilsRefVariant, ShallowConstness)
{
    auto x = 42;
    auto y = 1.25;
    auto z = -1.25;

    const auto u = rbox::ref_variant<int, double>(y);
    u.get<double>() += 1.75;
    EXPECT_EQ(3.0, y);

    auto v = rbox::ref_variant<int, double>(z);
    v = u;  // Rebinding reference only
    v.get<double>() += 10.5;
    EXPECT_EQ(-1.25, z);
    EXPECT_EQ(13.5, y);
}

TEST(UtilsRefVariant, CVQualifiers)
{
    auto x = 9;
    const auto cx = 19;
    volatile auto vx = 29;
    const volatile auto cvx = 39;

    auto v = rbox::ref_variant<int, const int, volatile int, const volatile int>(x);
    EXPECT_TRUE(holds_alternative<int>(v));
    EXPECT_EQ(9, get<int>(v));
    v.rebind(cx);
    EXPECT_TRUE(holds_alternative<const int>(v));
    EXPECT_EQ(19, get<const int>(v));
    v.rebind(vx);
    EXPECT_TRUE(holds_alternative<volatile int>(v));
    EXPECT_EQ(29, get<volatile int>(v));
    v.rebind(cvx);
    EXPECT_TRUE(holds_alternative<const volatile int>(v));
    EXPECT_EQ(39, get<const volatile int>(v));

    auto w = rbox::ref_variant<const volatile int, const int, volatile int, int>(x);
    EXPECT_TRUE(holds_alternative<int>(w));
    EXPECT_EQ(9, get<int>(w));
    w.rebind(cx);
    EXPECT_TRUE(holds_alternative<const int>(w));
    EXPECT_EQ(19, get<const int>(w));
    w.rebind(vx);
    EXPECT_TRUE(holds_alternative<volatile int>(w));
    EXPECT_EQ(29, get<volatile int>(w));
    w.rebind(cvx);
    EXPECT_TRUE(holds_alternative<const volatile int>(w));
    EXPECT_EQ(39, get<const volatile int>(w));
}

TEST(UtilsRefVariant, Polymorphism)
{
    struct animal_t {
        int tag = 0;
        virtual auto name() const -> std::string_view
        {
            return "animal";
        }
        virtual ~animal_t() = default;
        auto operator==(const animal_t&) const -> bool = default;
        auto operator<=>(const animal_t&) const = default;
    };

    struct cat_t : animal_t {
        double purr_db = 42.0;
        auto name() const -> std::string_view override
        {
            return "cat";
        }
        auto operator==(const cat_t&) const -> bool = default;
        auto operator<=>(const cat_t&) const = default;
    };

    struct dog_t : animal_t {
        int tail_wags = 999;
        auto name() const -> std::string_view override
        {
            return "dog";
        }
        auto operator==(const dog_t&) const -> bool = default;
        auto operator<=>(const dog_t&) const = default;
    };

    auto a = animal_t{};
    auto c = cat_t{};
    auto d = dog_t{};

    auto visitor = [](const animal_t& a) -> std::string_view {
        return a.name();
    };

    // Store base type
    auto v1 = rbox::ref_variant<animal_t, cat_t, dog_t>(a);
    EXPECT_TRUE(v1.holds_alternative<animal_t>());
    EXPECT_FALSE(v1.holds_alternative<cat_t>());
    EXPECT_FALSE(v1.holds_alternative<dog_t>());
    EXPECT_EQ(0, v1.index());
    EXPECT_EQ("animal", visit(visitor, v1));

    // Store derived type via base reference (implicit conversion)
    auto v2 = rbox::ref_variant<animal_t, cat_t, dog_t>(c);
    EXPECT_TRUE(v2.holds_alternative<cat_t>());
    EXPECT_FALSE(v2.holds_alternative<animal_t>());
    EXPECT_FALSE(v2.holds_alternative<dog_t>());
    EXPECT_EQ(1, v2.index());
    EXPECT_EQ("cat", visit(visitor, v2));
    EXPECT_EQ(42.0, v2.get<cat_t>().purr_db);

    // Store dog
    auto v3 = rbox::ref_variant<animal_t, cat_t, dog_t>(d);
    EXPECT_TRUE(v3.holds_alternative<dog_t>());
    EXPECT_EQ(2, v3.index());
    EXPECT_EQ("dog", visit(visitor, v3));
    EXPECT_EQ(999, v3.get<dog_t>().tail_wags);

    // Rebinding between base and derived
    v1.rebind(c);
    EXPECT_TRUE(v1.holds_alternative<cat_t>());
    EXPECT_EQ("cat", v1.visit(visitor));

    v1.rebind(a);
    EXPECT_TRUE(v1.holds_alternative<animal_t>());
    EXPECT_EQ("animal", v1.visit(visitor));

    // Comparison: same alternative with different referenced values
    auto c2 = cat_t{};
    c2.purr_db = 99.0;
    auto v4 = rbox::ref_variant<animal_t, cat_t, dog_t>(c2);
    EXPECT_EQ(v2, v2);
    EXPECT_NE(v2, v4);  // Same alternative, but purr_db differs (42.0 vs 99.0)

    // Different alternatives → compare by index
    EXPECT_LT(v1, v2);  // animal_t(index 0) < cat_t(index 1)
    EXPECT_LT(v2, v3);  // cat_t(index 1) < dog_t(index 2)
}

TEST(UtilsRefVariant, PolymorphismAccessViaBasePointer)
{
    struct base_t {
        int value = 10;
        virtual int id() const
        {
            return value;
        }
        virtual ~base_t() = default;
        auto operator==(const base_t&) const -> bool = default;
        auto operator<=>(const base_t&) const = default;
    };

    struct derived_t : base_t {
        int extra = 20;
        int id() const override
        {
            return value + extra;
        }
    };

    auto b = base_t{};
    auto d = derived_t{};

    auto v = rbox::ref_variant<base_t, derived_t>(d);
    EXPECT_TRUE(v.holds_alternative<derived_t>());
    EXPECT_EQ(30, v.get<derived_t>().id());

    // Access via std::get<base_t> is not directly allowed since the stored
    // alternative is derived, but you can access derived data via the visitor.
    // When binding a derived_t, you access it as derived_t.
    // When binding a base_t, you access it as base_t.
    v.rebind(b);
    EXPECT_TRUE(v.holds_alternative<base_t>());
    EXPECT_EQ(10, v.get<base_t>().id());
}

// -------- optional_ref_variant --------

TEST(UtilsOptionalRefVariant, DefaultConstruction)
{
    auto v = rbox::optional_ref_variant<int, double>{};
    EXPECT_TRUE(v.valueless());
    EXPECT_EQ(rbox::npos, v.index());
    EXPECT_FALSE(v.holds_alternative<int>());
    EXPECT_FALSE(v.holds_alternative<double>());
    EXPECT_EQ(nullptr, v.get_if<int>());
    EXPECT_EQ(nullptr, v.get_if<double>());
    EXPECT_EQ(nullptr, v.get_if<0>());
    EXPECT_EQ(nullptr, v.get_if<1>());
}

TEST(UtilsOptionalRefVariant, ValueConstruction)
{
    auto x = 42;
    auto v = rbox::optional_ref_variant<int, double>(x);
    EXPECT_FALSE(v.valueless());
    EXPECT_EQ(0, v.index());
    EXPECT_TRUE(v.holds_alternative<int>());
    EXPECT_FALSE(v.holds_alternative<double>());
    EXPECT_EQ(42, v.get<int>());
    EXPECT_EQ(42, v.get<0>());
    EXPECT_EQ(42, *v.get_if<int>());
    EXPECT_EQ(nullptr, v.get_if<double>());
}

TEST(UtilsOptionalRefVariant, ConstructionSecondAlternative)
{
    auto y = 1.25;
    auto v = rbox::optional_ref_variant<int, double>(y);
    EXPECT_TRUE(!v.valueless());
    EXPECT_EQ(1, v.index());
    EXPECT_TRUE(v.holds_alternative<double>());
    EXPECT_EQ(1.25, v.get<double>());
}

TEST(UtilsOptionalRefVariant, FreeFunctions)
{
    auto x = 42;
    auto y = 1.25;
    auto v1 = rbox::optional_ref_variant<int, double>(x);
    EXPECT_TRUE(holds_alternative<int>(v1));
    EXPECT_EQ(42, get<int>(v1));
    EXPECT_EQ(42, get<0>(v1));
    EXPECT_NE(nullptr, get_if<int>(v1));
    EXPECT_EQ(nullptr, get_if<double>(v1));
    EXPECT_NE(nullptr, get_if<0>(v1));
    EXPECT_EQ(nullptr, get_if<1>(v1));

    auto v2 = rbox::optional_ref_variant<int, double>(y);
    EXPECT_TRUE(holds_alternative<double>(v2));
    EXPECT_EQ(1.25, get<double>(v2));

    // Free get_if on valueless
    auto v3 = rbox::optional_ref_variant<int, double>{};
    EXPECT_EQ(nullptr, get_if<int>(v3));
    EXPECT_EQ(nullptr, get_if<0>(v3));
}

TEST(UtilsOptionalRefVariant, Visitation)
{
    auto x = 42;
    auto y = 1.25;

    auto v1 = rbox::optional_ref_variant<int, double>(x);
    auto r1 = v1.try_visit([](auto& val) {
        return static_cast<int>(val + 1.75);
    });
    EXPECT_TRUE(r1.has_value());
    EXPECT_EQ(43, *r1);

    auto v2 = rbox::optional_ref_variant<int, double>(y);
    auto r2 = v2.try_visit([](auto& val) {
        return static_cast<int>(val + 1.75);
    });
    EXPECT_TRUE(r2.has_value());
    EXPECT_EQ(3, *r2);

    // Free function visit
    auto r1f = try_visit(
        [](auto& val) {
            return val;
        },
        v1);
    EXPECT_TRUE(r1f.has_value());
    EXPECT_EQ(42, *r1f);

    // Valuless visit returns nullopt
    auto v3 = rbox::optional_ref_variant<int, double>{};
    auto r3 = v3.try_visit([](const auto& val) {
        return val;
    });
    EXPECT_EQ(std::nullopt, r3);
}

TEST(UtilsOptionalRefVariant, Rebinding)
{
    auto x = 42;
    auto y = 1.25;

    auto v = rbox::optional_ref_variant<int, double>(x);
    EXPECT_EQ(42, v.get<int>());

    v.rebind(y);
    EXPECT_TRUE(v.holds_alternative<double>());
    EXPECT_EQ(1, v.index());
    EXPECT_EQ(1.25, v.get<double>());

    // Mutate through the reference
    v.get<double>() += 10.0;
    EXPECT_EQ(11.25, y);

    // Rebind back
    v.rebind(x);
    EXPECT_TRUE(v.holds_alternative<int>());
    EXPECT_EQ(42, v.get<int>());
}

TEST(UtilsOptionalRefVariant, Reset)
{
    auto x = 42;
    auto v = rbox::optional_ref_variant<int, double>(x);
    EXPECT_TRUE(!v.valueless());

    v.reset();
    EXPECT_TRUE(v.valueless());
    EXPECT_EQ(rbox::npos, v.index());
    EXPECT_EQ(nullptr, v.get_if<int>());
    EXPECT_EQ(nullptr, v.get_if<double>());

    // Can rebind after reset
    v.rebind(x);
    EXPECT_TRUE(!v.valueless());
    EXPECT_EQ(42, v.get<int>());
}

TEST(UtilsOptionalRefVariant, ShallowConstness)
{
    auto x = 42;
    auto y = 1.25;

    const auto v = rbox::optional_ref_variant<int, double>(y);
    v.get<double>() += 1.75;
    EXPECT_EQ(3.0, y);
    EXPECT_EQ(1, v.index());
    EXPECT_TRUE(v.holds_alternative<double>());

    // Rebinding (not affected by constness — rebind is always non-const)
    const auto u = v;
    EXPECT_EQ(1, u.index());
    EXPECT_TRUE(u.holds_alternative<double>());
    EXPECT_EQ(3.0, u.get<double>());
}

TEST(UtilsOptionalRefVariant, Comparison)
{
    auto x1 = 42;
    auto x2 = 42;
    auto y = 1.25;

    auto v1 = rbox::optional_ref_variant<int, double>(x1);
    auto v2 = rbox::optional_ref_variant<int, double>(x2);
    auto v3 = rbox::optional_ref_variant<int, double>(y);
    auto v_empty = rbox::optional_ref_variant<int, double>{};

    // Same alternative, same value
    EXPECT_EQ(v1, v2);

    // Same alternative, different value
    x1 = 99;
    EXPECT_NE(v1, v2);

    // Different alternatives
    EXPECT_NE(v1, v3);
    EXPECT_NE(v2, v3);

    // Comparison with valueless
    EXPECT_NE(v1, v_empty);
    EXPECT_EQ(v_empty, v_empty);
    EXPECT_FALSE(v1 == v_empty);
}

TEST(UtilsOptionalRefVariant, DefaultConstructedCopied)
{
    auto v1 = rbox::optional_ref_variant<int, double>{};
    auto v2 = v1;
    EXPECT_TRUE(v1.valueless());
    EXPECT_TRUE(v2.valueless());
    EXPECT_EQ(v1, v2);
    EXPECT_EQ(v2, v1);
}

TEST(UtilsOptionalRefVariant, CopyAndAssignment)
{
    auto x = 42;
    auto y = 1.25;

    auto v1 = rbox::optional_ref_variant<int, double>(x);
    auto v2 = v1;  // Copy construction
    EXPECT_EQ(v1, v2);
    EXPECT_EQ(42, v2.get<int>());

    // Copy assignment
    auto v3 = rbox::optional_ref_variant<int, double>(y);
    v3 = v1;
    EXPECT_EQ(v1, v3);
    EXPECT_EQ(42, v3.get<int>());

    // Self-assignment
    v1 = v1;
    EXPECT_EQ(42, v1.get<int>());
}

TEST(UtilsOptionalRefVariant, PolymorphismSupport)
{
    struct animal_t {
        int id = 0;
        virtual auto kind() const -> std::string_view
        {
            return "animal";
        }
        virtual ~animal_t() = default;
        auto operator==(const animal_t&) const -> bool = default;
    };

    struct cat_t : animal_t {
        auto kind() const -> std::string_view override
        {
            return "cat";
        }
    };

    auto a = animal_t{};
    a.id = 1;
    auto c = cat_t{};
    c.id = 2;

    auto v = rbox::optional_ref_variant<animal_t, cat_t>(c);
    EXPECT_TRUE(v.holds_alternative<cat_t>());

    // Default-constructed, then rebind
    auto v2 = rbox::optional_ref_variant<animal_t, cat_t>{};
    v2.rebind(a);
    EXPECT_TRUE(v2.holds_alternative<animal_t>());

    auto visitor = [](const animal_t& a) -> std::string_view {
        return a.kind();
    };
    EXPECT_EQ("cat", *v.try_visit(visitor));
    EXPECT_EQ("animal", *v2.try_visit(visitor));

    // Valuless visit
    v2.reset();
    auto r = v2.try_visit([](const auto&) {
        return 42;
    });
    EXPECT_FALSE(r.has_value());
}

TEST(UtilsOptionalRefVariant, MultipleAlternatives)
{
    auto a = 42;
    auto b = 1.25;
    auto c = 42L;
    auto d = 'X';

    auto v = rbox::optional_ref_variant<int, double, long, char>(a);
    EXPECT_EQ(0, v.index());
    EXPECT_EQ(42, v.get<int>());

    v.rebind(b);
    EXPECT_EQ(1, v.index());
    EXPECT_EQ(1.25, v.get<double>());

    v.rebind(c);
    EXPECT_EQ(2, v.index());
    EXPECT_EQ(42L, v.get<long>());

    v.rebind(d);
    EXPECT_EQ(3, v.index());
    EXPECT_EQ('X', v.get<char>());

    v.reset();
    EXPECT_TRUE(v.valueless());
}
