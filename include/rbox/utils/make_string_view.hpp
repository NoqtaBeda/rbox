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

#ifndef RBOX_UTILS_MAKE_STRING_VIEW_HPP
#define RBOX_UTILS_MAKE_STRING_VIEW_HPP

#include <rbox/type_traits/string_like_types.hpp>
#include <string_view>

namespace rbox {
template <string_like StringT>
constexpr auto make_string_view(const StringT& str)
{
    using CharT = char_type_t<StringT>;
    if constexpr (std::is_pointer_v<StringT>) {
        auto res = std::basic_string_view<CharT>{};
        if (str != nullptr) {
            res = str;
        }
        return res;
    } else {
        return std::basic_string_view<CharT>{str};
    }
}
}  // namespace rbox

#endif  // RBOX_UTILS_MAKE_STRING_VIEW_HPP
