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

#ifndef RBOX_UTILS_STRING_ENCODING_CONSUME_INVALID_SEQUENCE_HPP
#define RBOX_UTILS_STRING_ENCODING_CONSUME_INVALID_SEQUENCE_HPP

#include <rbox/utils/string_encoding/common.hpp>
#include <rbox/utils/string_encoding/impl/helpers.hpp>

namespace rbox {
// -------- Consuming Invalid UTF Sequence --------

// Consumes the maximal prefix of continuous invalid UTF-8, UTF-16 or UTF-32 units.
// Let ret be the returned pointer, [input, ret) is expected to be replaced as a single
// placeholder '�' according to Unicode 11 or later standard.
//
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
constexpr auto consume_invalid_sequence_impl(const char8_t* input, const char8_t* input_end)
    -> const char8_t*
{
    for (; input < input_end; ++input) {
        auto c = static_cast<uint8_t>(*input);
        if (c < 0x80) {
            return input;  // Valid ASCII character, stop here
        }
        if (c <= 0xC1 || c >= 0xF5) {
            continue;  // Invalid leading byte, continue consuming
        }
        auto invalid = false;
        if (c <= 0xDF) {
            invalid = (input + 2 > input_end)                // Truncated
                   || (input[1] < 0x80 || input[1] > 0xBF);  // Invalid 2nd byte
        } else if (c == 0xE0) {
            invalid = (input + 3 > input_end)                // Truncated
                   || (input[1] < 0xA0 || input[1] > 0xBF)   // Overlong: < U+0800
                   || (input[2] < 0x80 || input[2] > 0xBF);  // Invalid 3rd byte
        } else if (c >= 0xE1 && c <= 0xEC) {
            invalid = (input + 3 > input_end)                // Truncated
                   || (input[1] < 0x80 || input[1] > 0xBF)   // Invalid 2nd byte
                   || (input[2] < 0x80 || input[2] > 0xBF);  // Invalid 3rd byte
        } else if (c == 0xED) {
            invalid = (input + 3 > input_end)                // Truncated
                   || (input[1] < 0x80 || input[1] > 0x9F)   // Surrogates: U+D800-U+DFFF
                   || (input[2] < 0x80 || input[2] > 0xBF);  // Invalid 3rd byte
        } else if (c >= 0xEE && c <= 0xEF) {
            invalid = (input + 3 > input_end)                // Truncated
                   || (input[1] < 0x80 || input[1] > 0xBF)   // Invalid 2nd byte
                   || (input[2] < 0x80 || input[2] > 0xBF);  // Invalid 3rd byte
        } else if (c == 0xF0) {
            invalid = (input + 4 > input_end)                // Truncated
                   || (input[1] < 0x90 || input[1] > 0xBF)   // Overlong: < U+10000
                   || (input[2] < 0x80 || input[2] > 0xBF)   // Invalid 3rd byte
                   || (input[3] < 0x80 || input[3] > 0xBF);  // Invalid 4th byte
        } else if (c >= 0xF1 && c <= 0xF3) {
            invalid = (input + 4 > input_end)                // Truncated
                   || (input[1] < 0x80 || input[1] > 0xBF)   // Invalid 2nd byte
                   || (input[2] < 0x80 || input[2] > 0xBF)   // Invalid 3rd byte
                   || (input[3] < 0x80 || input[3] > 0xBF);  // Invalid 4th byte
        } else if (c == 0xF4) {
            invalid = (input + 4 > input_end)                // Truncated
                   || (input[1] < 0x80 || input[1] > 0x8F)   // > U+10FFFF
                   || (input[2] < 0x80 || input[2] > 0xBF)   // Invalid 3rd byte
                   || (input[3] < 0x80 || input[3] > 0xBF);  // Invalid 4th byte
        }
        if (!invalid) {
            return input;
        }
    }
    return input;
}

constexpr auto consume_invalid_sequence_impl(const char16_t* input, const char16_t* input_end)
    -> const char16_t*
{
    for (; input < input_end; ++input) {
        auto c = static_cast<char16_t>(*input);
        auto is_high_surrogate = impl::is_high_surrogate(c);
        auto is_low_surrogate = impl::is_low_surrogate(c);

        if (!is_high_surrogate && !is_low_surrogate) {
            break;  // (1) valid BMP character
        }
        if (is_low_surrogate) {
            continue;  // (2) Orphaned low surrogate (0xDC00-0xDFFF)
        }
        if (is_high_surrogate) {
            if (input + 1 < input_end && impl::is_low_surrogate(input[1])) {
                break;  // (3) Valid surrogate pair (high followed by low)
            }
            // (4) Orphaned high surrogate (0xD800-0xDBFF) without low
        }
    }
    return input;
}

constexpr auto consume_invalid_sequence_impl(const char32_t* input, const char32_t* input_end)
    -> const char32_t*
{
    for (; input < input_end; ++input) {
        if (is_valid_code_point(*input)) break;
    }
    return input;
}

template <class CharT>
constexpr auto consume_invalid_sequence_impl(const CharT* input, const CharT* input_end)
    -> const CharT*
{
    using canonical_char_type = canonical_char_type_t<CharT>;
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
        const auto* res = consume_invalid_sequence_impl(buffer, buffer + n);
        auto d = res - buffer;
        delete[] buffer;
        return input + d;
    } else {
        const auto* res = consume_invalid_sequence_impl(
            reinterpret_cast<const canonical_char_type*>(input),
            reinterpret_cast<const canonical_char_type*>(input_end));
        return reinterpret_cast<const CharT*>(res);
    }
}
}  // namespace impl

#define RBOX_CONSUMING_FN(fn, in_concept)                                            \
    struct fn##_t {                                                                  \
        template <impl::in_concept CharT>                                            \
        static constexpr auto operator()(const CharT* input, const CharT* input_end) \
            -> const CharT*                                                          \
        {                                                                            \
            return impl::consume_invalid_sequence_impl(input, input_end);            \
        }                                                                            \
    };                                                                               \
    constexpr auto fn = fn##_t{};

RBOX_CONSUMING_FN(consume_utf8_invalid_sequence, one_byte_utf_input)
RBOX_CONSUMING_FN(consume_utf16_invalid_sequence, two_byte_utf_input)
RBOX_CONSUMING_FN(consume_utf32_invalid_sequence, four_byte_utf_input)
RBOX_CONSUMING_FN(consume_utf_invalid_sequence, any_utf_input)

#undef RBOX_CONSUMING_FN
}  // namespace rbox

#endif  // RBOX_UTILS_STRING_ENCODING_CONSUME_INVALID_SEQUENCE_HPP
