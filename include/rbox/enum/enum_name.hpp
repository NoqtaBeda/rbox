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

#ifndef RBOX_ENUM_ENUM_NAME_HPP
#define RBOX_ENUM_ENUM_NAME_HPP

#include <rbox/enum/impl/enum_name_map.hpp>
#include <rbox/enum/impl/promoted.hpp>
#include <rbox/utils/concepts.hpp>

namespace rbox {
struct enum_name_t {
    template <enum_type E>
    static constexpr auto operator()(E value) -> std::string_view
    {
        return impl::enum_name_map_v<E>[impl::promoted(value)];
    }
};

struct enum_name_opt_t {
    template <enum_type E>
    static constexpr auto operator()(E value) -> std::optional<std::string_view>
    {
        auto p = impl::enum_name_map_v<E>.find(impl::promoted(value));
        if (p.has_value()) {
            return static_cast<std::string_view>(*p);
        }
        return std::nullopt;
    }
};

constexpr auto enum_name = enum_name_t{};
constexpr auto enum_name_opt = enum_name_opt_t{};
}  // namespace rbox

#endif  // RBOX_ENUM_ENUM_NAME_HPP
