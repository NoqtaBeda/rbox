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

extern "C" {
// Linear search (sparse)
[[gnu::visibility("default")]]
double test_sparse_linear(int64_t key)
{
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(
        std::initializer_list<std::pair<int64_t, double>>{
            {-100, 1.25},
            {200, 3.5},
            {-300, 5.75},
            {400, 7.0},
            {-500, 8.25},
        },
        {.binary_search_threshold = 100});

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return NAN;
}

// Binary search (sparse)
[[gnu::visibility("default")]]
int64_t test_sparse_binary(uint64_t key)
{
    using Value = std::pair<uint64_t, int64_t>;
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(
        std::initializer_list<std::pair<uint64_t, Value>>{
            {static_cast<uint64_t>(-200), {12, -12}},
            {static_cast<uint64_t>(-100), {24, -24}},
            {100, {36, -36}},
            {300, {48, -48}},
            {500, {60, -60}},
        },
        rbox::integral_key_fixed_map_options{
            .binary_search_threshold = 1,
        });

    if (auto opt = map.find(key); opt) {
        return opt->first;
    }
    return -1;
}
}  // extern "C"
