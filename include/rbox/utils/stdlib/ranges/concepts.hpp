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

#ifndef RBOX_UTILS_STDLIB_RANGES_CONCEPTS_HPP
#define RBOX_UTILS_STDLIB_RANGES_CONCEPTS_HPP

/**
 * Minimal include for std::ranges core concepts and utilities:
 *   std::ranges::range, input_range, contiguous_range
 *   std::ranges::range_value_t, iterator_t, etc.
 */

#if __has_include(<__ranges/concepts.h>)
#include <__ranges/concepts.h>  // Clang libc++ (minimal)
#elif __has_include(<bits/ranges_base.h>)
#include <bits/ranges_base.h>  // GCC libstdc++
#else
#include <ranges>  // Fallback
#endif

#endif  // RBOX_UTILS_STDLIB_RANGES_CONCEPTS_HPP
