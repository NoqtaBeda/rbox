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

#ifndef RBOX_UTILS_FUNCTIONAL_GENERIC_COMPARISON_HPP
#define RBOX_UTILS_FUNCTIONAL_GENERIC_COMPARISON_HPP

#include <functional>
#include <rbox/utils/compare.hpp>
#include <rbox/utils/functional/integral_comparison.hpp>
#include <type_traits>

namespace rbox {
// -------- Extension of generic comparison --------
// (1) Less constraints than std::ranges::less, etc. and std::compare_three_way.
// (2) Uses signedness-safe integral comparison if both operands are integers.

template <class T, class U>
constexpr auto is_less_comparable_v = is_operator_lt_comparable_v<T, U>;

struct less_t {
    template <class T, class U>
        requires (is_less_comparable_v<T, U>)
    static constexpr bool operator()(const T& t, const U& u)
    {
        if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
            return cmp_less(t, u);
        } else {
            return t < u;
        }
    }
};

template <class T, class U>
constexpr auto is_greater_comparable_v =
    is_operator_gt_comparable_v<T, U> || is_operator_lt_comparable_v<U, T>;

struct greater_t {
    template <class T, class U>
        requires (is_greater_comparable_v<T, U>)
    static constexpr bool operator()(const T& t, const U& u)
    {
        if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
            return cmp_greater(t, u);
        } else if constexpr (is_operator_gt_comparable_v<T, U>) {
            return t > u;
        } else {
            return u < t;
        }
    }
};

template <class T, class U>
constexpr auto is_less_equal_comparable_v =
    is_operator_le_comparable_v<T, U>
    || (is_operator_lt_comparable_v<T, U> && is_operator_eq_comparable_v<T, U>);

struct less_equal_t {
    // Note: !(u < t) may be incorrect behavior for partial ordering.
    template <class T, class U>
        requires (is_less_equal_comparable_v<T, U>)
    static constexpr bool operator()(const T& t, const U& u)
    {
        if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
            return cmp_less_equal(t, u);
        } else if constexpr (is_operator_le_comparable_v<T, U>) {
            return t <= u;
        } else {
            return t < u || t == u;
        }
    }
};

template <class T, class U>
constexpr auto is_greater_equal_comparable_v =
    is_operator_ge_comparable_v<T, U> || is_operator_le_comparable_v<U, T>
    || (is_operator_gt_comparable_v<T, U> && is_operator_eq_comparable_v<T, U>)
    || (is_operator_lt_comparable_v<U, T> && is_operator_eq_comparable_v<U, T>);

struct greater_equal_t {
    // Note: !(t < u) may be incorrect behavior for partial ordering
    template <class T, class U>
        requires (is_greater_equal_comparable_v<T, U>)
    static constexpr bool operator()(const T& t, const U& u)
    {
        if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
            return cmp_greater_equal(t, u);
        } else if constexpr (is_operator_ge_comparable_v<T, U>) {
            return t >= u;
        } else if constexpr (is_operator_le_comparable_v<U, T>) {
            return u <= t;
        } else if constexpr (
            is_operator_gt_comparable_v<T, U> && is_operator_eq_comparable_v<T, U>) {
            return t > u || t == u;
        } else {
            return u < t || u == t;
        }
    }
};

template <class T, class U>
constexpr auto is_equal_comparable_v = is_operator_eq_comparable_v<T, U>;

struct equal_t {
    template <class T, class U>
        requires (is_equal_comparable_v<T, U>)
    static constexpr bool operator()(const T& t, const U& u)
    {
        if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
            return cmp_equal(t, u);
        } else {
            return t == u;
        }
    }
};

template <class T, class U>
constexpr auto is_not_equal_comparable_v =
    is_operator_ne_comparable_v<T, U> || is_operator_eq_comparable_v<T, U>;

struct not_equal_t {
    template <class T, class U>
        requires (is_not_equal_comparable_v<T, U>)
    static constexpr bool operator()(const T& t, const U& u)
    {
        if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
            return cmp_not_equal(t, u);
        } else if constexpr (is_operator_ne_comparable_v<T, U>) {
            return t != u;
        } else {
            return !(t == u);
        }
    }
};

template <class T, class U>
constexpr auto is_compare_three_way_comparable_v =
    (std::is_integral_v<T> && std::is_integral_v<U>) || is_operator_3way_comparable_v<T, U>
    || (is_less_comparable_v<T, U> && is_greater_comparable_v<T, U> && is_equal_comparable_v<T, U>);

struct compare_three_way_t {
private:
    template <class T, class U>
    static constexpr auto do_indirect_compare(const T& t, const U& u) -> std::partial_ordering
    {
        if (less_t::operator()(t, u)) {
            return std::partial_ordering::less;
        }
        if (greater_t::operator()(t, u)) {
            return std::partial_ordering::greater;
        }
        if (equal_t::operator()(t, u)) {
            return std::partial_ordering::equivalent;
        }
        return std::partial_ordering::unordered;
    }

public:
    template <class T, class U>
        requires (is_compare_three_way_comparable_v<T, U>)
    static constexpr auto operator()(const T& t, const U& u)
    {
        if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
            return cmp_three_way(t, u);
        } else if constexpr (is_operator_3way_comparable_v<T, U>) {
            return t <=> u;
        } else {
            return do_indirect_compare(t, u);
        }
    }
};

// Naming is analogous to C++ standard variant.
constexpr auto less = less_t{};
constexpr auto greater = greater_t{};
constexpr auto less_equal = less_equal_t{};
constexpr auto greater_equal = greater_equal_t{};
constexpr auto equal = equal_t{};
constexpr auto not_equal = not_equal_t{};
constexpr auto compare_three_way = compare_three_way_t{};

template <class T, class U = T>
    requires (is_compare_three_way_comparable_v<T, U>)
using compare_three_way_result = std::invoke_result_t<compare_three_way_t, const T&, const U&>;
}  // namespace rbox

#endif  // RBOX_UTILS_FUNCTIONAL_GENERIC_COMPARISON_HPP
