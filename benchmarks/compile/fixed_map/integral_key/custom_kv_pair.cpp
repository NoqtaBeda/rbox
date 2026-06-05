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

#include <rbox/fixed_map/integral_key.hpp>

extern "C" {
[[gnu::visibility("default")]]
const char* test_custom_kv_pair(char key)
{
    constexpr auto map = RBOX_INTEGRAL_KEY_FIXED_MAP(
        std::initializer_list<std::pair<char, const char*>>{
            {"Apple"[0], "Apple"},
            {"Banana"[0], "Banana"},
            {"Cat"[0], "Cat"},
            {"Dog"[0], "Dog"},
            {"Foo"[0], "Foo"},
            {"Horse"[0], "Horse"},
            {"Island"[0], "Island"},
            {"Rabbit"[0], "Rabbit"},
            {"Snake"[0], "Snake"},
            {"Zebra"[0], "Zebra"},
        },
        rbox::integral_key_fixed_map_options{
            .min_load_factor = 0.5,
            .binary_search_threshold = 8,
        });

    if (auto opt = map.find(key); opt) {
        return opt->head;
    }
    return nullptr;
}
}  // extern "C"
