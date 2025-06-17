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
    using element_type = std::conditional_t<A, aligned<V>, V>;

public:
    constexpr auto size() const -> size_t
    {
        return static_cast<size_t>(max_key - min_key + 1);
    }

    constexpr auto find(key_type key) const -> std::optional<const value_type&>
    {
        if (key >= min_key && key <= max_key) {
            return unwrap(entries[key - min_key]);
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
            return unwrap(entries[key - min_key]);
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
    using element_type = std::conditional_t<A, aligned<V>, V>;

public:
    constexpr auto size() const -> size_t
    {
        return actual_size;
    }

    constexpr auto find(key_type key) const -> std::optional<const value_type&>
    {
        if (key >= min_key && key <= max_key) {
            const auto& target = unwrap(entries[key - min_key]);
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
            const auto& target = unwrap(entries[key - min_key]);
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
    using element_pair_type = meta_pair<V, bool>;
    using element_type = std::conditional_t<A, aligned<element_pair_type>, element_pair_type>;

public:
    constexpr auto size() const -> size_t
    {
        return actual_size;
    }

    constexpr auto find(key_type key) const -> std::optional<const value_type&>
    {
        if (key >= min_key && key <= max_key) {
            const auto& target_entry = unwrap(entries[key - min_key]);
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
            const auto& target_entry = unwrap(entries[key - min_key]);
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

template <class K, class V>
consteval auto make_fully_dense_with_ikey(
    std::span<const meta_pair<K, V>> sorted_input,
    dense_with_ikey_options options,
    K min_key,
    K max_key) -> std::meta::info
{
    auto n = sorted_input.size();
    if (options.adjusts_alignment) {
        // (2.1) with alignment optimization
        auto aligned_values = std::vector<aligned<V>>{};
        aligned_values.reserve(n);
        for (const auto& [_, v] : sorted_input) {
            aligned_values.push_back(aligned{v});
        }
        auto entries = std::define_static_array(aligned_values);
        auto obj = fully_dense_with_ikey<true, K, V>{entries.data(), min_key, max_key};
        return std::meta::reflect_constant(obj);
    } else {
        // (2.2) without alignment optimization
        auto values = std::vector<V>{};
        values.reserve(n);
        for (const auto& [_, v] : sorted_input) {
            values.push_back(v);
        }
        auto entries = std::define_static_array(values);
        auto obj = fully_dense_with_ikey<false, K, V>{entries.data(), min_key, max_key};
        return std::meta::reflect_constant(obj);
    }
}

template <class K, class V>
consteval auto make_non_null_dense_with_ikey(
    std::span<const meta_pair<K, V>> sorted_input,
    dense_with_ikey_options options,
    K min_key,
    K max_key) -> std::meta::info
{
    auto n = sorted_input.size();
    auto table_size = static_cast<size_t>(max_key - min_key + 1);

    if (options.adjusts_alignment) {
        auto aligned_values = std::vector<aligned<V>>(table_size, aligned<V>{});
        for (const auto& entry : sorted_input) {
            aligned_values[entry.first - min_key].underlying = entry.second;
        }
        auto entries = std::define_static_array(aligned_values);
        auto obj = non_null_dense_with_ikey<true, K, V>{entries.data(), n, min_key, max_key};
        return std::meta::reflect_constant(obj);
    } else {
        auto values = std::vector<V>(table_size, V{});
        for (const auto& entry : sorted_input) {
            values[entry.first - min_key] = entry.second;
        }
        auto entries = std::define_static_array(values);
        auto obj = non_null_dense_with_ikey<false, K, V>{entries.data(), n, min_key, max_key};
        return std::meta::reflect_constant(obj);
    }
}

template <class K, class V>
consteval auto make_dense_with_ikey(
    std::span<const meta_pair<K, V>> sorted_input,
    dense_with_ikey_options options,
    K min_key,
    K max_key) -> std::meta::info
{
    auto n = sorted_input.size();
    auto table_size = static_cast<size_t>(max_key - min_key + 1);

    if (options.adjusts_alignment) {
        using aligned_pair_type = aligned<meta_pair<V, bool>>;
        auto aligned_values = std::vector<aligned_pair_type>(table_size, aligned_pair_type{});
        for (const auto& entry : sorted_input) {
            auto& dest = aligned_values[entry.first - min_key].underlying;
            dest.first = entry.second;
            dest.second = true;  // is_valid flag
        }
        auto entries = std::define_static_array(aligned_values);
        auto obj = dense_with_ikey<true, K, V>{entries.data(), n, min_key, max_key};
        return std::meta::reflect_constant(obj);
    } else {
        auto values = std::vector<meta_pair<V, bool>>(table_size, meta_pair<V, bool>{});
        for (const auto& entry : sorted_input) {
            auto& dest = values[entry.first - min_key];
            dest.first = entry.second;
            dest.second = true;  // is_valid flag
        }
        auto entries = std::define_static_array(values);
        auto obj = dense_with_ikey<false, K, V>{entries.data(), n, min_key, max_key};
        return std::meta::reflect_constant(obj);
    }
}

template <class K, class V>
consteval auto make_dense_with_ikey(
    std::span<const meta_pair<K, V>> sorted_input, dense_with_ikey_options options)
    -> std::meta::info
{
    auto n = sorted_input.size();
    if (n == 0) {
        // (1) Empty
        return make_empty_with_ikey<V>();
    }
    auto min_key = sorted_input.front().first;
    auto max_key = sorted_input.back().first;
    if (max_key - min_key + 1 == n) {
        // (2) Fully dense
        return make_fully_dense_with_ikey(sorted_input, options, min_key, max_key);
    }
    if constexpr (may_have_holes<V>) {
        auto has_holes = false;
        for (auto i = 0zU; !has_holes && i < n; i++) {
            has_holes |= (sorted_input[i].second == default_v<V>);
        }
        // (3) Non-null dense (no value happen to be equal to default_v<V>)
        if (!has_holes) {
            return make_non_null_dense_with_ikey(sorted_input, options, min_key, max_key);
        }
    }
    // (4) Dense (with an additional flag for validation)
    return make_dense_with_ikey(sorted_input, options, min_key, max_key);
}
}  // namespace rbox::impl::map

#endif  // RBOX_FIXED_MAP_CANDIDATES_INTEGRAL_DENSE_HPP
