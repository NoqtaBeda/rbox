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

#ifndef RBOX_ENUM_ENUM_CONTAINS_HPP
#define RBOX_ENUM_ENUM_CONTAINS_HPP

#include <rbox/enum/impl/enum_from_string_map.hpp>
#include <rbox/enum/impl/enum_name_map.hpp>
#include <rbox/enum/impl/promoted.hpp>
#include <rbox/utils/concepts.hpp>
#include <rbox/utils/functional/integral_comparison.hpp>

namespace rbox {
template <enum_type E>
struct enum_contains_t {
private:
    using ENoCV = std::remove_cv_t<E>;
    using EUnderlying = std::underlying_type_t<ENoCV>;

public:
    static constexpr bool operator()(ENoCV value)
    {
        auto key = impl::promoted(value);
        return impl::enum_name_map_v<E>.find(key).has_value();
    }

    static constexpr bool operator()(std::integral auto value)
    {
        if (!in_range<EUnderlying>(value)) {
            return false;
        }
        auto value_as_enum = static_cast<ENoCV>(value);
        return impl::enum_name_map_v<E>.find(impl::promoted(value_as_enum)).has_value();
    }

    static constexpr bool operator()(std::string_view str)
    {
        return impl::enum_from_string_map_v<E>.find(str).has_value();
    }
};

template <enum_type E>
constexpr auto enum_contains = enum_contains_t<std::remove_cv_t<E>>{};

template <enum_type E>
struct ascii_ci_enum_contains_t {
private:
    using ENoCV = std::remove_cv_t<E>;

public:
    static constexpr bool operator()(std::string_view str)
    {
        return impl::enum_from_ci_string_map_v<ENoCV>.find(str).has_value();
    }
};

template <enum_type E>
constexpr auto ascii_ci_enum_contains = ascii_ci_enum_contains_t<std::remove_cv_t<E>>{};
}  // namespace rbox

#endif  // RBOX_ENUM_ENUM_CONTAINS_HPP
