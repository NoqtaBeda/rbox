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

#ifndef RBOX_ENUM_IMPL_PROMOTED_HPP
#define RBOX_ENUM_IMPL_PROMOTED_HPP

#include <concepts>
#include <rbox/utils/concepts.hpp>
#include <type_traits>

namespace rbox::impl {
template <class T>
struct promoted_traits;

template <std::signed_integral T>
struct promoted_traits<T> {
    using type = std::conditional_t<sizeof(T) <= sizeof(ssize_t), ssize_t, T>;
};

template <std::unsigned_integral T>
struct promoted_traits<T> {
    using type = std::conditional_t<sizeof(T) <= sizeof(size_t), size_t, T>;
};

template <enum_type T>
struct promoted_traits<T> {
    using type = typename promoted_traits<std::underlying_type_t<T>>::type;
};

template <class T>
using promoted_t = typename promoted_traits<T>::type;

template <class T>
constexpr auto promoted(T value)
{
    static_assert(sizeof(T) <= sizeof(int64_t), "Extended big integer types are not supported.");
    return static_cast<promoted_t<T>>(value);
}
}  // namespace rbox::impl

#endif  // RBOX_ENUM_IMPL_PROMOTED_HPP
