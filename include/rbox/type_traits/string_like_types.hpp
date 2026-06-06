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

#ifndef RBOX_TYPE_TRAITS_STRING_LIKE_TYPES_HPP
#define RBOX_TYPE_TRAITS_STRING_LIKE_TYPES_HPP

#include <meta>
#include <rbox/type_traits/arithmetic_types.hpp>
#include <rbox/utils/config.hpp>
#include <rbox/utils/stdlib/ranges/concepts.hpp>
#include <string_view>

namespace rbox {
namespace impl {
template <class R>
concept convertible_to_string_view = requires (R range) { std::basic_string_view(range); };

consteval bool is_string_like_impl(std::meta::info T)
{
    if (std::meta::is_reference_type(T)) {
        return false;
    }
    auto D = std::meta::decay(T);
    if (std::meta::is_pointer_type(D)) {
        auto V = std::meta::remove_pointer(D);
        if (std::meta::is_volatile_type(V)) {
            return false;
        }
        auto is_char_c = std::meta::substitute(^^char_type, RBOX_IL(V));
        return std::meta::extract<bool>(is_char_c);
    }
    auto is_range_c = std::meta::substitute(^^std::ranges::contiguous_range, RBOX_IL(T));
    if (!std::meta::extract<bool>(is_range_c)) {
        return false;
    }
    auto V = std::meta::substitute(^^std::ranges::range_value_t, RBOX_IL(T));
    auto is_char_c = std::meta::substitute(^^char_type, RBOX_IL(V));
    if (!std::meta::extract<bool>(is_char_c)) {
        return false;
    }
    auto is_convertible_c = std::meta::substitute(^^convertible_to_string_view, RBOX_IL(D));
    return std::meta::extract<bool>(is_convertible_c);
}

// Precondition: T is ensured to be string-like type by the checks above.
consteval auto get_char_type(std::meta::info T)
{
    auto is_range = std::meta::extract<bool>(
        std::meta::substitute(^^std::ranges::contiguous_range, RBOX_IL(T)));
    if (is_range) {
        return std::meta::substitute(^^std::ranges::range_value_t, RBOX_IL(T));
    }
    auto D = std::meta::decay(T);
    if (!std::meta::is_pointer_type(D)) {
        compile_error("Implementation error: Expects D to be [const] CharT*");
    }
    return std::meta::remove_cv(std::meta::remove_pointer(D));
}
}  // namespace impl

template <class T>
concept string_like = impl::is_string_like_impl(^^T);

template <string_like T>
using char_type_t = [:impl::get_char_type(^^T):];

template <class T, class CharT>
concept string_like_of =
    string_like<T> && char_type<CharT> && std::is_same_v<char_type_t<T>, CharT>;
}  // namespace rbox

#endif  // RBOX_TYPE_TRAITS_STRING_LIKE_TYPES_HPP
