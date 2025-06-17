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

#ifndef RBOX_UTILS_STRING_ENCODING_ESCAPING_HPP
#define RBOX_UTILS_STRING_ENCODING_ESCAPING_HPP

#include <rbox/utils/ctype/char_categories.hpp>
#include <rbox/utils/string_encoding/common.hpp>

namespace rbox {
namespace impl {
// Note: Escaping '/' is common practice for JSON serialization
constexpr std::pair<char, char> ascii_json_escape_map[] = {
    {'"', '"'},
    {'\\', '\\'},
    {'\b', 'b'},
    {'\f', 'f'},
    {'\n', 'n'},
    {'\r', 'r'},
    {'\t', 't'},
    {'/', '/'},
};

constexpr std::pair<char, char> ascii_display_escape_map[] = {
    {'\\', '\\'},
    {'\0', '0'},
    {'\a', 'a'},
    {'\b', 'b'},
    {'\f', 'f'},
    {'\n', 'n'},
    {'\r', 'r'},
    {'\t', 't'},
    {'\v', 'v'},
};

#define RBOX_CHECK_BUFFER_SIZE(n)                  \
    if constexpr (Checks) {                        \
        if (dest + (n) >= dest_end) [[unlikely]] { \
            return {escaping_status::error, dest}; \
        }                                          \
    }

template <escaping_mode Mode, bool Checks>
struct write_escaped_character_base_t;

template <bool Checks>
struct write_escaped_character_base_t<escaping_mode::json, Checks> {
    // Assumes when Checks == false: Buffer size is enough (>= 6)
    template <impl::any_utf_input CharT>
    static constexpr auto operator()(CharT* dest, const CharT* dest_end, char32_t c)
        -> std::pair<escaping_status, CharT*>
    {
        // Marks every branch as [[unlikely]] since escaping characters are rare in real-world text.
        template for (constexpr auto entry : impl::ascii_json_escape_map)
        {
            if (c == entry.first) [[unlikely]] {
                // Note: dest_end is unused if Checks == false
                RBOX_CHECK_BUFFER_SIZE(2);  // 2 : Length of R"(\n)", R"(\n)", etc.
                *dest++ = '\\';
                *dest++ = entry.second;
                return {escaping_status::done, dest};
            }
        }
        if (c < 0x20) [[unlikely]] {
            RBOX_CHECK_BUFFER_SIZE(6);  // 6 : Length of R"(\u00xx)"
            *dest++ = '\\';
            *dest++ = 'u';
            *dest++ = '0';
            *dest++ = '0';
            *dest++ = static_cast<CharT>(c >= 0x10 ? '1' : '0');
            auto low = c & 0xF;
            *dest++ = static_cast<CharT>(low < 10 ? '0' + low : 'A' + low - 10);
            return {escaping_status::done, dest};
        }
        // Does nothing if c is not an escaping character
        return {escaping_status::no_escape, dest};
    }
};

template <bool Checks>
struct write_escaped_character_base_t<escaping_mode::display_char, Checks> {
    // Assumes when Checks == false: Buffer size is enough (>= 4)
    template <impl::any_utf_input CharT>
    static constexpr auto operator()(CharT* dest, const CharT* dest_end, char32_t c)
        -> std::pair<escaping_status, CharT*>
    {
        // Marks every branch as [[unlikely]] since escaping characters are rare in real-world text.
        if (c == '\\') [[unlikely]] {
            // Note: dest_end is unused if Checks == false
            RBOX_CHECK_BUFFER_SIZE(2);  // 2 : Length of R"(\')" or R"(\"")"
            *dest++ = '\\';
            *dest++ = c;
            return {escaping_status::done, dest};
        }
        template for (constexpr auto entry : impl::ascii_display_escape_map)
        {
            if (c == entry.first) [[unlikely]] {
                RBOX_CHECK_BUFFER_SIZE(2);  // 2 : Length of R"(\n)", R"(\n)", etc.
                *dest++ = '\\';
                *dest++ = entry.second;
                return {escaping_status::done, dest};
            }
        }
        if (c < 0x80 && !ascii_isprint(c)) [[unlikely]] {
            RBOX_CHECK_BUFFER_SIZE(4);  // 4 : Length of R"(\x??)" where ?? are hex digits
            *dest++ = '\\';
            *dest++ = 'x';
            auto high = c >> 8;
            *dest++ = static_cast<CharT>(high < 10 ? '0' + high : 'A' + high - 10);
            auto low = c & 0xF;
            *dest++ = static_cast<CharT>(low < 10 ? '0' + low : 'A' + low - 10);
            return {escaping_status::done, dest};
        }
        // Note: Non-printable Unicode code points beyond 0x80 can not be escaped.
        return {escaping_status::no_escape, dest};
    }
};

template <bool Checks>
struct write_escaped_character_base_t<escaping_mode::display_string, Checks> {
    // Assumes when Checks == false: Buffer size is enough (>= 4)
    template <impl::any_utf_input CharT>
    static constexpr auto operator()(CharT* dest, const CharT* dest_end, char32_t c)
        -> std::pair<escaping_status, CharT*>
    {
        // Marks every branch as [[unlikely]] since escaping characters are rare in real-world text.
        if (c == '\"') [[unlikely]] {
            // Note: dest_end is unused if Checks == false
            RBOX_CHECK_BUFFER_SIZE(2);  // 2 : Length of R"(\')" or R"(\"")"
            *dest++ = '\\';
            *dest++ = c;
            return {escaping_status::done, dest};
        }
        template for (constexpr auto entry : impl::ascii_display_escape_map)
        {
            if (c == entry.first) [[unlikely]] {
                RBOX_CHECK_BUFFER_SIZE(2);  // 2 : Length of R"(\n)", R"(\n)", etc.
                *dest++ = '\\';
                *dest++ = entry.second;
                return {escaping_status::done, dest};
            }
        }
        if (c < 0x80 && !ascii_isprint(c)) [[unlikely]] {
            RBOX_CHECK_BUFFER_SIZE(4);  // 4 : Length of R"(\x??)" where ?? are hex digits
            *dest++ = '\\';
            *dest++ = 'x';
            auto high = c >> 8;
            *dest++ = static_cast<CharT>(high < 10 ? '0' + high : 'A' + high - 10);
            auto low = c & 0xF;
            *dest++ = static_cast<CharT>(low < 10 ? '0' + low : 'A' + low - 10);
            return {escaping_status::done, dest};
        }
        // Note: Non-printable Unicode code points beyond 0x80 can not be escaped.
        return {escaping_status::no_escape, dest};
    }
};

#undef RBOX_CHECK_BUFFER_SIZE
}  // namespace impl

template <escaping_mode Mode>
using write_escaped_character_by_t = impl::write_escaped_character_base_t<Mode, true>;

template <escaping_mode Mode>
struct write_escaped_character_by_unsafe_t : impl::write_escaped_character_base_t<Mode, false> {
    using base_type = impl::write_escaped_character_base_t<Mode, false>;
    using base_type::operator();

    template <impl::any_utf_input CharT>
    static constexpr auto operator()(CharT* dest, char32_t c) -> std::pair<escaping_status, CharT*>
    {
        // dest_end is unused when Checks == false
        auto dest_end = static_cast<const CharT*>(nullptr);
        return base_type::operator()(dest, dest_end, c);
    }
};

template <escaping_mode Mode>
constexpr auto write_escaped_character_by = write_escaped_character_by_t<Mode>{};

template <escaping_mode Mode>
constexpr auto write_escaped_character_by_unsafe = write_escaped_character_by_unsafe_t<Mode>{};

#define RBOX_WRITE_ESCAPED_CHARACTER_FN(fn, mode)                                   \
    using fn##_t = write_escaped_character_by_t<escaping_mode::mode>;               \
    using fn##_unsafe_t = write_escaped_character_by_unsafe_t<escaping_mode::mode>; \
    constexpr auto fn = fn##_t{};                                                   \
    constexpr auto fn##_unsafe = fn##_unsafe_t{};

RBOX_WRITE_ESCAPED_CHARACTER_FN(write_escaped_character_for_json, json)
RBOX_WRITE_ESCAPED_CHARACTER_FN(write_escaped_character_for_char, display_char)
RBOX_WRITE_ESCAPED_CHARACTER_FN(write_escaped_character_for_string, display_string)

#undef RBOX_WRITE_ESCAPED_CHARACTER_FN
}  // namespace rbox

#endif  // RBOX_UTILS_STRING_ENCODING_ESCAPING_HPP
