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

#ifndef RBOX_UTILS_STRING_ENCODING_ENCODE_CODE_POINT_HPP
#define RBOX_UTILS_STRING_ENCODING_ENCODE_CODE_POINT_HPP

#include <rbox/utils/string_encoding/common.hpp>
#include <rbox/utils/string_encoding/impl/helpers.hpp>

namespace rbox {
// -------- Encoding Single Code Point --------

#define RBOX_CHECK_BUFFER_SIZE(n)                           \
    if constexpr (Checks) {                                 \
        if (dest + (n) > dest_end) [[unlikely]] {           \
            return {dest, encoding_status::buffer_run_out}; \
        }                                                   \
    }
#define RBOX_CHECK_CODE_POINT(code_point)                      \
    if constexpr (Checks) {                                    \
        if (!is_valid_code_point(code_point)) [[unlikely]] {   \
            return {dest, encoding_status::invalid_character}; \
        }                                                      \
    }

namespace impl {
template <bool Checks>
constexpr auto encode_code_point_impl(char8_t* dest, const char8_t* dest_end, char32_t code_point)
    -> std::pair<char8_t*, encoding_status>
{
    // Assumes when Checks == false: (1) Buffer size is enough; (2) code_point is valid
    RBOX_CHECK_CODE_POINT(code_point);
    // (1) ASCII
    if (code_point < 0x80) {
        RBOX_CHECK_BUFFER_SIZE(1);
        *dest = static_cast<char8_t>(code_point);
        return {dest + 1, encoding_status::done};
    }
    // (2) 2-byte sequence
    if (code_point < 0x800) {
        RBOX_CHECK_BUFFER_SIZE(2);
        dest[0] = static_cast<char8_t>(0xC0 | (code_point >> 6));
        dest[1] = static_cast<char8_t>(0x80 | (code_point & 0x3F));
        return {dest + 2, encoding_status::done};
    }
    // (3) 3-byte sequence
    if (code_point < 0x10000) {
        RBOX_CHECK_BUFFER_SIZE(3);
        dest[0] = static_cast<char8_t>(0xE0 | (code_point >> 12));
        dest[1] = static_cast<char8_t>(0x80 | ((code_point >> 6) & 0x3F));
        dest[2] = static_cast<char8_t>(0x80 | (code_point & 0x3F));
        return {dest + 3, encoding_status::done};
    }
    // (4) 4-byte sequence
    RBOX_CHECK_BUFFER_SIZE(4);
    dest[0] = static_cast<char8_t>(0xF0 | (code_point >> 18));
    dest[1] = static_cast<char8_t>(0x80 | ((code_point >> 12) & 0x3F));
    dest[2] = static_cast<char8_t>(0x80 | ((code_point >> 6) & 0x3F));
    dest[3] = static_cast<char8_t>(0x80 | (code_point & 0x3F));
    return {dest + 4, encoding_status::done};
}

template <bool Checks>
constexpr auto encode_code_point_impl(char16_t* dest, const char16_t* dest_end, char32_t code_point)
    -> std::pair<char16_t*, encoding_status>
{
    // Assumes when Checks == false: (1) Buffer size is enough; (2) code_point is valid
    RBOX_CHECK_CODE_POINT(code_point);
    // (1) BMP (Basic Multilingual Plane)
    if (code_point <= 0xFFFF) {
        RBOX_CHECK_BUFFER_SIZE(1);
        *dest = static_cast<char16_t>(code_point);
        return {dest + 1, encoding_status::done};
    }
    // (2) Surrogate pair
    RBOX_CHECK_BUFFER_SIZE(2);
    code_point -= 0x10000;
    auto high = static_cast<char16_t>(0xD800 + (code_point >> 10));
    auto low = static_cast<char16_t>(0xDC00 + (code_point & 0x3FF));
    dest[0] = high;
    dest[1] = low;
    return {dest + 2, encoding_status::done};
}

template <bool Checks>
constexpr auto encode_code_point_impl(char32_t* dest, const char32_t* dest_end, char32_t code_point)
    -> std::pair<char32_t*, encoding_status>
{
    // Assumes when Checks == false: (1) Buffer size is enough; (2) code_point is valid
    RBOX_CHECK_CODE_POINT(code_point);
    RBOX_CHECK_BUFFER_SIZE(1);
    *dest = code_point;
    return {dest + 1, encoding_status::done};
}

template <bool Checks, class CharT>
constexpr auto encode_code_point_impl(CharT* dest, const CharT* dest_end, char32_t code_point)
    -> std::pair<CharT*, encoding_status>
{
    using canonical_char_type = canonical_char_type_t<CharT>;
    if consteval {
        if (dest_end == nullptr) {
            canonical_char_type buffer[4];
            auto [p, s] = encode_code_point_impl<Checks>(buffer, buffer + 4, code_point);
            auto* t = dest;
            for (auto* s = buffer; s != p; ++s) {
                *t++ = static_cast<CharT>(*s);
            }
            return {t, s};
        } else {
            auto n = dest_end - dest;
            auto buffer = new canonical_char_type[n];
            auto [p, s] = encode_code_point_impl<Checks>(buffer, buffer + n, code_point);
            auto* t = dest;
            for (auto* s = buffer; s != p; ++s) {
                *t++ = static_cast<CharT>(*s);
            }
            delete[] buffer;
            return {t, s};
        }
    } else {
        auto [p, s] = encode_code_point_impl<Checks>(
            reinterpret_cast<canonical_char_type*>(dest),
            reinterpret_cast<const canonical_char_type*>(dest_end),
            code_point);
        return {reinterpret_cast<CharT*>(p), s};
    }
}
}  // namespace impl

#define RBOX_ENCODING_FN(fn, out_concept)                                                         \
    struct fn##_t {                                                                               \
        template <impl::out_concept CharT>                                                        \
        static constexpr auto operator()(CharT* dest, const CharT* dest_end, char32_t code_point) \
            -> std::pair<CharT*, encoding_status>                                                 \
        {                                                                                         \
            return impl::encode_code_point_impl<true>(dest, dest_end, code_point);                \
        }                                                                                         \
    };                                                                                            \
                                                                                                  \
    struct fn##_unsafe_t {                                                                        \
        template <impl::out_concept CharT>                                                        \
        static constexpr auto operator()(CharT* dest, const CharT* dest_end, char32_t code_point) \
            -> CharT*                                                                             \
        {                                                                                         \
            auto [written_until, _] =                                                             \
                impl::encode_code_point_impl<false>(dest, dest_end, code_point);                  \
            return written_until;                                                                 \
        }                                                                                         \
                                                                                                  \
        template <impl::out_concept CharT>                                                        \
        static constexpr auto operator()(CharT* dest, char32_t code_point) -> CharT*              \
        {                                                                                         \
            auto [written_until, _] = impl::encode_code_point_impl<false>(                        \
                dest, static_cast<const CharT*>(nullptr), code_point);                            \
            return written_until;                                                                 \
        }                                                                                         \
    };                                                                                            \
                                                                                                  \
    constexpr auto fn = fn##_t{};                                                                 \
    constexpr auto fn##_unsafe = fn##_unsafe_t{};

RBOX_ENCODING_FN(encode_code_point, any_utf_input)
RBOX_ENCODING_FN(encode_code_point_to_utf8, one_byte_utf_input)
RBOX_ENCODING_FN(encode_code_point_to_utf16, two_byte_utf_input)

#undef RBOX_ENCODING_FN
#undef RBOX_CHECK_BUFFER_SIZE
#undef RBOX_CHECK_CODE_POINT
}  // namespace rbox

#endif  // RBOX_UTILS_STRING_ENCODING_ENCODE_CODE_POINT_HPP
