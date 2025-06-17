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

#ifndef RBOX_UTILS_INDICES_VIEW_HPP
#define RBOX_UTILS_INDICES_VIEW_HPP

#include <cstddef>

namespace rbox {
// Expects indices_view(n) to have the same behavior as std::views::iota(size_t(0), n)
struct indices_view {
    size_t n;

    struct iterator {
        using difference_type = ptrdiff_t;

        size_t current;

        constexpr size_t operator*() const
        {
            return current;
        }
        constexpr iterator& operator++()
        {
            ++current;
            return *this;
        }
        constexpr iterator operator++(int)
        {
            auto tmp = *this;
            ++current;
            return tmp;
        }
        constexpr iterator& operator+=(difference_type n)
        {
            current = static_cast<size_t>(static_cast<difference_type>(current) + n);
            return *this;
        }
        constexpr bool operator==(const iterator& other) const
        {
            return current == other.current;
        }
        constexpr bool operator!=(const iterator& other) const
        {
            return current != other.current;
        }
        constexpr difference_type operator-(const iterator& other) const
        {
            return static_cast<difference_type>(current)
                 - static_cast<difference_type>(other.current);
        }

        friend constexpr iterator operator+(const iterator& it, difference_type n)
        {
            auto res = it;
            res += n;
            return res;
        }
        friend constexpr iterator operator+(difference_type n, const iterator& it)
        {
            return it + n;
        }
    };

    constexpr iterator begin() const
    {
        return {0};
    }
    constexpr iterator end() const
    {
        return {n};
    }
};
}  // namespace rbox

#endif  // RBOX_UTILS_INDICES_VIEW_HPP
