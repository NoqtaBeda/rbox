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
#define MAKE_LOOKUP_FUNCTION(suffix, A, CharT, char_tag, LIT)                       \
    [[gnu::visibility("default")]]                                                  \
    bool test_by_hash_table_1_##suffix##_##char_tag(size_t* dest, const CharT* key) \
    {                                                                               \
        using Value = std::pair<size_t, size_t>;                                    \
        constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(                             \
            std::initializer_list<std::pair<const CharT*, Value>>{                  \
                {LIT("Apple"), {0, 1}},                                             \
                {LIT("Banana"), {1, 10}},                                           \
                {LIT("Cat"), {2, 20}},                                              \
                {LIT("Dog"), {3, 30}},                                              \
                {LIT("Horse"), {4, 40}},                                            \
                {LIT("Rabbit"), {5, 50}},                                           \
                {LIT("Squirrow"), {6, 60}},                                         \
                {LIT("Sheep"), {7, 70}},                                            \
            },                                                                      \
            rbox::string_key_fixed_map_options{                                     \
                .ascii_case_insensitive = false,                                    \
                .adjusts_alignment = A,                                             \
                .min_load_factor = 0.5,                                             \
            });                                                                     \
                                                                                    \
        if (auto opt = map.find(key); opt) {                                        \
            dest[0] = opt->first;                                                   \
            dest[1] = opt->second;                                                  \
            return true;                                                            \
        }                                                                           \
        return false;                                                               \
    }

#define MAKE_CI_LOOKUP_FUNCTION(CharT, char_tag, LIT)            \
    [[gnu::visibility("default")]]                               \
    int test_by_hash_table_1_ci_##char_tag(const CharT* key)     \
    {                                                            \
        constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(          \
            std::initializer_list<std::pair<const CharT*, int>>{ \
                {LIT("Apple"), 0},                               \
                {LIT("BANANA"), 1},                              \
                {LIT("CAT"), 2},                                 \
                {LIT("dog"), 3},                                 \
                {LIT("HORSE"), 4},                               \
                {LIT("RaBbIt"), 5},                              \
                {LIT("Squirrow"), 6},                            \
                {LIT("shEEp"), 7},                               \
            },                                                   \
            rbox::string_key_fixed_map_options{                  \
                .ascii_case_insensitive = true,                  \
                .min_load_factor = 0.5,                          \
            });                                                  \
                                                                 \
        if (auto opt = map.find(key); opt) {                     \
            return *opt;                                         \
        }                                                        \
        return -1;                                               \
    }

#define RBOX_BM_LIT_char(x) x
#define RBOX_BM_LIT_wchar_t(x) L##x
#define RBOX_BM_LIT_char8_t(x) u8##x
#define RBOX_BM_LIT_char16_t(x) u##x
#define RBOX_BM_LIT_char32_t(x) U##x

#define MAKE_LOOKUP_VARIANTS(CharT, char_tag)                                     \
    MAKE_LOOKUP_FUNCTION(_normal, false, CharT, char_tag, RBOX_BM_LIT_##char_tag) \
    MAKE_LOOKUP_FUNCTION(_a, true, CharT, char_tag, RBOX_BM_LIT_##char_tag)       \
    MAKE_CI_LOOKUP_FUNCTION(CharT, char_tag, RBOX_BM_LIT_##char_tag)

MAKE_LOOKUP_VARIANTS(char, char)
MAKE_LOOKUP_VARIANTS(wchar_t, wchar_t)
MAKE_LOOKUP_VARIANTS(char8_t, char8_t)
MAKE_LOOKUP_VARIANTS(char16_t, char16_t)
MAKE_LOOKUP_VARIANTS(char32_t, char32_t)
}  // extern "C"
