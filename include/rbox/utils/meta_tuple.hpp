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

#ifndef RBOX_UTILS_META_TUPLE_HPP
#define RBOX_UTILS_META_TUPLE_HPP

#include <rbox/utils/functional/tuple_comparison.hpp>
#include <rbox/utils/meta_utility.hpp>

namespace rbox {
namespace impl {
consteval bool is_elementwise_constructible(size_t N, std::initializer_list<std::meta::info> args)
{
    if (args.size() != 2 * N) {
        return false;
    }
    auto args_data = args.begin();
    for (auto i = 0zU; i < N; i++) {
        auto to = args_data[i];
        auto from = args_data[i + N];
        if (!std::meta::is_constructible_type(to, RBOX_IL(from))) {
            return false;
        }
    }
    return true;
}

template <size_t N, class... Args>
constexpr auto is_elementwise_constructible_v = is_elementwise_constructible(N, RBOX_IL(^^Args...));
}  // namespace impl

template <class... Args>
struct meta_tuple {
    struct underlying_type;

    consteval
    {
        auto elems = std::array{^^Args...};
        auto members = std::vector<std::meta::info>{};
        auto n = elems.size();

        members.reserve(n);
        // v1, v2, v3, ...
        char buffer[32] = {'v'};
        for (auto i = 0zU; i < n; i++) {
            auto num = i + 1;
            auto digits = 0zU;
            for (auto temp = num; temp > 0; ++digits, temp /= 10);
            for (auto j = digits; j > 0zU; --j) {
                buffer[j] = static_cast<char>('0' + (num % 10));
                num /= 10;
            }
            auto options = std::meta::data_member_options{
                .name = buffer,
            };
            auto cur_member = std::meta::data_member_spec(elems[i], options);
            members.push_back(cur_member);
        }
        std::meta::define_aggregate(^^underlying_type, members);
    }

    // Trivial construction
    constexpr meta_tuple() = default;

    template <class... CtorArgs>
        requires (impl::is_elementwise_constructible_v<sizeof...(Args), Args..., CtorArgs...>)
    constexpr meta_tuple(CtorArgs&&... args) : elements{std::forward<CtorArgs>(args)...}
    {
    }

    constexpr meta_tuple(const meta_tuple&) = default;
    constexpr meta_tuple(meta_tuple&&) = default;

    constexpr auto operator=(const meta_tuple&) -> meta_tuple& = default;
    constexpr auto operator=(meta_tuple&&) -> meta_tuple& = default;

private:
    using self_type = meta_tuple<Args...>;

    static consteval auto ith_field(size_t i) -> std::meta::info
    {
        return all_direct_nonstatic_data_members_v<underlying_type>[i];
    }

public:
    template <size_t I>
    friend constexpr auto& get(meta_tuple& tuple)
    {
        return tuple.elements.[:ith_field(I):];
    }

    template <size_t I>
    friend constexpr const auto& get(const meta_tuple& tuple)
    {
        return tuple.elements.[:ith_field(I):];
    }

    template <size_t I>
    friend constexpr auto&& get(meta_tuple&& tuple)
    {
        return std::move(tuple.elements.[:ith_field(I):]);
    }

    template <size_t I>
    friend constexpr const auto&& get(const meta_tuple&& tuple)
    {
        return std::move(tuple.elements.[:ith_field(I):]);
    }

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

    underlying_type elements;
};

// Deduction guide (cvref removed)
template <class... Args>
meta_tuple(Args...) -> meta_tuple<Args...>;
}  // namespace rbox

template <class... Args>
struct std::tuple_size<rbox::meta_tuple<Args...>> {
    static constexpr auto value = sizeof...(Args);
};

template <size_t I, class... Args>
struct std::tuple_element<I, rbox::meta_tuple<Args...>> {
    using type = Args...[I];
};

#endif  // RBOX_UTILS_META_TUPLE_HPP
