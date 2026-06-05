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
// Signed fully dense
[[gnu::visibility("default")]]
int32_t test_fully_dense_signed(int32_t key)
{
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(
        std::initializer_list<std::pair<int32_t, int32_t>>{
            {-1, 123},
            {1, 456},
            {3, 789},
            {-2, 12},
            {0, 34},
            {2, 56},
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

// Unsigned fully dense
[[gnu::visibility("default")]]
bool test_fully_dense_unsigned(double* dest, uint64_t key)
{
    struct point_t {
        double x, y, z;
    };
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(
        std::initializer_list<std::pair<uint64_t, point_t>>{
            {10, {1.25, -1.25, 2.0}},
            {12, {2.5, -2.5, 3.0}},
            {11, {3.75, -3.75, 4.0}},
            {14, {5.0, -5.0, 5.0}},
            {13, {6.25, -6.25, 6.0}},
        });

    if (auto opt = map.find(key); opt) {
        dest[0] = opt->x;
        dest[1] = opt->y;
        dest[2] = opt->z;
        return true;
    }
    return false;
}
}  // extern "C"
