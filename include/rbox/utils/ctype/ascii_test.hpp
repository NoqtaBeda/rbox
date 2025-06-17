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

#ifndef RBOX_UTILS_CTYPE_ASCII_TEST_HPP
#define RBOX_UTILS_CTYPE_ASCII_TEST_HPP

#include <rbox/type_traits/arithmetic_types.hpp>
#include <rbox/type_traits/string_like_types.hpp>
#include <rbox/utils/make_string_view.hpp>

namespace rbox {
struct is_ascii_char_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127;
    }
};
constexpr auto is_ascii_char = is_ascii_char_t{};

struct is_ascii_string_t {
    static constexpr bool operator()(const string_like auto& str)
    {
        auto sv = make_string_view(str);
        for (auto c : sv) {
            if (!is_ascii_char(c)) {
                return false;
            }
        }
        return true;
    }
};
constexpr auto is_ascii_string = is_ascii_string_t{};
}  // namespace rbox

#endif  // RBOX_UTILS_CTYPE_ASCII_TEST_HPP
