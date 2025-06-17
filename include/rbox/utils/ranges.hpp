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

#ifndef RBOX_UTILS_RANGES_HPP
#define RBOX_UTILS_RANGES_HPP

#include <ranges>
#include <type_traits>

namespace rbox {
// -------- Extension of Ranges concepts --------

template <class T>
concept non_range = !std::ranges::range<T>;

#if __cpp_lib_ranges_as_const >= 202207L
#define RBOX_CONST_RANGE_CONCEPTS_FOR_EACH(F) F(constant_range)
#else
#define RBOX_CONST_RANGE_CONCEPTS_FOR_EACH(F)  // No-op
#endif

#define RBOX_RANGE_CONCEPTS_FOR_EACH(F) \
    F(range)                            \
    F(borrowed_range)                   \
    F(sized_range)                      \
    F(view)                             \
    F(input_range)                      \
    F(forward_range)                    \
    F(bidirectional_range)              \
    F(random_access_range)              \
    F(contiguous_range)                 \
    F(common_range)                     \
    F(viewable_range)                   \
    RBOX_CONST_RANGE_CONCEPTS_FOR_EACH(F)

#define RBOX_DEFINE_RANGE_CONCEPTS(range_concept)                                             \
    template <class Range, class T>                                                           \
    concept range_concept##_of = std::ranges::range_concept<Range>                            \
                              && std::is_convertible_v<std::ranges::range_value_t<Range>, T>; \
                                                                                              \
    template <class Range, class T>                                                           \
    concept range_concept##_of_exactly =                                                      \
        std::ranges::range_concept<Range> && std::is_same_v<std::ranges::range_value_t<Range>, T>;

RBOX_RANGE_CONCEPTS_FOR_EACH(RBOX_DEFINE_RANGE_CONCEPTS)

template <class Range, class T>
concept output_range_of = std::ranges::output_range<Range, T>;

template <class Range, class T>
concept output_range_of_exactly =
    std::ranges::output_range<Range, T> && std::is_same_v<std::ranges::range_value_t<Range>, T>;

#undef RBOX_DEFINE_RANGE_CONCEPTS
#undef RBOX_RANGE_CONCEPTS_FOR_EACH

#define RBOX_RANGE_CONCEPTS_FOR_EACH(F) \
    F(borrowed_range)                   \
    F(sized_range)                      \
    F(view)                             \
    F(input_range)                      \
    F(forward_range)                    \
    F(bidirectional_range)              \
    F(random_access_range)              \
    F(contiguous_range)                 \
    F(viewable_range)                   \
    RBOX_CONST_RANGE_CONCEPTS_FOR_EACH(F)

#define RBOX_DEFINE_RANGE_CONCEPTS(range_concept)                                                 \
    template <class Range, class T>                                                               \
    concept common_##range_concept##_of =                                                         \
        std::ranges::range_concept<Range>                                                         \
        && std::is_convertible_v<std::ranges::range_value_t<Range>, T>                            \
        && std::ranges::common_range<Range>;                                                      \
                                                                                                  \
    template <class Range, class T>                                                               \
    concept common_##range_concept##_of_exactly =                                                 \
        std::ranges::range_concept<Range> && std::is_same_v<std::ranges::range_value_t<Range>, T> \
        && std::ranges::common_range<Range>;

RBOX_RANGE_CONCEPTS_FOR_EACH(RBOX_DEFINE_RANGE_CONCEPTS)

#undef RBOX_DEFINE_RANGE_CONCEPTS
#undef RBOX_RANGE_CONCEPTS_FOR_EACH
#undef RBOX_CONST_RANGE_CONCEPTS_FOR_EACH

template <class Range, class T>
concept common_output_range_of = output_range_of<Range, T> && std::ranges::common_range<Range>;

template <class Range, class T>
concept common_output_range_of_exactly =
    output_range_of_exactly<Range, T> && std::ranges::common_range<Range>;
}  // namespace rbox

#endif  // RBOX_UTILS_RANGES_HPP
