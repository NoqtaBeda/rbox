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

#ifndef RBOX_TYPE_TRAITS_VARIANT_LIKE_TYPES_HPP
#define RBOX_TYPE_TRAITS_VARIANT_LIKE_TYPES_HPP

#include <concepts>
#include <cstddef>
#include <meta>
#include <rbox/type_traits/type_comparison.hpp>
#include <rbox/utils/config.hpp>
#include <utility>
#include <variant>

namespace rbox {
namespace impl {
template <class T>
concept has_variant_size = requires {
    { std::variant_size<T>::value } -> std::convertible_to<size_t>;
};

template <class T, size_t I>
concept has_variant_alternative = requires { std::void_t<std::variant_alternative_t<I, T>>(); };

template <class T, class A>
concept has_member_holds_alternative = requires (const T& v) {
    { v.template holds_alternative<A>() } -> std::same_as<bool>;
};

// holds_alternative<A>(v) can be found via ADL
template <class T, class A>
concept has_free_holds_alternative = requires (const T& v) {
    { holds_alternative<A>(v) } -> std::same_as<bool>;
};

// clang-format off
template <class T, class A>
concept has_variant_member_get_ith = requires (T v) {
    {
        v.template get<A>()
    } -> same_as_without_cvref<A>;
    {
        std::as_const(v).template get<A>()
    } -> same_as_without_cvref<A>;
    {
        std::move(v).template get<A>()
    } -> same_as_without_cvref<A>;
    {
        std::move(std::as_const(v)).template get<A>()
    } -> same_as_without_cvref<A>;
};

// get<A>(v) can be found via ADL.
template <class T, class A>
concept has_variant_free_get_ith = requires (T v) {
    {
        get<A>(v)
    } -> same_as_without_cvref<A>;
    {
        get<A>(std::as_const(v))
    } -> same_as_without_cvref<A>;
    {
        get<A>(std::move(v))
    } -> same_as_without_cvref<A>;
    {
        get<A>(std::move(std::as_const(v)))
    } -> same_as_without_cvref<A>;
};
// clang-format on

consteval bool is_variant_like(std::meta::info T)
{
    T = std::meta::remove_cv(T);
    auto has_variant_size_c = std::meta::substitute(^^has_variant_size, RBOX_IL(T));
    if (!std::meta::extract<bool>(has_variant_size_c)) {
        return false;
    }
    auto n = std::meta::variant_size(T);
    for (auto i = 0zU; i < n; i++) {
        auto I = std::meta::reflect_constant(i);
        auto has_type_c = std::meta::substitute(^^has_variant_alternative, RBOX_IL(T, I));
        if (!std::meta::extract<bool>(has_type_c)) {
            return false;
        }
    }
    for (auto i = 0zU; i < n; i++) {
        auto params = {T, std::meta::variant_alternative(i, T)};
        auto has_free_check_c = std::meta::substitute(^^has_free_holds_alternative, params);
        auto has_member_check_c = std::meta::substitute(^^has_member_holds_alternative, params);
        if (!std::meta::extract<bool>(has_free_check_c)
            && !std::meta::extract<bool>(has_member_check_c)) {
            return false;
        }
        auto has_free_get_c = std::meta::substitute(^^has_variant_free_get_ith, params);
        if (std::meta::extract<bool>(has_free_get_c)) {
            continue;  // Pass
        }
        auto has_member_get_c = std::meta::substitute(^^has_variant_member_get_ith, params);
        if (!std::meta::extract<bool>(has_member_get_c)) {
            return false;
        }
    }
    return true;
}
}  // namespace impl

template <class T>
concept variant_like = impl::is_variant_like(^^T);
}  // namespace rbox

#endif  // RBOX_TYPE_TRAITS_VARIANT_LIKE_TYPES_HPP
