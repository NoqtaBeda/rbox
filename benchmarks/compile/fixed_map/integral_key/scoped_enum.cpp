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

enum class opcode : unsigned {
    nop = -1000u,
    invalid = -900u,
    placeholder = -800u,
    mov = 0,
    add = 1,
    sub = 2,
    mul = 3,
    div = 4,
    rdiv = 5,
    mod = 6,
    bit_and = 10,
    bit_or = 20,
    bit_xor = 40,
    bit_shl = 80,
    bit_shr = 160,
};

extern "C" {
[[gnu::visibility("default")]]
double test_scoped_enum(unsigned key)
{
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(
        std::initializer_list<std::pair<opcode, double>>{
            {opcode::mov, 0.25},
            {opcode::add, 0.5},
            {opcode::sub, 0.75},
            {opcode::mul, 1.25},
            {opcode::div, 1.5},
            {opcode::rdiv, 1.75},
            {opcode::mod, 0.0},
            {opcode::bit_and, 2.5},
            {opcode::bit_or, 2.75},
            {opcode::bit_xor, 3.25},
            {opcode::bit_shl, 3.5},
            {opcode::bit_shr, 3.75},
            {opcode::nop, -10.25},
            {opcode::invalid, -10.5},
            {opcode::placeholder, -10.75},
        },
        rbox::integral_key_fixed_map_options{
            .min_load_factor = 0.5,
            .binary_search_threshold = 6,
        });

    if (auto opt = map.find(static_cast<opcode>(key)); opt) {
        return *opt;
    }
    return NAN;
}
}  // extern "C"
