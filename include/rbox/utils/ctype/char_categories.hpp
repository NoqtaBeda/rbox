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

#ifndef RBOX_UTILS_CTYPE_CHAR_CATEGORIES_HPP
#define RBOX_UTILS_CTYPE_CHAR_CATEGORIES_HPP

#include <cstdint>
#include <rbox/type_traits/arithmetic_types.hpp>

namespace rbox {
namespace impl {
constexpr uint8_t ctype_print_mask = 1u;
constexpr uint8_t ctype_space_mask = 2u;
constexpr uint8_t ctype_blank_mask = 4u;
constexpr uint8_t ctype_punct_mask = 8u;
constexpr uint8_t ctype_upper_mask = 16u;
constexpr uint8_t ctype_lower_mask = 32u;
constexpr uint8_t ctype_digit_mask = 64u;
constexpr uint8_t ctype_xdigit_mask = 128u;

constexpr uint8_t ctype_alpha_mask = ctype_upper_mask | ctype_lower_mask;
constexpr uint8_t ctype_alnum_mask = ctype_alpha_mask | ctype_digit_mask;
constexpr uint8_t ctype_graph_mask = ctype_alnum_mask | ctype_punct_mask;

constexpr uint8_t ctype_flag_table[128] = {
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x6,  0x2,  0x2,  0x2,  0x2,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x7,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,
    0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0x9,  0x9,  0x9,  0x9,  0x9,  0x9,
    0x9,  0x91, 0x91, 0x91, 0x91, 0x91, 0x91, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x9,  0x9,  0x9,  0x9,  0x9,
    0x9,  0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
    0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x9,  0x9,  0x9,  0x9,  0x0,
};
}  // namespace impl

struct ascii_isalnum_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_alnum_mask) != 0;
    }
};

struct ascii_isalpha_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_alpha_mask) != 0;
    }
};

struct ascii_islower_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_lower_mask) != 0;
    }
};

struct ascii_isupper_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_upper_mask) != 0;
    }
};

struct ascii_isdigit_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_digit_mask) != 0;
    }
};

struct ascii_isxdigit_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_xdigit_mask) != 0;
    }
};

struct ascii_isblank_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_blank_mask) != 0;
    }
};

struct ascii_iscntrl_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_print_mask) == 0;
    }
};

struct ascii_isgraph_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_graph_mask) != 0;
    }
};

struct ascii_isspace_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_space_mask) != 0;
    }
};

struct ascii_isprint_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_print_mask) != 0;
    }
};

struct ascii_ispunct_t {
    static constexpr bool operator()(non_bool_integral auto c)
    {
        return c >= 0 && c <= 127 && (impl::ctype_flag_table[c] & impl::ctype_punct_mask) != 0;
    }
};

constexpr auto ascii_isalnum = ascii_isalnum_t{};
constexpr auto ascii_isalpha = ascii_isalpha_t{};
constexpr auto ascii_islower = ascii_islower_t{};
constexpr auto ascii_isupper = ascii_isupper_t{};
constexpr auto ascii_isdigit = ascii_isdigit_t{};
constexpr auto ascii_isxdigit = ascii_isxdigit_t{};
constexpr auto ascii_isblank = ascii_isblank_t{};
constexpr auto ascii_iscntrl = ascii_iscntrl_t{};
constexpr auto ascii_isgraph = ascii_isgraph_t{};
constexpr auto ascii_isspace = ascii_isspace_t{};
constexpr auto ascii_isprint = ascii_isprint_t{};
constexpr auto ascii_ispunct = ascii_ispunct_t{};

}  // namespace rbox

#endif  // RBOX_UTILS_CTYPE_CHAR_CATEGORIES_HPP
