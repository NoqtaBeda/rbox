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

#ifndef RBOX_ENUM_IMPL_ENUM_NAME_MAP_HPP
#define RBOX_ENUM_IMPL_ENUM_NAME_MAP_HPP

#include <rbox/enum/enum_names.hpp>
#include <rbox/enum/impl/promoted.hpp>
#include <rbox/fixed_map/integral_key.hpp>
#include <rbox/utils/meta_string_view.hpp>
#include <rbox/utils/stdlib/algorithm/stable_sort.hpp>
#include <rbox/utils/stdlib/algorithm/unique.hpp>
#include <utility>

namespace rbox::impl {
template <class E>
consteval auto make_enum_name_map_kv_pairs()
{
    using kv_pair_t = std::pair<promoted_t<E>, meta_string_view>;
    auto n = enum_count_v<E>;
    auto res = std::vector<kv_pair_t>(n);

    auto* res_data = res.data();
    auto* res_data_end = res_data + n;

    auto entries = std::meta::enumerators_of(^^E);
    const auto* entries_data = entries.data();
    const auto* names_data = enum_names_v<E>.data();

    for (auto i = 0zU; i < n; i++) {
        auto ev = std::meta::extract<E>(entries_data[i]);
        auto msv = meta_string_view(names_data[i]);
        res_data[i] = {promoted(ev), msv};
    }
    // Original order is preserved for multiple entries with the same underlying value
    std::stable_sort(res_data, res_data_end, [](const kv_pair_t& a, const kv_pair_t& b) {
        return a.first < b.first;
    });
    auto* dup_begin =
        std::unique(res_data, res_data_end, [](const kv_pair_t& a, const kv_pair_t& b) {
            return a.first == b.first;
        });
    res.resize(dup_begin - res_data);
    return res;
}

template <class E>
consteval auto make_enum_name_map()
{
    constexpr auto options = integral_key_fixed_map_options{
        .already_sorted = true,
        .already_unique = true,
        .adjusts_alignment = true,
    };
    return RBOX_INTEGRAL_KEY_FIXED_MAP(make_enum_name_map_kv_pairs<E>(), options);
}

template <class E>
constexpr auto enum_name_map_v = make_enum_name_map<E>();
}  // namespace rbox::impl

#endif  // RBOX_ENUM_IMPL_ENUM_NAME_MAP_HPP
