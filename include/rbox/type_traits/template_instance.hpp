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

#ifndef RBOX_TYPE_TRAITS_TEMPLATE_INSTANCE_HPP
#define RBOX_TYPE_TRAITS_TEMPLATE_INSTANCE_HPP

#include <type_traits>

namespace rbox {
namespace impl {
template <class T, template <class...> class Templ>
struct is_template_instance_of : std::false_type {};

template <template <class...> class Templ, class... Args>
struct is_template_instance_of<Templ<Args...>, Templ> : std::true_type {};

template <class T, template <auto...> class Templ>
struct is_nontype_template_instance_of : std::false_type {};

template <template <auto...> class Templ, auto... Args>
struct is_nontype_template_instance_of<Templ<Args...>, Templ> : std::true_type {};
}  // namespace impl

template <class T, template <class...> class Templ>
concept template_instance_of = impl::is_template_instance_of<std::remove_cv_t<T>, Templ>::value;

template <class T, template <auto...> class Templ>
concept nontype_template_instance_of =
    impl::is_nontype_template_instance_of<std::remove_cv_t<T>, Templ>::value;
}  // namespace rbox

#endif  // RBOX_TYPE_TRAITS_TEMPLATE_INSTANCE_HPP
