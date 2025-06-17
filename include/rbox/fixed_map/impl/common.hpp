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

#ifndef RBOX_FIXED_MAP_IMPL_COMMON_HPP
#define RBOX_FIXED_MAP_IMPL_COMMON_HPP

#include <bit>
#include <utility>

namespace rbox::impl::map {
template <class V>
constexpr auto default_v = V{};

template <class T>
struct aligned {
    static constexpr auto alignment = std::bit_ceil(sizeof(T));
    alignas(alignment) T underlying;
};

struct to_aligned_t {
    template <class T>
    static constexpr auto operator()(T value) -> aligned<T>
    {
        return {std::move(value)};
    }
};

template <class T>
constexpr auto unwrap(T& value) -> const T&
{
    return value;
}

template <class T>
constexpr auto unwrap(const aligned<T>& wrapper) -> const T&
{
    return wrapper.underlying;
}
}  // namespace rbox::impl::map

#endif  // RBOX_FIXED_MAP_IMPL_COMMON_HPP
