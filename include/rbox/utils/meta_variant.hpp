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

#ifndef RBOX_UTILS_META_VARIANT_HPP
#define RBOX_UTILS_META_VARIANT_HPP

#include <functional>
#include <rbox/type_traits/template_instance.hpp>
#include <rbox/type_traits/trivial_types.hpp>
#include <rbox/utils/compare.hpp>
#include <rbox/utils/config.hpp>
#include <rbox/utils/constant.hpp>
#include <rbox/utils/indices_view.hpp>
#include <rbox/utils/meta_utility.hpp>
#include <utility>  // std::unreachable
#include <variant>

#ifdef RBOX_NO_EXCEPTIONS
#include <cstdio>
#include <cstdlib>
#endif

namespace rbox {
namespace impl {
consteval bool are_meta_variant_alternatives(std::span<const std::meta::info> args)
{
    if (args.empty()) {
        return false;  // At least 1 alternative is required
    }
    for (auto cur : args) {
        if (std::meta::is_void_type(cur)) {
            return false;
        }
        if (std::meta::is_reference_type(cur) || std::meta::is_array_type(cur)) {
            return false;
        }
        auto is_trivial_c = std::meta::substitute(^^trivial_type, RBOX_IL(cur));
        if (!std::meta::extract<bool>(is_trivial_c)) {
            return false;
        }
    }
    auto n = args.size();
    for (auto i = 0zU; i < n; i++) {
        for (auto j = i + 1; j < n; j++) {
            if (std::meta::is_same_type(args[i], args[j])) {
                return false;
            }
        }
    }
    return true;
}

consteval size_t meta_variant_ctor_index_of(
    std::meta::info T, std::span<const std::meta::info> args)
{
    auto n = args.size();
    // (1) Exact match
    for (auto i = 0zU; i < n; i++) {
        auto cur = args[i];
        if (std::meta::is_same_type(T, cur)) {
            return i;
        }
    }
    // (2) Conversion match: T -> exactly one of Args...
    auto candidate = npos;
    for (auto i = 0zU; i < n; i++) {
        auto cur = args[i];
        if (!std::meta::is_convertible_type(T, cur)) {
            continue;
        }
        if (candidate == npos) {
            candidate = i;
        } else {
            return npos;  // Ambiguity
        }
    }
    return candidate;
}

consteval size_t meta_variant_alternative_index_of(
    std::meta::info T, std::span<const std::meta::info> args)
{
    auto n = args.size();
    // Exact match
    for (auto i = 0zU; i < n; i++) {
        auto cur = args[i];
        if (std::meta::is_same_type(T, cur)) {
            return i;
        }
    }
    return npos;
}

template <class... Args>
constexpr auto are_meta_variant_alternatives_v = are_meta_variant_alternatives(RBOX_IL(^^Args...));

template <class T, class... Args>
constexpr auto meta_variant_ctor_index_v = meta_variant_ctor_index_of(^^T, RBOX_IL(^^Args...));

template <class T, class... Args>
constexpr auto meta_variant_alternative_index_v =
    meta_variant_alternative_index_of(^^T, RBOX_IL(^^Args...));

template <class T, class... Args>
concept meta_variant_ctor_arg = meta_variant_ctor_index_v<T, Args...> != npos;

template <class T, class... Args>
concept meta_variant_alternative_arg = meta_variant_alternative_index_v<T, Args...> != npos;
}  // namespace impl

template <class... Args>
    requires (impl::are_meta_variant_alternatives_v<Args...>)
struct meta_variant {
    union underlying_type;

    consteval
    {
        auto elems = std::array{^^Args...};
        auto members = std::vector<std::meta::info>{};
        auto n = elems.size();

        members.reserve(n);
        // v1, v2, v3, ...
        char buffer[32] = {'v'};
        for (auto i = 0zU; i < n; i++) {
            auto num = i + 1;
            auto digits = 0zU;
            for (auto temp = num; temp > 0; ++digits, temp /= 10);
            for (auto j = digits; j > 0zU; --j) {
                buffer[j] = static_cast<char>('0' + (num % 10));
                num /= 10;
            }
            auto options = std::meta::data_member_options{
                .name = buffer,
            };
            auto cur_member = std::meta::data_member_spec(elems[i], options);
            members.push_back(cur_member);
        }
        std::meta::define_aggregate(^^underlying_type, members);
    }

private:
    static consteval auto ith_field(size_t i) -> std::meta::info
    {
        return all_direct_nonstatic_data_members_v<underlying_type>[i];
    }

public:
    // Trivial construction
    constexpr meta_variant() = default;

    template <impl::meta_variant_ctor_arg<Args...> T>
    constexpr meta_variant(T value) noexcept
    {
        constexpr auto I = impl::meta_variant_ctor_index_v<T, Args...>;
        alternative_index = I;
        alternatives.[:ith_field(I):] = std::move(value);
    }

    template <impl::meta_variant_ctor_arg<Args...> T>
    constexpr meta_variant& operator=(T value) noexcept
    {
        constexpr auto I = impl::meta_variant_ctor_index_v<T, Args...>;
        alternative_index = I;
        alternatives.[:ith_field(I):] = std::move(value);
        return *this;
    }

    constexpr meta_variant(const meta_variant& rhs) noexcept = default;
    constexpr meta_variant(meta_variant&& rhs) noexcept = default;

    constexpr meta_variant& operator=(const meta_variant& rhs) noexcept = default;
    constexpr meta_variant& operator=(meta_variant&& rhs) noexcept = default;

    constexpr ~meta_variant() noexcept = default;

    template <impl::meta_variant_ctor_arg<Args...> T>
    constexpr void emplace(T value)
    {
        constexpr auto I = impl::meta_variant_ctor_index_v<T, Args...>;
        alternative_index = I;
        alternatives.[:ith_field(I):] = std::move(value);
    }

    constexpr auto index() const -> size_t
    {
        return alternative_index;
    }

    template <impl::meta_variant_alternative_arg<Args...> T>
    constexpr auto holds_alternative() const -> bool
    {
        return alternative_index == impl::meta_variant_alternative_index_v<T, Args...>;
    }

    template <impl::meta_variant_alternative_arg<Args...> T, class Self>
    constexpr decltype(auto) get(this Self&& self)
    {
        constexpr auto I = impl::meta_variant_alternative_index_v<T, Args...>;
        return std::forward<Self>(self).template get<I>();
    }

    template <size_t I, class Self>
        requires (I < sizeof...(Args))
    constexpr decltype(auto) get(this Self&& self)
    {
        if (I == self.alternative_index) [[likely]] {
            return std::forward_like<Self>(self.alternatives.[:ith_field(I):]);
        }
#ifdef RBOX_NO_EXCEPTIONS
        std::fprintf(stderr, "Bad ref_variant access. Abort.\n");
        std::abort();
#else
        throw std::bad_variant_access();
#endif
    }

    template <impl::meta_variant_alternative_arg<Args...> T>
    constexpr auto get_if() & -> auto*
    {
        constexpr auto I = impl::meta_variant_alternative_index_v<T, Args...>;
        return get_if<I>();
    }

    template <impl::meta_variant_alternative_arg<Args...> T>
    constexpr auto get_if() const& -> const auto*
    {
        constexpr auto I = impl::meta_variant_alternative_index_v<T, Args...>;
        return get_if<I>();
    }

    template <size_t I>
        requires (I < sizeof...(Args))
    constexpr auto get_if() & -> auto*
    {
        using Ret = Args...[I];
        if (I == alternative_index) [[likely]] {
            auto& field = alternatives.[:ith_field(I):];
            return &field;
        }
        return static_cast<Ret*>(nullptr);
    }

    template <size_t I>
        requires (I < sizeof...(Args))
    constexpr auto get_if() const& -> const auto*
    {
        using Ret = Args...[I];
        if (I == alternative_index) [[likely]] {
            auto& field = alternatives.[:ith_field(I):];
            return &field;
        }
        return static_cast<const Ret*>(nullptr);
    }

    template <class Self, class Func>
    constexpr decltype(auto) visit(this Self&& self, Func&& func)
    {
        template for (constexpr auto I : rbox::indices_view{sizeof...(Args)})
        {
            if (self.alternative_index == I) {
                auto& field = self.alternatives.[:ith_field(I):];
                return std::invoke(std::forward<Func>(func), std::forward_like<Self>(field));
            }
        }
        std::unreachable();
    }

    // Note: It's intentional to keep the fields public so that meta_variant is a structural type.
    size_t alternative_index;
    underlying_type alternatives;
};

template <class... Args>
    requires (is_operator_eq_comparable_v<Args, Args> && ...)
constexpr bool operator==(const meta_variant<Args...>& lhs, const meta_variant<Args...>& rhs)
{
    using underlying_type = typename meta_variant<Args...>::underlying_type;
    if (lhs.alternative_index != rhs.alternative_index) {
        return false;
    }
    template for (constexpr auto I : rbox::indices_view{sizeof...(Args)})
    {
        if (lhs.alternative_index == I) {
            constexpr auto F = all_direct_nonstatic_data_members_v<underlying_type>[I];
            return lhs.alternatives.[:F:] == rhs.alternatives.[:F:];
        }
    }
    std::unreachable();
}

template <class... Args>
    requires (is_operator_3way_comparable_v<Args, Args> && ...)
constexpr auto operator<=>(const meta_variant<Args...>& lhs, const meta_variant<Args...>& rhs)
    -> std::common_comparison_category_t<std::compare_three_way_result_t<Args>...>
{
    using underlying_type = typename meta_variant<Args...>::underlying_type;
    if (lhs.alternative_index != rhs.alternative_index) {
        return lhs.alternative_index <=> rhs.alternative_index;
    }
    template for (constexpr auto I : rbox::indices_view{sizeof...(Args)})
    {
        if (lhs.alternative_index == I) {
            constexpr auto F = all_direct_nonstatic_data_members_v<underlying_type>[I];
            return lhs.alternatives.[:F:] <=> rhs.alternatives.[:F:];
        }
    }
    std::unreachable();
}

// -------- Free API functions of meta_variant --------

template <class T, class... Args>
    requires (impl::meta_variant_alternative_arg<T, Args...>)
constexpr auto holds_alternative(const meta_variant<Args...>& v) -> bool
{
    return v.template holds_alternative<T>();
}

template <class T, class Variant>
    requires (template_instance_of<std::remove_cvref_t<Variant>, meta_variant>)
constexpr decltype(auto) get(Variant&& v)
{
    return std::forward<Variant>(v).template get<T>();
}

template <size_t I, class Variant>
    requires (template_instance_of<std::remove_cvref_t<Variant>, meta_variant>)
constexpr decltype(auto) get(Variant&& v)
{
    return std::forward<Variant>(v).template get<I>();
}

template <class T, class... Args>
    requires (impl::meta_variant_alternative_arg<T, Args...>)
constexpr auto* get_if(meta_variant<Args...>& v)
{
    return v.template get_if<T>();
}

template <size_t I, class... Args>
    requires (I < sizeof...(Args))
constexpr auto* get_if(meta_variant<Args...>& v)
{
    return v.template get_if<I>();
}

template <class T, class... Args>
    requires (impl::meta_variant_alternative_arg<T, Args...>)
constexpr auto* get_if(const meta_variant<Args...>& v)
{
    return v.template get_if<T>();
}

template <size_t I, class... Args>
    requires (I < sizeof...(Args))
constexpr auto* get_if(const meta_variant<Args...>& v)
{
    return v.template get_if<I>();
}

template <class Func, class Variant>
    requires (template_instance_of<std::remove_cvref_t<Variant>, meta_variant>)
constexpr decltype(auto) visit(Func&& func, Variant&& v)
{
    return std::forward<Variant>(v).visit(std::forward<Func>(func));
}
}  // namespace rbox

template <class... Args>
struct std::variant_size<rbox::meta_variant<Args...>>
    : std::integral_constant<size_t, sizeof...(Args)> {};

template <size_t I, class... Args>
    requires (I < sizeof...(Args))
struct std::variant_alternative<I, rbox::meta_variant<Args...>> {
    using type = Args...[I];
};
#endif  // RBOX_UTILS_META_VARIANT_HPP
