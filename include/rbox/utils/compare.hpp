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

#ifndef RBOX_UTILS_COMPARE_HPP
#define RBOX_UTILS_COMPARE_HPP

#include <compare>

namespace rbox {
// clang-format off
template <class T>
concept three_way_comparison_result =
       std::is_same_v<std::remove_cv_t<T>, std::strong_ordering>
    || std::is_same_v<std::remove_cv_t<T>, std::weak_ordering>
    || std::is_same_v<std::remove_cv_t<T>, std::partial_ordering>;
// clang-format on

template <class T, class U>
constexpr bool is_operator_eq_comparable_v = requires (const T& t, const U& u) {
    { t == u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_operator_ne_comparable_v = requires (const T& t, const U& u) {
    { t != u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_operator_lt_comparable_v = requires (const T& t, const U& u) {
    { t < u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_operator_gt_comparable_v = requires (const T& t, const U& u) {
    { t > u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_operator_le_comparable_v = requires (const T& t, const U& u) {
    { t <= u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_operator_ge_comparable_v = requires (const T& t, const U& u) {
    { t >= u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_operator_3way_comparable_v = requires (const T& t, const U& u) {
    { t <=> u } -> three_way_comparison_result;
};
}  // namespace rbox

#endif  // RBOX_UTILS_COMPARE_HPP
