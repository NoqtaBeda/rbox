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

// Hash collision pairs (same as in the test); only first element of each pair is used as key
#define HASH_COLLISION_KEYS(LIT)                                                  \
    {LIT("_wSYZDRpiQJf8Rfv"), {0, 1}}, {LIT("_lUwq4JUvMNVZ2Kc"), {1, 2}},         \
        {LIT("_EQJUIE23PenIm2I"), {2, 3}}, {LIT("_Kwg43nspyUx82Hg"), {3, 4}},     \
        {LIT("_zdYtgKeY1l7CFyd"), {4, 5}}, {LIT("_2P2H907ksk6vQFW"), {5, 6}},     \
        {LIT("_iN42c6SyFQJqSC4"), {6, 7}}, {LIT("_gDcf6PmY8L7E2Y3"), {7, 8}},     \
        {LIT("_EWNizOU0M0tF2Kj"), {8, 9}}, {LIT("_AyshlQKfxmMdGE4"), {9, 10}},    \
        {LIT("_7SwO68tn5JnfpWe"), {10, 11}}, {LIT("_HucDVkbRnhdAFDN"), {11, 12}}, \
        {LIT("_4KGSiDd_WBUdLEg"), {12, 13}}, {LIT("_ysndSaMOUqxeqEt"), {13, 14}}, \
        {LIT("_MV7jDdjEkL7C_mE"), {14, 15}}, {LIT("_Qyfre0rOlxRvRWf"), {15, 16}}, \
        {LIT("_XQUd5ANFbrdOirf"), {16, 17}}, {LIT("_RhPAaEW2gV0iA2v"), {17, 18}}, \
        {LIT("_jc9Nad159YjNyn6"), {18, 19}}, {LIT("_aedZVfoeGRrSCuZ"), {19, 20}}, \
        {LIT("_cpedvFH0008U_Sz"), {20, 21}},                                      \
    {                                                                             \
        LIT("_ALPOBd813kHWVxu"), {21, 22}                                         \
    }

extern "C" {
#define MAKE_LOOKUP_FUNCTION(suffix, A, BT, CharT, char_tag, LIT)                            \
    [[gnu::visibility("default")]]                                                           \
    bool test_by_hash_search_3_##suffix##_##char_tag(size_t* dest, const CharT* key)         \
    {                                                                                        \
        using Value = std::pair<size_t, size_t>;                                             \
        constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(                                      \
            std::initializer_list<std::pair<const CharT*, Value>>{HASH_COLLISION_KEYS(LIT)}, \
            rbox::string_key_fixed_map_options{                                              \
                .ascii_case_insensitive = false,                                             \
                .adjusts_alignment = A,                                                      \
                .max_n_iterations = 0, /* Disables hash table */                             \
                .binary_search_threshold = BT,                                               \
            });                                                                              \
                                                                                             \
        if (auto opt = map.find(key); opt) {                                                 \
            dest[0] = opt->first;                                                            \
            dest[1] = opt->second;                                                           \
            return true;                                                                     \
        }                                                                                    \
        return false;                                                                        \
    }

#define RBOX_BM_LIT_char(x) x
#define RBOX_BM_LIT_wchar_t(x) L##x
#define RBOX_BM_LIT_char8_t(x) u8##x
#define RBOX_BM_LIT_char16_t(x) u##x
#define RBOX_BM_LIT_char32_t(x) U##x

#define MAKE_LOOKUP_VARIANTS(CharT, char_tag)                                           \
    MAKE_LOOKUP_FUNCTION(_linear, false, 1000, CharT, char_tag, RBOX_BM_LIT_##char_tag) \
    MAKE_LOOKUP_FUNCTION(_binary, false, 4, CharT, char_tag, RBOX_BM_LIT_##char_tag)    \
    MAKE_LOOKUP_FUNCTION(_binary_a, true, 4, CharT, char_tag, RBOX_BM_LIT_##char_tag)

MAKE_LOOKUP_VARIANTS(char, char)
MAKE_LOOKUP_VARIANTS(wchar_t, wchar_t)
MAKE_LOOKUP_VARIANTS(char8_t, char8_t)
MAKE_LOOKUP_VARIANTS(char16_t, char16_t)
MAKE_LOOKUP_VARIANTS(char32_t, char32_t)
}  // extern "C"
