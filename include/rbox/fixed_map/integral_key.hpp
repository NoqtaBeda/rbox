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

#ifndef RBOX_FIXED_MAP_INTEGRAL_KEY_HPP
#define RBOX_FIXED_MAP_INTEGRAL_KEY_HPP

#include <rbox/fixed_map/candidates/enum_wrapper.hpp>
#include <rbox/fixed_map/candidates/integral_general.hpp>
#include <rbox/to_static_storage.hpp>
#include <rbox/type_traits/tuple_like_types.hpp>
#include <rbox/utils/stdlib/algorithm/sort.hpp>
#include <rbox/utils/tuple_element.hpp>

namespace rbox {
struct integral_key_fixed_map_options {
    bool already_sorted = false;
    bool already_unique = false;
    bool adjusts_alignment = false;
    double min_load_factor = 0.5;
    size_t dense_lookup_threshold = 4;
    size_t binary_search_threshold = 8;
};

namespace impl::map {
consteval bool is_kv_pair_with_ikey(std::meta::info T)
{
    if (!extract<bool>(^^pair_like, T)) {
        return false;
    }
    auto K = std::meta::remove_cvref(std::meta::tuple_element(0, T));
    if (std::meta::is_enum_type(K)) {
        return true;
    }
    if (std::meta::is_same_type(K, ^^bool)) {
        return false;
    }
    return std::meta::is_integral_type(K);
}

template <class KVPair>
concept kv_pair_with_ikey = is_kv_pair_with_ikey(std::meta::remove_cv(^^KVPair));

template <class K, class V>
consteval bool is_dense_closed_range(
    const meta_pair<K, V>* head, const meta_pair<K, V>* tail, double min_load_factor)
{
    auto n_slots = static_cast<double>(tail->first) - static_cast<double>(head->first) + 1.0;
    auto n_non_hole_entries = static_cast<double>(tail - head + 1);
    return n_slots * min_load_factor <= n_non_hole_entries;
}

template <class K, class V>
consteval auto find_longest_dense_subrange(
    const std::vector<meta_pair<K, V>>& sorted_kv_pairs, double min_load_factor)
    -> meta_pair<const meta_pair<K, V>*, const meta_pair<K, V>*>
{
    const auto* begin = sorted_kv_pairs.data();
    const auto* end = sorted_kv_pairs.data() + sorted_kv_pairs.size();
    // Fast path: fully continuous
    if (is_dense_closed_range(begin, end - 1, min_load_factor)) {
        return meta_pair{begin, end};
    }

    const auto* max_len_head = begin;
    const auto* max_len_tail = begin;
    for (const auto *head = begin, *tail = begin + 1; tail < end; ++tail) {
        // Finds longest [head, tail]
        for (; head < tail && !is_dense_closed_range(head, tail, min_load_factor); ++head);
        if (tail - head > max_len_tail - max_len_head) {
            max_len_head = head;
            max_len_tail = tail;
        }
    }
    return meta_pair{max_len_head, max_len_tail + 1};
}

template <class K, class V>
consteval auto make_with_ikey(
    std::vector<meta_pair<K, V>> kv_pairs, const integral_key_fixed_map_options& options)
    -> std::meta::info
{
    auto n = kv_pairs.size();
    auto* kv_pairs_begin = kv_pairs.data();
    auto* kv_pairs_end = kv_pairs_begin + n;

    // (1) Empty
    if (kv_pairs.empty()) {
        return make_empty_with_ikey<V>();
    }
    // Preprocessing & duplication check
    if (!options.already_sorted) {
        auto compares_first_fn = [](const meta_pair<K, V>& a, const meta_pair<K, V>& b) {
            return a.first < b.first;
        };
        std::sort(kv_pairs_begin, kv_pairs_end, compares_first_fn);
    }

    if (!options.already_unique && n >= 2) {
        for (const auto* it = kv_pairs_begin; it + 1 < kv_pairs_end; ++it) {
            if (it->first == (it + 1)->first) {
                compile_error("Duplicated keys are not allowed.");
            }
        }
    }
    auto [dense_begin, dense_end] = find_longest_dense_subrange(kv_pairs, options.min_load_factor);
    if (dense_end - dense_begin == n) {
        // (2) Dense
        auto dense_options = dense_with_ikey_options{
            .adjusts_alignment = options.adjusts_alignment,
        };
        return make_dense_with_ikey(meta_span{kv_pairs_begin, n}, dense_options);
    }
    if (dense_end - dense_begin < options.dense_lookup_threshold) {
        // (3) Sparse
        auto sparse_options = sparse_with_ikey_options{
            .adjusts_alignment = options.adjusts_alignment,
            .binary_search_threshold = options.binary_search_threshold,
        };
        return make_sparse_with_ikey(meta_span{kv_pairs_begin, n}, sparse_options);
    }
    // (4) General
    auto general_options = general_with_ikey_options{
        .adjusts_alignment = options.adjusts_alignment,
        .binary_search_threshold = options.binary_search_threshold,
    };
    auto left_sparse = meta_span{kv_pairs_begin, dense_begin};
    auto right_sparse = meta_span{dense_end, kv_pairs_end};
    return make_general_with_ikey(left_sparse, right_sparse, general_options);
}
}  // namespace impl::map

template <std::ranges::input_range KVPairRange>
    requires (impl::map::kv_pair_with_ikey<std::ranges::range_value_t<KVPairRange>>)
consteval auto make_integral_key_fixed_map(
    const KVPairRange& kv_pairs, const integral_key_fixed_map_options& options = {})
    -> std::meta::info
{
    using KVPair = std::ranges::range_value_t<KVPairRange>;
    using K = rbox::tuple_element_t<0, KVPair>;
    using V = rbox::tuple_element_t<1, KVPair>;

    if constexpr (std::is_enum_v<K>) {
        // (1) Enum key
        using U = std::underlying_type_t<K>;
        auto converted = std::vector<meta_pair<U, to_static_storage_result_t<V>>>{};
        for (const auto& [k, v] : kv_pairs) {
            converted.push_back({std::to_underlying(k), to_static_storage(v)});
        }
        auto underlying = impl::map::make_with_ikey(std::move(converted), options);

        auto params_il = {^^K, underlying};
        return std::meta::substitute(^^impl::map::enum_wrapper_v, params_il);
    } else {
        // (2) Integeral key
        auto converted = std::vector<meta_pair<K, to_static_storage_result_t<V>>>{};
        for (const auto& [k, v] : kv_pairs) {
            converted.push_back({k, to_static_storage(v)});
        }
        return impl::map::make_with_ikey(std::move(converted), options);
    }
}
}  // namespace rbox

#define RBOX_INTEGRAL_KEY_FIXED_MAP(kv_pairs, ...) \
    [:rbox::make_integral_key_fixed_map(kv_pairs, ##__VA_ARGS__):]

#endif  // RBOX_FIXED_MAP_INTEGRAL_KEY_HPP
