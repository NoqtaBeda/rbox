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

template <>
struct std::formatter<rbox::meta_string_view, char> : std::formatter<std::string_view, char> {
    template <class FormatContext>
    auto format(rbox::meta_string_view str, FormatContext& ctx) const -> FormatContext::iterator
    {
        return std::formatter<std::string_view, char>::format(
            std::string_view(str.head, str.n), ctx);
    }
};

template <>
struct std::formatter<rbox::meta_wstring_view, wchar_t>
    : std::formatter<std::wstring_view, wchar_t> {
    template <class FormatContext>
    auto format(rbox::meta_wstring_view str, FormatContext& ctx) const -> FormatContext::iterator
    {
        return std::formatter<std::wstring_view, wchar_t>::format(
            std::wstring_view(str.head, str.n), ctx);
    }
};

#if __has_include(<fmt/format.h>)
template <>
struct fmt::formatter<rbox::meta_string_view, char> : fmt::formatter<std::string_view, char> {
    template <class FormatContext>
    auto format(rbox::meta_string_view str, FormatContext& ctx) const -> FormatContext::iterator
    {
        return fmt::formatter<std::string_view, char>::format(
            std::string_view(str.head, str.n), ctx);
    }
};

template <>
struct fmt::formatter<rbox::meta_wstring_view, wchar_t>
    : fmt::formatter<std::wstring_view, wchar_t> {
    template <class FormatContext>
    auto format(rbox::meta_wstring_view str, FormatContext& ctx) const -> FormatContext::iterator
    {
        return fmt::formatter<std::wstring_view, wchar_t>::format(
            std::wstring_view(str.head, str.n), ctx);
    }
};
#endif

#endif  // RBOX_UTILS_FORMAT_HPP
