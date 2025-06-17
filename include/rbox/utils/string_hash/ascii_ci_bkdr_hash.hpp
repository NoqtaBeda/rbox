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

#ifndef RBOX_UTILS_STRING_HASH_ASCII_CI_BKDR_HASH_HPP
#define RBOX_UTILS_STRING_HASH_ASCII_CI_BKDR_HASH_HPP

#include <rbox/type_traits/string_like_types.hpp>
#include <rbox/utils/ctype/case_conversion.hpp>
#include <rbox/utils/make_string_view.hpp>

namespace rbox {

struct ascii_ci_bkdr_hash_t {
    template <char_type CharT>
    static constexpr auto operator()(const CharT* begin, const CharT* end) -> size_t
    {
        auto res = size_t{0};
        for (; begin < end; ++begin) {
            auto cur = ascii_tolower(*begin);
            res = res * 131zU + static_cast<size_t>(cur);
        }
        return res;
    }

    template <string_like StringT>
    static constexpr auto operator()(const StringT& str) -> size_t
    {
        auto sv = make_string_view(str);
        return operator()(sv.data(), sv.data() + sv.size());
    }
};

constexpr auto ascii_ci_bkdr_hash = ascii_ci_bkdr_hash_t{};
}  // namespace rbox

#endif  // RBOX_UTILS_STRING_HASH_ASCII_CI_BKDR_HASH_HPP
