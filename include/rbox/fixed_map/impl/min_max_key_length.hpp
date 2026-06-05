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

#ifndef RBOX_FIXED_MAP_IMPL_MIN_MAX_KEY_LENGTH_HPP
#define RBOX_FIXED_MAP_IMPL_MIN_MAX_KEY_LENGTH_HPP

#include <rbox/utils/meta_pair.hpp>
#include <rbox/utils/meta_span.hpp>
#include <rbox/utils/meta_string_view.hpp>

namespace rbox::impl::map {
template <class CharT, class V>
consteval auto min_max_key_length(meta_span<meta_pair<meta_basic_string_view<CharT>, V>> kv_pairs)
    -> meta_pair<size_t, size_t>
{
    auto min_length = static_cast<size_t>(-1);  // SIZE_MAX
    auto max_length = 0zU;
    for (const auto& entry : kv_pairs) {
        auto cur_length = entry.first.n;
        if (cur_length < min_length) {
            min_length = cur_length;
        }
        if (cur_length > max_length) {
            max_length = cur_length;
        }
    }
    return meta_pair{min_length, max_length};
}
}  // namespace rbox::impl::map

#endif  // RBOX_FIXED_MAP_IMPL_MIN_MAX_KEY_LENGTH_HPP
