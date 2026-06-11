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

#ifndef RBOX_UTILS_CONFIG_HPP
#define RBOX_UTILS_CONFIG_HPP

#include <initializer_list>

#define RBOX_IL(...)      \
    std::initializer_list \
    {                     \
        __VA_ARGS__       \
    }

#ifdef __clang__
#define RBOX_ALWAYS_INLINE [[clang::always_inline]]
#define RBOX_ALWAYS_INLINE_CALL [[clang::always_inline]]
#define RBOX_NO_INLINE [[gnu::noinline]]
#elifdef __GNUC__
#define RBOX_ALWAYS_INLINE [[gnu::always_inline]]
#define RBOX_ALWAYS_INLINE_CALL
#define RBOX_NO_INLINE [[gnu::noinline]]
#else
#define RBOX_ALWAYS_INLINE
#define RBOX_ALWAYS_INLINE_CALL
#define RBOX_NO_INLINE
#endif

namespace rbox {
[[noreturn]] int compile_error(const char* msg) noexcept;
}  // namespace rbox

#define RBOX_ERROR_IF_CONSTEVAL(msg)    \
    do {                                \
        if consteval {                  \
            ::rbox::compile_error(msg); \
        }                               \
    } while (false)
#define RBOX_UNREACHABLE(msg)         \
    do {                              \
        RBOX_ERROR_IF_CONSTEVAL(msg); \
        __builtin_unreachable();      \
    } while (false)

#endif  // RBOX_UTILS_CONFIG_HPP
