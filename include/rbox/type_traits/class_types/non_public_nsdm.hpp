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

#ifndef RBOX_TYPE_TRAITS_CLASS_TYPES_NON_PUBLIC_NSDM_HPP
#define RBOX_TYPE_TRAITS_CLASS_TYPES_NON_PUBLIC_NSDM_HPP

#include <meta>
#include <rbox/utils/config.hpp>

// NSDM: Non-Static Data Members
namespace rbox {
namespace impl {
consteval bool is_class_type_with_non_public_nsdm(std::meta::info T);
}  // namespace impl

template <class T>
concept class_without_non_public_nonstatic_data_members =
    !impl::is_class_type_with_non_public_nsdm(^^T);

namespace impl {
consteval bool is_class_type_with_non_public_nsdm(std::meta::info T)
{
    T = std::meta::remove_cv(T);
    if (!std::meta::is_class_type(T)) {
        return false;
    }
    auto unprivileged = std::meta::access_context::unprivileged();
    if (std::meta::has_inaccessible_nonstatic_data_members(T, unprivileged)) {
        return true;
    }
    auto unchecked = std::meta::access_context::unchecked();
    for (auto base : std::meta::bases_of(T, unchecked)) {
        if (std::meta::is_public(base)) {
            auto B = std::meta::type_of(base);
            auto c = std::meta::substitute(
                ^^class_without_non_public_nonstatic_data_members, RBOX_IL(B));
            if (!std::meta::extract<bool>(c)) {
                return true;
            }
        } else if (!std::meta::is_empty_type(std::meta::type_of(base))) {
            return true;
        }
    }
    return false;
}
}  // namespace impl
}  // namespace rbox

#endif  // RBOX_TYPE_TRAITS_CLASS_TYPES_NON_PUBLIC_NSDM_HPP
