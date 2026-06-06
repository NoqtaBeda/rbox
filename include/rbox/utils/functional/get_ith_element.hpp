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

#ifndef RBOX_UTILS_FUNCTIONAL_GET_ITH_ELEMENT_HPP
#define RBOX_UTILS_FUNCTIONAL_GET_ITH_ELEMENT_HPP

#include <rbox/type_traits/tuple_like_types.hpp>
#include <rbox/utils/tuple_size.hpp>

namespace rbox {
// -------- Generic tuple-like access --------
namespace impl {
template <class T, size_t I>
concept tuple_like_with_index = tuple_like<T> && (I < rbox::tuple_size_v<T>);
}  // namespace impl

template <size_t I>
struct get_ith_element_t {
    template <class T>
        requires (impl::tuple_like_with_index<std::remove_cvref_t<T>, I>)
    static constexpr decltype(auto) operator()(T&& tuple)
    {
        if constexpr (impl::has_tuple_free_get_ith<std::remove_cvref_t<T>, I>) {
            return get<I>(std::forward<T>(tuple));
        } else {
            return std::forward<T>(tuple).template get<I>();
        }
    }
};

using get_first_t = get_ith_element_t<0>;
using get_second_t = get_ith_element_t<1>;
using get_third_t = get_ith_element_t<2>;

constexpr auto get_first = get_ith_element_t<0>{};
constexpr auto get_second = get_ith_element_t<1>{};
constexpr auto get_third = get_ith_element_t<2>{};

template <size_t I>
constexpr auto get_ith_element = get_ith_element_t<I>{};
}  // namespace rbox

#endif  // RBOX_UTILS_FUNCTIONAL_GET_ITH_ELEMENT_HPP
