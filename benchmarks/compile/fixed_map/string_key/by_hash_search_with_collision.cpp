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

// 44 strings that all have the same hash value (BKDR hash collision)
#define HASH_COLLISION_KEYS(LIT)                                                \
    {LIT("0BCPElfPXEtMOUE"), {0, 0}}, {LIT("2P2H907ksk6vQFW"), {1, 1}},         \
        {LIT("4KGSiDd_WBUdLEg"), {2, 2}}, {LIT("6_Cm_AklFmYKc4S"), {3, 3}},     \
        {LIT("7SwO68tn5JnfpWe"), {4, 4}}, {LIT("ALPOBd813kHWVxu"), {5, 5}},     \
        {LIT("AyshlQKfxmMdGE4"), {6, 6}}, {LIT("Bvo3i5j5_ZFZsZc"), {7, 7}},     \
        {LIT("DTkO8hjT4NsMLxv"), {8, 8}}, {LIT("EK0OTLFzgRM6lDN"), {9, 9}},     \
        {LIT("EQJUIE23PenIm2I"), {10, 10}}, {LIT("EWNizOU0M0tF2Kj"), {11, 11}}, \
        {LIT("EbYwQ4i8J4uLwgA"), {12, 12}}, {LIT("HsMqQzGTXfJGVZw"), {13, 13}}, \
        {LIT("HucDVkbRnhdAFDN"), {14, 14}}, {LIT("ITD_KpPq2hDzrZT"), {15, 15}}, \
        {LIT("Kwg43nspyUx82Hg"), {16, 16}}, {LIT("MV7jDdjEkL7C_mE"), {17, 17}}, \
        {LIT("MkFJCq2VQyMywJf"), {18, 18}}, {LIT("Qyfre0rOlxRvRWf"), {19, 19}}, \
        {LIT("RhPAaEW2gV0iA2v"), {20, 20}}, {LIT("SuHWKjS3W6SeuEt"), {21, 21}}, \
        {LIT("VZN4nduCikBzm_h"), {22, 22}}, {LIT("XQUd5ANFbrdOirf"), {23, 23}}, \
        {LIT("_EnPg50lJUbrQcu"), {24, 24}}, {LIT("aedZVfoeGRrSCuZ"), {25, 25}}, \
        {LIT("cpedvFH0008U_Sz"), {26, 26}}, {LIT("cuFFJIHGp_jNJKS"), {27, 27}}, \
        {LIT("eEq531p7C604fY2"), {28, 28}}, {LIT("fs5vbC2dMMgh7HS"), {29, 29}}, \
        {LIT("gDcf6PmY8L7E2Y3"), {30, 30}}, {LIT("gJnJJ7jhR6pCClY"), {31, 31}}, \
        {LIT("iN42c6SyFQJqSC4"), {32, 32}}, {LIT("jc9Nad159YjNyn6"), {33, 33}}, \
        {LIT("lUwq4JUvMNVZ2Kc"), {34, 34}}, {LIT("ltDwvfM7t4q8Epd"), {35, 35}}, \
        {LIT("pBFHYjoPHfC7_SI"), {36, 36}}, {LIT("pnyyUTzU793Dx3C"), {37, 37}}, \
        {LIT("ua233bXVs05ligl"), {38, 38}}, {LIT("wSYZDRpiQJf8Rfv"), {39, 39}}, \
        {LIT("wlAsvdpajDERFnE"), {40, 40}}, {LIT("wo6UUYzXIiZphUo"), {41, 41}}, \
        {LIT("ysndSaMOUqxeqEt"), {42, 42}},                                     \
    {                                                                           \
        LIT("zdYtgKeY1l7CFyd"), {43, 43}                                        \
    }

extern "C" {
#define MAKE_LOOKUP_FUNCTION(suffix, AA, CharT, char_tag, LIT)                                    \
    [[gnu::visibility("default")]]                                                                \
    bool test_by_hash_search_with_collision_##suffix##_##char_tag(size_t* dest, const CharT* key) \
    {                                                                                             \
        using Value = std::pair<size_t, size_t>;                                                  \
        constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(                                           \
            std::initializer_list<std::pair<const CharT*, Value>>{HASH_COLLISION_KEYS(LIT)},      \
            rbox::string_key_fixed_map_options{                                                   \
                .ascii_case_insensitive = false,                                                  \
                .adjusts_alignment = AA,                                                          \
                .min_load_factor = 1e-10,                                                         \
                .binary_search_threshold = 100000,                                                \
            });                                                                                   \
                                                                                                  \
        if (auto opt = map.find(key); opt) {                                                      \
            dest[0] = opt->first;                                                                 \
            dest[1] = opt->second;                                                                \
            return true;                                                                          \
        }                                                                                         \
        return false;                                                                             \
    }

#define RBOX_BM_LIT_char(x) x
#define RBOX_BM_LIT_wchar_t(x) L##x
#define RBOX_BM_LIT_char8_t(x) u8##x
#define RBOX_BM_LIT_char16_t(x) u##x
#define RBOX_BM_LIT_char32_t(x) U##x

#define MAKE_LOOKUP_VARIANTS(CharT, char_tag)                                     \
    MAKE_LOOKUP_FUNCTION(_normal, false, CharT, char_tag, RBOX_BM_LIT_##char_tag) \
    MAKE_LOOKUP_FUNCTION(_aa, true, CharT, char_tag, RBOX_BM_LIT_##char_tag)

MAKE_LOOKUP_VARIANTS(char, char)
MAKE_LOOKUP_VARIANTS(wchar_t, wchar_t)
MAKE_LOOKUP_VARIANTS(char8_t, char8_t)
MAKE_LOOKUP_VARIANTS(char16_t, char16_t)
MAKE_LOOKUP_VARIANTS(char32_t, char32_t)
}  // extern "C"
