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

#ifndef RBOX_UTILS_REF_VARIANT_HPP
#define RBOX_UTILS_REF_VARIANT_HPP

#include <functional>
#include <meta>
#include <rbox/utils/compare.hpp>
#include <rbox/utils/config.hpp>
#include <rbox/utils/constant.hpp>
#include <rbox/utils/indices_view.hpp>
#include <rbox/utils/meta_utility.hpp>
#include <utility>
#include <variant>

#ifdef RBOX_NO_EXCEPTIONS
#include <cstdio>
#include <cstdlib>
#endif

namespace rbox {
namespace impl {
consteval bool are_ref_variant_alternatives(std::span<const std::meta::info> args)
{
    if (args.empty()) {
        return false;  // Requires at least 1 candidate
    }
    for (auto cur : args) {
        if (std::meta::is_void_type(cur) || std::meta::is_reference_type(cur)
            || std::meta::is_array_type(cur)) {
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

consteval size_t ref_variant_alternative_index_of(
    std::meta::info T, std::span<const std::meta::info> args)
{
    auto n = args.size();
    // (1) Exact match
    for (auto i = 0zU; i < n; i++) {
        auto cur = args[i];
        if (std::meta::is_same_type(T, cur)) return i;
    }
    // (2) Conversion match: T* -> exactly one of Args*...
    auto candidate = npos;
    auto Tp = std::meta::add_pointer(T);
    for (auto i = 0zU; i < n; i++) {
        auto curp = std::meta::add_pointer(args[i]);
        if (!std::meta::is_convertible_type(Tp, curp)) {
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

template <class... Args>
constexpr auto are_ref_variant_alternatives_v = are_ref_variant_alternatives(RBOX_IL(^^Args...));

template <class T, class... Args>
constexpr auto ref_variant_alternative_index_v =
    ref_variant_alternative_index_of(^^T, RBOX_IL(^^Args...));

template <class T, class... Args>
concept ref_variant_ctor_arg =
    std::is_lvalue_reference_v<T>
    && ref_variant_alternative_index_v<std::remove_reference_t<T>, Args...> != npos;

template <class T, class... Args>
concept ref_variant_alternative_arg = ref_variant_alternative_index_v<T, Args...> != npos;
}  // namespace impl

// -------- ref_variant --------

template <class... Args>
    requires (impl::are_ref_variant_alternatives_v<Args...>)
struct ref_variant {
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
            auto curp = std::meta::add_pointer(elems[i]);
            auto cur_member = std::meta::data_member_spec(curp, options);
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
    // Non-explicit: Implicit conversion is accepted behavior
    template <impl::ref_variant_ctor_arg<Args...> T>
    constexpr ref_variant(T&& obj) noexcept
    {
        rebind(obj);
    }

    // Shallow assignment (rebinding the internal reference).
    // Consistent behavior with std::optional<T&> and std::reference_wrapper<T>.
    constexpr ref_variant(const ref_variant&) noexcept = default;
    constexpr ref_variant& operator=(const ref_variant&) noexcept = default;

    constexpr ref_variant(ref_variant&&) noexcept = default;
    constexpr ref_variant& operator=(ref_variant&&) noexcept = default;

    constexpr ~ref_variant() noexcept = default;

    template <impl::ref_variant_ctor_arg<Args...> T>
    constexpr void rebind(T&& obj)
    {
        using TNoRef = std::remove_reference_t<T>;
        constexpr auto I = impl::ref_variant_alternative_index_v<TNoRef, Args...>;
        alternative_index = I;
        alternatives.[:ith_field(I):] = &obj;
    }

    constexpr auto index() const -> size_t
    {
        return alternative_index;
    }

    template <impl::ref_variant_alternative_arg<Args...> T>
    constexpr auto holds_alternative() const -> bool
    {
        constexpr auto I = impl::ref_variant_alternative_index_v<T, Args...>;
        return alternative_index == I;
    }

    // Shallow const: cv-qualifiers of ref_variant itself does not propagate to its referenced
    // value. Consistent behavior with std::optional<T&> and std::reference_wrapper<T>
    template <impl::ref_variant_alternative_arg<Args...> T>
    constexpr auto get() const -> auto&
    {
        constexpr auto I = impl::ref_variant_alternative_index_v<T, Args...>;
        return get<I>();
    }

    template <size_t I>
        requires (I < sizeof...(Args))
    constexpr auto get() const -> auto&
    {
        if (alternative_index == I) [[likely]] {
            auto* ptr = alternatives.[:ith_field(I):];
            return *ptr;
        }
#ifdef RBOX_NO_EXCEPTIONS
        std::fprintf(stderr, "Bad ref_variant access. Abort.\n");
        std::abort();
#else
        throw std::bad_variant_access();
#endif
    }

    template <impl::ref_variant_alternative_arg<Args...> T>
    constexpr auto get_if() const -> auto*
    {
        constexpr auto I = impl::ref_variant_alternative_index_v<T, Args...>;
        return get_if<I>();
    }

    template <size_t I>
        requires (I < sizeof...(Args))
    constexpr auto get_if() const -> std::add_pointer_t<Args... [I]> {
        if (alternative_index == I) [[likely]] {
            auto* ptr = alternatives.[:ith_field(I):];
            return ptr;
        }
        return nullptr;
    }

    template <class Func>
    constexpr decltype(auto) visit(Func&& func) const
    {
        template for (constexpr auto I : rbox::indices_view{sizeof...(Args)})
        {
            if (alternative_index == I) {
                auto* ptr = alternatives.[:ith_field(I):];
                return std::invoke(std::forward<Func>(func), *ptr);
            }
        }
        std::unreachable();
    }

    // Note: It's intentional to keep the fields public so that ref_variant is a structural type.
    size_t alternative_index;
    underlying_type alternatives;
};

// Similar with std::reference_wrapper (whose operator== compares the referenced values as well).
template <class... Args>
    requires (is_operator_eq_comparable_v<Args, Args> && ...)
constexpr bool operator==(const ref_variant<Args...>& lhs, const ref_variant<Args...>& rhs)
{
    using underlying_type = typename ref_variant<Args...>::underlying_type;
    if (lhs.alternative_index != rhs.alternative_index) {
        return false;
    }
    template for (constexpr auto I : rbox::indices_view{sizeof...(Args)})
    {
        if (lhs.alternative_index == I) {
            constexpr auto F = all_direct_nonstatic_data_members_v<underlying_type>[I];
            return *lhs.alternatives.[:F:] == *rhs.alternatives.[:F:];
        }
    }
    std::unreachable();
}

// Alternative index as first key, and referenced value as second key.
// Similar with std::reference_wrapper (whose operator<=> compares the referenced values as well).
template <class... Args>
    requires (is_operator_3way_comparable_v<Args, Args> && ...)
constexpr auto operator<=>(const ref_variant<Args...>& lhs, const ref_variant<Args...>& rhs)
    -> std::common_comparison_category_t<std::compare_three_way_result_t<Args>...>
{
    using underlying_type = typename ref_variant<Args...>::underlying_type;
    if (lhs.alternative_index != rhs.alternative_index) {
        return lhs.alternative_index <=> rhs.alternative_index;
    }
    template for (constexpr auto I : rbox::indices_view{sizeof...(Args)})
    {
        if (lhs.alternative_index == I) {
            constexpr auto F = all_direct_nonstatic_data_members_v<underlying_type>[I];
            return *lhs.alternatives.[:F:] <=> *rhs.alternatives.[:F:];
        }
    }
    std::unreachable();
}

template <class T, class... Args>
    requires (impl::ref_variant_alternative_arg<T, Args...>)
constexpr auto holds_alternative(const ref_variant<Args...>& v) -> bool
{
    return v.template holds_alternative<T>();
}

template <class T, class... Args>
    requires (impl::ref_variant_alternative_arg<T, Args...>)
constexpr auto get(const ref_variant<Args...>& v) -> auto&
{
    return v.template get<T>();
}

template <size_t I, class... Args>
    requires (I < sizeof...(Args))
constexpr auto get(const ref_variant<Args...>& v) -> auto&
{
    return v.template get<I>();
}

template <class T, class... Args>
    requires (impl::ref_variant_alternative_arg<T, Args...>)
constexpr auto get_if(const ref_variant<Args...>& v) -> auto*
{
    return v.template get_if<T>();
}

template <size_t I, class... Args>
    requires (I < sizeof...(Args))
constexpr auto get_if(const ref_variant<Args...>& v) -> auto*
{
    return v.template get_if<I>();
}

template <class Func, class... Args>
constexpr decltype(auto) visit(Func&& func, const ref_variant<Args...>& v)
{
    return v.visit(std::forward<Func>(func));
}

// -------- optional_ref_variant

template <class... Args>
    requires (impl::are_ref_variant_alternatives_v<Args...>)
struct optional_ref_variant {
    union underlying_type;

    consteval
    {
        auto elems = std::array{^^Args...};
        auto members = std::vector<std::meta::info>{};
        auto n = elems.size();

        members.reserve(n);
        // v1, v2, v3...
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
            auto curp = std::meta::add_pointer(elems[i]);
            auto cur_member = std::meta::data_member_spec(curp, options);
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
    // Holds no value
    constexpr optional_ref_variant() noexcept : alternative_index(npos) {}

    // Non-explicit: Implicit conversion is accepted behavior
    template <impl::ref_variant_ctor_arg<Args...> T>
    constexpr optional_ref_variant(T&& obj) noexcept
    {
        rebind(obj);
    }

    // Shallow assignment (rebinding the internal reference).
    // Consistent behavior with std::optional<T&> and std::reference_wrapper<T>.
    constexpr optional_ref_variant(const optional_ref_variant&) noexcept = default;
    constexpr optional_ref_variant& operator=(const optional_ref_variant&) noexcept = default;

    constexpr optional_ref_variant(optional_ref_variant&&) noexcept = default;
    constexpr optional_ref_variant& operator=(optional_ref_variant&&) noexcept = default;

    constexpr ~optional_ref_variant() noexcept = default;

    template <impl::ref_variant_ctor_arg<Args...> T>
    constexpr void rebind(T&& obj)
    {
        using TNoRef = std::remove_reference_t<T>;
        constexpr auto I = impl::ref_variant_alternative_index_v<TNoRef, Args...>;
        alternative_index = I;
        alternatives.[:ith_field(I):] = &obj;
    }

    constexpr void reset()
    {
        alternative_index = npos;
        alternatives.[:ith_field(0):] = nullptr;
    }

    constexpr auto index() const -> size_t
    {
        return alternative_index;
    }

    constexpr auto has_value() const -> bool
    {
        return alternative_index != npos;
    }

    constexpr auto valueless() const -> bool
    {
        return alternative_index == npos;
    }

    template <impl::ref_variant_alternative_arg<Args...> T>
    constexpr auto holds_alternative() const -> bool
    {
        constexpr auto I = impl::ref_variant_alternative_index_v<T, Args...>;
        return alternative_index == I;
    }

    // Shallow const: cv-qualifiers of ref_variant itself does not propagate to its referenced
    // value. Consistent behavior with std::optional<T&> and std::reference_wrapper<T>
    template <impl::ref_variant_alternative_arg<Args...> T>
    constexpr auto get() const -> auto&
    {
        constexpr auto I = impl::ref_variant_alternative_index_v<T, Args...>;
        return get<I>();
    }

    template <size_t I>
        requires (I < sizeof...(Args))
    constexpr auto get() const -> auto&
    {
        if (alternative_index == I) [[likely]] {
            auto* ptr = alternatives.[:ith_field(I):];
            return *ptr;
        }
#ifdef RBOX_NO_EXCEPTIONS
        std::fprintf(stderr, "Bad ref_variant access. Abort.\n");
        std::abort();
#else
        throw std::bad_variant_access();
#endif
    }

    template <impl::ref_variant_alternative_arg<Args...> T>
    constexpr auto get_if() const -> auto*
    {
        constexpr auto I = impl::ref_variant_alternative_index_v<T, Args...>;
        return get_if<I>();
    }

    template <size_t I>
        requires (I < sizeof...(Args))
    constexpr auto get_if() const -> std::add_pointer_t<Args... [I]> {
        if (alternative_index == I) [[likely]] {
            auto* ptr = alternatives.[:ith_field(I):];
            return ptr;
        }
        return nullptr;
    }

    template <class Func>
    constexpr auto try_visit(Func&& func) const
    {
        using First = Args...[0];
        using Ret = std::optional<std::invoke_result_t<Func, First&>>;

        template for (constexpr auto I : rbox::indices_view{sizeof...(Args)})
        {
            if (alternative_index == I) {
                auto* ptr = alternatives.[:ith_field(I):];
                return Ret{std::invoke(std::forward<Func>(func), *ptr)};
            }
        }
        return Ret{};  // nullopt
    }

    // Note: It's intentional to keep the fields public so that ref_variant is a structural type.
    size_t alternative_index;
    underlying_type alternatives;
};

// Similar with std::reference_wrapper (whose operator== compares the referenced values as well).
template <class... Args>
    requires (is_operator_eq_comparable_v<Args, Args> && ...)
constexpr bool operator==(
    const optional_ref_variant<Args...>& lhs, const optional_ref_variant<Args...>& rhs)
{
    using underlying_type = typename optional_ref_variant<Args...>::underlying_type;
    if (lhs.alternative_index != rhs.alternative_index) {
        return false;
    }
    template for (constexpr auto I : rbox::indices_view{sizeof...(Args)})
    {
        if (lhs.alternative_index == I) {
            constexpr auto F = all_direct_nonstatic_data_members_v<underlying_type>[I];
            return *lhs.alternatives.[:F:] == *rhs.alternatives.[:F:];
        }
    }
    return true;  // Both are valueless
}

// Alternative index as first key, and referenced value as second key.
// Similar with std::reference_wrapper (whose operator<=> compares the referenced values as well).
template <class... Args>
    requires (is_operator_3way_comparable_v<Args, Args> && ...)
constexpr auto operator<=>(
    const optional_ref_variant<Args...>& lhs, const optional_ref_variant<Args...>& rhs)
    -> std::common_comparison_category_t<std::compare_three_way_result_t<Args>...>
{
    using underlying_type = typename ref_variant<Args...>::underlying_type;
    if (lhs.alternative_index != rhs.alternative_index) {
        // npos (which is -1) should be the smallest index
        return static_cast<ssize_t>(lhs.alternative_index)
           <=> static_cast<ssize_t>(rhs.alternative_index);
    }
    template for (constexpr auto I : rbox::indices_view{sizeof...(Args)})
    {
        if (lhs.alternative_index == I) {
            constexpr auto F = all_direct_nonstatic_data_members_v<underlying_type>[I];
            return *lhs.alternatives.[:F:] <=> *rhs.alternatives.[:F:];
        }
    }
    return std::strong_ordering::equal;  // Both are valueless
}

template <class T, class... Args>
    requires (impl::ref_variant_alternative_arg<T, Args...>)
constexpr auto holds_alternative(const optional_ref_variant<Args...>& v) -> bool
{
    return v.template holds_alternative<T>();
}

template <class T, class... Args>
    requires (impl::ref_variant_alternative_arg<T, Args...>)
constexpr auto get(const optional_ref_variant<Args...>& v) -> auto&
{
    return v.template get<T>();
}

template <size_t I, class... Args>
    requires (I < sizeof...(Args))
constexpr auto get(const optional_ref_variant<Args...>& v) -> auto&
{
    return v.template get<I>();
}

template <class T, class... Args>
    requires (impl::ref_variant_alternative_arg<T, Args...>)
constexpr auto get_if(const optional_ref_variant<Args...>& v) -> auto*
{
    return v.template get_if<T>();
}

template <size_t I, class... Args>
    requires (I < sizeof...(Args))
constexpr auto get_if(const optional_ref_variant<Args...>& v) -> auto*
{
    return v.template get_if<I>();
}

template <class Func, class... Args>
constexpr auto try_visit(Func&& func, const optional_ref_variant<Args...>& v)
{
    return v.try_visit(std::forward<Func>(func));
}

template <class... Args>
using cref_variant = ref_variant<const Args...>;

template <class... Args>
using optional_cref_variant = optional_ref_variant<const Args...>;
}  // namespace rbox

template <class... Args>
struct std::variant_size<rbox::ref_variant<Args...>>
    : std::integral_constant<std::size_t, sizeof...(Args)> {};

template <std::size_t I, class... Args>
struct std::variant_alternative<I, rbox::ref_variant<Args...>> {
    using type = Args...[I];
};

template <class... Args>
struct std::variant_size<rbox::optional_ref_variant<Args...>>
    : std::integral_constant<std::size_t, sizeof...(Args)> {};

template <std::size_t I, class... Args>
struct std::variant_alternative<I, rbox::optional_ref_variant<Args...>> {
    using type = Args...[I];
};

#endif  // RBOX_UTILS_REF_VARIANT_HPP
