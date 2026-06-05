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

#ifndef RBOX_FIXED_MAP_CANDIDATES_INTEGRAL_DENSE_HPP
#define RBOX_FIXED_MAP_CANDIDATES_INTEGRAL_DENSE_HPP

#include <optional>
#include <rbox/fixed_map/candidates/integral_empty.hpp>
#include <rbox/utils/define_static_array.hpp>
#include <rbox/utils/functional/integral_comparison.hpp>
#include <rbox/utils/meta_pair.hpp>
#include <rbox/utils/meta_string_view.hpp>
#include <rbox/utils/meta_utility.hpp>

namespace rbox::impl::map {
template <scalar_type T>
constexpr bool is_hole(T value)
{
    return value == T{};
}

template <class T>
constexpr bool is_hole(meta_span<T> range)
{
    return range.head == nullptr;
}

template <class CharT>
constexpr bool is_hole(meta_basic_string_view<CharT> str)
{
    return str.head == nullptr;
}

template <class T>
concept may_have_holes = requires (T value) {
    { rbox::impl::map::is_hole(value) } -> std::same_as<bool>;
};

template <bool A, class K, class V>
struct fully_dense_with_ikey {
    using key_type = K;
    using value_type = V;

private:
    using element_type = aligned_if_t<A, V>;

public:
    constexpr auto size() const -> size_t
    {
        return static_cast<size_t>(max_key - min_key + 1);
    }

    constexpr auto find(key_type key) const -> std::optional<const value_type&>
    {
        if (key >= min_key && key <= max_key) {
            return entries[key - min_key];
        }
        return std::nullopt;
    }

    constexpr auto find(non_bool_integral auto key) const -> std::optional<const value_type&>
    {
        if (!in_range<key_type>(key)) {
            return std::nullopt;
        }
        return find(static_cast<key_type>(key));
    }

    constexpr auto operator[](key_type key) const -> const value_type&
    {
        if (key >= min_key && key <= max_key) {
            return entries[key - min_key];
        }
        return default_v<value_type>;
    }

    constexpr auto operator[](non_bool_integral auto key) const -> const value_type&
    {
        if (!in_range<key_type>(key)) {
            return default_v<value_type>;
        }
        return operator[](static_cast<key_type>(key));
    }

    const element_type* entries;
    key_type min_key;
    key_type max_key;
};

template <bool A, class K, class V>
struct non_null_dense_with_ikey {
    using key_type = K;
    using value_type = V;

private:
    using element_type = aligned_if_t<A, V>;

public:
    constexpr auto size() const -> size_t
    {
        return actual_size;
    }

    constexpr auto find(key_type key) const -> std::optional<const value_type&>
    {
        if (key >= min_key && key <= max_key) {
            const value_type& target = entries[key - min_key];
            if (!is_hole(target)) {
                return target;
            }
        }
        return std::nullopt;
    }

    constexpr auto find(non_bool_integral auto key) const -> std::optional<const value_type&>
    {
        if (!in_range<key_type>(key)) {
            return std::nullopt;
        }
        return find(static_cast<key_type>(key));
    }

    constexpr auto operator[](key_type key) const -> const value_type&
    {
        if (key >= min_key && key <= max_key) {
            const value_type& target = entries[key - min_key];
            if (!is_hole(target)) {
                return target;
            }
        }
        return default_v<value_type>;
    }

    constexpr auto operator[](non_bool_integral auto key) const -> const value_type&
    {
        if (!in_range<key_type>(key)) {
            return default_v<value_type>;
        }
        return operator[](static_cast<key_type>(key));
    }

    const element_type* entries;
    size_t actual_size;
    key_type min_key;
    key_type max_key;
};

template <bool A, class K, class V>
struct dense_with_ikey {
    using key_type = K;
    using value_type = V;

private:
    using raw_element_type = meta_pair<V, bool>;
    using element_type = std::conditional_t<A, aligned<raw_element_type>, raw_element_type>;

public:
    constexpr auto size() const -> size_t
    {
        return actual_size;
    }

    constexpr auto find(key_type key) const -> std::optional<const value_type&>
    {
        if (key >= min_key && key <= max_key) {
            const raw_element_type& target_entry = entries[key - min_key];
            if (target_entry.second) {
                return target_entry.first;
            }
        }
        return std::nullopt;
    }

    constexpr auto find(non_bool_integral auto key) const -> std::optional<const value_type&>
    {
        if (!in_range<key_type>(key)) {
            return std::nullopt;
        }
        return find(static_cast<key_type>(key));
    }

    constexpr auto operator[](key_type key) const -> const value_type&
    {
        if (key >= min_key && key <= max_key) {
            const raw_element_type& target_entry = entries[key - min_key];
            if (target_entry.second) {
                return target_entry.first;
            }
        }
        return default_v<value_type>;
    }

    constexpr auto operator[](non_bool_integral auto key) const -> const value_type&
    {
        if (!in_range<key_type>(key)) {
            return default_v<value_type>;
        }
        return operator[](static_cast<key_type>(key));
    }

    const element_type* entries;
    size_t actual_size;
    key_type min_key;
    key_type max_key;
};

// -------- Builder --------

struct dense_with_ikey_options {
    bool adjusts_alignment;
};

template <bool A, class K, class V>
consteval auto make_fully_dense_with_ikey(
    meta_span<meta_pair<K, V>> sorted_input, K min_key, K max_key) -> std::meta::info
{
    auto n = sorted_input.n;
    auto values = std::vector<aligned_if_t<A, V>>{};
    values.reserve(n);
    for (const auto& cur : sorted_input) {
        values.push_back(cur.second);
    }
    const auto* values_data = std::define_static_array(values).data();
    auto obj = fully_dense_with_ikey<A, K, V>{values_data, min_key, max_key};
    return std::meta::reflect_constant(obj);
}

template <bool A, class K, class V>
consteval auto make_non_null_dense_with_ikey(
    meta_span<meta_pair<K, V>> sorted_input, K min_key, K max_key) -> std::meta::info
{
    auto n = sorted_input.n;
    auto table_size = static_cast<size_t>(max_key - min_key + 1);

    using element_type = aligned_if_t<A, V>;
    auto values = std::vector<element_type>(table_size, element_type{});
    for (const auto& cur : sorted_input) {
        V& dest = values[cur.first - min_key];
        dest = cur.second;
    }
    const auto* values_data = std::define_static_array(values).data();
    auto obj = non_null_dense_with_ikey<A, K, V>{values_data, n, min_key, max_key};
    return std::meta::reflect_constant(obj);
}

template <bool A, class K, class V>
consteval auto make_generic_dense_with_ikey(
    meta_span<meta_pair<K, V>> sorted_input, K min_key, K max_key) -> std::meta::info
{
    auto n = sorted_input.n;
    auto table_size = static_cast<size_t>(max_key - min_key + 1);

    using element_type = aligned_if_t<A, meta_pair<V, bool>>;
    auto values = std::vector<element_type>(table_size, element_type{});
    for (const auto& cur : sorted_input) {
        meta_pair<V, bool>& dest = values[cur.first - min_key];
        dest.first = cur.second;
        dest.second = true;  // is_valid flag
    }
    const auto* values_data = std::define_static_array(values).data();
    auto obj = dense_with_ikey<A, K, V>{values_data, n, min_key, max_key};
    return std::meta::reflect_constant(obj);
}

template <class K, class V>
consteval auto make_dense_with_ikey(
    meta_span<meta_pair<K, V>> sorted_input, dense_with_ikey_options options) -> std::meta::info
{
    auto n = sorted_input.n;
    if (n == 0) {
        // (1) Empty
        return make_empty_with_ikey<V>();
    }

    using call_signature = std::meta::info(meta_span<meta_pair<K, V>>, K, K);
    auto A = std::meta::reflect_constant(options.adjusts_alignment);

    auto min_key = sorted_input.front().first;
    auto max_key = sorted_input.back().first;
    if (max_key - min_key + 1 == n) {
        // (2) Fully dense
        auto fn = extract<call_signature*>(^^make_fully_dense_with_ikey, A, ^^K, ^^V);
        return fn(sorted_input, min_key, max_key);
    }
    if constexpr (may_have_holes<V>) {
        auto has_holes = false;
        for (const auto *it = sorted_input.head, *end = it + n; !has_holes && it < end; it++) {
            has_holes |= is_hole(it->second);
        }
        // (3) Non-null dense (no value happen to be equal to default_v<V>)
        if (!has_holes) {
            auto fn = extract<call_signature*>(^^make_non_null_dense_with_ikey, A, ^^K, ^^V);
            return fn(sorted_input, min_key, max_key);
        }
    }
    // (4) Dense (with an additional flag for validation)
    auto fn = extract<call_signature*>(^^make_generic_dense_with_ikey, A, ^^K, ^^V);
    return fn(sorted_input, min_key, max_key);
}
}  // namespace rbox::impl::map

#endif  // RBOX_FIXED_MAP_CANDIDATES_INTEGRAL_DENSE_HPP
