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

#ifndef RBOX_UTILS_FUNCTIONAL_TUPLE_COMPARISON_HPP
#define RBOX_UTILS_FUNCTIONAL_TUPLE_COMPARISON_HPP

#include <meta>
#include <rbox/type_traits/tuple_like_types.hpp>
#include <rbox/utils/functional/generic_comparison.hpp>
#include <rbox/utils/functional/get_ith_element.hpp>
#include <rbox/utils/indices_view.hpp>

namespace rbox {
// -------- Generic tuple-like comparison --------

namespace impl {
consteval bool is_elementwise_comparable_by(  // Precondition: T and U are tuple_like
    std::meta::info T,
    std::meta::info U,
    std::meta::info target_concept)
{
    auto N = std::meta::tuple_size(T);
    if (std::meta::tuple_size(U) != N) {
        return false;
    }
    for (auto i = 0zU; i < N; i++) {
        auto A = std::meta::tuple_element(i, T);
        auto B = std::meta::tuple_element(i, U);
        auto result_c = std::meta::substitute(target_concept, RBOX_IL(A, B));
        if (!std::meta::extract<bool>(result_c)) {
            return false;
        }
    }
    return true;
}

consteval auto elementwise_compare_3way_result(std::meta::info T, std::meta::info U)
    -> std::meta::info
{
    T = std::meta::remove_cvref(T);
    U = std::meta::remove_cvref(U);

    auto N = std::meta::tuple_size(T);
    auto results = std::vector<std::meta::info>{};
    results.reserve(N);

    for (auto i = 0zU; i < N; i++) {
        auto Ti = std::meta::tuple_element(i, T);
        auto Tu = std::meta::tuple_element(i, U);
        results.push_back(std::meta::substitute(^^compare_three_way_result, RBOX_IL(Ti, Tu)));
    }
    return std::meta::substitute(^^std::common_comparison_category_t, results);
}

template <class T, class U>
using elementwise_compare_3way_result_t = [:elementwise_compare_3way_result(^^T, ^^U):];
}  // namespace impl

template <class T, class U>
constexpr auto is_elementwise_less_comparable_v =
    tuple_like<T> && tuple_like<U>
    && impl::is_elementwise_comparable_by(^^T, ^^U, ^^is_less_comparable_v)
    && impl::is_elementwise_comparable_by(^^T, ^^U, ^^is_equal_comparable_v);

struct elementwise_less_t {
    template <class T, class U>
        requires (is_elementwise_less_comparable_v<T, U>)
    static constexpr auto operator()(const T& t, const U& u) -> bool
    {
        template for (constexpr auto I : rbox::indices_view{std::tuple_size_v<T>})
        {
            const auto& a = get_ith_element<I>(t);
            const auto& b = get_ith_element<I>(u);
            if (less(a, b)) return true;
            if (!equal(a, b)) return false;  // Including (a > b) and unordered case
        }
        return false;
    }
};

template <class T, class U>
constexpr auto is_elementwise_greater_comparable_v =
    tuple_like<T> && tuple_like<U>
    && impl::is_elementwise_comparable_by(^^T, ^^U, ^^is_greater_comparable_v)
    && impl::is_elementwise_comparable_by(^^T, ^^U, ^^is_equal_comparable_v);

struct elementwise_greater_t {
    template <class T, class U>
        requires (is_elementwise_greater_comparable_v<T, U>)
    static constexpr auto operator()(const T& t, const U& u) -> bool
    {
        template for (constexpr auto I : rbox::indices_view{std::tuple_size_v<T>})
        {
            const auto& a = get_ith_element<I>(t);
            const auto& b = get_ith_element<I>(u);
            if (greater(a, b)) return true;
            if (!equal(a, b)) return false;  // Including (a < b) and unordered case
        }
        return false;
    }
};

template <class T, class U>
constexpr auto is_elementwise_less_equal_comparable_v = is_elementwise_less_comparable_v<T, U>;

struct elementwise_less_equal_t {
    template <class T, class U>
        requires (is_elementwise_less_equal_comparable_v<T, U>)
    static constexpr auto operator()(const T& t, const U& u) -> bool
    {
        template for (constexpr auto I : rbox::indices_view{std::tuple_size_v<T>})
        {
            const auto& a = get_ith_element<I>(t);
            const auto& b = get_ith_element<I>(u);
            if (less(a, b)) return true;
            if (!equal(a, b)) return false;  // Including (a > b) and unordered case
        }
        return true;
    }
};

template <class T, class U>
constexpr auto is_elementwise_greater_equal_comparable_v =
    is_elementwise_greater_comparable_v<T, U>;

struct elementwise_greater_equal_t {
    template <class T, class U>
        requires (is_elementwise_greater_equal_comparable_v<T, U>)
    static constexpr auto operator()(const T& t, const U& u) -> bool
    {
        template for (constexpr auto I : rbox::indices_view{std::tuple_size_v<T>})
        {
            const auto& a = get_ith_element<I>(t);
            const auto& b = get_ith_element<I>(u);
            if (greater(a, b)) return true;
            if (!equal(a, b)) return false;  // Including (a < b) and unordered case
        }
        return true;
    }
};

template <class T, class U>
constexpr auto is_elementwise_equal_comparable_v =
    tuple_like<T> && tuple_like<U>
    && impl::is_elementwise_comparable_by(^^T, ^^U, ^^is_equal_comparable_v);

struct elementwise_equal_t {
    // returns true only if get<I>(t) == get<I>(u) for every index I
    template <class T, class U>
        requires (is_elementwise_equal_comparable_v<T, U>)
    static constexpr auto operator()(const T& t, const U& u) -> bool
    {
        template for (constexpr auto I : rbox::indices_view{std::tuple_size_v<T>})
        {
            const auto& a = get_ith_element<I>(t);
            const auto& b = get_ith_element<I>(u);
            if (!equal(a, b)) return false;
        }
        return true;
    }
};

template <class T, class U>
constexpr auto is_elementwise_not_equal_comparable_v =
    tuple_like<T> && tuple_like<U>
    && impl::is_elementwise_comparable_by(^^T, ^^U, ^^is_not_equal_comparable_v);

struct elementwise_not_equal_t {
    // returns true if get<I>(t) != get<I>(u) for ANY index I
    template <class T, class U>
        requires (is_elementwise_not_equal_comparable_v<T, U>)
    static constexpr auto operator()(const T& t, const U& u) -> bool
    {
        template for (constexpr auto I : rbox::indices_view{std::tuple_size_v<T>})
        {
            const auto& a = get_ith_element<I>(t);
            const auto& b = get_ith_element<I>(u);
            if (not_equal(a, b)) return true;
        }
        return false;
    }
};

template <class T, class U>
constexpr auto is_elementwise_compare_three_way_comparable_v =
    tuple_like<T> && tuple_like<U>
    && impl::is_elementwise_comparable_by(^^T, ^^U, ^^is_compare_three_way_comparable_v);

struct elementwise_compare_three_way_t {
    template <class T, class U>
        requires (is_elementwise_compare_three_way_comparable_v<T, U>)
    static constexpr auto operator()(const T& t, const U& u)
        -> impl::elementwise_compare_3way_result_t<T, U>
    {
        constexpr auto N = std::tuple_size_v<T>;
        template for (constexpr auto I : rbox::indices_view{N})
        {
            const auto& a = get_ith_element<I>(t);
            const auto& b = get_ith_element<I>(u);
            if (auto cur_res = compare_three_way(a, b); cur_res != 0) {
                return cur_res;
            }
        }
        // Implicitly converts to impl::elementwise_compare_3way_result_t<T, U>
        return std::strong_ordering::equal;
    }
};

constexpr auto elementwise_less = elementwise_less_t{};
constexpr auto elementwise_greater = elementwise_greater_t{};
constexpr auto elementwise_less_equal = elementwise_less_equal_t{};
constexpr auto elementwise_greater_equal = elementwise_greater_equal_t{};
constexpr auto elementwise_equal = elementwise_equal_t{};
constexpr auto elementwise_not_equal = elementwise_not_equal_t{};
constexpr auto elementwise_compare_three_way = elementwise_compare_three_way_t{};

}  // namespace rbox

#endif  // RBOX_UTILS_FUNCTIONAL_TUPLE_COMPARISON_HPP
