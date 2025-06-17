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

#ifndef RBOX_UTILS_DEFINE_STATIC_ARRAY_HPP
#define RBOX_UTILS_DEFINE_STATIC_ARRAY_HPP

#include <ranges>
#include <rbox/utils/config.hpp>
#include <rbox/utils/meta_span.hpp>

namespace rbox {
template <std::ranges::input_range Range>
consteval auto define_static_array(Range&& range) /* -> meta_span<T> */
{
    using T = std::ranges::range_value_t<Range>;
    auto span = std::define_static_array(std::forward<Range>(range));
    return meta_span<T>(span);
}
}  // namespace rbox

#endif  // RBOX_UTILS_DEFINE_STATIC_ARRAY_HPP
