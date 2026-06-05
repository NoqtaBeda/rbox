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
#define MAKE_LOOKUP_FUNCTION(suffix, CI, AA, CharT, char_tag, LIT)                   \
    [[gnu::visibility("default")]]                                                   \
    bool test_by_hash_search_2_##suffix##_##char_tag(size_t* dest, const CharT* key) \
    {                                                                                \
        using Value = std::pair<size_t, size_t>;                                     \
        constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(                              \
            std::initializer_list<std::pair<const CharT*, Value>>{                   \
                {LIT("Amiya"), {0, 1}},                                              \
                {LIT("Kal'tsit"), {1, 10}},                                          \
                {LIT("Blaze"), {2, 20}},                                             \
                {LIT("Rosmontis"), {3, 30}},                                         \
                {LIT("Logos"), {4, 40}},                                             \
                {LIT("Ch'en"), {5, 50}},                                             \
                {LIT("Crownslayer"), {6, 60}},                                       \
                {LIT("Sharp"), {7, 70}},                                             \
            },                                                                       \
            rbox::string_key_fixed_map_options{                                      \
                .ascii_case_insensitive = CI,                                        \
                .adjusts_alignment = AA,                                             \
                .max_n_iterations = 0, /* Disables hash table */                     \
                .binary_search_threshold = 8,                                        \
            });                                                                      \
                                                                                     \
        if (auto opt = map.find(key); opt) {                                         \
            dest[0] = opt->first;                                                    \
            dest[1] = opt->second;                                                   \
            return true;                                                             \
        }                                                                            \
        return false;                                                                \
    }

#define RBOX_BM_LIT_char(x) x
#define RBOX_BM_LIT_wchar_t(x) L##x
#define RBOX_BM_LIT_char8_t(x) u8##x
#define RBOX_BM_LIT_char16_t(x) u##x
#define RBOX_BM_LIT_char32_t(x) U##x

#define MAKE_LOOKUP_VARIANTS(CharT, char_tag)                                            \
    MAKE_LOOKUP_FUNCTION(_normal, false, false, CharT, char_tag, RBOX_BM_LIT_##char_tag) \
    MAKE_LOOKUP_FUNCTION(_aa, false, true, CharT, char_tag, RBOX_BM_LIT_##char_tag)      \
    MAKE_LOOKUP_FUNCTION(_ci, true, false, CharT, char_tag, RBOX_BM_LIT_##char_tag)      \
    MAKE_LOOKUP_FUNCTION(_ci_aa, true, true, CharT, char_tag, RBOX_BM_LIT_##char_tag)

MAKE_LOOKUP_VARIANTS(char, char)
MAKE_LOOKUP_VARIANTS(wchar_t, wchar_t)
MAKE_LOOKUP_VARIANTS(char8_t, char8_t)
MAKE_LOOKUP_VARIANTS(char16_t, char16_t)
MAKE_LOOKUP_VARIANTS(char32_t, char32_t)
}  // extern "C"
