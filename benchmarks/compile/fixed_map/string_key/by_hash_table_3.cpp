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

// Keys: "BA"[0x2207], "Ba"[0x2227], plus "AA".."AZ"[0x2184~0x219d] = 28 entries
#define HASH_TABLE_3_KEYS(LIT)                                               \
    {LIT("BA"), {1, 10}},     /* 0x2207 */                                   \
        {LIT("Ba"), {2, 20}}, /* 0x2227 */                                   \
        {LIT("AA"), {65, 66}}, {LIT("AB"), {66, 67}}, {LIT("AC"), {67, 68}}, \
        {LIT("AD"), {68, 69}}, {LIT("AE"), {69, 70}}, {LIT("AF"), {70, 71}}, \
        {LIT("AG"), {71, 72}}, {LIT("AH"), {72, 73}}, {LIT("AI"), {73, 74}}, \
        {LIT("AJ"), {74, 75}}, {LIT("AK"), {75, 76}}, {LIT("AL"), {76, 77}}, \
        {LIT("AM"), {77, 78}}, {LIT("AN"), {78, 79}}, {LIT("AO"), {79, 80}}, \
        {LIT("AP"), {80, 81}}, {LIT("AQ"), {81, 82}}, {LIT("AR"), {82, 83}}, \
        {LIT("AS"), {83, 84}}, {LIT("AT"), {84, 85}}, {LIT("AU"), {85, 86}}, \
        {LIT("AV"), {86, 87}}, {LIT("AW"), {87, 88}}, {LIT("AX"), {88, 89}}, \
        {LIT("AY"), {89, 90}},                                               \
    {                                                                        \
        LIT("AZ"), {90, 91}                                                  \
    }

#define HASH_TABLE_3_CI_KEYS(LIT)                                                            \
    {LIT("BA"), 1},     /* 0x3287 */                                                         \
        {LIT("bb"), 2}, /* 0x3288 */                                                         \
        {LIT("AA"), 65}, {LIT("AB"), 66}, {LIT("AC"), 67}, {LIT("AD"), 68}, {LIT("AE"), 69}, \
        {LIT("AF"), 70}, {LIT("AG"), 71}, {LIT("AH"), 72}, {LIT("AI"), 73}, {LIT("AJ"), 74}, \
        {LIT("AK"), 75}, {LIT("AL"), 76}, {LIT("AM"), 77}, {LIT("AN"), 78}, {LIT("AO"), 79}, \
        {LIT("AP"), 80}, {LIT("AQ"), 81}, {LIT("AR"), 82}, {LIT("AS"), 83}, {LIT("AT"), 84}, \
        {LIT("AU"), 85}, {LIT("AV"), 86}, {LIT("AW"), 87}, {LIT("AX"), 88}, {LIT("AY"), 89}, \
        {LIT("AZ"), 90}

extern "C" {
#define MAKE_LOOKUP_FUNCTION(suffix, A, CharT, char_tag, LIT)                              \
    [[gnu::visibility("default")]]                                                         \
    bool test_by_hash_table_3_##suffix##_##char_tag(size_t* dest, const CharT* key)        \
    {                                                                                      \
        using Value = std::pair<size_t, size_t>;                                           \
        constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(                                    \
            std::initializer_list<std::pair<const CharT*, Value>>{HASH_TABLE_3_KEYS(LIT)}, \
            rbox::string_key_fixed_map_options{                                            \
                .ascii_case_insensitive = false,                                           \
                .adjusts_alignment = A,                                                    \
                .min_load_factor = 0.25,                                                   \
            });                                                                            \
                                                                                           \
        if (auto opt = map.find(key); opt) {                                               \
            dest[0] = opt->first;                                                          \
            dest[1] = opt->second;                                                         \
            return true;                                                                   \
        }                                                                                  \
        return false;                                                                      \
    }

#define MAKE_CI_LOOKUP_FUNCTION(CharT, char_tag, LIT)                                       \
    [[gnu::visibility("default")]]                                                          \
    bool test_by_hash_table_3_ci_##char_tag(size_t* dest, const CharT* key)                 \
    {                                                                                       \
        constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(                                     \
            std::initializer_list<std::pair<const CharT*, int>>{HASH_TABLE_3_CI_KEYS(LIT)}, \
            rbox::string_key_fixed_map_options{                                             \
                .ascii_case_insensitive = true,                                             \
                .min_load_factor = 0.25,                                                    \
            });                                                                             \
                                                                                            \
        if (auto opt = map.find(key); opt) {                                                \
            *dest = *opt;                                                                   \
            return true;                                                                    \
        }                                                                                   \
        return false;                                                                       \
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
