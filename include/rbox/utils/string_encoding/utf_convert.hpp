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

#ifndef RBOX_UTILS_STRING_ENCODING_UTF_CONVERT_HPP
#define RBOX_UTILS_STRING_ENCODING_UTF_CONVERT_HPP

#include <rbox/utils/string_encoding/common.hpp>
#include <rbox/utils/string_encoding/decode_code_point.hpp>
#include <rbox/utils/string_encoding/encode_code_point.hpp>
#include <rbox/utils/string_encoding/escaping.hpp>
#include <rbox/utils/string_encoding/impl/helpers.hpp>

namespace rbox {
// -------- UTF Conversion --------

template <class OutT, class InT>
struct encode_result_t {
    OutT* out_ptr;
    const InT* in_ptr;
    encoding_status status;
};

namespace impl {
template <escaping_mode Mode>
constexpr auto utf_convert_impl(
    char16_t* dest, const char16_t* dest_end, const char8_t* input, const char8_t* input_end)
    -> encode_result_t<char16_t, char8_t>
{
    while (input < input_end) {
        if constexpr (Mode != escaping_mode::none) {
            auto [esc_status, next_dest] =
                write_escaped_character_by<Mode>(dest, dest_end, static_cast<char32_t>(*input));
            if (esc_status == escaping_status::done) [[unlikely]] {
                dest = next_dest;
                input += 1;
                continue;
            } else if (esc_status == escaping_status::error) [[unlikely]] {
                return {dest, input, encoding_status::buffer_run_out};
            }
        }
        auto [code_point, next_input] = decode_code_point_from_utf8(input, input_end);
        if (code_point == invalid_code_point) [[unlikely]] {
            return {dest, input, encoding_status::invalid_character};
        }
        size_t needed = impl::utf16_encoded_length(code_point);
        if (dest + needed > dest_end) [[unlikely]] {
            return {dest, input, encoding_status::buffer_run_out};
        }
        dest = encode_code_point_to_utf16_unsafe(dest, code_point);
        input = next_input;
    }
    return {dest, input, encoding_status::done};
}

template <escaping_mode Mode>
constexpr auto utf_convert_impl(
    char32_t* dest, const char32_t* dest_end, const char8_t* input, const char8_t* input_end)
    -> encode_result_t<char32_t, char8_t>
{
    while (input < input_end) {
        if (dest >= dest_end) [[unlikely]] {
            return {dest, input, encoding_status::buffer_run_out};
        }
        if constexpr (Mode != escaping_mode::none) {
            auto [esc_status, next_dest] =
                write_escaped_character_by<Mode>(dest, dest_end, static_cast<char32_t>(*input));
            if (esc_status == escaping_status::done) [[unlikely]] {
                dest = next_dest;
                input += 1;
                continue;
            } else if (esc_status == escaping_status::error) [[unlikely]] {
                return {dest, input, encoding_status::buffer_run_out};
            }
        }
        auto [code_point, next_input] = decode_code_point_from_utf8(input, input_end);
        if (code_point == invalid_code_point) [[unlikely]] {
            return {dest, input, encoding_status::invalid_character};
        }
        *dest++ = code_point;
        input = next_input;
    }
    return {dest, input, encoding_status::done};
}

template <escaping_mode Mode>
constexpr auto utf_convert_impl(
    char8_t* dest, const char8_t* dest_end, const char16_t* input, const char16_t* input_end)
    -> encode_result_t<char8_t, char16_t>
{
    while (input < input_end) {
        if constexpr (Mode != escaping_mode::none) {
            auto [esc_status, esc_dest] =
                write_escaped_character_by<Mode>(dest, dest_end, static_cast<char32_t>(*input));
            if (esc_status == escaping_status::done) [[unlikely]] {
                dest = esc_dest;
                input += 1;
                continue;
            } else if (esc_status == escaping_status::error) [[unlikely]] {
                return {dest, input, encoding_status::buffer_run_out};
            }
        }
        auto [code_point, next_input] = decode_code_point_from_utf16(input, input_end);
        if (code_point == invalid_code_point) [[unlikely]] {
            return {dest, input, encoding_status::invalid_character};
        }
        auto needed = impl::utf8_encoded_length(code_point);
        if (dest + needed > dest_end) [[unlikely]] {
            return {dest, input, encoding_status::buffer_run_out};
        }
        dest = encode_code_point_to_utf8_unsafe(dest, code_point);
        input = next_input;
    }
    return {dest, input, encoding_status::done};
}

template <escaping_mode Mode>
constexpr auto utf_convert_impl(
    char32_t* dest, const char32_t* dest_end, const char16_t* input, const char16_t* input_end)
    -> encode_result_t<char32_t, char16_t>
{
    while (input < input_end) {
        if (dest >= dest_end) [[unlikely]] {
            return {dest, input, encoding_status::buffer_run_out};
        }
        if constexpr (Mode != escaping_mode::none) {
            auto [esc_status, esc_dest] =
                write_escaped_character_by<Mode>(dest, dest_end, static_cast<char32_t>(*input));
            if (esc_status == escaping_status::done) [[unlikely]] {
                dest = esc_dest;
                input += 1;
                continue;
            } else if (esc_status == escaping_status::error) [[unlikely]] {
                return {dest, input, encoding_status::buffer_run_out};
            }
        }
        auto [code_point, next_input] = decode_code_point_from_utf16(input, input_end);
        if (code_point == invalid_code_point) [[unlikely]] {
            return {dest, input, encoding_status::invalid_character};
        }
        *dest++ = code_point;
        input = next_input;
    }
    return {dest, input, encoding_status::done};
}

template <escaping_mode Mode>
constexpr auto utf_convert_impl(
    char8_t* dest, const char8_t* dest_end, const char32_t* input, const char32_t* input_end)
    -> encode_result_t<char8_t, char32_t>
{
    for (; input < input_end; ++input) {
        auto code_point = *input;
        if (!is_valid_code_point(code_point)) [[unlikely]] {
            return {dest, input, encoding_status::invalid_character};
        }
        if constexpr (Mode != escaping_mode::none) {
            auto [esc_status, esc_dest] =
                write_escaped_character_by<Mode>(dest, dest_end, code_point);
            if (esc_status == escaping_status::done) [[unlikely]] {
                dest = esc_dest;
                continue;
            } else if (esc_status == escaping_status::error) [[unlikely]] {
                return {dest, input, encoding_status::buffer_run_out};
            }
        }
        auto needed = impl::utf8_encoded_length(code_point);
        if (dest + needed > dest_end) [[unlikely]] {
            return {dest, input, encoding_status::buffer_run_out};
        }
        dest = encode_code_point_to_utf8_unsafe(dest, code_point);
    }
    return {dest, input, encoding_status::done};
}

template <escaping_mode Mode>
constexpr auto utf_convert_impl(
    char16_t* dest, const char16_t* dest_end, const char32_t* input, const char32_t* input_end)
    -> encode_result_t<char16_t, char32_t>
{
    for (; input < input_end; ++input) {
        auto code_point = *input;
        if (!is_valid_code_point(code_point)) [[unlikely]] {
            return {dest, input, encoding_status::invalid_character};
        }
        if constexpr (Mode != escaping_mode::none) {
            auto [esc_status, esc_dest] =
                write_escaped_character_by<Mode>(dest, dest_end, code_point);
            if (esc_status == escaping_status::done) [[unlikely]] {
                dest = esc_dest;
                continue;
            } else if (esc_status == escaping_status::error) [[unlikely]] {
                return {dest, input, encoding_status::buffer_run_out};
            }
        }
        auto needed = impl::utf16_encoded_length(code_point);
        if (dest + needed > dest_end) [[unlikely]] {
            return {dest, input, encoding_status::buffer_run_out};
        }
        dest = encode_code_point_to_utf16_unsafe(dest, code_point);
    }
    return {dest, input, encoding_status::done};
}

template <escaping_mode Mode, class OutT, class InT>
constexpr auto utf_convert_impl(
    OutT* dest, const OutT* dest_end, const InT* input, const InT* input_end)
    -> encode_result_t<OutT, InT>
{
    using canonical_out_type = canonical_char_type_t<OutT>;
    using canonical_in_type = canonical_char_type_t<InT>;
    if consteval {
        auto dest_n = dest_end - dest;
        auto input_n = input_end - input;
        auto dest_buffer = new canonical_out_type[dest_n];
        auto input_buffer = new canonical_in_type[input_n];
        for (auto i = 0z; i < input_n; ++i) {
            input_buffer[i] = static_cast<canonical_in_type>(input[i]);
        }
        auto [po, pi, s] = utf_convert_impl<Mode>(
            dest_buffer, dest_buffer + dest_n, input_buffer, input_buffer + input_n);
        auto* ro = dest;
        for (auto* s = dest_buffer; s != po; ++s) {
            *ro++ = static_cast<OutT>(*s);
        }
        auto* ri = input + (pi - input_buffer);
        delete[] dest_buffer;
        delete[] input_buffer;
        return {ro, ri, s};
    } else {
        auto res = utf_convert_impl<Mode>(
            reinterpret_cast<canonical_out_type*>(dest),
            reinterpret_cast<const canonical_out_type*>(dest_end),
            reinterpret_cast<const canonical_in_type*>(input),
            reinterpret_cast<const canonical_in_type*>(input_end));
        return {
            reinterpret_cast<OutT*>(res.out_ptr),
            reinterpret_cast<const InT*>(res.in_ptr),
            res.status};
    }
}
}  // namespace impl

#define RBOX_ENCODING_FN(fn, in_concept, out_concept)                                 \
    template <escaping_mode Mode>                                                     \
    struct fn##_by_t {                                                                \
        template <impl::out_concept OutT, impl::in_concept InT>                       \
        static constexpr auto operator()(                                             \
            OutT* dest, const OutT* dest_end, const InT* input, const InT* input_end) \
            -> encode_result_t<OutT, InT>                                             \
        {                                                                             \
            return impl::utf_convert_impl<Mode>(dest, dest_end, input, input_end);    \
        }                                                                             \
    };                                                                                \
    template <escaping_mode Mode>                                                     \
    using fn##_by_t = fn##_by_t<Mode>;                                                \
    template <escaping_mode Mode>                                                     \
    constexpr auto fn##_by = fn##_by_t<Mode>{};                                       \
                                                                                      \
    using fn##_t = fn##_by_t<escaping_mode::none>;                                    \
    constexpr auto fn = fn##_by_t<escaping_mode::none>{};

RBOX_ENCODING_FN(utf8_to_utf16, one_byte_utf_input, two_byte_utf_input)
RBOX_ENCODING_FN(utf8_to_utf32, one_byte_utf_input, four_byte_utf_input)
RBOX_ENCODING_FN(utf16_to_utf8, two_byte_utf_input, one_byte_utf_input)
RBOX_ENCODING_FN(utf16_to_utf32, two_byte_utf_input, four_byte_utf_input)
RBOX_ENCODING_FN(utf32_to_utf8, four_byte_utf_input, one_byte_utf_input)
RBOX_ENCODING_FN(utf32_to_utf16, four_byte_utf_input, two_byte_utf_input)
RBOX_ENCODING_FN(utf_convert, any_utf_input, any_utf_input)  // Dispatcher

#undef RBOX_ENCODING_FN
}  // namespace rbox

#endif  // RBOX_UTILS_STRING_ENCODING_UTF_CONVERT_HPP
