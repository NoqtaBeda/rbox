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

#ifndef RBOX_ENUM_ENUM_META_ENTRIES_HPP
#define RBOX_ENUM_ENUM_META_ENTRIES_HPP

#include <rbox/enum/enum_count.hpp>
#include <rbox/enum/enum_entry_order.hpp>
#include <rbox/utils/concepts.hpp>
#include <rbox/utils/stdlib/algorithm/sort.hpp>
#include <utility>

namespace rbox {
namespace impl {
template <class E, enum_entry_order Order>
constexpr auto enum_meta_entries_array_v = invalid_enum_entry_order_tag;

template <class E>
consteval auto make_enum_meta_entries_original_order()
/* -> std::array<std::meta::info, N> */
{
    constexpr auto N = enum_count_v<E>;
    auto res = std::array<std::meta::info, N>{};
    auto enums = std::meta::enumerators_of(^^E);
    auto it = res.begin();
    for (auto e : enums) {
        *it++ = e;
    }
    return res;
}

template <class E>
constexpr auto enum_meta_entries_array_v<E, enum_entry_order::original> =
    make_enum_meta_entries_original_order<E>();

template <class E>
consteval auto make_enum_meta_entries_sorted_by_value()
/* -> std::array<std::meta::info, N> */
{
    constexpr const auto& orig_order = enum_meta_entries_array_v<E, enum_entry_order::original>;
    constexpr auto N = orig_order.size();

    if constexpr (N == 0) {
        return std::array<std::meta::info, N>{};
    } else {
        using iv_pair_t = std::pair<size_t, std::underlying_type_t<E>>;
        auto iv_pairs = std::array<iv_pair_t, N>{};
        auto* iv_pairs_data = iv_pairs.data();
        auto* iv_pairs_data_end = iv_pairs_data + N;
        auto index = 0zU;
        template for (constexpr auto e : orig_order)
        {
            iv_pairs_data[index] = {index, std::to_underlying([:e:])};
            index += 1;
        }
        std::sort(iv_pairs_data, iv_pairs_data_end, [](const iv_pair_t& a, const iv_pair_t& b) {
            return a.second < b.second;
        });
        auto res = std::array<std::meta::info, N>{};
        auto* res_data = res.data();
        for (auto i = 0zU; i < N; i++) {
            res_data[i] = orig_order[iv_pairs_data[i].first];
        }
        return res;
    }
}

template <class E>
constexpr auto enum_meta_entries_array_v<E, enum_entry_order::by_value> =
    make_enum_meta_entries_sorted_by_value<E>();

template <class E>
consteval auto make_enum_meta_entries_sorted_by_name()
/* -> std::array<std::meta::info, N> */
{
    constexpr const auto& orig_order = enum_meta_entries_array_v<E, enum_entry_order::original>;
    constexpr auto N = orig_order.size();

    if constexpr (N == 0) {
        return std::array<std::meta::info, N>{};
    } else {
        using iv_pair_t = std::pair<size_t, std::string_view>;
        auto iv_pairs = std::array<iv_pair_t, N>{};
        auto* iv_pairs_data = iv_pairs.data();
        auto* iv_pairs_data_end = iv_pairs_data + N;
        auto index = 0zU;
        template for (constexpr auto e : orig_order)
        {
            iv_pairs_data[index] = {index, std::meta::identifier_of(e)};
            index += 1;
        }
        std::sort(iv_pairs_data, iv_pairs_data_end, [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
        auto res = std::array<std::meta::info, N>{};
        auto* res_data = res.data();
        for (auto i = 0zU; i < N; i++) {
            res_data[i] = orig_order[iv_pairs_data[i].first];
        }
        return res;
    }
}

template <class E>
constexpr auto enum_meta_entries_array_v<E, enum_entry_order::by_name> =
    make_enum_meta_entries_sorted_by_name<E>();
}  // namespace impl

template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr auto enum_meta_entries_v =
    std::span{impl::enum_meta_entries_array_v<std::remove_cv_t<E>, Order>};
}  // namespace rbox

#endif  // RBOX_ENUM_ENUM_META_ENTRIES_HPP
