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

#ifndef RBOX_FIXED_MAP_CANDIDATES_STRING_BY_HASH_TABLE_HPP
#define RBOX_FIXED_MAP_CANDIDATES_STRING_BY_HASH_TABLE_HPP

#include <algorithm>
#include <optional>
#include <rbox/fixed_map/candidates/string_empty.hpp>
#include <rbox/fixed_map/impl/common.hpp>
#include <rbox/fixed_map/impl/min_max_key_length.hpp>
#include <rbox/fixed_map/impl/string_policy.hpp>
#include <rbox/utils/config.hpp>
#include <rbox/utils/constant.hpp>
#include <rbox/utils/indices_view.hpp>
#include <rbox/utils/meta_triple.hpp>

namespace rbox::impl::map {
// Precondition: No hash collision; No input key is hashed to 0
template <bool A, size_t P, class CharT, class V, template <class> class Policy>
struct hash_table_with_skey {
    using character_type = CharT;
    using key_type = meta_basic_string_view<CharT>;
    using value_type = V;

private:
    using raw_element_type = meta_triple<size_t, meta_basic_string_view<CharT>, V>;
    using element_type = aligned_if_t<A, raw_element_type>;

public:
#define RBOX_STRING_BY_HASH_TABLE_DO_FIND(fallback)                     \
    do {                                                                \
        auto len = key.length();                                        \
        if (len < min_length || len > max_length) {                     \
            return fallback;                                            \
        }                                                               \
        auto key_hash = Policy<CharT>::hash(key);                       \
        auto start_index = key_hash % modulo;                           \
        template for (constexpr auto I : rbox::indices_view{P + 1})     \
        {                                                               \
            const raw_element_type& cur = entries[start_index + I * I]; \
            if (cur.first == 0) {                                       \
                return fallback;                                        \
            }                                                           \
            if (cur.first == key_hash) {                                \
                if (Policy<CharT>::equals(cur.second, key)) {           \
                    return cur.third;                                   \
                }                                                       \
                return fallback;                                        \
            }                                                           \
        }                                                               \
    } while (false)

    constexpr auto size() const -> size_t
    {
        return actual_size;
    }

    constexpr auto find(std::basic_string_view<CharT> key) const -> std::optional<const value_type&>
    {
        RBOX_STRING_BY_HASH_TABLE_DO_FIND(std::nullopt);
        return std::nullopt;
    }

    constexpr auto operator[](std::basic_string_view<CharT> key) const -> const value_type&
    {
        RBOX_STRING_BY_HASH_TABLE_DO_FIND(default_v<value_type>);
        return default_v<value_type>;
    }

#undef RBOX_STRING_BY_HASH_TABLE_DO_FIND

    const element_type* entries;  // Entry range size = modulo + P^2
    size_t min_length;
    size_t max_length;
    size_t actual_size;
    size_t modulo;
};

// -------- Builder --------

struct hash_table_with_skey_options {
    bool ascii_case_insensitive;
    bool adjusts_alignment;
    double min_load_factor;
    size_t max_n_hash_probing_attempts;
    size_t max_n_iterations;
};

// Returns the best probing size P, or npos if test fails.
consteval auto test_hash_modulo(
    size_t* tags, size_t g, const size_t* hash_values, size_t n, size_t m, size_t p) -> size_t
{
    auto best_p = 0;
    for (auto i = 0zU; i < n; i++) {
        auto ok = false;
        auto base_index = hash_values[i] % m;
        for (auto j = 0zU; !ok && j <= p; j++) {
            auto cur_index = base_index + j * j;
            if (tags[cur_index] == g) {
                continue;  // Visited before
            }
            tags[cur_index] = g;
            ok = true;
            // Updates best_p by the maximal probing step required
            if (j > best_p) {
                best_p = j;
            }
        }
        if (!ok) return npos;
    }
    return best_p;
}

// Returns (m, P), or (0, 0) on failure
consteval auto find_best_hash_modulo(
    meta_span<size_t> hash_values, const hash_table_with_skey_options& options)
    -> meta_pair<size_t, size_t>
{
    auto n = hash_values.n;
    auto p = options.max_n_hash_probing_attempts;
    auto limit = static_cast<size_t>(n / options.min_load_factor);

    auto max_n_iterations = options.max_n_iterations;
    auto n_iterations = 0;

    auto tags = std::vector<size_t>(limit + p * p);
    auto tags_data = tags.data();
    auto hash_values_data = hash_values.head;

    for (auto m = std::bit_ceil(n); m <= limit; m *= 2) {
        if (n_iterations++ >= max_n_iterations) {
            return meta_pair{0zU, 0zU};
        }
        // n_iterations = 1, 2, 3...
        auto best_p = test_hash_modulo(tags_data, n_iterations, hash_values_data, n, m, p);
        if (best_p != npos) {
            return meta_pair{m, best_p};
        }
    }
    for (auto m = n + !!(n % 2 == 0); m <= limit; m += 2) {
        if (n_iterations++ >= max_n_iterations) {
            return meta_pair{0zU, 0zU};
        }
        auto best_p = test_hash_modulo(tags_data, n_iterations, hash_values_data, n, m, p);
        if (best_p != npos) {
            return meta_pair{m, best_p};
        }
    }
    return meta_pair{0zU, 0zU};
}

template <bool A, size_t P, class CharT, class V, template <class> class Policy>
consteval auto make_hash_table_with_skey_impl(
    meta_span<meta_pair<meta_basic_string_view<CharT>, V>> kv_pairs,
    meta_span<size_t> hash_values,
    size_t modulo) -> std::meta::info
{
    using raw_entry_type = meta_triple<size_t, meta_basic_string_view<CharT>, V>;
    using entry_type = aligned_if_t<A, raw_entry_type>;

    auto n = kv_pairs.n;
    const auto* kv_pairs_data = kv_pairs.head;
    const auto* hash_values_data = hash_values.head;

    auto entries = std::vector<entry_type>(modulo + P * P, entry_type{});
    const auto* entries_data = entries.data();

    for (size_t i = 0; i < n; i++) {
        auto cur_hash_value = hash_values_data[i];
        auto base_index = cur_hash_value % modulo;
        auto index = npos;

        for (size_t j = 0; j <= P; j++) {
            auto cur_index = base_index + j * j;
            const raw_entry_type& cur_entry = entries_data[cur_index];
            if (cur_entry.first == 0) {
                index = cur_index;
                break;
            }
        }
        if (index == npos) {
            compile_error("Implementation error: Hash table probing exhausted.");
        }
        const auto& cur_kv_pair = kv_pairs_data[i];
        entries[index] = meta_triple{cur_hash_value, cur_kv_pair.first, cur_kv_pair.second};
    }

    auto min_max_length = min_max_key_length(kv_pairs);
    entries_data = std::define_static_array(entries).data();

    auto res = hash_table_with_skey<A, P, CharT, V, Policy>{
        .entries = entries_data,
        .min_length = min_max_length.first,
        .max_length = min_max_length.second,
        .actual_size = n,
        .modulo = modulo,
    };
    return std::meta::reflect_constant(res);
}

template <class CharT, class V>
consteval auto try_make_hash_table_with_skey(
    meta_span<meta_pair<meta_basic_string_view<CharT>, V>> kv_pairs,
    meta_span<size_t> hash_values,
    const hash_table_with_skey_options& options) -> std::optional<std::meta::info>
{
    // (1) Empty
    if (kv_pairs.n == 0) {
        return make_empty_with_skey<CharT, V>();
    }
    // (2) Hash table
    auto [modulo, best_p] = find_best_hash_modulo(hash_values, options);
    if (modulo == 0) {
        return std::nullopt;  // Not found
    }
    using call_signature = std::meta::info(
        meta_span<meta_pair<meta_basic_string_view<CharT>, V>>, meta_span<size_t>, size_t);
    auto A = std::meta::reflect_constant(options.adjusts_alignment);
    auto P = std::meta::reflect_constant(best_p);
    auto policy = get_skey_policy_template(options.ascii_case_insensitive);
    auto fn =
        extract<call_signature*>(^^make_hash_table_with_skey_impl, A, P, ^^CharT, ^^V, policy);
    return fn(kv_pairs, hash_values, modulo);
}
};  // namespace rbox::impl::map

#endif  // RBOX_FIXED_MAP_CANDIDATES_STRING_BY_HASH_TABLE_HPP
