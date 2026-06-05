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

#include <rbox/fixed_map/string_key.hpp>

extern "C" {
#define MAKE_LOOKUP_FUNCTION(suffix, CI, BT, CharT, char_tag, LIT)    \
    [[gnu::visibility("default")]]                                    \
    int test_by_hash_search_1_##suffix##_##char_tag(const CharT* key) \
    {                                                                 \
        constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(               \
            std::initializer_list<std::pair<const CharT*, int>>{      \
                {LIT("Apple"), 0},                                    \
                {LIT("Banana"), 1},                                   \
                {LIT("Cat"), 2},                                      \
                {LIT("Dog"), 3},                                      \
                {LIT("Horse"), 4},                                    \
                {LIT("Rabbit"), 5},                                   \
                {LIT("Pineapple"), 6},                                \
                {LIT("Sheep"), 7},                                    \
            },                                                        \
            rbox::string_key_fixed_map_options{                       \
                .ascii_case_insensitive = CI,                         \
                .max_n_iterations = 0, /* Disables hash table */      \
                .binary_search_threshold = BT,                        \
            });                                                       \
                                                                      \
        if (auto opt = map.find(key); opt) {                          \
            return *opt;                                              \
        }                                                             \
        return -1;                                                    \
    }

// Helper to prefix string literals for each character type
#define RBOX_BM_LIT_char(x) x
#define RBOX_BM_LIT_wchar_t(x) L##x
#define RBOX_BM_LIT_char8_t(x) u8##x
#define RBOX_BM_LIT_char16_t(x) u##x
#define RBOX_BM_LIT_char32_t(x) U##x

#define MAKE_LOOKUP_VARIANTS(CharT, char_tag)                                               \
    MAKE_LOOKUP_FUNCTION(_linear, false, 9, CharT, char_tag, RBOX_BM_LIT_##char_tag)        \
    MAKE_LOOKUP_FUNCTION(_binary_search, false, 8, CharT, char_tag, RBOX_BM_LIT_##char_tag) \
    MAKE_LOOKUP_FUNCTION(_ci_linear, true, 9, CharT, char_tag, RBOX_BM_LIT_##char_tag)      \
    MAKE_LOOKUP_FUNCTION(_ci_binary_search, true, 8, CharT, char_tag, RBOX_BM_LIT_##char_tag)

MAKE_LOOKUP_VARIANTS(char, char)
MAKE_LOOKUP_VARIANTS(wchar_t, wchar_t)
MAKE_LOOKUP_VARIANTS(char8_t, char8_t)
MAKE_LOOKUP_VARIANTS(char16_t, char16_t)
MAKE_LOOKUP_VARIANTS(char32_t, char32_t)
}  // extern "C"
