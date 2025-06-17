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

#ifndef RBOX_UTILS_STRING_ENCODING_IMPL_HELPERS_HPP
#define RBOX_UTILS_STRING_ENCODING_IMPL_HELPERS_HPP

#include <meta>
#include <rbox/type_traits/arithmetic_types.hpp>

namespace rbox::impl {
consteval auto canonical_char_type_of(std::meta::info T) -> std::meta::info
{
    if (size_of(T) == 1) {
        return ^^char8_t;
    } else if (size_of(T) == 2) {
        return ^^char16_t;
    } else if (size_of(T) == 4) {
        return ^^char32_t;
    } else {
        compile_error("Invalid integral type.");
    }
}

template <class T>
using canonical_char_type_t = [:canonical_char_type_of(^^T):];

template <class T>
concept one_byte_utf_input = non_bool_integral<T> && (sizeof(T) == 1);

template <class T>
concept two_byte_utf_input = non_bool_integral<T> && (sizeof(T) == 2);

template <class T>
concept four_byte_utf_input = non_bool_integral<T> && (sizeof(T) == 4);

template <class T>
concept any_utf_input = one_byte_utf_input<T> || two_byte_utf_input<T> || four_byte_utf_input<T>;

constexpr bool is_high_surrogate(char16_t c)
{
    return c >= 0xD800 && c <= 0xDBFF;
}

constexpr bool is_low_surrogate(char16_t c)
{
    return c >= 0xDC00 && c <= 0xDFFF;
}

constexpr size_t utf8_encoded_length(char32_t code_point)
{
    if (code_point < 0x80) return 1;
    if (code_point < 0x800) return 2;
    if (code_point < 0x10000) return 3;
    return 4;
}

constexpr size_t utf16_encoded_length(char32_t code_point)
{
    return (code_point <= 0xFFFF) ? 1 : 2;
}
}  // namespace rbox::impl

#endif  // RBOX_UTILS_STRING_ENCODING_IMPL_HELPERS_HPP
