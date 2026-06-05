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

#include <cmath>
#include <rbox/fixed_map/integral_key.hpp>

enum opcode {
    opcode_nop = -1000,
    opcode_invalid = -900,
    opcode_placeholder = -800,
    opcode_mov = 0,
    opcode_add = 1,
    opcode_sub = 2,
    opcode_mul = 3,
    opcode_div = 4,
    opcode_rdiv = 5,
    opcode_mod = 6,
    opcode_bit_and = 10,
    opcode_bit_or = 20,
    opcode_bit_xor = 40,
    opcode_bit_shl = 80,
    opcode_bit_shr = 160,
};

extern "C" {
[[gnu::visibility("default")]]
double test_unscoped_enum(int key)
{
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(
        std::initializer_list<std::pair<opcode, double>>{
            {opcode_mov, 0.25},
            {opcode_add, 0.5},
            {opcode_sub, 0.75},
            {opcode_mul, 1.25},
            {opcode_div, 1.5},
            {opcode_rdiv, 1.75},
            {opcode_mod, 2.25},
            {opcode_bit_and, 2.5},
            {opcode_bit_or, 2.75},
            {opcode_bit_xor, 3.25},
            {opcode_bit_shl, 3.5},
            {opcode_bit_shr, 3.75},
            {opcode_nop, -10.25},
            {opcode_invalid, -10.5},
            {opcode_placeholder, -10.75},
        },
        rbox::integral_key_fixed_map_options{
            .min_load_factor = 0.9,
            .binary_search_threshold = 4,
        });

    if (auto opt = map.find(static_cast<opcode>(key)); opt) {
        return *opt;
    }
    return NAN;
}
}  // extern "C"
