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

#include <rbox/fixed_map/integral_key.hpp>

extern "C" {
// Signed integral key dense (non-null type)
[[gnu::visibility("default")]]
const char* test_dense_signed(char key)
{
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(
        std::initializer_list<std::pair<char, const char*>>{
            {'a', "Apple"},
            {'b', "Banana"},
            {'c', "Cat"},
            {'d', "Dog"},
            {'h', "Hedgedog"},
            {'i', "Ice"},
            {'j', "John"},
        },
        rbox::integral_key_fixed_map_options{
            .already_sorted = true,
            .already_unique = true,
            .min_load_factor = 0.5,
        });

    if (auto opt = map.find(key); opt) {
        return opt->head;
    }
    return nullptr;
}

// Signed integral key dense with zero value (non-null type)
[[gnu::visibility("default")]]
int test_dense_signed_zero_val(int8_t key)
{
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(
        std::initializer_list<std::pair<int8_t, int8_t>>{
            {-3, -10},
            {-2, 0},
            {-1, 10},
            {0, -20},
            {1, 0},
            {3, 30},
        },
        rbox::integral_key_fixed_map_options{
            .already_sorted = true,
            .already_unique = true,
            .min_load_factor = 0.5,
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -128;
}

// Unsigned integral key dense (non-null type)
[[gnu::visibility("default")]]
const char* test_dense_unsigned(uint8_t key)
{
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(
        std::initializer_list<std::pair<uint8_t, const char*>>{
            {126, "Apple"},
            {128, "Banana"},
            {130, "Cat"},
            {132, "Dog"},
            {134, "Hedgedog"},
            {136, "Ice"},
        },
        rbox::integral_key_fixed_map_options{
            .min_load_factor = 0.5,
        });

    if (auto opt = map.find(key); opt) {
        return opt->head;
    }
    return nullptr;
}
}  // extern "C"
