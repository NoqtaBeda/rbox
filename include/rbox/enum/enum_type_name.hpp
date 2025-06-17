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

#ifndef RBOX_ENUM_ENUM_TYPE_NAME_HPP
#define RBOX_ENUM_ENUM_TYPE_NAME_HPP

#include <rbox/utils/concepts.hpp>
#include <rbox/utils/meta_utility.hpp>

namespace rbox {
template <enum_type E>
constexpr auto enum_type_name_v =  //
    std::meta::has_identifier(std::meta::remove_cvref(^^E))
        ? std::meta::identifier_of(std::meta::remove_cvref(^^E))
        : std::string_view{};

template <enum_type E>
constexpr auto enum_type_display_string_v =
    std::string_view{std::meta::display_string_of(std::meta::dealias(^^E))};
}  // namespace rbox

#endif  // RBOX_ENUM_ENUM_TYPE_NAME_HPP
