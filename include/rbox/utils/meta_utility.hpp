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

#ifndef RBOX_UTILS_META_UTILITY_HPP
#define RBOX_UTILS_META_UTILITY_HPP

#include <concepts>
#include <meta>
#include <rbox/utils/concepts.hpp>

#define RBOX_CURRENT_CONTEXT std::meta::access_context::current()

namespace rbox {
consteval auto unprivileged_context()
{
    return std::meta::access_context::unprivileged();
}

consteval auto unchecked_context()
{
    return std::meta::access_context::unchecked();
}

#define RBOX_DEFINE_QUERY_FN_WITH_ACCESS_CONTEXT(fn)                   \
    /** Gets all direct members that are accessible in global scope */ \
    consteval auto public_direct_##fn##_of(std::meta::info a)          \
    {                                                                  \
        return std::meta::fn##_of(a, rbox::unprivileged_context());    \
    }                                                                  \
    /* Gets all direct members regardless of their accessibility */    \
    consteval auto all_direct_##fn##_of(std::meta::info a)             \
    {                                                                  \
        return std::meta::fn##_of(a, rbox::unchecked_context());       \
    }

#define RBOX_DEFINE_QUERY_V_WITH_ACCESS_CONTEXT(fn)             \
    template <class_or_union_type T>                            \
    constexpr auto public_direct_##fn##_v =                     \
        std::define_static_array(public_direct_##fn##_of(^^T)); \
                                                                \
    template <class_or_union_type T>                            \
    constexpr auto all_direct_##fn##_v = std::define_static_array(all_direct_##fn##_of(^^T));

RBOX_DEFINE_QUERY_FN_WITH_ACCESS_CONTEXT(members)
RBOX_DEFINE_QUERY_FN_WITH_ACCESS_CONTEXT(bases)
RBOX_DEFINE_QUERY_FN_WITH_ACCESS_CONTEXT(static_data_members)
RBOX_DEFINE_QUERY_FN_WITH_ACCESS_CONTEXT(nonstatic_data_members)

RBOX_DEFINE_QUERY_V_WITH_ACCESS_CONTEXT(members)
RBOX_DEFINE_QUERY_V_WITH_ACCESS_CONTEXT(bases)
RBOX_DEFINE_QUERY_V_WITH_ACCESS_CONTEXT(static_data_members)
RBOX_DEFINE_QUERY_V_WITH_ACCESS_CONTEXT(nonstatic_data_members)

#undef RBOX_DEFINE_QUERY_FN_WITH_ACCESS_CONTEXT

template <class T, std::same_as<std::meta::info>... Args>
consteval T extract(std::meta::info templ, Args... templ_params)
{
    return std::meta::extract<T>(std::meta::substitute(templ, {templ_params...}));
}
}  // namespace rbox

#endif  // RBOX_UTILS_META_UTILITY_HPP
