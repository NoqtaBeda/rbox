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
#include <rbox/fixed_map/impl/string_policy.hpp>
#include <rbox/utils/define_static_array.hpp>
#include <rbox/utils/meta_pair.hpp>
#include <rbox/utils/meta_span.hpp>
#include <rbox/utils/meta_triple.hpp>
#include <rbox/utils/meta_utility.hpp>

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
    constexpr auto size() const -> size_t
    {
        return entries.size();
    }

    constexpr auto find(std::basic_string_view<CharT> key) const -> std::optional<const value_type&>
    {
        auto len = key.length();
        if (len < min_length || len > max_length) {
            return std::nullopt;
        }
        auto hash = Policy<CharT>::hash(key);
        for (const auto& cur : entries) {
            if (hash != cur.first) continue;
            if (Policy<CharT>::equals(cur.second, key)) {
                return cur.third;
            }
            break;
        }
        return std::nullopt;
    }

    constexpr auto operator[](std::basic_string_view<CharT> key) const -> const value_type&
    {
        auto p = find(key);
        return p ? *p : default_v<value_type>;
    }

    meta_span<element_type> entries;
    size_t min_length;
    size_t max_length;
};

template <bool A, bool C, class CharT, class V, template <class> class Policy>
struct binary_hash_search_with_skey {
    using character_type = CharT;
    using key_type = meta_basic_string_view<CharT>;
    using value_type = V;

private:
    using raw_element_type = meta_triple<size_t, meta_basic_string_view<CharT>, V>;
    using element_type = std::conditional_t<A, aligned<raw_element_type>, raw_element_type>;

public:
    constexpr auto size() const -> size_t
    {
        return entries.size();
    }

    constexpr auto find(std::basic_string_view<CharT> key) const -> std::optional<const value_type&>
    {
        auto len = key.length();
        if (len < min_length || len > max_length) {
            return std::nullopt;
        }
        auto hash = Policy<CharT>::hash(key);
        constexpr auto hash_proj = [](const auto& entry) {
            return unwrap(entry).first;
        };
        auto first = entries.begin();
        auto last = entries.end();
        auto dist = last - first;
        while (dist > 0) {
            auto half = dist / 2;
            auto mid = first + half;
            if (hash_proj(*mid) < hash) {
                first = mid + 1;
                dist -= half + 1;
            } else {
                dist = half;
            }
        }
        if constexpr (C) {
            for (auto it = first; it != last && hash_proj(*it) == hash; ++it) {
                const auto& [_, k, v] = unwrap(*it);
                if (Policy<CharT>::equals(k, key)) return v;
            }
            return std::nullopt;
        } else {
            if (first == last) {
                return std::nullopt;
            }
            const auto& cur = unwrap(*first);
            if (cur.first == hash && Policy<CharT>::equals(cur.second, key)) {
                return cur.third;
            }
            return std::nullopt;
        }
    }

    constexpr auto operator[](std::basic_string_view<CharT> key) const -> const value_type&
    {
        auto p = find(key);
        return p ? *p : default_v<value_type>;
    }

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
    std::span<const meta_pair<meta_basic_string_view<CharT>, V>> kv_pairs,
    std::span<const size_t> hash_values) -> std::meta::info
{
    // Makes obj
    auto min_length = SIZE_MAX;
    auto max_length = 0zU;
    for (const auto& entry : kv_pairs) {
        auto len = entry.first.length();
        min_length = std::min(min_length, len);
        max_length = std::max(max_length, len);
    }

    auto n = kv_pairs.size();
    auto entries = std::vector<meta_triple<size_t, meta_basic_string_view<CharT>, V>>{};
    entries.reserve(n);
    for (auto i = 0zU; i < n; i++) {
        const auto& [k, v] = kv_pairs[i];
        entries.push_back(meta_triple{hash_values[i], k, v});
    }

    auto obj = linear_hash_search_with_skey<CharT, V, Policy>{
        .entries = rbox::define_static_array(entries),
        .min_length = min_length,
        .max_length = max_length,
    };
    return std::meta::reflect_constant(obj);
}

template <bool A, bool C, class CharT, class V, template <class> class Policy>
consteval auto make_binary_hash_search_with_skey(
    std::span<const meta_pair<meta_basic_string_view<CharT>, V>> kv_pairs,
    std::span<const size_t> hash_values) -> std::meta::info
{
    using entry_type = meta_triple<size_t, meta_basic_string_view<CharT>, V>;
    // Makes obj
    auto to_length = [](const auto& entry) {
        return entry.first.length();
    };
    auto min_length = SIZE_MAX;
    auto max_length = 0zU;
    for (const auto& entry : kv_pairs) {
        auto len = entry.first.length();
        min_length = std::min(min_length, len);
        max_length = std::max(max_length, len);
    }

    auto n = kv_pairs.size();
    auto entries = std::vector<entry_type>{};
    entries.reserve(n);
    for (auto i = 0zU; i < n; i++) {
        const auto& [k, v] = kv_pairs[i];
        entries.push_back(meta_triple{hash_values[i], k, v});
    }
    // Sorts by hash value
    std::ranges::sort(entries, {}, &entry_type::first);

    auto obj = binary_hash_search_with_skey<A, C, CharT, V, Policy>{
        .min_length = min_length,
        .max_length = max_length,
    };
    if constexpr (A) {
        auto aligned_entries = std::vector<aligned<entry_type>>{};
        aligned_entries.reserve(n);
        for (const auto& entry : entries) {
            aligned_entries.push_back(aligned{entry});
        }
        obj.entries = rbox::define_static_array(aligned_entries);
    } else {
        obj.entries = rbox::define_static_array(entries);
    }
    return std::meta::reflect_constant(obj);
}

template <class CharT, class V>
consteval auto make_hash_search_with_skey(
    std::span<const meta_pair<meta_basic_string_view<CharT>, V>> kv_pairs,
    std::span<const size_t> hash_values,
    bool has_hash_collision,
    const hash_search_with_skey_options& options) -> std::meta::info
{
    // (1) Empty
    if (kv_pairs.empty()) {
        return make_empty_with_skey<CharT, V>();
    }

    using call_signature = std::meta::info(
        std::span<const meta_pair<meta_basic_string_view<CharT>, V>>, std::span<const size_t>);
    // (2) Linear search
    if (!has_hash_collision && kv_pairs.size() < options.binary_search_threshold) {
        auto policy = get_skey_policy_template(options.ascii_case_insensitive);
        auto fn =
            extract<call_signature*>(^^make_linear_hash_search_with_skey, ^^CharT, ^^V, policy);
        return fn(kv_pairs, hash_values);
    }
    // (3) Binary search
    auto A = std::meta::reflect_constant(options.adjusts_alignment);
    auto C = std::meta::reflect_constant(has_hash_collision);
    auto policy = get_skey_policy_template(options.ascii_case_insensitive);
    auto fn =
        extract<call_signature*>(^^make_binary_hash_search_with_skey, A, C, ^^CharT, ^^V, policy);
    return fn(kv_pairs, hash_values);
}
}  // namespace rbox::impl::map

#endif  // RBOX_FIXED_MAP_CANDIDATES_STRING_BY_HASH_SEARCH_HPP
