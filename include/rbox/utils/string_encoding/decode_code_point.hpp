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

#ifndef RBOX_UTILS_STRING_ENCODING_DECODE_CODE_POINT_HPP
#define RBOX_UTILS_STRING_ENCODING_DECODE_CODE_POINT_HPP

#include <rbox/utils/string_encoding/common.hpp>
#include <rbox/utils/string_encoding/impl/helpers.hpp>

namespace rbox {
// -------- Decoding Single Code Point --------

// Unicode 11 UTF-8 Encoding Rules (Chapter 3, Table 3-7):
// +------------------+----------+----------+----------+----------+
// | Code Point Range | 1st Byte | 2nd Byte | 3rd Byte | 4th Byte |
// +------------------+----------+----------+----------+----------+
// | U+0000-U+007F    | 00-7F    | -        | -        | -        |
// +------------------+----------+----------+----------+----------+
// | U+0080-U+07FF    | C2-DF    | 80-BF    | -        | -        |
// +------------------+----------+----------+----------+----------+
// | U+0800-U+0FFF    | E0       | A0-BF    | 80-BF    | -        |
// | U+1000-U+CFFF    | E1-EC    | 80-BF    | 80-BF    | -        |
// | U+D000-U+DFFF    | ED       | 80-9F    | 80-BF    | -        |
// | U+E000-U+FFFF    | EE-EF    | 80-BF    | 80-BF    | -        |
// +------------------+----------+----------+----------+----------+
// | U+10000-U+3FFFF  | F0       | 90-BF    | 80-BF    | 80-BF    |
// | U+40000-U+FFFFF  | F1-F3    | 80-BF    | 80-BF    | 80-BF    |
// | U+100000-U+10FFFF| F4       | 80-8F    | 80-BF    | 80-BF    |
// +------------------+----------+----------+----------+----------+

namespace impl {
constexpr auto decode_code_point_impl(const char8_t* input, const char8_t* input_end)
    -> std::pair<char32_t, const char8_t*>
{
    if (input >= input_end) [[unlikely]] {
        return {invalid_code_point, input};
    }

    const char8_t* original = input;
    auto c0 = static_cast<uint8_t>(*input++);
    if (c0 < 0x80) {
        return {static_cast<char32_t>(c0), input};
    }
    if (c0 < 0xC0) [[unlikely]] {
        return {invalid_code_point, original};
    }

    char32_t code_point = 0;
    size_t num_continuations = 0;

    if (c0 < 0xE0) {
        code_point = c0 & 0x1F;
        num_continuations = 1;
        if (c0 < 0xC2) [[unlikely]] {
            return {invalid_code_point, original};
        }
    } else if (c0 < 0xF0) {
        code_point = c0 & 0x0F;
        num_continuations = 2;
    } else if (c0 < 0xF8) {
        code_point = c0 & 0x07;
        num_continuations = 3;
        if (c0 > 0xF4) [[unlikely]] {
            return {invalid_code_point, original};
        }
    } else [[unlikely]] {
        return {invalid_code_point, original};
    }

    if (input + num_continuations > input_end) [[unlikely]] {
        return {invalid_code_point, original};
    }
    for (size_t i = 0; i < num_continuations; ++i) {
        auto c = static_cast<uint8_t>(*input++);
        if ((c & 0xC0) != 0x80) [[unlikely]] {
            return {invalid_code_point, original};
        }
        code_point = (code_point << 6) | (c & 0x3F);
    }

    if (num_continuations == 2) {
        if (code_point < 0x800 || (code_point >= 0xD800 && code_point <= 0xDFFF)) [[unlikely]] {
            return {invalid_code_point, original};
        }
    } else if (num_continuations == 3) {
        if (code_point < 0x10000 || code_point > 0x10FFFF) [[unlikely]] {
            return {invalid_code_point, original};
        }
    }
    return {code_point, input};
}

constexpr auto decode_code_point_impl(const char16_t* input, const char16_t* input_end)
    -> std::pair<char32_t, const char16_t*>
{
    if (input >= input_end) [[unlikely]] {
        return {invalid_code_point, input};
    }

    const char16_t* original = input;
    auto c0 = static_cast<char16_t>(*input++);
    if (c0 < 0xD800 || c0 > 0xDFFF) {
        return {static_cast<char32_t>(c0), input};
    }
    if (c0 >= 0xDC00) [[unlikely]] {
        return {invalid_code_point, original};
    }
    if (input >= input_end) [[unlikely]] {
        return {invalid_code_point, original};
    }

    auto c1 = static_cast<char16_t>(*input++);
    if (c1 < 0xDC00 || c1 > 0xDFFF) [[unlikely]] {
        return {invalid_code_point, original};
    }

    auto code_point = 0x10000 + ((char32_t(c0) - 0xD800) << 10) + (char32_t(c1) - 0xDC00);
    return {static_cast<char32_t>(code_point), input};
}

constexpr auto decode_code_point_impl(const char32_t* input, const char32_t* input_end)
    -> std::pair<char32_t, const char32_t*>
{
    if (input >= input_end || !is_valid_code_point(*input)) [[unlikely]] {
        return {invalid_code_point, input};
    }
    return {*input, input + 1};
}

template <class CharT>
constexpr auto decode_code_point_impl(const CharT* input, const CharT* input_end)
    -> std::pair<char32_t, const CharT*>
{
    using canonical_char_type = impl::canonical_char_type_t<CharT>;
    if consteval {
        if (input_end == nullptr) {
            auto* q = input;
            while (*q) ++q;
            input_end = q;
        }
        auto n = input_end - input;
        auto buffer = new canonical_char_type[n];
        for (auto i = 0z; i < n; ++i) {
            buffer[i] = static_cast<canonical_char_type>(input[i]);
        }
        auto [c, p] = decode_code_point_impl(buffer, buffer + n);
        auto d = p - buffer;
        delete[] buffer;
        return {c, input + d};
    } else {
        auto [c, p] = decode_code_point_impl(
            reinterpret_cast<const canonical_char_type*>(input),
            reinterpret_cast<const canonical_char_type*>(input_end));
        return {c, reinterpret_cast<const CharT*>(p)};
    }
}
}  // namespace impl

#define RBOX_DECODE_FN(fn, in_concept)                                               \
    struct fn##_t {                                                                  \
        template <impl::in_concept CharT>                                            \
        static constexpr auto operator()(const CharT* input, const CharT* input_end) \
            -> std::pair<char32_t, const CharT*>                                     \
        {                                                                            \
            return impl::decode_code_point_impl(input, input_end);                   \
        }                                                                            \
    };                                                                               \
    constexpr auto fn = fn##_t{};

RBOX_DECODE_FN(decode_code_point, any_utf_input)
RBOX_DECODE_FN(decode_code_point_from_utf8, one_byte_utf_input)
RBOX_DECODE_FN(decode_code_point_from_utf16, two_byte_utf_input)

#undef RBOX_DECODE_FN
}  // namespace rbox

#endif  // RBOX_UTILS_STRING_ENCODING_DECODE_CODE_POINT_HPP
