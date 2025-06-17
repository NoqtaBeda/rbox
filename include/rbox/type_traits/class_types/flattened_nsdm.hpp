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

#ifndef RBOX_TYPE_TRAITS_CLASS_TYPES_FLATTENED_NSDM_HPP
#define RBOX_TYPE_TRAITS_CLASS_TYPES_FLATTENED_NSDM_HPP

#include <rbox/type_traits/class_types/ambiguous_inheritance.hpp>
#include <rbox/type_traits/class_types/non_public_nsdm.hpp>
#include <rbox/type_traits/class_types/virtual_inheritance.hpp>
#include <rbox/utils/meta_span.hpp>
#include <rbox/utils/meta_utility.hpp>

namespace rbox {
struct flattened_data_member_info {
    std::meta::info info;
    std::meta::member_offset actual_offset;

    consteval auto type() const -> std::meta::info
    {
        return std::meta::type_of(info);
    }

    // Which class current member is defined directly in.
    consteval auto direct_parent() const -> std::meta::info
    {
        return std::meta::parent_of(info);
    }

    consteval auto offset_in_parent() const -> std::meta::member_offset
    {
        return std::meta::offset_of(info);
    }

    consteval size_t offset_bits_in_parent() const
    {
        auto offset = offset_in_parent();
        return offset.bytes * 8 + offset.bits;
    }

    consteval size_t offset_bytes_in_parent() const
    {
        auto offset = offset_in_parent();
        if (offset.bits != 0) {
            compile_error("Can not get offset bytes of bit-fields.");
        }
        return offset.bytes;
    }

    consteval size_t actual_offset_bits() const
    {
        return actual_offset.bytes * 8 + actual_offset.bits;
    }

    consteval size_t actual_offset_bytes() const
    {
        if (actual_offset.bits != 0) {
            compile_error("Can not get offset bytes of bit-fields.");
        }
        return actual_offset.bytes;
    }
};

namespace impl {
consteval bool is_flattenable_aggregate(std::meta::info T);

consteval auto walk_all_nsdm(std::meta::info T) -> std::meta::info;
consteval auto walk_public_nsdm(std::meta::info T) -> std::meta::info;

template <class T>
constexpr const auto& all_flattened_nsdm_arr_v = [:walk_all_nsdm(^^T):];

template <class T>
constexpr const auto& public_flattened_nsdm_arr_v = [:walk_public_nsdm(^^T):];
}  // namespace impl

// -------- Partially Flattenable --------

template <class T>
concept partially_flattenable_class =
    class_without_virtual_inheritance<T> && class_without_ambiguous_inheritance<T>;

template <class T>
concept partially_flattenable =
    std::is_scalar_v<T> || std::is_array_v<T> || partially_flattenable_class<T>;

// -------- (Fully) Flattenable --------

template <class T>
concept flattenable_class =
    partially_flattenable_class<T> && class_without_non_public_nonstatic_data_members<T>;

template <class T>
concept flattenable = std::is_scalar_v<T> || std::is_array_v<T> || flattenable_class<T>;

// -------- (Fully) Flattenable Aggregate --------

template <class T>
concept flattenable_aggregate_class =
    flattenable_class<T> && impl::is_flattenable_aggregate(std::meta::remove_cv(^^T));

template <class T>
concept flattenable_aggregate = std::is_array_v<T> || flattenable_aggregate_class<T>;

// -------- Flattened Member List --------

template <partially_flattenable_class T>
constexpr auto all_flattened_nonstatic_data_members_v =
    meta_span(impl::all_flattened_nsdm_arr_v<std::remove_cv_t<T>>);

template <partially_flattenable_class T>
constexpr auto public_flattened_nonstatic_data_members_v =
    meta_span(impl::public_flattened_nsdm_arr_v<std::remove_cv_t<T>>);

namespace impl {
consteval bool is_flattenable_aggregate(std::meta::info T)
{
    // assertion: std::meta::is_class_type(T)
    if (!is_aggregate_type(T)) {
        return false;
    }
    for (auto base : all_direct_bases_of(T)) {
        if (!std::meta::is_public(base) || std::meta::is_virtual(base)) {
            return false;
        }
        if (!extract<bool>(^^flattenable_aggregate, std::meta::type_of(base))) {
            return false;
        }
    }
    for (auto member : all_direct_nonstatic_data_members_of(T)) {
        if (!std::meta::is_public(member)) {
            return false;
        }
    }
    return true;
}

consteval void walk_all_nsdm_dfs(
    std::vector<flattened_data_member_info>& dest, std::meta::info T, ptrdiff_t base_offset_bytes)
{
    for (auto base : all_direct_bases_of(T)) {
        // assertion: not std::meta::is_virtual(base)
        auto next_base_offset_bytes = base_offset_bytes + std::meta::offset_of(base).bytes;
        walk_all_nsdm_dfs(dest, std::meta::type_of(base), next_base_offset_bytes);
    }
    for (auto member : all_direct_nonstatic_data_members_of(T)) {
        auto actual_offset = std::meta::member_offset{
            .bytes = std::meta::offset_of(member).bytes + base_offset_bytes,
            .bits = std::meta::offset_of(member).bits,
        };
        dest.push_back({.info = member, .actual_offset = actual_offset});
    }
}

consteval void walk_public_nsdm_dfs(
    std::vector<flattened_data_member_info>& dest, std::meta::info T, ptrdiff_t base_offset_bytes)
{
    for (auto base : public_direct_bases_of(T)) {
        // assertion: std::meta::is_virtual(base)
        auto next_base_offset_bytes = base_offset_bytes + std::meta::offset_of(base).bytes;
        walk_public_nsdm_dfs(dest, std::meta::type_of(base), next_base_offset_bytes);
    }
    for (auto member : public_direct_nonstatic_data_members_of(T)) {
        auto actual_offset = std::meta::member_offset{
            .bytes = std::meta::offset_of(member).bytes + base_offset_bytes,
            .bits = std::meta::offset_of(member).bits,
        };
        dest.push_back({.info = member, .actual_offset = actual_offset});
    }
}

consteval auto walk_all_nsdm(std::meta::info T) -> std::meta::info
{
    auto members = std::vector<flattened_data_member_info>{};
    walk_all_nsdm_dfs(members, T, 0);
    return std::meta::reflect_constant_array(members);
}

consteval auto walk_public_nsdm(std::meta::info T) -> std::meta::info
{
    auto members = std::vector<flattened_data_member_info>{};
    walk_public_nsdm_dfs(members, T, 0);
    return std::meta::reflect_constant_array(members);
}
}  // namespace impl
}  // namespace rbox

#endif  // RBOX_TYPE_TRAITS_CLASS_TYPES_FLATTENED_NSDM_HPP
