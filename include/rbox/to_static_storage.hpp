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

#ifndef RBOX_TO_STATIC_STORAGE_HPP
#define RBOX_TO_STATIC_STORAGE_HPP

#include <rbox/type_traits/arithmetic_types.hpp>
#include <rbox/type_traits/class_types/flattened_nsdm.hpp>
#include <rbox/type_traits/trivial_types.hpp>
#include <rbox/type_traits/tuple_like_types.hpp>
#include <rbox/type_traits/variant_like_types.hpp>
#include <rbox/utils/concepts.hpp>
#include <rbox/utils/define_static_array.hpp>
#include <rbox/utils/define_static_string.hpp>
#include <rbox/utils/functional/get_ith_alternative.hpp>
#include <rbox/utils/meta_pair.hpp>
#include <rbox/utils/meta_triple.hpp>
#include <rbox/utils/meta_tuple.hpp>
#include <rbox/utils/meta_utility.hpp>
#include <rbox/utils/meta_variant.hpp>
#include <rbox/utils/tuple_size.hpp>

namespace rbox {
namespace impl {
consteval auto is_promotable_to_static_storage(std::meta::info T) -> bool;
consteval auto make_structural_mirror_members(std::meta::info T) -> std::vector<std::meta::info>;
}  // namespace impl

template <class T>
concept promotable_to_static_storage =
    impl::is_promotable_to_static_storage(std::meta::remove_cv(^^T));

struct to_static_storage_t {
    template <promotable_to_static_storage T>
    static consteval auto operator()(const T& value);

    template <promotable_to_static_storage T>
    static consteval auto operator()(std::initializer_list<T> values);
};

constexpr auto to_static_storage = to_static_storage_t{};

template <promotable_to_static_storage T>
using to_static_storage_result_t = decltype(to_static_storage(std::declval<T>()));

template <flattenable_class T>
struct structural_mirror {
    class type;

    consteval
    {
        auto members = impl::make_structural_mirror_members(^^T);
        std::meta::define_aggregate(^^type, members);
    }
};

template <class T>
using structural_mirror_t = typename structural_mirror<T>::type;

namespace impl {
consteval bool is_promotable_to_static_storage(std::meta::info T)
{
    // Fast path for scalar types (arithmetic, enum, pointer, etc.)
    if (std::meta::is_scalar_type(T)) {
        return true;
    }
    if (std::meta::is_function_type(T)) {
        return true;  // Will be converted to function pointer
    }
    // (1) Range
    if (extract<bool>(^^std::ranges::input_range, T)) {
        auto U = std::meta::substitute(^^std::ranges::range_value_t, RBOX_IL(T));
        return extract<bool>(^^promotable_to_static_storage, U);
    }
    // (2) Tuple
    if (extract<bool>(^^tuple_like, T)) {
        for (auto i = 0zU, n = std::meta::tuple_size(T); i < n; i++) {
            auto U = std::meta::tuple_element(i, T);
            if (!extract<bool>(^^promotable_to_static_storage, U)) {
                return false;
            }
        }
        return true;
    }
    // (3) Variant
    if (extract<bool>(^^variant_like, T)) {
        for (auto i = 0zU, n = std::meta::variant_size(T); i < n; i++) {
            auto U = std::meta::variant_alternative(i, T);
            if (!extract<bool>(^^promotable_to_static_storage, U)) {
                return false;
            }
            // meta_variant requires all of its alternatives to be trivial
            auto R = std::meta::substitute(^^to_static_storage_result_t, RBOX_IL(U));
            if (!extract<bool>(^^trivial_type, R)) {
                return false;
            }
        }
        return true;
    }
    // (4) Structural types (lvalue references, or objects as identity)
    if (std::meta::is_structural_type(T)) {
        return true;
    }
    // (5) Object as structural mirror
    if (extract<bool>(^^flattenable_class, T)) {
        for (auto member : all_flattened_nonstatic_data_members_of(T)) {
            if (!std::meta::has_identifier(member.info)) {
                return false;
            }
            auto U = std::meta::type_of(member.info);
            if (!extract<bool>(^^promotable_to_static_storage, U)) {
                return false;
            }
        }
        return true;
    }
    // Not promotable otherwise
    return false;
}

consteval auto make_structural_mirror_members(std::meta::info T) -> std::vector<std::meta::info>
{
    auto members = all_flattened_nonstatic_data_members_of(T);
    auto n = members.n;

    auto res = std::vector<std::meta::info>(n);

    for (auto i = 0zU; i < n; i++) {
        auto cur = members[i].info;
        auto U = std::meta::type_of(cur);

        if (!std::meta::has_identifier(cur)) {
            compile_error("Anonymous union members are not supported for structural mirroring.");
        }
        auto name = std::meta::identifier_of(cur);

        if (std::meta::is_bit_field(cur)) {
            auto bit_width = std::meta::bit_size_of(cur);
            res[i] = std::meta::data_member_spec(U, {.name = name, .bit_width = bit_width});
            continue;
        }
        if (std::meta::is_reference_type(U)) {
            res[i] = std::meta::data_member_spec(U, {.name = name});  // References unchanged
            continue;
        }
        if (std::meta::is_empty_type(U)) {
            res[i] = std::meta::data_member_spec(U, {.name = name, .no_unique_address = true});
            continue;
        }
        auto S = std::meta::substitute(^^to_static_storage_result_t, RBOX_IL(U));
        res[i] = std::meta::data_member_spec(S, {.name = name});
    }
    return res;
}

consteval auto variant_to_static_storage_result(std::meta::info T) -> std::meta::info
{
    auto n = std::meta::variant_size(T);
    auto promoted = std::vector<std::meta::info>();
    promoted.reserve(n);
    for (auto i = 0zU; i < n; i++) {
        auto A = std::meta::variant_alternative(i, T);
        A = std::meta::substitute(^^to_static_storage_result_t, RBOX_IL(A));
        promoted.push_back(A);
    }
    return std::meta::substitute(^^meta_variant, promoted);
}

template <class T>
using variant_to_static_storage_result_t = [:variant_to_static_storage_result(^^T):];

template <class InputRange>
consteval auto range_to_static_storage(const InputRange& range)
{
    using ValueT = std::ranges::range_value_t<InputRange>;
    if constexpr (char_type<ValueT>) {
        return rbox::define_static_string(range);
    } else if constexpr (scalar_type<ValueT> && !pointer_type<ValueT>) {
        return rbox::define_static_array(range);
    } else {
        auto converted = std::vector<to_static_storage_result_t<ValueT>>{};
        for (const auto& value : range) {
            converted.push_back(to_static_storage(value));
        }
        return rbox::define_static_array(converted);
    }
}

template <size_t... Is, class TupleLike>
consteval auto tuple_like_to_static_storage(const TupleLike& tuple, std::index_sequence<Is...>)
{
    return meta_tuple{to_static_storage(get_ith_element<Is>(tuple))...};
}

template <class TupleLike>
consteval auto tuple_like_to_static_storage(const TupleLike& tuple)
{
    constexpr auto N = rbox::tuple_size_v<TupleLike>;
    if constexpr (N == 2) {
        auto v1 = to_static_storage(get_first(tuple));
        auto v2 = to_static_storage(get_second(tuple));
        return meta_pair{v1, v2};
    } else if constexpr (N == 3) {
        auto v1 = to_static_storage(get_first(tuple));
        auto v2 = to_static_storage(get_second(tuple));
        auto v3 = to_static_storage(get_third(tuple));
        return meta_triple{v1, v2, v3};
    } else {
        return tuple_like_to_static_storage(tuple, std::make_index_sequence<N>{});
    }
}

template <class VariantLike>
consteval auto variant_like_to_static_storage(const VariantLike& variant)
    -> variant_to_static_storage_result_t<VariantLike>
{
    using Ret = variant_to_static_storage_result_t<VariantLike>;
    template for (constexpr auto I : indices_view{std::variant_size_v<VariantLike>})
    {
        if (holds_ith_alternative<I>(variant)) {
            return Ret{to_static_storage(get_ith_alternative<I>(variant))};
        }
    }
    compile_error("Valueless variant");
}

template <class Pointer>
    requires (std::is_pointer_v<Pointer>)
consteval auto pointer_to_static_storage(Pointer ptr)
{
    using T = std::remove_pointer_t<Pointer>;
    if constexpr (char_type<T>) {
        auto* tail = ptr;
        while (*tail != '\0') ++tail;
        return rbox::define_static_string(std::basic_string_view{ptr, tail});
    } else {
        return ptr;
    }
}

consteval auto pointer_to_static_storage(std::nullptr_t)
{
    return nullptr;
}

template <size_t I, class Flattenable>
consteval decltype(auto) ith_flattened_member_to_static_storage(const Flattenable& obj)
{
    constexpr auto from = all_flattened_nonstatic_data_members_v<Flattenable>[I].info;

    if constexpr (std::meta::is_reference_type(std::meta::type_of(from))) {
        return obj.[:from:];  // as lvalue-reference
    } else {
        return to_static_storage(obj.[:from:]);  // as prvalue
    }
}

template <class Flattenable, size_t... Is>
consteval auto flattenable_class_to_static_storage(
    const Flattenable& obj, std::index_sequence<Is...>)
{
    using Ret = structural_mirror_t<Flattenable>;
    return Ret{ith_flattened_member_to_static_storage<Is>(obj)...};
}

template <class Flattenable>
consteval auto flattenable_class_to_static_storage(const Flattenable& obj)
{
    constexpr auto N = all_flattened_nonstatic_data_members_v<Flattenable>.size();

    auto indices = std::make_index_sequence<N>();
    return flattenable_class_to_static_storage(obj, indices);
}
}  // namespace impl

template <promotable_to_static_storage T>
consteval auto to_static_storage_t::operator()(const T& value)
{
    if constexpr (std::ranges::input_range<T>) {
        return impl::range_to_static_storage(value);
    } else if constexpr (tuple_like<T>) {
        return impl::tuple_like_to_static_storage(value);
    } else if constexpr (variant_like<T>) {
        return impl::variant_like_to_static_storage(value);
    } else if constexpr (std::is_pointer_v<T> || std::is_null_pointer_v<T>) {
        return impl::pointer_to_static_storage(value);
    } else if constexpr (std::is_function_v<T>) {
        return static_cast<std::add_pointer_t<T>>(value);
    } else if constexpr (std::is_structural_v<T>) {
        return value;  // Identity
    } else {
        static_assert(flattenable_class<T>, "Invalid input type.");
        return impl::flattenable_class_to_static_storage(value);
    }
}

template <promotable_to_static_storage T>
consteval auto to_static_storage_t::operator()(std::initializer_list<T> values)
{
    return impl::range_to_static_storage(values);
}
}  // namespace rbox

#endif  // RBOX_TO_STATIC_STORAGE_HPP
