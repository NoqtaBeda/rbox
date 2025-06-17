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

#ifndef RBOX_UTILS_CONCEPTS_HPP
#define RBOX_UTILS_CONCEPTS_HPP

#include <type_traits>

namespace rbox {
#define RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(category) \
    template <class T>                              \
    concept category##_type = std::is_##category##_v<T>;

RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(abstract)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(aggregate)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(arithmetic)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(array)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(bounded_array)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(class)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(compound)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(empty)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(enum)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(final)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(function)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(fundamental)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(lvalue_reference)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(implicit_lifetime)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(member_function_pointer)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(member_object_pointer)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(member_pointer)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(object)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(pointer)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(polymorphic)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(reference)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(rvalue_reference)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(scalar)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(scoped_enum)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(structural)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(unbounded_array)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT(union)

#define RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(category) \
    template <class T>                                  \
    concept non_##category##_type = !std::is_##category##_v<T>;

// Some std traits are not listed below due to ambiguity concerns.
// e.g. Does 'non_bounded_array' refer to an array type that is not bounded,
//      or any type that is not a bounded array?
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(abstract)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(aggregate)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(arithmetic)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(array)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(class)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(compound)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(empty)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(enum)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(final)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(function)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(fundamental)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(object)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(pointer)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(polymorphic)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(reference)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(scalar)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(structural)
RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG(union)

#undef RBOX_DEFINE_TYPE_CATEGORY_CONCEPT
#undef RBOX_DEFINE_TYPE_CATEGORY_CONCEPT_NEG

template <class T>
concept class_or_union_type = std::is_class_v<T> || std::is_union_v<T>;

template <class T>
concept integral_or_enum_type = std::is_integral_v<T> || std::is_enum_v<T>;
}  // namespace rbox

#endif  // RBOX_UTILS_CONCEPTS_HPP
