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

#ifndef RBOX_UTILS_ADDRESSABLE_MEMBER_HPP
#define RBOX_UTILS_ADDRESSABLE_MEMBER_HPP

#include <meta>
#include <rbox/utils/config.hpp>

namespace rbox {
consteval bool is_addressable_class_member(std::meta::info m)
{
    if (!std::meta::is_class_member(m) || std::meta::is_template(m) || std::meta::is_type(m)) {
        return false;
    }
    if (std::meta::is_nonstatic_data_member(m)) {
        return !std::meta::is_reference_type(std::meta::type_of(m)) && !std::meta::is_bit_field(m);
    } else if (std::meta::is_function(m)) {
        // Member functions (including static and non-static)
        return !std::meta::is_constructor(m) && !std::meta::is_destructor(m)
            && !std::meta::is_deleted(m);
    } else {
        // Static data members: always supports addressing (even if it's a reference)
        return true;
    }
}

consteval bool is_addressable_non_class_member(std::meta::info m)
{
    if (std::meta::is_class_member(m) || std::meta::is_template(m)) {
        return false;
    }
    return std::meta::is_variable(m) || (std::meta::is_function(m) && !std::meta::is_deleted(m));
}
}  // namespace rbox

#endif  // RBOX_UTILS_ADDRESSABLE_MEMBER_HPP
