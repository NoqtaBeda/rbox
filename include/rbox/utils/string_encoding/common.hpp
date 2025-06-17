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

#ifndef RBOX_UTILS_STRING_ENCODING_COMMON_HPP
#define RBOX_UTILS_STRING_ENCODING_COMMON_HPP

namespace rbox {
enum class escaping_mode {
    none = 0,
    json = 1,            // Following RFC 8259 standard
    display_char = 2,    // Diaplaying character values during local debugging
    display_string = 3,  // Diaplaying string values during local debugging
};

enum class escaping_status {
    done = 0,
    no_escape = 1,
    error = 2,
};

enum class encoding_status {
    done = 0,
    invalid_character = 1,
    buffer_run_out = 2,
};

constexpr auto invalid_code_point = static_cast<char32_t>(-1);
constexpr auto replacement_code_point = static_cast<char32_t>(0xFFFD);  // '�'

// -------- Code Point Validation --------

constexpr bool is_valid_code_point(char32_t code_point)
{
    return code_point <= 0xD7FF || (code_point >= 0xE000 && code_point <= 0x10FFFF);
}
}  // namespace rbox

#endif  // RBOX_UTILS_STRING_ENCODING_COMMON_HPP
