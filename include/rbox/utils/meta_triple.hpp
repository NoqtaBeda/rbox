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

#ifndef RBOX_UTILS_META_TRIPLE_HPP
#define RBOX_UTILS_META_TRIPLE_HPP

#include <rbox/utils/functional/tuple_comparison.hpp>
#include <tuple>
#include <utility>

namespace rbox {
template <class T, class U, class V>
struct meta_triple {
    T first;
    U second;
    V third;

    template <size_t I>
        requires (I == 0 || I == 1 || I == 2)
    friend constexpr auto& get(meta_triple& self)
    {
        if constexpr (I == 0) {
            return self.first;
        } else if constexpr (I == 1) {
            return self.second;
        } else {
            return self.third;
        }
    }

    template <size_t I>
        requires (I == 0 || I == 1 || I == 2)
    friend constexpr const auto& get(const meta_triple& self)
    {
        if constexpr (I == 0) {
            return self.first;
        } else if constexpr (I == 1) {
            return self.second;
        } else {
            return self.third;
        }
    }

    template <size_t I>
        requires (I == 0 || I == 1 || I == 2)
    friend constexpr auto&& get(meta_triple&& self)
    {
        if constexpr (I == 0) {
            return std::move(self.first);
        } else if constexpr (I == 1) {
            return std::move(self.second);
        } else {
            return std::move(self.third);
        }
    }

    template <size_t I>
        requires (I == 0 || I == 1 || I == 2)
    friend constexpr const auto&& get(const meta_triple&& self)
    {
        if constexpr (I == 0) {
            return std::move(self.first);
        } else if constexpr (I == 1) {
            return std::move(self.second);
        } else {
            return std::move(self.third);
        }
    }

private:
    using self_type = meta_triple<T, U, V>;

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

template <class T, class U, class V>
struct std::tuple_size<rbox::meta_triple<T, U, V>> : std::integral_constant<size_t, 3> {};

template <size_t I, class T, class U, class V>
    requires (I == 0 || I == 1 || I == 2)
struct std::tuple_element<I, rbox::meta_triple<T, U, V>> {
    using type = std::conditional_t<I == 0, T, std::conditional_t<I == 1, U, V>>;
};

#endif  // RBOX_UTILS_META_TRIPLE_HPP
