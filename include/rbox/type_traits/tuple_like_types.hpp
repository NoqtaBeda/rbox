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

#ifndef RBOX_TYPE_TRAITS_TUPLE_LIKE_TYPES_HPP
#define RBOX_TYPE_TRAITS_TUPLE_LIKE_TYPES_HPP

#include <concepts>
#include <cstddef>
#include <meta>
#include <rbox/type_traits/type_comparison.hpp>
#include <rbox/utils/config.hpp>
#include <rbox/utils/tuple_element.hpp>
#include <rbox/utils/tuple_size.hpp>
#include <utility>

namespace rbox {
namespace impl {
template <class T>
concept has_tuple_size = requires {
    { std::tuple_size<T>::value } -> std::convertible_to<size_t>;
};

// clang-format off
template <class T, size_t I>
concept has_tuple_member_get_ith = requires (T t) {
    {
        t.template get<I>()
    } -> same_as_without_cvref<rbox::tuple_element_t<I, T>>;
    {
        std::as_const(t).template get<I>()
    } -> same_as_without_cvref<rbox::tuple_element_t<I, T>>;
    {
        std::move(t).template get<I>()
    } -> same_as_without_cvref<rbox::tuple_element_t<I, T>>;
    {
        std::move(std::as_const(t)).template get<I>()
    } -> same_as_without_cvref<rbox::tuple_element_t<I, T>>;
};

// get<I>(t) can be found via ADL.
template <class T, size_t I>
concept has_tuple_free_get_ith = requires (T t) {
    {
        get<I>(t)
    } -> same_as_without_cvref<rbox::tuple_element_t<I, T>>;
    {
        get<I>(std::as_const(t))
    } -> same_as_without_cvref<rbox::tuple_element_t<I, T>>;
    {
        get<I>(std::move(t))
    } -> same_as_without_cvref<rbox::tuple_element_t<I, T>>;
    {
        get<I>(std::move(std::as_const(t)))
    } -> same_as_without_cvref<rbox::tuple_element_t<I, T>>;
};
// clang-format on

consteval bool is_tuple_like(std::meta::info T)
{
    T = std::meta::remove_cv(T);
    auto has_tuple_size_c = std::meta::substitute(^^has_tuple_size, RBOX_IL(T));
    if (!std::meta::extract<bool>(has_tuple_size_c)) {
        return false;
    }
    auto n = std::meta::tuple_size(T);
    for (auto i = 0zU; i < n; i++) {
        auto params = {T, std::meta::reflect_constant(i)};
        auto has_free_get_c = std::meta::substitute(^^has_tuple_free_get_ith, params);
        if (std::meta::extract<bool>(has_free_get_c)) {
            continue;  // Pass
        }
        auto has_member_get_c = std::meta::substitute(^^has_tuple_member_get_ith, params);
        if (!std::meta::extract<bool>(has_member_get_c)) {
            return false;
        }
    }
    return true;
}
}  // namespace impl

template <class T>
concept tuple_like = impl::is_tuple_like(^^T);

template <class T>
concept pair_like = tuple_like<T> && rbox::tuple_size_v<std::remove_cv_t<T>> == 2;

namespace impl {
consteval bool is_tuple_like_of_exactly(std::meta::info T, std::span<const std::meta::info> args)
{
    T = std::meta::remove_cv(T);
    auto tuple_like_c = std::meta::substitute(^^tuple_like, RBOX_IL(T));
    if (!std::meta::extract<bool>(tuple_like_c)) {
        return false;
    }
    auto n = std::meta::tuple_size(T);
    if (args.size() != n) {
        return false;
    }
    for (auto i = 0zU; i < n; i++) {
        auto Ti = std::meta::tuple_element(i, T);
        if (!std::meta::is_same_type(Ti, args[i])) {
            return false;
        }
    }
    return true;
}

consteval bool is_tuple_like_of(std::meta::info T, std::span<const std::meta::info> args)
{
    T = std::meta::remove_cv(T);
    auto tuple_like_c = std::meta::substitute(^^tuple_like, RBOX_IL(T));
    if (!std::meta::extract<bool>(tuple_like_c)) {
        return false;
    }
    auto n = std::meta::tuple_size(T);
    if (args.size() != n) {
        return false;
    }
    for (auto i = 0zU; i < n; i++) {
        auto Ti = std::meta::tuple_element(i, T);
        if (!std::meta::is_convertible_type(Ti, args[i])) {
            return false;
        }
    }
    return true;
}
}  // namespace impl

template <class T, class... Args>
concept tuple_like_of_exactly = impl::is_tuple_like_of_exactly(^^T, RBOX_IL(^^Args...));

template <class T, class... Args>
concept tuple_like_of = impl::is_tuple_like_of(^^T, RBOX_IL(^^Args...));
}  // namespace rbox

#endif  // RBOX_TYPE_TRAITS_TUPLE_LIKE_TYPES_HPP
