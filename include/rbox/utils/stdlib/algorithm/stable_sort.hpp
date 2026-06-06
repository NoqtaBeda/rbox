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

#ifndef RBOX_UTILS_STDLIB_ALGORITHM_STABLE_SORT_HPP
#define RBOX_UTILS_STDLIB_ALGORITHM_STABLE_SORT_HPP

/**
 * Minimal include for std::stable_sort.
 *
 *  - Clang (libc++):    <__algorithm/stable_sort.h>
 *  - GCC (libstdc++):   <bits/stl_algo.h>
 *  - Fallback:          <algorithm>
 */

#if __has_include(<__algorithm/stable_sort.h>)
#include <__algorithm/stable_sort.h>  // Clang libc++ (minimal)
#elif __has_include(<bits/stl_algo.h>)
#include <bits/stl_algo.h>  // GCC libstdc++
#else
#include <algorithm>  // Fallback
#endif

#endif  // RBOX_UTILS_STDLIB_ALGORITHM_STABLE_SORT_HPP
