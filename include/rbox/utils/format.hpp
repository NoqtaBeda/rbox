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

#ifndef RBOX_UTILS_FORMAT_HPP
#define RBOX_UTILS_FORMAT_HPP

#include <format>
#include <rbox/utils/meta_string_view.hpp>

#if __has_include(<fmt/format.h>)
#include <fmt/format.h>
#endif

template <class CharT>
struct std::formatter<rbox::meta_basic_string_view<CharT>, CharT>
    : std::formatter<std::basic_string_view<CharT>, CharT> {
    using self_type = rbox::meta_basic_string_view<CharT>;

    template <class FormatContext>
    auto format(self_type str, FormatContext& ctx) const -> FormatContext::iterator
    {
        const auto* end = str.head + str.n;
        for (const auto* p = str.head; p < end; ++p) {
            *out++ = *p;
        }
        return out;
    }
};

#if __has_include(<fmt/format.h>)
template <class CharT>
struct fmt::formatter<rbox::meta_basic_string_view<CharT>, CharT>
    : fmt::formatter<std::basic_string_view<CharT>, CharT> {
    using self_type = rbox::meta_basic_string_view<CharT>;

    template <class FormatContext>
    auto format(self_type str, FormatContext& ctx) const -> FormatContext::iterator
    {
        const auto* end = str.head + str.n;
        for (const auto* p = str.head; p < end; ++p) {
            *out++ = *p;
        }
        return out;
    }
};
#endif

#endif  // RBOX_UTILS_FORMAT_HPP
