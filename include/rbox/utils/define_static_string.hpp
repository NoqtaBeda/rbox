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

#ifndef RBOX_UTILS_DEFINE_STATIC_STRING_HPP
#define RBOX_UTILS_DEFINE_STATIC_STRING_HPP

#include <ranges>
#include <rbox/type_traits/arithmetic_types.hpp>
#include <rbox/utils/config.hpp>
#include <rbox/utils/meta_string_view.hpp>

namespace rbox {
template <std::ranges::input_range Range>
    requires (char_type<std::ranges::range_value_t<Range>>)
consteval auto define_static_string(Range&& range) /* -> meta_basic_string_view<CharT> */
{
    using CharT = std::ranges::range_value_t<Range>;
    auto c_str = std::define_static_string(std::forward<Range>(range));
    return meta_basic_string_view<CharT>(c_str);
}
}  // namespace rbox

#endif  // RBOX_UTILS_DEFINE_STATIC_STRING_HPP
