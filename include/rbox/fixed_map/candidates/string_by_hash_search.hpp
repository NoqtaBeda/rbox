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

#ifndef RBOX_FIXED_MAP_CANDIDATES_STRING_BY_HASH_SEARCH_HPP
#define RBOX_FIXED_MAP_CANDIDATES_STRING_BY_HASH_SEARCH_HPP

#include <optional>
#include <rbox/fixed_map/candidates/string_empty.hpp>
#include <rbox/fixed_map/impl/min_max_key_length.hpp>
#include <rbox/fixed_map/impl/string_policy.hpp>
#include <rbox/utils/define_static_array.hpp>
#include <rbox/utils/meta_pair.hpp>
#include <rbox/utils/meta_span.hpp>
#include <rbox/utils/meta_triple.hpp>
#include <rbox/utils/meta_utility.hpp>
#include <rbox/utils/stdlib/algorithm/sort.hpp>

namespace rbox::impl::map {
// Precondition: No hash collision
template <class CharT, class V, template <class> class Policy>
struct linear_hash_search_with_skey {
    using character_type = CharT;
    using key_type = meta_basic_string_view<CharT>;
    using value_type = V;

private:
    using element_type = meta_triple<size_t, meta_basic_string_view<CharT>, V>;

public:
#define RBOX_STRING_BY_LINEAR_HASH_SEARCH_DO_FIND(fallback) \
    do {                                                    \
        auto len = key.length();                            \
        if (len < min_length || len > max_length) {         \
            return fallback;                                \
        }                                                   \
        auto hash = Policy<CharT>::hash(key);               \
        for (const auto& cur : entries) {                   \
            if (hash != cur.first) {                        \
                continue;                                   \
            }                                               \
            if (Policy<CharT>::equals(cur.second, key)) {   \
                return cur.third;                           \
            }                                               \
            return fallback;                                \
        }                                                   \
    } while (false)

    constexpr auto size() const -> size_t
    {
        return entries.n;
    }

    constexpr auto find(std::basic_string_view<CharT> key) const -> std::optional<const value_type&>
    {
        RBOX_STRING_BY_LINEAR_HASH_SEARCH_DO_FIND(std::nullopt);
        return std::nullopt;
    }

    constexpr auto operator[](std::basic_string_view<CharT> key) const -> const value_type&
    {
        RBOX_STRING_BY_LINEAR_HASH_SEARCH_DO_FIND(default_v<value_type>);
        return default_v<value_type>;
    }

#undef RBOX_STRING_BY_LINEAR_HASH_SEARCH_DO_FIND

    meta_span<element_type> entries;
    size_t min_length;
    size_t max_length;
};

// Precondition: No hash collision
template <bool A, class CharT, class V, template <class> class Policy>
struct binary_hash_search_with_skey {
    using character_type = CharT;
    using key_type = meta_basic_string_view<CharT>;
    using value_type = V;

private:
    using raw_element_type = meta_triple<size_t, meta_basic_string_view<CharT>, V>;
    using element_type = aligned_if_t<A, raw_element_type>;

public:
#define RBOX_STRING_BY_HASH_SEARCH_DO_FIND(fallback)          \
    do {                                                      \
        auto len = key.length();                              \
        if (len < min_length || len > max_length) {           \
            return fallback;                                  \
        }                                                     \
        auto hash = Policy<CharT>::hash(key);                 \
                                                              \
        const auto* first = entries.head;                     \
        const auto* last = entries.head + entries.n;          \
        while (first < last) {                                \
            const auto* mid = first + (last - first) / 2;     \
            const raw_element_type& cur = *mid;               \
                                                              \
            if (cur.first == hash) [[unlikely]] {             \
                if (Policy<CharT>::equals(cur.second, key)) { \
                    return cur.third;                         \
                }                                             \
                return fallback;                              \
            }                                                 \
            if (cur.first < hash) {                           \
                first = mid + 1;                              \
            } else {                                          \
                last = mid;                                   \
            }                                                 \
        }                                                     \
    } while (false)

    constexpr auto size() const -> size_t
    {
        return entries.n;
    }

    constexpr auto find(std::basic_string_view<CharT> key) const -> std::optional<const value_type&>
    {
        RBOX_STRING_BY_HASH_SEARCH_DO_FIND(std::nullopt);
        return std::nullopt;
    }

    constexpr auto operator[](std::basic_string_view<CharT> key) const -> const value_type&
    {
        RBOX_STRING_BY_HASH_SEARCH_DO_FIND(default_v<value_type>);
        return default_v<value_type>;
    }

#undef RBOX_STRING_BY_HASH_SEARCH_DO_FIND

    meta_span<element_type> entries;
    size_t min_length;
    size_t max_length;
};

// Precondition: Hash collisions may exist
template <bool A, class CharT, class V, template <class> class Policy>
struct colliding_binary_hash_search_with_skey {
    using character_type = CharT;
    using key_type = meta_basic_string_view<CharT>;
    using value_type = V;

private:
    using raw_element_type = meta_triple<size_t, meta_basic_string_view<CharT>, V>;
    using element_type = aligned_if_t<A, raw_element_type>;

public:
#define RBOX_STRING_BY_COLLIDING_HASH_SEARCH_DO_FIND(fallback) \
    do {                                                       \
        auto len = key.length();                               \
        if (len < min_length || len > max_length) {            \
            return fallback;                                   \
        }                                                      \
        auto hash = Policy<CharT>::hash(key);                  \
                                                               \
        const auto* first = entries.head;                      \
        const auto* last = entries.head + entries.n;           \
        while (first < last) {                                 \
            const auto* mid = first + (last - first) / 2;      \
            const raw_element_type& cur = *mid;                \
                                                               \
            if (cur.first < hash) {                            \
                first = mid + 1;                               \
            } else {                                           \
                last = mid;                                    \
            }                                                  \
        }                                                      \
        const auto* scan_end = entries.head + entries.n;       \
        for (const auto* it = first; it < scan_end; ++it) {    \
            const raw_element_type& cur = *it;                 \
            if (cur.first != hash) {                           \
                break;                                         \
            }                                                  \
            if (Policy<CharT>::equals(cur.second, key)) {      \
                return cur.third;                              \
            }                                                  \
        }                                                      \
    } while (false)

    constexpr auto size() const -> size_t
    {
        return entries.n;
    }

    constexpr auto find(std::basic_string_view<CharT> key) const -> std::optional<const value_type&>
    {
        RBOX_STRING_BY_COLLIDING_HASH_SEARCH_DO_FIND(std::nullopt);
        return std::nullopt;
    }

    constexpr auto operator[](std::basic_string_view<CharT> key) const -> const value_type&
    {
        RBOX_STRING_BY_COLLIDING_HASH_SEARCH_DO_FIND(default_v<value_type>);
        return default_v<value_type>;
    }

#undef RBOX_STRING_BY_COLLIDING_HASH_SEARCH_DO_FIND

    meta_span<element_type> entries;
    size_t min_length;
    size_t max_length;
};

// -------- Builder --------

struct hash_search_with_skey_options {
    bool ascii_case_insensitive;
    bool adjusts_alignment;
    size_t binary_search_threshold;
};

template <class CharT, class V, template <class> class Policy>
consteval auto make_linear_hash_search_with_skey(
    meta_span<meta_pair<meta_basic_string_view<CharT>, V>> kv_pairs, meta_span<size_t> hash_values)
    -> std::meta::info
{
    auto n = kv_pairs.n;
    auto entries = std::vector<meta_triple<size_t, meta_basic_string_view<CharT>, V>>{};
    entries.reserve(n);

    const auto* kv_pairs_data = kv_pairs.head;
    const auto* hash_values_data = hash_values.head;
    for (auto i = 0zU; i < n; i++) {
        const auto& cur = kv_pairs_data[i];
        entries.push_back(meta_triple{hash_values_data[i], cur.first, cur.second});
    }

    auto min_max_length = min_max_key_length(kv_pairs);
    auto entries_span = rbox::define_static_array(entries);

    auto obj = linear_hash_search_with_skey<CharT, V, Policy>{
        .entries = entries_span,
        .min_length = min_max_length.first,
        .max_length = min_max_length.second,
    };
    return std::meta::reflect_constant(obj);
}

template <bool A, bool C, class CharT, class V, template <class> class Policy>
consteval auto make_binary_hash_search_with_skey(
    meta_span<meta_pair<meta_basic_string_view<CharT>, V>> kv_pairs, meta_span<size_t> hash_values)
    -> std::meta::info
{
    using raw_entry_type = meta_triple<size_t, meta_basic_string_view<CharT>, V>;
    using entry_type = aligned_if_t<A, raw_entry_type>;

    auto n = kv_pairs.n;
    auto entries = std::vector<entry_type>{};
    entries.reserve(n);

    const auto* kv_pairs_data = kv_pairs.head;
    const auto* hash_values_data = hash_values.head;
    for (auto i = 0zU; i < n; i++) {
        const auto& cur = kv_pairs_data[i];
        entries.push_back(meta_triple{hash_values_data[i], cur.first, cur.second});
    }
    auto* entries_data = entries.data();

    // Sorts by hash value
    std::sort(entries_data, entries_data + n, [](const raw_entry_type& a, const raw_entry_type& b) {
        return a.first < b.first;
    });

    auto min_max_length = min_max_key_length(kv_pairs);
    auto entries_span = rbox::define_static_array(entries);

    using result_type = std::conditional_t<
        C,  // The colliding variant is required when hash collisions exist
        colliding_binary_hash_search_with_skey<A, CharT, V, Policy>,
        binary_hash_search_with_skey<A, CharT, V, Policy>>;

    auto obj = result_type{
        .entries = entries_span,
        .min_length = min_max_length.first,
        .max_length = min_max_length.second,
    };
    return std::meta::reflect_constant(obj);
}

template <class CharT, class V>
consteval auto make_hash_search_with_skey(
    meta_span<meta_pair<meta_basic_string_view<CharT>, V>> kv_pairs,
    meta_span<size_t> hash_values,
    bool has_hash_collision,
    const hash_search_with_skey_options& options) -> std::meta::info
{
    auto n = kv_pairs.n;
    // (1) Empty
    if (n == 0) {
        return make_empty_with_skey<CharT, V>();
    }

    using call_signature =
        std::meta::info(meta_span<meta_pair<meta_basic_string_view<CharT>, V>>, meta_span<size_t>);

    auto policy = get_skey_policy_template(options.ascii_case_insensitive);
    // (2) Linear search
    if (!has_hash_collision && n < options.binary_search_threshold) {
        auto fn =
            extract<call_signature*>(^^make_linear_hash_search_with_skey, ^^CharT, ^^V, policy);
        return fn(kv_pairs, hash_values);
    }
    // (3) Binary search
    auto A = std::meta::reflect_constant(options.adjusts_alignment);
    auto C = std::meta::reflect_constant(has_hash_collision);
    auto fn =
        extract<call_signature*>(^^make_binary_hash_search_with_skey, A, C, ^^CharT, ^^V, policy);
    return fn(kv_pairs, hash_values);
}
}  // namespace rbox::impl::map

#endif  // RBOX_FIXED_MAP_CANDIDATES_STRING_BY_HASH_SEARCH_HPP
