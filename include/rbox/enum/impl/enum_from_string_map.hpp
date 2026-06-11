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

#ifndef RBOX_ENUM_IMPL_ENUM_FROM_STRING_MAP_HPP
#define RBOX_ENUM_IMPL_ENUM_FROM_STRING_MAP_HPP

#include <rbox/enum/enum_names.hpp>
#include <rbox/enum/impl/promoted.hpp>
#include <rbox/fixed_map/string_key.hpp>
#include <rbox/utils/meta_string_view.hpp>
#include <utility>

namespace rbox::impl {
template <class E>
consteval auto make_enum_from_string_kv_pairs()
{
    using kv_pair_t = std::pair<meta_string_view, promoted_t<E>>;
    auto res = std::vector<kv_pair_t>{};
    res.reserve(enum_count_v<E>);

    auto entries = std::meta::enumerators_of(^^E);
    for (auto i = 0zU, n = enum_count_v<E>; i < n; i++) {
        auto ev = std::meta::extract<E>(entries[i]);
        auto msv = meta_string_view(null_terminated, enum_names_v<E>[i]);
        res.emplace_back(msv, promoted(ev));
    }
    return res;
}

template <class E>
consteval auto make_enum_from_string_map()
{
    constexpr auto options = string_key_fixed_map_options{
        .already_ascii_only = enum_names_are_ascii_only_v<E>,
        .already_unique = true,
        .ascii_case_insensitive = false,
        .adjusts_alignment = true,
    };
    return RBOX_STRING_KEY_FIXED_MAP(make_enum_from_string_kv_pairs<E>(), options);
}

template <class E>
consteval auto make_enum_from_ci_string_map()
{
    constexpr auto options = string_key_fixed_map_options{
        .already_ascii_only = enum_names_are_ascii_only_v<E>,
        .already_unique = false,
        .ascii_case_insensitive = true,
        .adjusts_alignment = true,
    };
    return RBOX_STRING_KEY_FIXED_MAP(make_enum_from_string_kv_pairs<E>(), options);
}

template <class E>
constexpr auto enum_from_string_map_v = make_enum_from_string_map<E>();

template <class E>
constexpr auto enum_from_ci_string_map_v = make_enum_from_ci_string_map<E>();
}  // namespace rbox::impl

#endif  // RBOX_ENUM_IMPL_ENUM_FROM_STRING_MAP_HPP
