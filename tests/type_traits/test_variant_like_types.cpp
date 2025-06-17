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

#include <rbox/type_traits/variant_like_types.hpp>
#include <rbox/utils/meta_variant.hpp>
#include <rbox/utils/ref_variant.hpp>
#include <tuple>
#include <utility>
#include <variant>

#include "tests/test_options.hpp"

TEST(TypeTraits, VariantLikeTypes)
{
    static_assert(rbox::variant_like<std::variant<int, int*, int**>>);
    static_assert(rbox::variant_like<const std::variant<int, int*, int**>>);
    static_assert(rbox::variant_like<volatile std::variant<int, int*, int**>>);
    static_assert(rbox::variant_like<const volatile std::variant<int, int*, int**>>);

    static_assert(rbox::variant_like<rbox::meta_variant<int, int*, int**>>);
    static_assert(rbox::variant_like<const rbox::meta_variant<int, int*, int**>>);
    static_assert(rbox::variant_like<volatile rbox::meta_variant<int, int*, int**>>);
    static_assert(rbox::variant_like<const volatile rbox::meta_variant<int, int*, int**>>);

    static_assert(rbox::variant_like<rbox::ref_variant<int, int*, int**>>);
    static_assert(rbox::variant_like<const rbox::ref_variant<int, int*, int**>>);
    static_assert(rbox::variant_like<volatile rbox::ref_variant<int, int*, int**>>);
    static_assert(rbox::variant_like<const volatile rbox::ref_variant<int, int*, int**>>);

    static_assert(rbox::variant_like<rbox::optional_ref_variant<int, int*, int**>>);
    static_assert(rbox::variant_like<const rbox::optional_ref_variant<int, int*, int**>>);
    static_assert(rbox::variant_like<volatile rbox::optional_ref_variant<int, int*, int**>>);
    static_assert(rbox::variant_like<const volatile rbox::optional_ref_variant<int, int*, int**>>);

    // Excludes references
    static_assert(NOT rbox::variant_like<std::variant<int, int*, int**>&>);
    // Other types
    static_assert(NOT rbox::variant_like<void>);
    static_assert(NOT rbox::variant_like<int>);
    static_assert(NOT rbox::variant_like<std::tuple<int, double>>);
}
