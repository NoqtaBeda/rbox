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

#include <cstdio>
#include <numeric>
#include <print>
#include <ranges>
#include <rbox/to_static_storage.hpp>

#include "tests/test_options.hpp"

constexpr auto my_to_string(std::integral auto value) -> std::string
{
    char buffer[72];
    const char* pos = std::to_chars(buffer, buffer + 72, value).ptr;
    return std::string{buffer, static_cast<size_t>(pos - buffer)};
}

// Examples in docs/to_static_storage.md
namespace examples {
// (1) Range (including initializer list) to meta_span
constexpr auto span_1 = rbox::to_static_storage(std::vector<int>{1, 2, 3, 4, 5});
static_assert(std::is_same_v<std::remove_const_t<decltype(span_1)>, rbox::meta_span<int>>);

constexpr auto span_2 = rbox::to_static_storage({"Cat", "Dog", "Rabbit"});
static_assert(std::is_same_v<decltype(span_2), const rbox::meta_span<rbox::meta_string_view>>);

// (2) Tuple-like to structural tuple alternative (meta_pair, meta_triple, meta_tuple)
constexpr auto meta_tup =
    rbox::to_static_storage(std::tuple<int, double, std::u16string>{42, 3.14, u"Hello"});
static_assert(std::is_same_v<
              std::remove_const_t<decltype(meta_tup)>,
              rbox::meta_triple<int, double, rbox::meta_u16string_view>>);

// (3) Const char pointer to meta_string_view
constexpr auto cstr = rbox::to_static_storage("world");
static_assert(std::is_same_v<std::remove_const_t<decltype(cstr)>, rbox::meta_string_view>);

// (4) Identity
struct point_t {
    int x;
    int y;
};
constexpr auto val = rbox::to_static_storage(point_t{.x = 12, .y = 34});
static_assert(std::is_same_v<decltype(val), const point_t>);

// (5) Function to function pointer
using F = void(int, double);
static_assert(std::is_same_v<rbox::to_static_storage_result_t<F>, void (*)(int, double)>);

// (6) Pointer-to-member (identity)
constexpr auto mem_ptr = rbox::to_static_storage(&point_t::x);
static_assert(std::is_same_v<decltype(mem_ptr), int point_t::* const>);

// (7) Variant-like to meta_variant
constexpr auto var_val = rbox::to_static_storage(std::variant<int, std::string>(42));
static_assert(std::is_same_v<
              std::remove_const_t<decltype(var_val)>,
              rbox::meta_variant<int, rbox::meta_string_view>>);

// (8) Flattenable class to structural_mirror_t
struct widget_t {
    std::string name;
    std::vector<int> sizes;
};
constexpr auto mirrored = rbox::to_static_storage(widget_t{"gear", {3, 5, 7}});
static_assert(std::is_same_v<
              rbox::meta_string_view,  // std::string → meta_string_view
              decltype(mirrored.name)>);
static_assert(std::is_same_v<
              rbox::meta_span<int>,  // std::vector<int> → meta_span<int>
              decltype(mirrored.sizes)>);

// (9) Using to_static_storage_result_t
static_assert(
    std::is_same_v<rbox::to_static_storage_result_t<std::vector<int>>, rbox::meta_span<int>>);
}  // namespace examples

struct some_struct_t {
    const char* name;
    int x;
    int y;

    constexpr auto operator<=>(const some_struct_t& rhs) const -> std::weak_ordering = default;
};
constexpr auto some_struct_value = some_struct_t{.name = "hello", .x = 123, .y = 456};

constexpr auto fib_until(unsigned x) -> std::vector<unsigned>
{
    auto res = std::vector<unsigned>{1u, 2u};
    while (true) {
        auto n = res.size();
        auto next = res[n - 1] + res[n - 2];
        if (next > x) {
            break;
        }
        res.push_back(next);
    }
    return res;
}

constexpr auto fib_str_until(unsigned x) -> std::vector<std::string>
{
    auto values = std::vector<unsigned>{1u, 2u};
    while (true) {
        auto n = values.size();
        auto next = values[n - 1] + values[n - 2];
        if (next > x) {
            break;
        }
        values.push_back(next);
    }
    auto res = std::vector<std::string>{};
    for (auto i = 0zU, n = values.size(); i < n; i++) {
        res.push_back("[" + my_to_string(i) + "]=" + my_to_string(values[i]));
    }
    return res;
}

// std::tuple<long, std::vector<long>, std::wstring, std::vector<std::wstring>>
constexpr auto make_ds_tuple_1(unsigned n)
{
    auto to_wstring = [](auto value) {
        auto str = my_to_string(value);
        auto wstr = std::wstring{};
        wstr.assign_range(str);
        return wstr;
    };
    auto long_values = std::vector<long>{};
    for (auto i = 0u; i < n; i++) {
        long_values.push_back(i * i);
    }
    auto sum = std::reduce(long_values.begin(), long_values.end());
    auto wstring_values = std::vector<std::wstring>{};
    for (auto i = 0u; i < n; i++) {
        wstring_values.push_back(to_wstring(i * i));
    }
    auto sum_wstr = to_wstring(sum);
    return std::tuple{sum, std::move(long_values), std::move(sum_wstr), std::move(wstring_values)};
}

// Nested tuple:
//   std::tuple<long, std::vector<long>, std::wstring, std::vector<std::wstring>,
//              std::tuple<...>>
template <unsigned N>
constexpr auto make_ds_tuple_2()
{
    if constexpr (N == 0) {
        return make_ds_tuple_1(10);
    } else {
        auto next = make_ds_tuple_2<N - 1>();
        return std::tuple_cat(make_ds_tuple_1(10 - N), std::tuple<decltype(next)>{std::move(next)});
    }
}

// std::vector<std::tuple<...>>
constexpr auto make_ds_tuple_list(unsigned n)
{
    return std::views::iota(1u, n) | std::views::transform(make_ds_tuple_1)
         | std::ranges::to<std::vector>();
}

// Expected result type aliases (used across Tuple and ResultT tests)
using ds_tuple_1_conv_expected_type = rbox::meta_tuple<
    long,
    rbox::meta_span<long>,
    rbox::meta_wstring_view,
    rbox::meta_span<rbox::meta_wstring_view>>;

using ds_tuple_2_conv_expected_type = rbox::meta_tuple<
    long,
    rbox::meta_span<long>,
    rbox::meta_wstring_view,
    rbox::meta_span<rbox::meta_wstring_view>,
    rbox::meta_tuple<
        long,
        rbox::meta_span<long>,
        rbox::meta_wstring_view,
        rbox::meta_span<rbox::meta_wstring_view>,
        rbox::meta_tuple<
            long,
            rbox::meta_span<long>,
            rbox::meta_wstring_view,
            rbox::meta_span<rbox::meta_wstring_view>>>>;

TEST(ToStaticStorage, Identity)
{
    // -- arithmetic --
    constexpr auto some_int_conv = rbox::to_static_storage(42);
    static_assert(std::is_same_v<decltype(some_int_conv), const int>);
    static_assert(some_int_conv == 42);

    constexpr auto some_double_conv = rbox::to_static_storage(4.25);
    static_assert(std::is_same_v<decltype(some_double_conv), const double>);
    static_assert(some_double_conv == 4.25);

    // -- enum --
    constexpr auto errc_conv = rbox::to_static_storage(std::errc::connection_reset);
    static_assert(std::is_same_v<decltype(errc_conv), const std::errc>);
    static_assert(errc_conv == std::errc::connection_reset);

    // -- struct --
    constexpr auto struct_conv = rbox::to_static_storage(some_struct_value);
    static_assert(std::is_same_v<decltype(struct_conv), const some_struct_t>);
    static_assert(struct_conv == some_struct_value);

    // -- lambda without capture --
    constexpr auto lambda = [](const char* str) {
        std::println("{}", str);
    };
    constexpr auto lambda_conv = rbox::to_static_storage(lambda);
    static_assert(std::is_same_v<decltype(lambda_conv), decltype(lambda)>);
    static_assert(lambda_conv == lambda);
}

TEST(ToStaticStorage, Pointers)
{
    static constexpr auto some_int = 42;

    // -- raw pointer --
    constexpr auto int_ptr_conv = rbox::to_static_storage(&some_int);
    static_assert(std::is_same_v<decltype(int_ptr_conv), const int* const>);
    static_assert(*int_ptr_conv == some_int);

    // -- function pointer (explicit) --
    constexpr auto printf_fptr = printf;
    constexpr auto printf_fptr_conv = rbox::to_static_storage(printf_fptr);
    static_assert(std::is_same_v<decltype(printf_fptr_conv), int (*const)(const char*, ...)>);
    static_assert(printf_fptr_conv == printf);

    // -- function pointer (implicit conversion) --
    constexpr auto printf_conv = rbox::to_static_storage(printf);
    static_assert(std::is_same_v<decltype(printf_conv), int (*const)(const char*, ...)>);
    static_assert(printf_conv == printf);

    // -- nullptr --
    constexpr auto nullptr_conv = rbox::to_static_storage(nullptr);
    static_assert(std::is_same_v<decltype(nullptr_conv), const nullptr_t>);
    static_assert(nullptr_conv == nullptr);
}

TEST(ToStaticStorage, PointerToMember)
{
    // -- member object pointer --
    constexpr auto mem_obj_ptr_conv = rbox::to_static_storage(&some_struct_t::x);
    static_assert(std::is_same_v<decltype(mem_obj_ptr_conv), int some_struct_t::* const>);
    static_assert(some_struct_value.*mem_obj_ptr_conv == 123);

    // -- member function pointer --
    constexpr auto mem_fn_ptr_conv = rbox::to_static_storage(&some_struct_t::operator<=>);
    static_assert(
        std::is_same_v<
            decltype(mem_fn_ptr_conv),
            std::weak_ordering (some_struct_t::* const)(const some_struct_t&) const noexcept>);
    static_assert(
        (some_struct_value.*mem_fn_ptr_conv)(some_struct_value) == std::weak_ordering::equivalent);
}

TEST(ToStaticStorage, StringLike)
{
    // -- char16_t pointer → meta_u16string_view --
    constexpr const auto* u16_literal = u"你好，世界";
    constexpr auto u16_literal_conv = rbox::to_static_storage(u16_literal);
    static_assert(std::is_same_v<decltype(u16_literal_conv), const rbox::meta_u16string_view>);
    static_assert(u16_literal_conv.size() == 5);
    static_assert(u16_literal_conv[4] == u'界');
    static_assert(u16_literal_conv.head[u16_literal_conv.n] == '\0');

    // -- char array → meta_string_view --
    constexpr char char_array[] = {'W', 'e', 'l', 'c', 'o', 'm', 'e'};
    constexpr auto char_array_conv = rbox::to_static_storage(char_array);
    static_assert(std::is_same_v<decltype(char_array_conv), const rbox::meta_string_view>);
    static_assert(char_array_conv.size() == 7);
    static_assert(char_array_conv[4] == 'o');
    static_assert(char_array_conv.head[char_array_conv.n] == '\0');

    // -- char initializer_list → meta_string_view --
    constexpr auto char_il_conv = rbox::to_static_storage({'x', 'y', 'z', 'a', 'b', 'c'});
    static_assert(std::is_same_v<decltype(char_il_conv), const rbox::meta_string_view>);
    static_assert(char_il_conv == "xyzabc");
}

TEST(ToStaticStorage, Range)
{
    // -- C array → meta_span --
    constexpr long long_array[] = {1, 2, 3, 4, 6, 8};
    constexpr auto long_array_conv = rbox::to_static_storage(long_array);
    static_assert(std::is_same_v<decltype(long_array_conv), const rbox::meta_span<long>>);
    static_assert(long_array_conv.size() == 6);
    static_assert(long_array_conv[3] == 4);

    // -- initializer_list → meta_span --
    constexpr auto float_il_conv = rbox::to_static_storage({1.0f, 2.5f, 3.75f, 5.875f});
    static_assert(std::is_same_v<decltype(float_il_conv), const rbox::meta_span<float>>);
    static_assert(float_il_conv.size() == 4);
    static_assert(float_il_conv[2] == 3.75f);

    // -- std::vector → meta_span --
    constexpr auto fib_100_conv = rbox::to_static_storage(fib_until(100));
    static_assert(std::is_same_v<decltype(fib_100_conv), const rbox::meta_span<unsigned>>);
    // Fibonacci: 1, 2, 3, 5, 8, 13, 21, 34, 55, 89
    static_assert(fib_100_conv.size() == 10);
    static_assert(fib_100_conv.back() == 89);
    static_assert(std::reduce(fib_100_conv.begin(), fib_100_conv.end()) == 231);

    // -- std::vector<std::string> → meta_span<meta_string_view> --
    constexpr auto fib_str_100_conv = rbox::to_static_storage(fib_str_until(100));
    static_assert(
        std::is_same_v<decltype(fib_str_100_conv), const rbox::meta_span<rbox::meta_string_view>>);
    static_assert(fib_str_100_conv.size() == 10);
    static_assert(fib_str_100_conv.back() == "[9]=89");
}

TEST(ToStaticStorage, TupleLike)
{
    // -- std::tuple → meta_tuple --
    constexpr auto ds_tuple_1_conv = rbox::to_static_storage(make_ds_tuple_1(10));
    static_assert(std::is_same_v<decltype(ds_tuple_1_conv), const ds_tuple_1_conv_expected_type>);
    static_assert(get<0>(ds_tuple_1_conv) == 285);
    static_assert(get<1>(ds_tuple_1_conv).size() == 10);
    static_assert(get<1>(ds_tuple_1_conv)[9] == 81);
    static_assert(get<2>(ds_tuple_1_conv) == L"285");
    static_assert(get<3>(ds_tuple_1_conv).size() == 10);
    static_assert(get<3>(ds_tuple_1_conv)[8] == L"64");

    // -- nested std::tuple → nested meta_tuple --
    constexpr auto ds_tuple_2_conv = rbox::to_static_storage(make_ds_tuple_2<2>());
    static_assert(std::is_same_v<decltype(ds_tuple_2_conv), const ds_tuple_2_conv_expected_type>);
    static_assert(get<2>(ds_tuple_2_conv) == L"140");
    static_assert(get<0>(get<4>(ds_tuple_2_conv)) == 204);
    static_assert(get<2>(get<4>(get<4>(ds_tuple_2_conv))) == L"285");

    // -- std::vector<std::tuple<...>> → meta_span<meta_tuple<...>> --
    constexpr auto ds_tuple_list_conv = rbox::to_static_storage(make_ds_tuple_list(10));
    static_assert(std::is_same_v<
                  decltype(ds_tuple_list_conv),
                  const rbox::meta_span<ds_tuple_1_conv_expected_type>>);
    static_assert(ds_tuple_list_conv.size() == 9);
    static_assert(get<2>(ds_tuple_list_conv[7]) == L"140");
    static_assert(
        std::accumulate(
            get<3>(ds_tuple_list_conv[6]).begin(),
            get<3>(ds_tuple_list_conv[6]).end(),
            std::wstring{})
        == L"0149162536");
}

constexpr auto make_variant_string(size_t index) -> std::variant<int, std::string>
{
    if (index == 0) {
        return 84;
    } else {
        return std::string{"hello"};
    }
}

constexpr auto make_variant_vector(size_t index) -> std::variant<std::vector<int>, double>
{
    if (index == 0) {
        return std::vector<int>{1, 2, 3};
    } else {
        return 3.14;
    }
}

constexpr auto make_variant_tuple(size_t index)
    -> std::variant<std::vector<int>, std::tuple<long, std::string>, int>
{
    if (index == 0) {
        return std::vector<int>{10, 20, 30};
    } else if (index == 1) {
        return std::tuple{123L, std::string{"world"}};
    } else {
        return 99;
    }
}

using variant_tuple_expected_type =
    rbox::meta_variant<rbox::meta_span<int>, rbox::meta_pair<long, rbox::meta_string_view>, int>;

TEST(ToStaticStorage, Variant)
{
    // -- std::variant<int, double> → meta_variant<int, double> --
    //    alternative 0: int
    constexpr auto var_id_0 = rbox::to_static_storage(std::variant<int, double>(42));
    static_assert(std::is_same_v<decltype(var_id_0), const rbox::meta_variant<int, double>>);
    static_assert(var_id_0.index() == 0);
    static_assert(var_id_0.holds_alternative<int>());
    static_assert(var_id_0.get<int>() == 42);

    //    alternative 1: double
    constexpr auto var_id_1 = rbox::to_static_storage(std::variant<int, double>(3.14));
    static_assert(std::is_same_v<decltype(var_id_1), const rbox::meta_variant<int, double>>);
    static_assert(var_id_1.index() == 1);
    static_assert(var_id_1.holds_alternative<double>());
    static_assert(var_id_1.get<double>() == 3.14);

    // -- std::variant<int, std::string> → meta_variant<int, meta_string_view> --
    //    alternative 0: int
    constexpr auto var_is_0 = rbox::to_static_storage(make_variant_string(0));
    static_assert(
        std::is_same_v<decltype(var_is_0), const rbox::meta_variant<int, rbox::meta_string_view>>);
    static_assert(var_is_0.index() == 0);
    static_assert(var_is_0.holds_alternative<int>());
    static_assert(var_is_0.get<int>() == 84);

    //    alternative 1: std::string
    constexpr auto var_is_1 = rbox::to_static_storage(make_variant_string(1));
    static_assert(
        std::is_same_v<decltype(var_is_1), const rbox::meta_variant<int, rbox::meta_string_view>>);
    static_assert(var_is_1.index() == 1);
    static_assert(var_is_1.holds_alternative<rbox::meta_string_view>());
    static_assert(var_is_1.get<rbox::meta_string_view>() == "hello");

    // -- std::variant<std::vector<int>, double> → meta_variant<meta_span<int>, double> --
    //    alternative 0: std::vector<int>
    constexpr auto var_vd_0 = rbox::to_static_storage(make_variant_vector(0));
    static_assert(
        std::is_same_v<decltype(var_vd_0), const rbox::meta_variant<rbox::meta_span<int>, double>>);
    static_assert(var_vd_0.index() == 0);
    static_assert(var_vd_0.holds_alternative<rbox::meta_span<int>>());
    static_assert(var_vd_0.get<rbox::meta_span<int>>().size() == 3);
    static_assert(var_vd_0.get<rbox::meta_span<int>>()[1] == 2);

    //    alternative 1: double
    constexpr auto var_vd_1 = rbox::to_static_storage(make_variant_vector(1));
    static_assert(
        std::is_same_v<decltype(var_vd_1), const rbox::meta_variant<rbox::meta_span<int>, double>>);
    static_assert(var_vd_1.index() == 1);
    static_assert(var_vd_1.holds_alternative<double>());
    static_assert(var_vd_1.get<double>() == 3.14);

    // -- std::variant<std::vector<int>, std::tuple<long, std::string>, int>
    //    → meta_variant<meta_span<int>, meta_pair<long, meta_string_view>, int> --
    //    alternative 0: std::vector<int>
    constexpr auto var_vt_0 = rbox::to_static_storage(make_variant_tuple(0));
    static_assert(std::is_same_v<decltype(var_vt_0), const variant_tuple_expected_type>);
    static_assert(var_vt_0.index() == 0);
    static_assert(var_vt_0.holds_alternative<rbox::meta_span<int>>());
    static_assert(var_vt_0.get<rbox::meta_span<int>>().size() == 3);
    static_assert(var_vt_0.get<rbox::meta_span<int>>()[2] == 30);

    //    alternative 1: std::tuple<long, std::string>
    constexpr auto var_vt_1 = rbox::to_static_storage(make_variant_tuple(1));
    static_assert(std::is_same_v<decltype(var_vt_1), const variant_tuple_expected_type>);
    static_assert(var_vt_1.index() == 1);
    static_assert(var_vt_1.holds_alternative<rbox::meta_pair<long, rbox::meta_string_view>>());
    static_assert(var_vt_1.get<1>().first == 123L);
    static_assert(var_vt_1.get<1>().second == "world");

    //    alternative 2: int
    constexpr auto var_vt_2 = rbox::to_static_storage(make_variant_tuple(2));
    static_assert(std::is_same_v<decltype(var_vt_2), const variant_tuple_expected_type>);
    static_assert(var_vt_2.index() == 2);
    static_assert(var_vt_2.holds_alternative<int>());
    static_assert(var_vt_2.get<int>() == 99);

    // -- to_static_storage_result_t for variant --
    static_assert(std::is_same_v<
                  rbox::to_static_storage_result_t<std::variant<int, double>>,
                  rbox::meta_variant<int, double>>);
    static_assert(std::is_same_v<
                  rbox::to_static_storage_result_t<std::variant<int, std::string>>,
                  rbox::meta_variant<int, rbox::meta_string_view>>);
    static_assert(std::is_same_v<
                  rbox::to_static_storage_result_t<
                      std::variant<std::vector<int>, std::tuple<long, std::string>, int>>,
                  variant_tuple_expected_type>);
}

// std::variant<std::variant<int, double>, std::string>
constexpr auto make_nested_variant_id_string(size_t index)
    -> std::variant<std::variant<int, double>, std::string>
{
    if (index == 0) {
        return std::variant<int, double>(42);
    } else if (index == 1) {
        return std::variant<int, double>(3.14);
    } else {
        return std::string{"nested"};
    }
}

using nested_variant_id_string_expected_type =
    rbox::meta_variant<rbox::meta_variant<int, double>, rbox::meta_string_view>;

// std::variant<int, std::variant<std::string, std::vector<int>>>
constexpr auto make_nested_variant_int_variant(size_t index)
    -> std::variant<int, std::variant<std::string, std::vector<int>>>
{
    if (index == 0) {
        return 99;
    } else if (index == 1) {
        return std::variant<std::string, std::vector<int>>(std::string{"world"});
    } else {
        return std::variant<std::string, std::vector<int>>(std::vector<int>{1, 2, 3});
    }
}

using nested_variant_int_variant_expected_type =
    rbox::meta_variant<int, rbox::meta_variant<rbox::meta_string_view, rbox::meta_span<int>>>;

TEST(ToStaticStorage, NestedVariant)
{
    // -- std::variant<std::variant<int, double>, std::string>
    //    → meta_variant<meta_variant<int, double>, meta_string_view> --
    using inner_id = rbox::meta_variant<int, double>;

    //    alternative 0: inner variant holding int
    constexpr auto var_nid_0 = rbox::to_static_storage(make_nested_variant_id_string(0));
    static_assert(
        std::is_same_v<decltype(var_nid_0), const nested_variant_id_string_expected_type>);
    static_assert(var_nid_0.index() == 0);
    static_assert(var_nid_0.holds_alternative<inner_id>());
    static_assert(var_nid_0.get<inner_id>().index() == 0);
    static_assert(var_nid_0.get<inner_id>().holds_alternative<int>());
    static_assert(var_nid_0.get<inner_id>().get<int>() == 42);

    //    alternative 1: inner variant holding double
    constexpr auto var_nid_1 = rbox::to_static_storage(make_nested_variant_id_string(1));
    static_assert(
        std::is_same_v<decltype(var_nid_1), const nested_variant_id_string_expected_type>);
    static_assert(var_nid_1.index() == 0);
    static_assert(var_nid_1.holds_alternative<inner_id>());
    static_assert(var_nid_1.get<inner_id>().index() == 1);
    static_assert(var_nid_1.get<inner_id>().holds_alternative<double>());
    static_assert(var_nid_1.get<inner_id>().get<double>() == 3.14);

    //    alternative 2: string
    constexpr auto var_nid_2 = rbox::to_static_storage(make_nested_variant_id_string(2));
    static_assert(
        std::is_same_v<decltype(var_nid_2), const nested_variant_id_string_expected_type>);
    static_assert(var_nid_2.index() == 1);
    static_assert(var_nid_2.holds_alternative<rbox::meta_string_view>());
    static_assert(var_nid_2.get<rbox::meta_string_view>() == "nested");

    // -- std::variant<int, std::variant<std::string, std::vector<int>>>
    //    → meta_variant<int, meta_variant<meta_string_view, meta_span<int>>> --
    using inner_sv = rbox::meta_variant<rbox::meta_string_view, rbox::meta_span<int>>;

    //    alternative 0: int
    constexpr auto var_niv_0 = rbox::to_static_storage(make_nested_variant_int_variant(0));
    static_assert(
        std::is_same_v<decltype(var_niv_0), const nested_variant_int_variant_expected_type>);
    static_assert(var_niv_0.index() == 0);
    static_assert(var_niv_0.holds_alternative<int>());
    static_assert(var_niv_0.get<int>() == 99);

    //    alternative 1: inner variant holding string
    constexpr auto var_niv_1 = rbox::to_static_storage(make_nested_variant_int_variant(1));
    static_assert(
        std::is_same_v<decltype(var_niv_1), const nested_variant_int_variant_expected_type>);
    static_assert(var_niv_1.index() == 1);
    static_assert(var_niv_1.holds_alternative<inner_sv>());
    static_assert(var_niv_1.get<inner_sv>().index() == 0);
    static_assert(var_niv_1.get<inner_sv>().holds_alternative<rbox::meta_string_view>());
    static_assert(var_niv_1.get<inner_sv>().get<rbox::meta_string_view>() == "world");

    //    alternative 2: inner variant holding vector
    constexpr auto var_niv_2 = rbox::to_static_storage(make_nested_variant_int_variant(2));
    static_assert(
        std::is_same_v<decltype(var_niv_2), const nested_variant_int_variant_expected_type>);
    static_assert(var_niv_2.index() == 1);
    static_assert(var_niv_2.holds_alternative<inner_sv>());
    static_assert(var_niv_2.get<inner_sv>().index() == 1);
    static_assert(var_niv_2.get<inner_sv>().holds_alternative<rbox::meta_span<int>>());
    static_assert(var_niv_2.get<inner_sv>().get<rbox::meta_span<int>>().size() == 3);
    static_assert(var_niv_2.get<inner_sv>().get<rbox::meta_span<int>>()[2] == 3);

    // -- to_static_storage_result_t for nested variants --
    static_assert(
        std::is_same_v<
            rbox::to_static_storage_result_t<std::variant<std::variant<int, double>, std::string>>,
            nested_variant_id_string_expected_type>);
    static_assert(std::is_same_v<
                  rbox::to_static_storage_result_t<
                      std::variant<int, std::variant<std::string, std::vector<int>>>>,
                  nested_variant_int_variant_expected_type>);
}

// Non-trivial terminal types for VariantIsPromotable negative tests:
//   These types are structural (so identity promotion applies) but non-trivial
//   (so meta_variant rejects them as alternatives).

struct non_trivial_terminal_t {
    int x = 0;
    int y = 0;
};
static_assert(std::is_structural_v<non_trivial_terminal_t>);
static_assert(NOT rbox::trivial_type<non_trivial_terminal_t>);

//   A type that is not promotable at all (non-structural due to private member).
class non_structural_terminal_t {
    int x;

public:
    constexpr non_structural_terminal_t() : x(0) {}
    constexpr explicit non_structural_terminal_t(int v) : x(v) {}
};
static_assert(NOT std::is_structural_v<non_structural_terminal_t>);
static_assert(NOT rbox::trivial_type<non_structural_terminal_t>);

TEST(ToStaticStorage, VariantIsPromotable)
{
    // -- scalar alternatives --
    static_assert(rbox::promotable_to_static_storage<std::variant<int, double>>);
    static_assert(rbox::promotable_to_static_storage<std::variant<char, float, long>>);
    static_assert(rbox::promotable_to_static_storage<std::variant<std::errc, int>>);

    // -- string alternatives (promoted to meta_*string_view, which are trivial) --
    static_assert(rbox::promotable_to_static_storage<std::variant<int, std::string>>);
    static_assert(rbox::promotable_to_static_storage<std::variant<std::string, std::wstring>>);
    static_assert(rbox::promotable_to_static_storage<std::variant<std::u16string, std::string>>);

    // -- container alternatives (promoted to meta_span<T>, which is trivial) --
    static_assert(rbox::promotable_to_static_storage<std::variant<std::vector<int>, double>>);
    static_assert(rbox::promotable_to_static_storage<std::variant<std::vector<std::string>, int>>);
    static_assert(
        rbox::promotable_to_static_storage<std::variant<std::vector<int>, std::vector<long>>>);

    // -- tuple alternatives (promoted to meta_pair/meta_tuple, which are trivial) --
    static_assert(rbox::promotable_to_static_storage<std::variant<
                      int,
                      std::pair<double, std::vector<double>>,
                      std::tuple<long, std::vector<long>, std::string, std::vector<std::string>>>>);
    static_assert(rbox::promotable_to_static_storage<std::variant<
                      std::tuple<int, int*, int**>,
                      std::tuple<std::string, std::u8string, std::u16string, std::u32string>>>);

    // -- nested variant alternatives --
    static_assert(rbox::promotable_to_static_storage<
                  std::variant<std::variant<int, std::vector<int>>, std::string>>);
    static_assert(rbox::promotable_to_static_storage<
                  std::variant<int, std::variant<std::string, std::vector<int>>>>);

    // -- negative: terminal type is non-trivial (structural but non-trivial) --
    //   non_trivial_terminal_t → identity promotion → result is non-trivial
    static_assert(
        NOT rbox::promotable_to_static_storage<std::variant<non_trivial_terminal_t, int>>);
    // -- negative: alternative is not promotable at all (non-structural) --
    //   non_structural_terminal_t is not scalar/range/tuple/variant/structural
    static_assert(
        NOT rbox::promotable_to_static_storage<std::variant<non_structural_terminal_t, int>>);

    // -- mixed alternatives --
    using mixed_variant_t = std::variant<
        int,
        std::string,
        std::vector<int>,
        std::tuple<long, std::string>,
        std::variant<int, double>>;
    static_assert(rbox::promotable_to_static_storage<mixed_variant_t>);

    // -- to_static_storage_result_t cross-checks --
    static_assert(std::is_same_v<
                  rbox::to_static_storage_result_t<std::variant<int, double>>,
                  rbox::meta_variant<int, double>>);
    static_assert(std::is_same_v<
                  rbox::to_static_storage_result_t<std::variant<int, std::wstring>>,
                  rbox::meta_variant<int, rbox::meta_wstring_view>>);
    static_assert(std::is_same_v<
                  rbox::to_static_storage_result_t<std::variant<std::vector<int>, double>>,
                  rbox::meta_variant<rbox::meta_span<int>, double>>);
    static_assert(
        std::is_same_v<
            rbox::to_static_storage_result_t<std::variant<int, std::tuple<long, std::u8string>>>,
            rbox::meta_variant<int, rbox::meta_pair<long, rbox::meta_u8string_view>>>);
    static_assert(std::is_same_v<
                  rbox::to_static_storage_result_t<mixed_variant_t>,
                  rbox::meta_variant<
                      int,
                      rbox::meta_string_view,
                      rbox::meta_span<int>,
                      rbox::meta_pair<long, rbox::meta_string_view>,
                      rbox::meta_variant<int, double>>>);
}

struct with_vectors_t {
    std::vector<int> values;
    std::vector<std::vector<int>> nested_values;

    static constexpr auto make(int n) -> with_vectors_t
    {
        auto res = with_vectors_t{};
        for (int i = 0; i < n; i++) {
            res.values.push_back(i);
        }
        for (int i = 0; i < n; i++) {
            res.nested_values.emplace_back();
            for (int j = i; j < n; j++) {
                res.nested_values.back().push_back(i * n + j);
            }
        }
        return res;
    }
};

struct nested_with_vectors_t {
    with_vectors_t v1;
    std::vector<with_vectors_t> v2;
    std::vector<std::pair<std::u8string, with_vectors_t>> v3;

    static constexpr auto make() -> nested_with_vectors_t
    {
        auto res = nested_with_vectors_t{};

        res.v1 = with_vectors_t::make(5);
        for (int i = 1; i < 5; i++) {
            res.v2.push_back(with_vectors_t::make(i));
        }
        res.v3.emplace_back(u8"你好", with_vectors_t::make(6));
        res.v3.emplace_back(u8"世界", with_vectors_t::make(7));
        return res;
    }
};

TEST(ToStaticStorage, StructuralMirror)
{
    constexpr auto mirrored = rbox::to_static_storage(with_vectors_t::make(5));
    static_assert(std::is_same_v<
                  rbox::meta_span<int>,  // <-- std::vector<int>
                  decltype(mirrored.values)>);
    static_assert(std::is_same_v<
                  rbox::meta_span<rbox::meta_span<int>>,  // <-- std::vector<std::vector<int>>
                  decltype(mirrored.nested_values)>);

    static_assert(mirrored.values.size() == 5);
    static_assert(mirrored.values[4] == 4);

    static_assert(mirrored.nested_values.size() == 5);
    static_assert(mirrored.nested_values[4].size() == 1);
    static_assert(mirrored.nested_values[4][0] == 24);
}

TEST(ToStaticStorage, StructuralMirrorNested)
{
    using inner_mirrored_t = rbox::structural_mirror_t<with_vectors_t>;

    constexpr auto mirrored = rbox::to_static_storage(nested_with_vectors_t::make());
    static_assert(std::is_same_v<inner_mirrored_t, decltype(mirrored.v1)>);
    static_assert(std::is_same_v<rbox::meta_span<inner_mirrored_t>, decltype(mirrored.v2)>);
    static_assert(std::is_same_v<
                  rbox::meta_span<rbox::meta_pair<rbox::meta_u8string_view, inner_mirrored_t>>,
                  decltype(mirrored.v3)>);

    static_assert(mirrored.v1.values.size() == 5);

    static_assert(mirrored.v2.size() == 4);
    static_assert(mirrored.v2.front().values.size() == 1);
    static_assert(mirrored.v2.back().values.size() == 4);

    static_assert(mirrored.v3[0].first == u8"你好");
    static_assert(mirrored.v3[0].second.nested_values[5][0] == 35);
    static_assert(mirrored.v3[1].first == u8"世界");
    static_assert(mirrored.v3[1].second.nested_values[5][1] == 41);
    static_assert(mirrored.v3[1].second.nested_values[6][0] == 48);
}

struct with_bitfield_t {
    unsigned flag : 4;
    unsigned mode : 3;
    unsigned value : 17;
    int neg : 5;
    char small : 3;

    static constexpr auto make() -> with_bitfield_t
    {
        return with_bitfield_t{.flag = 5, .mode = 3, .value = 99999, .neg = -7, .small = 2};
    }
};

TEST(ToStaticStorage, StructuralMirrorBitfield)
{
    constexpr auto mirrored = rbox::to_static_storage(with_bitfield_t::make());

    // Exactly 32 bits in total.
    static_assert(sizeof(mirrored) == sizeof(uint32_t));

    // Bit-field members with scalar types → identity promotion
    static_assert(std::is_same_v<unsigned, decltype(mirrored.flag)>);
    static_assert(std::is_same_v<unsigned, decltype(mirrored.mode)>);
    static_assert(std::is_same_v<unsigned, decltype(mirrored.value)>);
    static_assert(std::is_same_v<int, decltype(mirrored.neg)>);
    static_assert(std::is_same_v<char, decltype(mirrored.small)>);

    static_assert(mirrored.flag == 5);
    static_assert(mirrored.mode == 3);
    static_assert(mirrored.value == 99999);
    static_assert(mirrored.neg == -7);
    static_assert(mirrored.small == 2);
}

struct with_reference_1_t {
    const int& int_ref;
    const double& double_ref;
};

TEST(ToStaticStorage, StructuralMirrorReference1)
{
    static constexpr int answer = 42;
    static constexpr double pi = 3.14159;

    constexpr auto mirrored =
        rbox::to_static_storage(with_reference_1_t{.int_ref = answer, .double_ref = pi});

    // with_reference_1_t itself is structural
    static_assert(std::is_same_v<const with_reference_1_t, decltype(mirrored)>);

    static_assert(mirrored.int_ref == answer);
    static_assert(mirrored.double_ref == pi);
    static_assert(&mirrored.int_ref == &answer);
    static_assert(&mirrored.double_ref == &pi);
}

std::string g_string;

struct with_reference_2_t {
    std::string& str_ref;
    const std::string* const str_ptr;
    std::vector<std::string> const str_list;
};

TEST(ToStaticStorage, StructuralMirrorReference2)
{
    constexpr auto mirrored = rbox::to_static_storage(
        with_reference_2_t{
            .str_ref = g_string,
            .str_ptr = &g_string,
            .str_list = {"Hello", "World", "of", "C++"},
        });

    static_assert(std::is_same_v<
                  std::string&,  // <-- std::string&
                  decltype(mirrored.str_ref)>);
    static_assert(std::is_same_v<
                  const std::string*,  // <-- const std::string* const
                  decltype(mirrored.str_ptr)>);
    static_assert(std::is_same_v<
                  rbox::meta_span<rbox::meta_string_view>,  // <-- std::vector<std::string> const
                  decltype(mirrored.str_list)>);

    static_assert(&mirrored.str_ref == &g_string);
    static_assert(mirrored.str_ptr == &g_string);

    static_assert(mirrored.str_list.size() == 4);
    static_assert(mirrored.str_list[0] == "Hello");
    static_assert(mirrored.str_list[1] == "World");
    static_assert(mirrored.str_list[2] == "of");
    static_assert(mirrored.str_list[3] == "C++");
}

struct empty_tag_t {};

struct with_empty_member_t {
    std::string name;
    empty_tag_t category;
};

TEST(ToStaticStorage, StructuralMirrorEmptyMember)
{
    constexpr auto mirrored = rbox::to_static_storage(with_empty_member_t{.name = "HelloWorld"});

    static_assert(std::is_same_v<rbox::meta_string_view, decltype(mirrored.name)>);
    static_assert(std::is_same_v<empty_tag_t, decltype(mirrored.category)>);

    // category is marked as [[no_unique_address]]
    static_assert(sizeof(mirrored) == sizeof(rbox::meta_string_view));

    static_assert(mirrored.name == "HelloWorld");
}

struct base_t {
    long base_id;
    std::string base_label;
};

struct derived_t : base_t {
    double extra;
    std::vector<int> data;
};

struct empty_marker_t {};

struct combo_base_t {
    int base_id;
};

struct combo_t : combo_base_t {
    const std::string& name_ref;
    int small_field : 7;
    empty_marker_t marker;
    std::vector<long> items;

    static constexpr auto make(const std::string& name) -> combo_t
    {
        return combo_t{combo_base_t{999}, name, 63, {}, {1L, 101L, 201L}};
    }
};

TEST(ToStaticStorage, StructuralMirrorInheritance)
{
    constexpr auto mirrored =
        rbox::to_static_storage(derived_t{base_t{200L, "derived"}, 3.14, {0, 10, 20, 30, 40}});

    // Flattened: members from both base_t and derived_t appear at top level
    static_assert(std::is_same_v<long, decltype(mirrored.base_id)>);
    static_assert(std::is_same_v<rbox::meta_string_view, decltype(mirrored.base_label)>);
    static_assert(std::is_same_v<double, decltype(mirrored.extra)>);
    static_assert(std::is_same_v<rbox::meta_span<int>, decltype(mirrored.data)>);

    static_assert(mirrored.base_id == 200L);
    static_assert(mirrored.base_label == "derived");
    static_assert(mirrored.extra == 3.14);
    static_assert(mirrored.data.size() == 5);
    static_assert(mirrored.data[0] == 0);
    static_assert(mirrored.data[4] == 40);
}

TEST(ToStaticStorage, StructuralMirrorCombined)
{
    static auto name_str = std::string{"hello_mirror"};
    constexpr auto mirrored = rbox::to_static_storage(combo_t::make(name_str));

    // Inheritance: base_id from combo_base_t
    static_assert(std::is_same_v<int, decltype(mirrored.base_id)>);
    // Reference → pointer
    static_assert(std::is_same_v<const std::string&, decltype(mirrored.name_ref)>);
    // Bit-field → identity
    static_assert(std::is_same_v<int, decltype(mirrored.small_field)>);
    // Empty type → identity (with [[no_unique_address]])
    static_assert(std::is_same_v<empty_marker_t, decltype(mirrored.marker)>);
    // Range → meta_span
    static_assert(std::is_same_v<rbox::meta_span<long>, decltype(mirrored.items)>);

    static_assert(&mirrored.name_ref == &name_str);

    static_assert(mirrored.base_id == 999);
    static_assert(mirrored.small_field == 63);
    static_assert(std::is_empty_v<decltype(mirrored.marker)>);
    static_assert(mirrored.items.size() == 3);
    static_assert(mirrored.items[0] == 1L);
    static_assert(mirrored.items[1] == 101L);
    static_assert(mirrored.items[2] == 201L);
}
