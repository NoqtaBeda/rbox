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

#ifndef RBOX_ENUM_ENUM_FLAGS_CAST_HPP
#define RBOX_ENUM_ENUM_FLAGS_CAST_HPP

#include <rbox/enum/enum_cast.hpp>
#include <rbox/enum/enum_flags_contains.hpp>
#include <rbox/utils/concepts.hpp>

namespace rbox {
namespace impl {
template <class E, class Delim>
constexpr auto enum_flags_cast_impl(std::string_view str, Delim delim) -> std::optional<E>
{
    auto underlying = std::underlying_type_t<E>{0};
    for (auto token : std::views::split(str, delim)) {
        auto trimmed = ascii_trim(token);
        if (trimmed.empty()) {
            continue;
        }
        auto opt = enum_cast<E>(trimmed);
        if (!opt.has_value()) {
            return std::nullopt;
        }
        underlying |= std::to_underlying(*opt);
    }
    return static_cast<E>(underlying);
}

template <class E, class Delim>
constexpr auto enum_flags_cast_ci_impl(std::string_view str, Delim delim) -> std::optional<E>
{
    auto underlying = std::underlying_type_t<E>{0};
    for (auto token : std::views::split(str, delim)) {
        auto trimmed = ascii_trim(token);
        if (trimmed.empty()) {
            continue;
        }
        auto opt = ascii_ci_enum_cast<E>(trimmed);
        if (!opt.has_value()) {
            return std::nullopt;
        }
        underlying |= std::to_underlying(*opt);
    }
    return static_cast<E>(underlying);
}
}  // namespace impl

template <enum_type E>
struct enum_flags_cast_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    static constexpr auto operator()(std::string_view str, char delim = '|') -> std::optional<ENoCV>
    {
        return impl::enum_flags_cast_impl<ENoCV>(str, delim);
    }

    static constexpr auto operator()(std::string_view str, std::string_view delim)
        -> std::optional<ENoCV>
    {
        return impl::enum_flags_cast_impl<ENoCV>(str, delim);
    }

    static constexpr auto operator()(std::integral auto value) -> std::optional<ENoCV>
    {
        if (enum_flags_contains<ENoCV>(value)) {
            return static_cast<ENoCV>(value);
        }
        return std::nullopt;
    }
};

template <enum_type E>
constexpr auto enum_flags_cast = enum_flags_cast_t<std::remove_cv_t<E>>{};

template <enum_type E>
struct ascii_ci_enum_flags_cast_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    static constexpr auto operator()(std::string_view str, char delim = '|') -> std::optional<ENoCV>
    {
        return impl::enum_flags_cast_ci_impl<ENoCV>(str, delim);
    }

    static constexpr auto operator()(std::string_view str, std::string_view delim)
        -> std::optional<ENoCV>
    {
        return impl::enum_flags_cast_ci_impl<ENoCV>(str, delim);
    }
};

template <enum_type E>
constexpr auto ascii_ci_enum_flags_cast = ascii_ci_enum_flags_cast_t<std::remove_cv_t<E>>{};
}  // namespace rbox

#endif  // RBOX_ENUM_ENUM_FLAGS_CAST_HPP
