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

#include <climits>
#include <cmath>
#include <rbox/fixed_map/integral_key.hpp>

extern "C" {
// int8 full range
[[gnu::visibility("default")]]
double test_fully_dense_int8(int8_t key)
{
    constexpr auto delta = 1000.0;
    constexpr auto make_kv_pairs = [delta]() constexpr {
        auto res = std::vector<std::pair<int8_t, double>>(1 << CHAR_BIT);
        for (auto i = SCHAR_MIN;; i++) {
            res[i - SCHAR_MIN] = {i, i + delta};
            if (i == SCHAR_MAX) {
                break;
            }
        }
        return res;
    };
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(make_kv_pairs());

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return NAN;
}

// uint8 full range
[[gnu::visibility("default")]]
double test_fully_dense_uint8(uint8_t key)
{
    constexpr auto delta = 1000.0;
    constexpr auto make_kv_pairs = [delta]() constexpr {
        auto res = std::vector<std::pair<uint8_t, double>>(1 << CHAR_BIT);
        for (auto i = 0; i <= UCHAR_MAX; i++) {
            res[i] = {static_cast<uint8_t>(i), i + delta};
        }
        return res;
    };
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(make_kv_pairs());

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return NAN;
}
}  // extern "C"
