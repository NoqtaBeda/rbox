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

#ifndef RBOX_ENUM_ENUM_NAMES_HPP
#define RBOX_ENUM_ENUM_NAMES_HPP

#include <rbox/enum/enum_meta_entries.hpp>
#include <rbox/utils/concepts.hpp>

namespace rbox {
namespace impl {
template <class E, enum_entry_order Order>
constexpr auto enum_names() /* -> std::array<std::string_view, N> */
{
    auto res = std::array<std::string_view, enum_count_v<E>>{};
    auto index = 0zU;
    for (auto e : enum_meta_entries_v<E, Order>) {
        res[index++] = std::define_static_string(std::meta::identifier_of(e));
    }
    return res;
}

consteval bool enum_names_are_ascii_only(std::meta::info E)
{
    for (auto entry : std::meta::enumerators_of(E)) {
        for (char c : std::meta::identifier_of(entry)) {
            if (c < 0 || c > 127) return false;
        }
    }
    return true;
}

template <class E, enum_entry_order Order>
constexpr auto enum_names_array_v = enum_names<E, Order>();
}  // namespace impl

template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr auto enum_names_v = std::span{impl::enum_names_array_v<std::remove_cv_t<E>, Order>};

template <enum_type E>
constexpr auto enum_names_are_ascii_only_v = impl::enum_names_are_ascii_only(^^E);
}  // namespace rbox

#endif  // RBOX_ENUM_ENUM_NAMES_HPP
