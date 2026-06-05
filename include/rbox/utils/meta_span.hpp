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

#ifndef RBOX_UTILS_META_SPAN_HPP
#define RBOX_UTILS_META_SPAN_HPP

#include <array>
#include <cstddef>
#include <span>

namespace rbox {
template <class T>
struct meta_span {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = const_pointer;
    using const_iterator = const_pointer;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    const T* head;
    size_t n;

    // Trivial construction
    constexpr meta_span() = default;

    consteval meta_span(const T* head, size_t n) : head(head), n(n) {}
    consteval meta_span(const T* head, const T* tail) : head(head), n(tail - head) {}

    template <size_t N>
    consteval meta_span(const T (&arr)[N]) : head(arr), n(N)
    {
    }

    template <size_t N>
    consteval meta_span(const std::array<T, N>& arr) : head(arr.data()), n(N)
    {
    }

    consteval meta_span(std::span<const T> span) : head(span.data()), n(span.size()) {}

    constexpr operator std::span<const T>() const
    {
        return {head, n};
    }

    constexpr auto operator[](size_t index) const -> const T&
    {
        return head[index];
    }

    constexpr auto front() const -> const T&
    {
        return head[0];
    }

    constexpr auto back() const -> const T&
    {
        return head[n - 1];
    }

    constexpr auto size() const -> size_t
    {
        return n;
    }

    constexpr auto empty() const -> bool
    {
        return n == 0;
    }

    constexpr auto data() const -> const T*
    {
        return head;
    }

    constexpr auto begin() const -> const T*
    {
        return head;
    }

    constexpr auto end() const -> const T*
    {
        return head + n;
    }

    constexpr auto cbegin() const -> const T*
    {
        return head;
    }

    constexpr auto cend() const -> const T*
    {
        return head + n;
    }

    constexpr auto first(size_t n) const -> meta_span
    {
        return subspan(0, n);
    }

    constexpr auto last(size_t n) const -> meta_span
    {
        return subspan(size() - n);
    }

    constexpr auto subspan(size_t offset, size_t count = std::dynamic_extent) const -> meta_span
    {
        auto res = meta_span{};
        res.head = this->head + offset;
        if (count == std::dynamic_extent) {
            res.n = this->n - offset;
        } else {
            res.n = count;
        }
        return res;
    }
};
}  // namespace rbox

#endif  // RBOX_UTILS_META_SPAN_HPP
