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

#ifndef RBOX_UTILS_TRIM_HPP
#define RBOX_UTILS_TRIM_HPP

#include <rbox/type_traits/string_like_types.hpp>
#include <rbox/utils/ctype/char_categories.hpp>
#include <rbox/utils/make_string_view.hpp>

namespace rbox {

struct ascii_trim_t {
    template <string_like StringT>
    static constexpr auto operator()(const StringT& str) /* -> std::basic_string_view */
    {
        auto sv = make_string_view(str);
        return do_trim(sv);
    }

private:
    template <class CharT, class Traits>
    static constexpr auto do_trim(std::basic_string_view<CharT, Traits> str)
        -> std::basic_string_view<CharT, Traits>
    {
        if (str.empty()) {
            return str;  // In case of null string-view {nullptr, nullptr}
        }
        auto head = str.begin();
        auto before_tail = str.end() - 1;
        for (; head <= before_tail && ascii_isspace(*head); ++head);
        for (; head <= before_tail && ascii_isspace(*before_tail); --before_tail);
        return {head, before_tail + 1};
    }
};

constexpr auto ascii_trim = ascii_trim_t{};

}  // namespace rbox

#endif  // RBOX_UTILS_TRIM_HPP
