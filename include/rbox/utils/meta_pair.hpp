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

#ifndef RBOX_UTILS_META_PAIR_HPP
#define RBOX_UTILS_META_PAIR_HPP

#include <rbox/utils/functional/tuple_comparison.hpp>
#include <tuple>
#include <utility>

namespace rbox {
template <class T, class U>
struct meta_pair {
    T first;
    U second;

    template <size_t I>
        requires (I == 0 || I == 1)
    friend constexpr auto& get(meta_pair& self)
    {
        if constexpr (I == 0) {
            return self.first;
        } else {
            return self.second;
        }
    }

    template <size_t I>
        requires (I == 0 || I == 1)
    friend constexpr const auto& get(const meta_pair& self)
    {
        if constexpr (I == 0) {
            return self.first;
        } else {
            return self.second;
        }
    }

    template <size_t I>
        requires (I == 0 || I == 1)
    friend constexpr auto&& get(meta_pair&& self)
    {
        if constexpr (I == 0) {
            return std::move(self.first);
        } else {
            return std::move(self.second);
        }
    }

    template <size_t I>
        requires (I == 0 || I == 1)
    friend constexpr const auto&& get(const meta_pair&& self)
    {
        if constexpr (I == 0) {
            return std::move(self.first);
        } else {
            return std::move(self.second);
        }
    }

private:
    using self_type = meta_pair<T, U>;

public:
    template <class TupleLike>
        requires (is_elementwise_equal_comparable_v<self_type, TupleLike>)
    constexpr auto operator==(const TupleLike& rhs) const -> bool
    {
        return elementwise_equal(*this, rhs);
    }

    template <class TupleLike>
        requires (is_elementwise_compare_three_way_comparable_v<self_type, TupleLike>)
    constexpr auto operator<=>(const TupleLike& rhs) const
    {
        return elementwise_compare_three_way(*this, rhs);
    }
};
}  // namespace rbox

template <class T, class U>
struct std::tuple_size<rbox::meta_pair<T, U>> : std::integral_constant<size_t, 2> {};

template <size_t I, class T, class U>
    requires (I == 0 || I == 1)
struct std::tuple_element<I, rbox::meta_pair<T, U>> {
    using type = std::conditional_t<I == 0, T, U>;
};

#endif  // RBOX_UTILS_META_PAIR_HPP
