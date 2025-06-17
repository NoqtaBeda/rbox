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

#ifndef RBOX_UTILS_CTYPE_CASE_CONVERSION_HPP
#define RBOX_UTILS_CTYPE_CASE_CONVERSION_HPP

#include <rbox/type_traits/string_like_types.hpp>
#include <rbox/utils/ctype/char_categories.hpp>
#include <rbox/utils/make_string_view.hpp>

namespace rbox {

struct ascii_tolower_t {
    template <char_type CharT>
    static constexpr auto operator()(CharT c) -> CharT
    {
        return static_cast<CharT>(ascii_isupper(c) ? c + 'a' - 'A' : c);
    }

    template <string_like StringT>
    static constexpr auto operator()(const StringT& str) /* -> std::basic_string<CharT> */
    {
        using CharT = char_type_t<StringT>;
        auto sv = make_string_view(str);
        auto res = std::basic_string<CharT>{};
        res.resize_and_overwrite(sv.length(), [sv](CharT* buffer, size_t n) {
            for (auto c : sv) {
                *buffer++ = operator()(c);
            }
            return n;
        });
        return res;
    }
};

struct ascii_toupper_t {
    template <char_type CharT>
    static constexpr auto operator()(CharT c) -> CharT
    {
        return static_cast<CharT>(ascii_islower(c) ? c + 'A' - 'a' : c);
    }

    template <string_like StringT>
    static constexpr auto operator()(const StringT& str) /* -> std::basic_string<CharT> */
    {
        using CharT = char_type_t<StringT>;
        auto sv = make_string_view(str);
        auto res = std::basic_string<CharT>{};
        res.resize_and_overwrite(sv.length(), [sv](CharT* buffer, size_t n) {
            for (auto c : sv) {
                *buffer++ = operator()(c);
            }
            return n;
        });
        return res;
    }
};

constexpr auto ascii_tolower = ascii_tolower_t{};
constexpr auto ascii_toupper = ascii_toupper_t{};

}  // namespace rbox

#endif  // RBOX_UTILS_CTYPE_CASE_CONVERSION_HPP
