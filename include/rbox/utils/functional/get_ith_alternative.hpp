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

#ifndef RBOX_UTILS_FUNCTIONAL_GET_ITH_ALTERNATIVE_HPP
#define RBOX_UTILS_FUNCTIONAL_GET_ITH_ALTERNATIVE_HPP

#include <rbox/type_traits/variant_like_types.hpp>

namespace rbox {
namespace impl {
template <class T, size_t I>
concept variant_like_with_index = variant_like<T> && (I < std::variant_size_v<T>);
}

template <size_t I>
struct holds_ith_alternative_t {
    template <impl::variant_like_with_index<I> T>
    static constexpr auto operator()(const T& variant) -> bool
    {
        using A = std::variant_alternative_t<I, T>;
        if constexpr (impl::has_free_holds_alternative<T, A>) {
            return holds_alternative<A>(variant);
        } else {
            return variant.template holds_alternative<A>();
        }
    }
};

template <size_t I>
constexpr auto holds_ith_alternative = holds_ith_alternative_t<I>{};

template <size_t I>
struct get_ith_alternative_t {
    template <class T>
        requires (impl::variant_like_with_index<std::remove_cvref_t<T>, I>)
    static constexpr decltype(auto) operator()(T&& variant)
    {
        using A = std::variant_alternative_t<I, std::remove_cvref_t<T>>;
        if constexpr (impl::has_variant_free_get_ith<std::remove_cvref_t<T>, A>) {
            return get<A>(std::forward<T>(variant));
        } else {
            return std::forward<T>(variant).template get<A>();
        }
    }
};

template <size_t I>
constexpr auto get_ith_alternative = get_ith_alternative_t<I>{};
}  // namespace rbox

#endif  // RBOX_UTILS_FUNCTIONAL_GET_ITH_ALTERNATIVE_HPP
