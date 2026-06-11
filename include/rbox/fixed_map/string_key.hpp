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

#ifndef RBOX_FIXED_MAP_STRING_KEY_HPP
#define RBOX_FIXED_MAP_STRING_KEY_HPP

#include <rbox/fixed_map/candidates/string_by_hash_search.hpp>
#include <rbox/fixed_map/candidates/string_by_hash_table.hpp>
#include <rbox/fixed_map/candidates/string_naive.hpp>
#include <rbox/to_static_storage.hpp>
#include <rbox/utils/ctype/ascii_test.hpp>
#include <rbox/utils/ctype/case_conversion.hpp>
#include <rbox/utils/define_static_string.hpp>
#include <rbox/utils/make_string_view.hpp>
#include <rbox/utils/stdlib/algorithm/sort.hpp>
#include <rbox/utils/tuple_element.hpp>

namespace rbox {
struct string_key_fixed_map_options {
    bool already_ascii_only = false;
    bool already_unique = false;
    bool ascii_case_insensitive = false;
    bool adjusts_alignment = false;
    double min_load_factor = 0.5;
    size_t max_n_hash_probing_attempts = 3;
    size_t max_n_iterations = 64;
    size_t optimization_threshold = 4;
    size_t binary_search_threshold = 8;
};

namespace impl::map {
consteval bool is_kv_pair_with_skey(std::meta::info T)
{
    if (!extract<bool>(^^pair_like, T)) {
        return false;
    }
    auto K = std::meta::remove_cvref(std::meta::tuple_element(0, T));
    return extract<bool>(^^string_like, K);
}

template <class KVPair>
concept kv_pair_with_skey = is_kv_pair_with_skey(std::meta::remove_cv(^^KVPair));

// Precondition: not hash_values.empty()
constexpr bool has_hash_collision(std::vector<uint64_t> hash_values)
{
    auto* first = hash_values.data();
    auto* last = first + hash_values.size();
    std::sort(first, last);
    if (*first == 0) return true;
    for (const auto* it = first; it + 1 < last; ++it) {
        if (*it == *(it + 1)) return true;
    }
    return false;
}

#define RBOX_PARAM(param) .param = options.param

// Precondition: All keys in kv_pairs are lower case if options.ascii_case_insensitive is true.
template <class CharT, class V>
consteval auto make_with_skey(
    std::vector<meta_pair<meta_basic_string_view<CharT>, V>> kv_pairs,
    const string_key_fixed_map_options& options) -> std::meta::info
{
    using kv_pair_t = meta_pair<meta_basic_string_view<CharT>, V>;
    auto n = kv_pairs.size();
    auto* kv_pairs_begin = kv_pairs.data();
    auto* kv_pairs_end = kv_pairs_begin + n;

    // (1) Empty
    if (kv_pairs.empty()) {
        return make_empty_with_skey<CharT, V>();
    }
    // Input validation
    if (!options.already_unique && n >= 2) {
        // Duplication check
        std::sort(kv_pairs_begin, kv_pairs_end, [](const kv_pair_t& a, const kv_pair_t& b) {
            return a.first < b.first;
        });
        for (const auto* it = kv_pairs_begin; it + 1 < kv_pairs_end; ++it) {
            if (it->first == (it + 1)->first) {
                compile_error("Duplicated keys are not allowed.");
            }
        }
    }
    if (options.ascii_case_insensitive && !options.already_ascii_only) {
        // ASCII check
        for (const auto* it = kv_pairs_begin; it < kv_pairs_end; ++it) {
            if (!is_ascii_string(it->first)) {
                compile_error("Only ASCII strings allowed.");
            }
        }
    }
    // (2) Naive
    if (n < options.optimization_threshold) {
        auto naive_options = naive_with_skey_options{
            RBOX_PARAM(ascii_case_insensitive),
        };
        return make_naive_with_skey(meta_span{kv_pairs_begin, n}, naive_options);
    }

    auto hash_values = std::vector<size_t>(n);
    auto* hash_values_begin = hash_values.data();
    for (size_t i = 0zU; i < n; i++) {
        hash_values_begin[i] = bkdr_hash(kv_pairs_begin[i].first);
    }
    auto has_collision = has_hash_collision(hash_values);
    // (3) Hash table
    if (!has_collision && options.max_n_iterations > 0) {
        auto res = try_make_hash_table_with_skey(
            meta_span{kv_pairs_begin, n},
            meta_span{hash_values_begin, n},
            hash_table_with_skey_options{
                RBOX_PARAM(ascii_case_insensitive),
                RBOX_PARAM(adjusts_alignment),
                RBOX_PARAM(min_load_factor),
                RBOX_PARAM(max_n_hash_probing_attempts),
                RBOX_PARAM(max_n_iterations),
            });
        if (res.has_value()) {
            return *res;
        }
    }
    // (4) Hash search
    return make_hash_search_with_skey(
        meta_span{kv_pairs_begin, n},
        meta_span{hash_values_begin, n},
        has_collision,
        hash_search_with_skey_options{
            RBOX_PARAM(ascii_case_insensitive),
            RBOX_PARAM(adjusts_alignment),
            RBOX_PARAM(binary_search_threshold),
        });
}

#undef RBOX_PARAM
}  // namespace impl::map

template <std::ranges::input_range KVPairRange>
    requires (impl::map::kv_pair_with_skey<std::ranges::range_value_t<KVPairRange>>)
consteval auto make_string_key_fixed_map(
    const KVPairRange& kv_pairs, const string_key_fixed_map_options& options = {})
    -> std::meta::info
{
    using KVPair = std::ranges::range_value_t<KVPairRange>;
    using K = rbox::tuple_element_t<0, KVPair>;
    using V = rbox::tuple_element_t<1, KVPair>;
    using S = meta_basic_string_view<char_type_t<K>>;

    auto converted = std::vector<meta_pair<S, to_static_storage_result_t<V>>>{};
    if (options.ascii_case_insensitive) {
        for (const auto& [k, v] : kv_pairs) {
            auto s = rbox::define_static_string(ascii_tolower(k));
            converted.push_back({s, to_static_storage(v)});
        }
    } else {
        for (const auto& [k, v] : kv_pairs) {
            auto s = rbox::define_static_string(make_string_view(k));
            converted.push_back({s, to_static_storage(v)});
        }
    }
    return impl::map::make_with_skey(std::move(converted), options);
}

template <impl::map::kv_pair_with_skey KVPair>
consteval auto make_string_key_fixed_map(
    std::initializer_list<KVPair> kv_pairs, const string_key_fixed_map_options& options = {})
    -> std::meta::info
{
    auto as_span = meta_span(kv_pairs.begin(), kv_pairs.size());
    return make_string_key_fixed_map(as_span, options);
}
}  // namespace rbox

#define RBOX_STRING_KEY_FIXED_MAP(kv_pairs, ...) \
    [:rbox::make_string_key_fixed_map(kv_pairs, ##__VA_ARGS__):]

#endif  // RBOX_FIXED_MAP_STRING_KEY_HPP
