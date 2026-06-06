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

#ifndef RBOX_ENUM_IMPL_ENUM_INDEX_MAP_HPP
#define RBOX_ENUM_IMPL_ENUM_INDEX_MAP_HPP

#include <rbox/enum/enum_names.hpp>
#include <rbox/enum/impl/promoted.hpp>
#include <rbox/fixed_map/integral_key.hpp>
#include <rbox/utils/stdlib/algorithm/stable_sort.hpp>
#include <rbox/utils/stdlib/algorithm/unique.hpp>
#include <utility>

namespace rbox::impl {
union enum_indices_t {
    struct {
        uint16_t original;
        uint16_t by_name;
        uint16_t by_value;
        uint16_t by_value_unique;
    };
    uint64_t as_u64;
};

template <enum_entry_order Order>
constexpr auto get(enum_indices_t indices)
{
    if constexpr (Order == enum_entry_order::original) {
        return indices.original;
    } else if constexpr (Order == enum_entry_order::by_name) {
        return indices.by_name;
    } else if constexpr (Order == enum_entry_order::by_value) {
        return indices.by_value;
    } else {
        static_assert(false, "Invalid enum entry order.");
    }
}

template <class E>
consteval auto make_enum_index_map_kv_pairs()
{
    using entry_tuple_t = std::pair<size_t, std::string_view>;
    using kv_pair_t = std::pair<promoted_t<E>, enum_indices_t>;

    auto entries = std::meta::enumerators_of(^^E);
    auto n = entries.size();
    const auto* entries_data = entries.data();
    const auto* entries_data_end = entries_data + n;

    auto entry_tuples = std::vector<entry_tuple_t>{};
    entry_tuples.reserve(n);
    for (auto i = 0zU; i < n; i++) {
        entry_tuples.emplace_back(i, std::meta::identifier_of(entries_data[i]));
    }
    auto* entry_tuples_data = entry_tuples.data();
    auto* entry_tuples_data_end = entry_tuples_data + n;

    auto res = std::vector<kv_pair_t>{};
    res.reserve(n);
    for (auto i = 0zU; i < n; i++) {
        auto k = promoted(std::meta::extract<E>(entries_data[i]));
        auto v = enum_indices_t{.original = static_cast<uint16_t>(i)};
        res.emplace_back(k, v);
    }
    auto* res_data = res.data();
    auto* res_data_end = res_data + n;

    std::stable_sort(
        entry_tuples_data,
        entry_tuples_data_end,
        [](const entry_tuple_t& a, const entry_tuple_t& b) {
            return a.second < b.second;
        });
    for (auto i = 0zU; i < n; i++) {
        res_data[entry_tuples_data[i].first].second.by_name = i;
    }
    // Original order is kept on underlying value duplication
    std::stable_sort(res_data, res_data_end, [](const kv_pair_t& a, const kv_pair_t& b) {
        return a.first < b.first;
    });
    for (auto i = 0zU; i < n; i++) {
        res_data[i].second.by_value = i;
    }
    auto* dup_begin =
        std::unique(res_data, res_data_end, [](const kv_pair_t& a, const kv_pair_t& b) {
            return a.first == b.first;
        });
    res.resize(dup_begin - res_data);
    for (auto i = 0zU, m = res.size(); i < m; i++) {
        res[i].second.by_value_unique = i;
    }
    return res;
}

constexpr auto enum_count_limit = std::numeric_limits<uint16_t>::max() - 1;

template <class E>
consteval auto make_enum_index_map()
{
    static_assert(enum_count_v<E> <= enum_count_limit, "Enum type too large.");
    constexpr auto options = integral_key_fixed_map_options{
        .already_sorted = true,
        .already_unique = true,
    };
    return RBOX_INTEGRAL_KEY_FIXED_MAP(make_enum_index_map_kv_pairs<E>(), options);
}

template <class E>
constexpr auto enum_index_map_v = make_enum_index_map<E>();
}  // namespace rbox::impl

#endif  // RBOX_ENUM_IMPL_ENUM_INDEX_MAP_HPP
