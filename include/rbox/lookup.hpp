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

#ifndef RBOX_LOOKUP_HPP
#define RBOX_LOOKUP_HPP

#include <rbox/fixed_map.hpp>
#include <rbox/type_traits/class_types/flattened_nsdm.hpp>
#include <rbox/type_traits/pointer_to_member.hpp>
#include <rbox/type_traits/template_instance.hpp>
#include <rbox/utils/addressable_member.hpp>
#include <rbox/utils/meta_variant.hpp>
#include <rbox/utils/stdlib/algorithm/sort.hpp>
#include <rbox/utils/stdlib/algorithm/unique.hpp>

namespace rbox {
namespace impl::lookup {
consteval auto transformed_key_type(std::meta::info result) -> std::meta::info
{
    result = std::meta::dealias(result);
    if (std::meta::has_template_arguments(result)
        && std::meta::template_of(result) == ^^std::optional) {
        return std::meta::template_arguments_of(result)[0];
    } else {
        return result;  // Unchanged
    }
}

consteval bool is_ikey_pattern_transform_fn(std::meta::info fn_type)
{
    auto params_il = {^^std::string_view};
    if (!std::meta::is_invocable_type(fn_type, params_il)) {
        return false;
    }
    auto result = transformed_key_type(invoke_result(fn_type, params_il));
    return extract<bool>(^^non_bool_integral, result) || extract<bool>(^^enum_type, result);
}

consteval bool is_skey_pattern_transform_fn(std::meta::info fn_type)
{
    auto params_il = {^^std::string_view};
    if (!std::meta::is_invocable_type(fn_type, params_il)) {
        return false;
    }
    auto result = transformed_key_type(invoke_result(fn_type, params_il));
    return extract<bool>(^^string_like_of, result, ^^char);
}

consteval bool is_ikey_member_transform_fn(std::meta::info fn_type)
{
    auto params_il = {^^std::meta::info};
    if (!std::meta::is_invocable_type(fn_type, params_il)) {
        return false;
    }
    auto result = transformed_key_type(invoke_result(fn_type, params_il));
    return extract<bool>(^^non_bool_integral, result) || extract<bool>(^^enum_type, result);
}

consteval bool is_skey_member_transform_fn(std::meta::info fn_type)
{
    auto params_il = {^^std::meta::info};
    if (!std::meta::is_invocable_type(fn_type, params_il)) {
        return false;
    }
    auto result = transformed_key_type(invoke_result(fn_type, params_il));
    return extract<bool>(^^string_like_of, result, ^^char);
}

template <class T>
using transformed_key_t = [:transformed_key_type(^^T):];

template <class T>
concept ikey_pattern_transform_fn = is_ikey_pattern_transform_fn(^^T);

template <class T>
concept skey_pattern_transform_fn = is_skey_pattern_transform_fn(^^T);

template <class T>
concept ikey_member_transform_fn = is_ikey_member_transform_fn(^^T);

template <class T>
concept skey_member_transform_fn = is_skey_member_transform_fn(^^T);

using direct_members_query_fn =  // std::meta::members_of(), etc.
    std::vector<std::meta::info>(std::meta::info, std::meta::access_context);

template <class TransformFn>
consteval auto lookup_nonstatic_data_members(std::meta::info T, const TransformFn& transform_fn)
{
    // TransformResult = std::optional<Ret>
    using TransformResult = std::invoke_result_t<TransformFn, std::meta::info>;
    using Ret = transformed_key_t<TransformResult>;

    // For non-static data members: public members only due to addressing constraints
    auto members = extract<meta_span<flattened_data_member_info>>(
        ^^public_flattened_nonstatic_data_members_v, T);

    auto res = std::vector<std::pair<Ret, std::meta::info>>{};
    for (auto [info, _] : members) {
        if (!is_addressable_class_member(info)) {
            continue;
        }
        if constexpr (template_instance_of<TransformResult, std::optional>) {
            if (auto tr = transform_fn(info)) {
                res.emplace_back(*tr, info);
            }
        } else {
            res.emplace_back(transform_fn(info), info);
        }
    }
    return res;
}

template <class Ret, class TransformFn>
consteval void lookup_class_members_impl(
    std::vector<std::pair<Ret, std::meta::info>>& dest,
    std::meta::info T,
    std::meta::access_context ctx,
    direct_members_query_fn* get_direct_members_fn,
    const TransformFn& transform_fn)
{
    using TransformResult = std::invoke_result_t<TransformFn, std::meta::info>;
    for (auto base : std::meta::bases_of(T, ctx)) {
        auto B = std::meta::type_of(base);
        lookup_class_members_impl(dest, B, ctx, get_direct_members_fn, transform_fn);
    }
    for (auto info : get_direct_members_fn(T, ctx)) {
        if (!is_addressable_class_member(info)) {
            continue;
        }
        if (std::meta::is_reference_type(std::meta::type_of(info))) {
            continue;  // References are filtered out in current implementation
        }
        if constexpr (template_instance_of<TransformResult, std::optional>) {
            if (auto tr = transform_fn(info)) {
                dest.emplace_back(*tr, info);
            }
        } else {
            dest.emplace_back(transform_fn(info), info);
        }
    }
}

template <class TransformFn>
consteval auto lookup_class_members(
    std::meta::info T,
    std::meta::access_context ctx,
    direct_members_query_fn* get_direct_members_fn,
    const TransformFn& transform_fn)
{
    // TransformResult = std::optional<Ret>
    using TransformResult = std::invoke_result_t<TransformFn, std::meta::info>;
    using Ret = transformed_key_t<TransformResult>;

    auto res = std::vector<std::pair<Ret, std::meta::info>>{};
    lookup_class_members_impl(res, T, ctx, get_direct_members_fn, transform_fn);
    return res;
}

template <class TransformFn>
consteval auto lookup_namespace_members(std::meta::info ns, const TransformFn& transform_fn)
{
    // TransformResult = std::optional<Ret>
    using TransformResult = std::invoke_result_t<TransformFn, std::meta::info>;
    using Ret = transformed_key_t<TransformResult>;

    auto res = std::vector<std::pair<Ret, std::meta::info>>{};
    auto ctx = unprivileged_context();
    for (auto info : std::meta::members_of(ns, ctx)) {
        if (!is_addressable_non_class_member(info)) {
            continue;
        }
        if (std::meta::is_reference_type(std::meta::type_of(info))) {
            continue;  // References are filtered out in current implementation
        }
        if constexpr (template_instance_of<TransformResult, std::optional>) {
            if (auto tr = transform_fn(info)) {
                res.emplace_back(*tr, info);
            }
        } else {
            res.emplace_back(transform_fn(info), info);
        }
    }
    return res;
}

// -------- Builder --------

// Workaround: this function returns a *pointer* to static storage.
// GCC 16.1 throws compile-time exception without this wrapper function. Possibly a compiler bug.
template <class Dest, class T, class Mem, std::meta::info Info>
consteval auto make_nonstatic_member_pointer() -> const Dest*
{
    static constexpr auto P = &[:Info:];
    static constexpr auto res = static_cast<Dest>(static_cast<Mem T::*>(P));
    return &res;
}

template <class Dest, class T, class Mem, std::meta::info Info>
constexpr auto nonstatic_member_pointer_v = make_nonstatic_member_pointer<Dest, T, Mem, Info>();

template <class Dest, std::meta::info Info>
constexpr auto static_member_pointer_v = static_cast<Dest>(&[:Info:]);

consteval auto extract_types(const std::vector<std::meta::info>& members)
    -> std::vector<std::meta::info>
{
    auto order_fn = [](std::meta::info A, std::meta::info B) {
        return std::meta::type_order(A, B) < 0;
    };
    auto equals_fn = [](std::meta::info A, std::meta::info B) {
        return std::meta::is_same_type(A, B);
    };

    auto types = std::vector<std::meta::info>(members.size());
    for (auto i = 0zU, n = members.size(); i < n; i++) {
        types[i] = std::meta::remove_reference(std::meta::type_of(members[i]));
    }
    auto* types_data = types.data();
    auto* types_data_end = types_data + types.size();
    std::sort(types_data, types_data_end, order_fn);

    const auto* dup_begin = std::unique(types_data, types_data_end, equals_fn);
    types.resize(dup_begin - types_data);
    return types;
}

// Precondition: !members.empty()
consteval auto common_member_type(std::meta::info T, const std::vector<std::meta::info>& members)
    -> std::meta::info
{
    auto types = extract_types(members);
    if (types.size() == 1) {
        // Common type is Mem T::*
        return std::meta::substitute(^^pointer_to_member_t, RBOX_IL(types[0], T));
    } else {
        // Common type is meta_variant<Mem1 T::*, Mem2 T::*, ...>
        auto variant_types = std::vector<std::meta::info>{};
        variant_types.reserve(types.size());
        for (auto type : types) {
            auto mptr_type = std::meta::substitute(^^pointer_to_member_t, RBOX_IL(type, T));
            variant_types.push_back(mptr_type);
        }
        return std::meta::substitute(^^meta_variant, variant_types);
    }
}

consteval auto common_pointer_type(const std::vector<std::meta::info>& members) -> std::meta::info
{
    auto types = extract_types(members);
    if (types.size() == 1) {
        // Common type is Mem*
        return std::meta::add_pointer(types[0]);
    } else {
        // Common type is meta_variant<Mem1*, Mem2*, ...>
        auto variant_types = std::vector<std::meta::info>{};
        variant_types.reserve(types.size());
        for (auto type : types) {
            auto ptr_type = std::meta::add_pointer(type);
            variant_types.push_back(ptr_type);
        }
        return std::meta::substitute(^^meta_variant, variant_types);
    }
}

template <class Options>
consteval auto build_empty_map() -> std::meta::info
{
    // void* as placeholder
    if constexpr (std::is_same_v<Options, integral_key_fixed_map_options>) {
        return map::make_empty_with_ikey<void*>();
    } else {
        return map::make_empty_with_skey<char, void*>();
    }
}

template <class Dest, class Key, class Options>
consteval auto build_nonstatic_member_map_impl(
    std::meta::info T,
    const std::vector<std::pair<Key, std::meta::info>>& entries,
    const Options& options) -> std::meta::info
{
    auto converted = std::vector<std::pair<Key, Dest>>{};
    converted.reserve(entries.size());
    for (const auto& [key, info] : entries) {
        auto Info = std::meta::reflect_constant(info);
        auto value = extract<const Dest*>(
            ^^nonstatic_member_pointer_v, ^^Dest, T, std::meta::type_of(info), Info);
        converted.emplace_back(key, *value);
    }
    if constexpr (std::is_same_v<Options, integral_key_fixed_map_options>) {
        return make_integral_key_fixed_map(converted, options);
    } else {
        return make_string_key_fixed_map(converted, options);
    }
}

template <class Dest, class Key, class Options>
consteval auto build_static_member_map_impl(
    const std::vector<std::pair<Key, std::meta::info>>& entries, const Options& options)
    -> std::meta::info
{
    auto converted = std::vector<std::pair<Key, Dest>>{};
    converted.reserve(entries.size());
    for (const auto& [key, info] : entries) {
        auto Info = std::meta::reflect_constant(info);
        auto value = extract<Dest>(^^static_member_pointer_v, ^^Dest, Info);
        converted.emplace_back(key, value);
    }
    if constexpr (std::is_same_v<Options, integral_key_fixed_map_options>) {
        return make_integral_key_fixed_map(converted, options);
    } else {
        return make_string_key_fixed_map(converted, options);
    }
}

template <class Key, class Options>
consteval auto build_nonstatic_member_map(
    std::meta::info T,
    const std::vector<std::pair<Key, std::meta::info>>& entries,
    const Options& options) -> std::meta::info
{
    if (entries.empty()) {
        return build_empty_map<Options>();
    }
    using call_signature = std::meta::info(
        std::meta::info, const std::vector<std::pair<Key, std::meta::info>>&, const Options&);

    auto members = std::vector<std::meta::info>{};
    members.reserve(entries.size());
    for (const auto& [k, m] : entries) {
        members.push_back(m);
    }
    auto fn = extract<call_signature*>(
        ^^build_nonstatic_member_map_impl, common_member_type(T, members), ^^Key, ^^Options);
    return fn(T, entries, options);
}

template <class Key, class Options>
consteval auto build_static_member_map(
    const std::vector<std::pair<Key, std::meta::info>>& entries, const Options& options)
    -> std::meta::info
{
    if (entries.empty()) {
        return build_empty_map<Options>();
    }
    using call_signature =
        std::meta::info(const std::vector<std::pair<Key, std::meta::info>>&, const Options&);

    auto members = std::vector<std::meta::info>{};
    members.reserve(entries.size());
    for (const auto& [k, m] : entries) {
        members.push_back(m);
    }
    auto fn = extract<call_signature*>(
        ^^build_static_member_map_impl, common_pointer_type(members), ^^Key, ^^Options);
    return fn(entries, options);
}

// -------- API Helpers --------

consteval auto decompose_prefix_suffix(std::string_view pattern)
    -> std::pair<std::string_view, std::string_view>
{
    auto star_pos = pattern.find('*');
    if (star_pos == npos) {
        compile_error("Invalid pattern: Expects format 'prefix*suffix'.");
    }
    auto next_star_pos = pattern.find('*', star_pos + 1);
    if (next_star_pos != npos) {
        compile_error("Multiple '*' is disallowed.");
    }
    return {pattern.substr(0, star_pos), pattern.substr(star_pos + 1)};
}

consteval auto rename_by_pattern(
    std::meta::info member, std::string_view prefix, std::string_view suffix)
    -> std::optional<std::string_view>
{
    if (!std::meta::has_identifier(member)) {
        return std::nullopt;
    }
    auto name = std::meta::identifier_of(member);
    if (!name.starts_with(prefix) || !name.ends_with(suffix)) {
        return std::nullopt;
    }
    name.remove_prefix(prefix.length());
    name.remove_suffix(suffix.length());
    return name;
}
}  // namespace impl::lookup

// -------- Non-static Data Members (public access only due to GCC 16.1 bug) --------

// Overload (1)
consteval auto make_public_nonstatic_data_member_fixed_map(
    std::meta::info T, std::string_view pattern, const string_key_fixed_map_options& options = {})
    -> std::meta::info
{
    using namespace impl::lookup;

    auto [prefix, suffix] = decompose_prefix_suffix(pattern);
    auto entries = lookup_nonstatic_data_members(T, [prefix, suffix](std::meta::info member) {
        return rename_by_pattern(member, prefix, suffix);
    });
    return build_nonstatic_member_map(T, entries, options);
}

#define RBOX_PUBLIC_NSDM_LOOKUP_API_2(transform_fn_concept, fixed_map_options_t)            \
    template <impl::lookup::transform_fn_concept TransformFn>                               \
    consteval auto make_public_nonstatic_data_member_fixed_map(                             \
        std::meta::info T,                                                                  \
        std::string_view pattern,                                                           \
        const TransformFn& transform_fn,                                                    \
        const fixed_map_options_t& options = {}) -> std::meta::info                         \
    {                                                                                       \
        using namespace impl::lookup;                                                       \
        using Ret = transformed_key_t<std::invoke_result_t<TransformFn, std::string_view>>; \
                                                                                            \
        auto [prefix, suffix] = decompose_prefix_suffix(pattern);                           \
        auto pattern_transform_fn = [&](std::meta::info member) -> std::optional<Ret> {     \
            auto name_opt = rename_by_pattern(member, prefix, suffix);                      \
            if (!name_opt) {                                                                \
                return std::nullopt;                                                        \
            }                                                                               \
            return transform_fn(*name_opt);                                                 \
        };                                                                                  \
        auto entries = lookup_nonstatic_data_members(T, pattern_transform_fn);              \
        return build_nonstatic_member_map(T, entries, options);                             \
    }

// Overload (2.1)
RBOX_PUBLIC_NSDM_LOOKUP_API_2(ikey_pattern_transform_fn, integral_key_fixed_map_options)
// Overload (2.2)
RBOX_PUBLIC_NSDM_LOOKUP_API_2(skey_pattern_transform_fn, string_key_fixed_map_options)

#define RBOX_PUBLIC_NSDM_LOOKUP_API_3(transform_fn_concept, fixed_map_options_t) \
    template <impl::lookup::transform_fn_concept TransformFn>                    \
    consteval auto make_public_nonstatic_data_member_fixed_map(                  \
        std::meta::info T,                                                       \
        const TransformFn& transform_fn,                                         \
        const fixed_map_options_t& options = {}) -> std::meta::info              \
    {                                                                            \
        using namespace impl::lookup;                                            \
        auto entries = lookup_nonstatic_data_members(T, transform_fn);           \
        return build_nonstatic_member_map(T, entries, options);                  \
    }

// Overload (3.1)
RBOX_PUBLIC_NSDM_LOOKUP_API_3(ikey_member_transform_fn, integral_key_fixed_map_options)
// Overload (3.2)
RBOX_PUBLIC_NSDM_LOOKUP_API_3(skey_member_transform_fn, string_key_fixed_map_options)

#undef RBOX_PUBLIC_NSDM_LOOKUP_API_2
#undef RBOX_PUBLIC_NSDM_LOOKUP_API_3
}  // namespace rbox

#define RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(T, ...) \
    [:rbox::make_public_nonstatic_data_member_fixed_map(^^T, ##__VA_ARGS__):]

// -------- Non-static Member Functions --------

namespace rbox {
// Overload (1)
consteval auto make_nonstatic_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info
{
    using namespace impl::lookup;

    auto [prefix, suffix] = decompose_prefix_suffix(pattern);
    auto transform_fn = [&](std::meta::info member) -> std::optional<std::string_view> {
        if (std::meta::is_static_member(member) || !std::meta::is_function(member)) {
            return std::nullopt;
        }
        return rename_by_pattern(member, prefix, suffix);
    };
    auto entries = lookup_class_members(T, ctx.via(T), std::meta::members_of, transform_fn);
    return build_nonstatic_member_map(T, entries, options);
}

consteval auto make_public_nonstatic_member_function_fixed_map(
    std::meta::info T, std::string_view pattern, const string_key_fixed_map_options& options = {})
    -> std::meta::info
{
    return make_nonstatic_member_function_fixed_map(T, pattern, options, unprivileged_context());
}

#define RBOX_NSMF_LOOKUP_API_2(transform_fn_concept, fixed_map_options_t)                     \
    template <impl::lookup::transform_fn_concept TransformFn>                                 \
    consteval auto make_nonstatic_member_function_fixed_map(                                  \
        std::meta::info T,                                                                    \
        std::string_view pattern,                                                             \
        const TransformFn& transform_fn,                                                      \
        const fixed_map_options_t& options = {},                                              \
        std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info              \
    {                                                                                         \
        using namespace impl::lookup;                                                         \
        using Ret = transformed_key_t<std::invoke_result_t<TransformFn, std::string_view>>;   \
                                                                                              \
        auto [prefix, suffix] = decompose_prefix_suffix(pattern);                             \
        auto pattern_transform_fn = [&](std::meta::info member) -> std::optional<Ret> {       \
            if (std::meta::is_static_member(member) || !std::meta::is_function(member)) {     \
                return std::nullopt;                                                          \
            }                                                                                 \
            auto name_opt = rename_by_pattern(member, prefix, suffix);                        \
            if (!name_opt) {                                                                  \
                return std::nullopt;                                                          \
            }                                                                                 \
            return transform_fn(*name_opt);                                                   \
        };                                                                                    \
        auto entries =                                                                        \
            lookup_class_members(T, ctx.via(T), std::meta::members_of, pattern_transform_fn); \
        return build_nonstatic_member_map(T, entries, options);                               \
    }                                                                                         \
                                                                                              \
    template <impl::lookup::transform_fn_concept TransformFn>                                 \
    consteval auto make_public_nonstatic_member_function_fixed_map(                           \
        std::meta::info T,                                                                    \
        std::string_view pattern,                                                             \
        const TransformFn& transform_fn,                                                      \
        const fixed_map_options_t& options = {}) -> std::meta::info                           \
    {                                                                                         \
        return make_nonstatic_member_function_fixed_map(                                      \
            T, pattern, transform_fn, options, unprivileged_context());                       \
    }

// Overload (2.1)
RBOX_NSMF_LOOKUP_API_2(ikey_pattern_transform_fn, integral_key_fixed_map_options)
// Overload (2.2)
RBOX_NSMF_LOOKUP_API_2(skey_pattern_transform_fn, string_key_fixed_map_options)

#define RBOX_NSMF_LOOKUP_API_3(transform_fn_concept, fixed_map_options_t)                     \
    template <impl::lookup::transform_fn_concept TransformFn>                                 \
    consteval auto make_nonstatic_member_function_fixed_map(                                  \
        std::meta::info T,                                                                    \
        const TransformFn& transform_fn,                                                      \
        const fixed_map_options_t& options = {},                                              \
        std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info              \
    {                                                                                         \
        using namespace impl::lookup;                                                         \
        using Ret = transformed_key_t<std::invoke_result_t<TransformFn, std::meta::info>>;    \
                                                                                              \
        auto wrapped_transform_fn = [&](std::meta::info member) -> std::optional<Ret> {       \
            if (std::meta::is_static_member(member) || !std::meta::is_function(member)) {     \
                return std::nullopt;                                                          \
            }                                                                                 \
            return transform_fn(member);                                                      \
        };                                                                                    \
        auto entries =                                                                        \
            lookup_class_members(T, ctx.via(T), std::meta::members_of, wrapped_transform_fn); \
        return build_nonstatic_member_map(T, entries, options);                               \
    }                                                                                         \
                                                                                              \
    template <impl::lookup::transform_fn_concept TransformFn>                                 \
    consteval auto make_public_nonstatic_member_function_fixed_map(                           \
        std::meta::info T,                                                                    \
        const TransformFn& transform_fn,                                                      \
        const fixed_map_options_t& options = {}) -> std::meta::info                           \
    {                                                                                         \
        return make_nonstatic_member_function_fixed_map(                                      \
            T, transform_fn, options, unprivileged_context());                                \
    }

// Overload (3.1)
RBOX_NSMF_LOOKUP_API_3(ikey_member_transform_fn, integral_key_fixed_map_options)
// Overload (3.2)
RBOX_NSMF_LOOKUP_API_3(skey_member_transform_fn, string_key_fixed_map_options)
}  // namespace rbox

#define RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...) \
    [:rbox::make_nonstatic_member_function_fixed_map(^^T, ##__VA_ARGS__):]

#define RBOX_PUBLIC_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...) \
    [:rbox::make_public_nonstatic_member_function_fixed_map(^^T, ##__VA_ARGS__):]

// -------- Static Data Members --------

namespace rbox {
// Overload (1)
consteval auto make_static_data_member_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info
{
    using namespace impl::lookup;

    auto [prefix, suffix] = decompose_prefix_suffix(pattern);
    auto transform_fn = [&](std::meta::info member) -> std::optional<std::string_view> {
        return rename_by_pattern(member, prefix, suffix);
    };
    auto entries =
        lookup_class_members(T, ctx.via(T), std::meta::static_data_members_of, transform_fn);
    return build_static_member_map(entries, options);
}

consteval auto make_public_static_data_member_fixed_map(
    std::meta::info T, std::string_view pattern, const string_key_fixed_map_options& options = {})
    -> std::meta::info
{
    return make_static_data_member_fixed_map(T, pattern, options, unprivileged_context());
}

#define RBOX_SDM_LOOKUP_API_2(transform_fn_concept, fixed_map_options_t)                    \
    template <impl::lookup::transform_fn_concept TransformFn>                               \
    consteval auto make_static_data_member_fixed_map(                                       \
        std::meta::info T,                                                                  \
        std::string_view pattern,                                                           \
        const TransformFn& transform_fn,                                                    \
        const fixed_map_options_t& options = {},                                            \
        std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info            \
    {                                                                                       \
        using namespace impl::lookup;                                                       \
        using Ret = transformed_key_t<std::invoke_result_t<TransformFn, std::string_view>>; \
                                                                                            \
        auto [prefix, suffix] = decompose_prefix_suffix(pattern);                           \
        auto pattern_transform_fn = [&](std::meta::info member) -> std::optional<Ret> {     \
            auto name_opt = rename_by_pattern(member, prefix, suffix);                      \
            if (!name_opt) {                                                                \
                return std::nullopt;                                                        \
            }                                                                               \
            return transform_fn(*name_opt);                                                 \
        };                                                                                  \
        auto entries = lookup_class_members(                                                \
            T, ctx.via(T), std::meta::static_data_members_of, pattern_transform_fn);        \
        return build_static_member_map(entries, options);                                   \
    }                                                                                       \
                                                                                            \
    template <impl::lookup::transform_fn_concept TransformFn>                               \
    consteval auto make_public_static_data_member_fixed_map(                                \
        std::meta::info T,                                                                  \
        std::string_view pattern,                                                           \
        const TransformFn& transform_fn,                                                    \
        const fixed_map_options_t& options = {}) -> std::meta::info                         \
    {                                                                                       \
        return make_static_data_member_fixed_map(                                           \
            T, pattern, transform_fn, options, unprivileged_context());                     \
    }

// Overload (2.1)
RBOX_SDM_LOOKUP_API_2(ikey_pattern_transform_fn, integral_key_fixed_map_options)
// Overload (2.2)
RBOX_SDM_LOOKUP_API_2(skey_pattern_transform_fn, string_key_fixed_map_options)

#define RBOX_SDM_LOOKUP_API_3(transform_fn_concept, fixed_map_options_t)                          \
    template <impl::lookup::transform_fn_concept TransformFn>                                     \
    consteval auto make_static_data_member_fixed_map(                                             \
        std::meta::info T,                                                                        \
        const TransformFn& transform_fn,                                                          \
        const fixed_map_options_t& options = {},                                                  \
        std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info                  \
    {                                                                                             \
        using namespace impl::lookup;                                                             \
        auto entries =                                                                            \
            lookup_class_members(T, ctx.via(T), std::meta::static_data_members_of, transform_fn); \
        return build_static_member_map(entries, options);                                         \
    }                                                                                             \
                                                                                                  \
    template <impl::lookup::transform_fn_concept TransformFn>                                     \
    consteval auto make_public_static_data_member_fixed_map(                                      \
        std::meta::info T,                                                                        \
        const TransformFn& transform_fn,                                                          \
        const fixed_map_options_t& options = {}) -> std::meta::info                               \
    {                                                                                             \
        return make_static_data_member_fixed_map(                                                 \
            T, transform_fn, options, unprivileged_context());                                    \
    }

// Overload (3.1)
RBOX_SDM_LOOKUP_API_3(ikey_member_transform_fn, integral_key_fixed_map_options)
// Overload (3.2)
RBOX_SDM_LOOKUP_API_3(skey_member_transform_fn, string_key_fixed_map_options)

#undef RBOX_SDM_LOOKUP_API_2
#undef RBOX_SDM_LOOKUP_API_3
}  // namespace rbox

#define RBOX_STATIC_DATA_MEMBER_FIXED_MAP(T, ...) \
    [:rbox::make_static_data_member_fixed_map(^^T, ##__VA_ARGS__):]

#define RBOX_PUBLIC_STATIC_DATA_MEMBER_FIXED_MAP(T, ...) \
    [:rbox::make_public_static_data_member_fixed_map(^^T, ##__VA_ARGS__):]

// -------- Static Member Functions --------

namespace rbox {
// Overload (1)
consteval auto make_static_member_function_fixed_map(
    std::meta::info T,
    std::string_view pattern,
    const string_key_fixed_map_options& options = {},
    std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info
{
    using namespace impl::lookup;

    auto [prefix, suffix] = decompose_prefix_suffix(pattern);
    auto transform_fn = [&](std::meta::info member) -> std::optional<std::string_view> {
        if (!std::meta::is_static_member(member) || !std::meta::is_function(member)) {
            return std::nullopt;
        }
        return rename_by_pattern(member, prefix, suffix);
    };
    auto entries = lookup_class_members(T, ctx.via(T), std::meta::members_of, transform_fn);
    return build_static_member_map(entries, options);
}

consteval auto make_public_static_member_function_fixed_map(
    std::meta::info T, std::string_view pattern, const string_key_fixed_map_options& options = {})
    -> std::meta::info
{
    return make_static_member_function_fixed_map(T, pattern, options, unprivileged_context());
}

#define RBOX_SMF_LOOKUP_API_2(transform_fn_concept, fixed_map_options_t)                      \
    template <impl::lookup::transform_fn_concept TransformFn>                                 \
    consteval auto make_static_member_function_fixed_map(                                     \
        std::meta::info T,                                                                    \
        std::string_view pattern,                                                             \
        const TransformFn& transform_fn,                                                      \
        const fixed_map_options_t& options = {},                                              \
        std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info              \
    {                                                                                         \
        using namespace impl::lookup;                                                         \
        using Ret = transformed_key_t<std::invoke_result_t<TransformFn, std::string_view>>;   \
                                                                                              \
        auto [prefix, suffix] = decompose_prefix_suffix(pattern);                             \
        auto pattern_transform_fn = [&](std::meta::info member) -> std::optional<Ret> {       \
            if (!std::meta::is_static_member(member) || !std::meta::is_function(member)) {    \
                return std::nullopt;                                                          \
            }                                                                                 \
            auto name_opt = rename_by_pattern(member, prefix, suffix);                        \
            if (!name_opt) {                                                                  \
                return std::nullopt;                                                          \
            }                                                                                 \
            return transform_fn(*name_opt);                                                   \
        };                                                                                    \
        auto entries =                                                                        \
            lookup_class_members(T, ctx.via(T), std::meta::members_of, pattern_transform_fn); \
        return build_static_member_map(entries, options);                                     \
    }                                                                                         \
                                                                                              \
    template <impl::lookup::transform_fn_concept TransformFn>                                 \
    consteval auto make_public_static_member_function_fixed_map(                              \
        std::meta::info T,                                                                    \
        std::string_view pattern,                                                             \
        const TransformFn& transform_fn,                                                      \
        const fixed_map_options_t& options = {}) -> std::meta::info                           \
    {                                                                                         \
        return make_static_member_function_fixed_map(                                         \
            T, pattern, transform_fn, options, unprivileged_context());                       \
    }

// Overload (2.1)
RBOX_SMF_LOOKUP_API_2(ikey_pattern_transform_fn, integral_key_fixed_map_options)
// Overload (2.2)
RBOX_SMF_LOOKUP_API_2(skey_pattern_transform_fn, string_key_fixed_map_options)

#define RBOX_SMF_LOOKUP_API_3(transform_fn_concept, fixed_map_options_t)                      \
    template <impl::lookup::transform_fn_concept TransformFn>                                 \
    consteval auto make_static_member_function_fixed_map(                                     \
        std::meta::info T,                                                                    \
        const TransformFn& transform_fn,                                                      \
        const fixed_map_options_t& options = {},                                              \
        std::meta::access_context ctx = RBOX_CURRENT_CONTEXT) -> std::meta::info              \
    {                                                                                         \
        using namespace impl::lookup;                                                         \
        using Ret = transformed_key_t<std::invoke_result_t<TransformFn, std::meta::info>>;    \
                                                                                              \
        auto wrapped_transform_fn = [&](std::meta::info member) -> std::optional<Ret> {       \
            if (!std::meta::is_static_member(member) || !std::meta::is_function(member)) {    \
                return std::nullopt;                                                          \
            }                                                                                 \
            return transform_fn(member);                                                      \
        };                                                                                    \
        auto entries =                                                                        \
            lookup_class_members(T, ctx.via(T), std::meta::members_of, wrapped_transform_fn); \
        return build_static_member_map(entries, options);                                     \
    }                                                                                         \
                                                                                              \
    template <impl::lookup::transform_fn_concept TransformFn>                                 \
    consteval auto make_public_static_member_function_fixed_map(                              \
        std::meta::info T,                                                                    \
        const TransformFn& transform_fn,                                                      \
        const fixed_map_options_t& options = {}) -> std::meta::info                           \
    {                                                                                         \
        return make_static_member_function_fixed_map(                                         \
            T, transform_fn, options, unprivileged_context());                                \
    }

// Overload (2.1)
RBOX_SMF_LOOKUP_API_3(ikey_member_transform_fn, integral_key_fixed_map_options)
// Overload (2.2)
RBOX_SMF_LOOKUP_API_3(skey_member_transform_fn, string_key_fixed_map_options)
}  // namespace rbox

#define RBOX_STATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...) \
    [:rbox::make_static_member_function_fixed_map(^^T, ##__VA_ARGS__):]

#define RBOX_PUBLIC_STATIC_MEMBER_FUNCTION_FIXED_MAP(T, ...) \
    [:rbox::make_public_static_member_function_fixed_map(^^T, ##__VA_ARGS__):]

// -------- Variables in Namespace --------

namespace rbox {
#define RBOX_V_OR_F_LOOKUP_API_1(v_or_f)                                                      \
    consteval auto make_##v_or_f##_fixed_map(                                                 \
        std::meta::info ns,                                                                   \
        std::string_view pattern,                                                             \
        const string_key_fixed_map_options& options = {}) -> std::meta::info                  \
    {                                                                                         \
        using namespace impl::lookup;                                                         \
                                                                                              \
        auto [prefix, suffix] = decompose_prefix_suffix(pattern);                             \
        auto entries = lookup_namespace_members(                                              \
            ns, [prefix, suffix](std::meta::info member) -> std::optional<std::string_view> { \
                if (!is_##v_or_f(member)) {                                                   \
                    return std::nullopt;                                                      \
                }                                                                             \
                return rename_by_pattern(member, prefix, suffix);                             \
            });                                                                               \
        return build_static_member_map(entries, options);                                     \
    }

// Overload (1)
RBOX_V_OR_F_LOOKUP_API_1(variable)

#define RBOX_V_OR_F_LOOKUP_API_2(v_or_f, i_or_s, integral_or_string)                        \
    template <impl::lookup::i_or_s##key_pattern_transform_fn TransformFn>                   \
    consteval auto make_##v_or_f##_fixed_map(                                               \
        std::meta::info ns,                                                                 \
        std::string_view pattern,                                                           \
        const TransformFn& transform_fn,                                                    \
        const integral_or_string##_key_fixed_map_options& options = {}) -> std::meta::info  \
    {                                                                                       \
        using namespace impl::lookup;                                                       \
        using Ret = transformed_key_t<std::invoke_result_t<TransformFn, std::string_view>>; \
                                                                                            \
        auto [prefix, suffix] = decompose_prefix_suffix(pattern);                           \
        auto pattern_transform_fn = [&](std::meta::info member) -> std::optional<Ret> {     \
            if (!is_##v_or_f(member)) {                                                     \
                return std::nullopt;                                                        \
            }                                                                               \
            auto name_opt = rename_by_pattern(member, prefix, suffix);                      \
            if (!name_opt) {                                                                \
                return std::nullopt;                                                        \
            }                                                                               \
            return transform_fn(*name_opt);                                                 \
        };                                                                                  \
        auto entries = lookup_namespace_members(ns, pattern_transform_fn);                  \
        return build_static_member_map(entries, options);                                   \
    }

// Overload (2.1)
RBOX_V_OR_F_LOOKUP_API_2(variable, i, integral)
// Overload (2.2)
RBOX_V_OR_F_LOOKUP_API_2(variable, s, string)

#define RBOX_V_OR_F_LOOKUP_API_3(v_or_f, i_or_s, integral_or_string)                       \
    template <impl::lookup::i_or_s##key_member_transform_fn TransformFn>                   \
    consteval auto make_##v_or_f##_fixed_map(                                              \
        std::meta::info ns,                                                                \
        const TransformFn& transform_fn,                                                   \
        const integral_or_string##_key_fixed_map_options& options = {}) -> std::meta::info \
    {                                                                                      \
        using namespace impl::lookup;                                                      \
        using Ret = transformed_key_t<std::invoke_result_t<TransformFn, std::meta::info>>; \
                                                                                           \
        auto wrapped_transform_fn = [&](std::meta::info member) -> std::optional<Ret> {    \
            if (!is_##v_or_f(member)) {                                                    \
                return std::nullopt;                                                       \
            }                                                                              \
            return transform_fn(member);                                                   \
        };                                                                                 \
        auto entries = lookup_namespace_members(ns, wrapped_transform_fn);                 \
        return build_static_member_map(entries, options);                                  \
    }

// Overload (3.1)
RBOX_V_OR_F_LOOKUP_API_3(variable, i, integral)
// Overload (3.2)
RBOX_V_OR_F_LOOKUP_API_3(variable, s, string)
}  // namespace rbox

#define RBOX_VARIABLE_FIXED_MAP(ns, ...) [:rbox::make_variable_fixed_map(^^ns, ##__VA_ARGS__):]

// -------- Functions in Namespace --------

namespace rbox {
// Overload (1)
RBOX_V_OR_F_LOOKUP_API_1(function)

// Overload (2.1)
RBOX_V_OR_F_LOOKUP_API_2(function, i, integral)
// Overload (2.2)
RBOX_V_OR_F_LOOKUP_API_2(function, s, string)

// Overload (3.1)
RBOX_V_OR_F_LOOKUP_API_3(function, i, integral)
// Overload (3.2)
RBOX_V_OR_F_LOOKUP_API_3(function, s, string)
}  // namespace rbox

#define RBOX_FUNCTION_FIXED_MAP(ns, ...) [:rbox::make_function_fixed_map(^^ns, ##__VA_ARGS__):]

#undef RBOX_V_OR_F_LOOKUP_API_1
#undef RBOX_V_OR_F_LOOKUP_API_2
#undef RBOX_V_OR_F_LOOKUP_API_3

#endif  // RBOX_LOOKUP_HPP
