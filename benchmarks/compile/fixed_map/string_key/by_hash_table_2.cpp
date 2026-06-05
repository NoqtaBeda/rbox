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
#define MAKE_LOOKUP_FUNCTION(CharT, char_tag, LIT)                  \
    [[gnu::visibility("default")]]                                  \
    int test_by_hash_table_2_##char_tag(const CharT* key)           \
    {                                                               \
        constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(             \
            std::initializer_list<std::pair<const CharT*, int>>{    \
                {LIT("Apple"), 0},      /* 0x0000'0004'841a'c9fa */ \
                {LIT("Banana"), 1},     /* 0x0000'0257'8da4'8005 */ \
                {LIT("Cat"), 2},        /* 0x0000'0000'0011'bd72 */ \
                {LIT("Dog"), 3},        /* 0x0000'0000'0012'0798 */ \
                {LIT("Horse"), 4},      /* 0x0000'0004'fed9'023d */ \
                {LIT("Rabbit"), 5},     /* 0x0000'02e7'444d'd47e */ \
                {LIT("Donkey"), 6},     /* 0x0000'026a'7a6f'a930 */ \
                {LIT("Sheep"), 7},      /* 0x0000'0005'befc'49e7 */ \
                {LIT("Pineapple"), 8},  /* 0x6143'2afc'63e3'd68a */ \
                {LIT("Watermelon"), 9}, /* 0x0b85'1508'69f4'1520 */ \
                {LIT("Capybara"), 10},  /* 0x009f'595e'1a65'd529 */ \
                {LIT("ABCDEF"), 11},    /* 0x0000'024c'6c2c'6a9f */ \
                {LIT("ABCDEV"), 12},    /* 0x0000'024c'6c2c'6aaf */ \
            },                                                      \
            rbox::string_key_fixed_map_options{                     \
                .ascii_case_insensitive = false,                    \
                .min_load_factor = 0.25,                            \
            });                                                     \
                                                                    \
        if (auto opt = map.find(key); opt) {                        \
            return *opt;                                            \
        }                                                           \
        return -1;                                                  \
    }

#define MAKE_CI_LOOKUP_FUNCTION(CharT, char_tag, LIT)               \
    [[gnu::visibility("default")]]                                  \
    int test_by_hash_table_2_ci_##char_tag(const CharT* key)        \
    {                                                               \
        constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(             \
            std::initializer_list<std::pair<const CharT*, int>>{    \
                {LIT("Apple"), 0},      /* 0x0000'0006'b5d1'941a */ \
                {LIT("BANANA"), 1},     /* 0x0000'0376'fe2d'ee65 */ \
                {LIT("cat"), 2},        /* 0x0000'0000'001a'1e92 */ \
                {LIT("dog"), 3},        /* 0x0000'0000'001a'68b8 */ \
                {LIT("HoRsE"), 4},      /* 0x0000'0007'308f'cc5d */ \
                {LIT("RaBBiT"), 5},     /* 0x0000'0406'b4d7'42de */ \
                {LIT("Donkey"), 6},     /* 0x0000'0389'eaf9'1790 */ \
                {LIT("shEEp"), 7},      /* 0x0000'0007'f0b3'1407 */ \
                {LIT("PineApple"), 8},  /* 0x87c7'401f'860c'8aaa */ \
                {LIT("WaterMelon"), 9}, /* 0xc11b'e602'e4c8'4180 */ \
                {LIT("cApYbArA"), 10},  /* 0x00ea'9ded'071d'd689 */ \
                {LIT("ABCDEF"), 11},    /* 0x0000'036e'12be'c81f */ \
                {LIT("ABCDEV"), 12},    /* 0x0000'036e'12be'c82f */ \
            },                                                      \
            rbox::string_key_fixed_map_options{                     \
                .ascii_case_insensitive = true,                     \
                .min_load_factor = 0.25,                            \
            });                                                     \
                                                                    \
        if (auto opt = map.find(key); opt) {                        \
            return *opt;                                            \
        }                                                           \
        return -1;                                                  \
    }

#define RBOX_BM_LIT_char(x) x
#define RBOX_BM_LIT_wchar_t(x) L##x
#define RBOX_BM_LIT_char8_t(x) u8##x
#define RBOX_BM_LIT_char16_t(x) u##x
#define RBOX_BM_LIT_char32_t(x) U##x

#define MAKE_LOOKUP_VARIANTS(CharT, char_tag)                     \
    MAKE_LOOKUP_FUNCTION(CharT, char_tag, RBOX_BM_LIT_##char_tag) \
    MAKE_CI_LOOKUP_FUNCTION(CharT, char_tag, RBOX_BM_LIT_##char_tag)

MAKE_LOOKUP_VARIANTS(char, char)
MAKE_LOOKUP_VARIANTS(wchar_t, wchar_t)
MAKE_LOOKUP_VARIANTS(char8_t, char8_t)
MAKE_LOOKUP_VARIANTS(char16_t, char16_t)
MAKE_LOOKUP_VARIANTS(char32_t, char32_t)
}  // extern "C"
