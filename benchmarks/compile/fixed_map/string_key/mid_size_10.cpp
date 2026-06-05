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

[[gnu::visibility("default")]]
int test_mid_size_10_char(const char* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char*, int>>{
            {"Ohb", 0},
            {"Vrpoi", 1},
            {"VgRVIfL", 2},
            {"BcbfnoGMb", 3},
            {"JmTPSIAoCLr", 4},
            {"ZaWZkSBvrjnW", 5},
            {"vgfygwwMqZcUDI", 6},
            {"hyfJsONxKmTecQoX", 7},
            {"sfogyrDOxkxwnQrSRP", 8},
            {"eMOkIUpkDyrOSJoRuXXd", 9}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_10_ci_char(const char* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char*, int>>{
            {"hbz", 0},
            {"kmicg", 1},
            {"swkgupm", 2},
            {"uoeiehxrr", 3},
            {"ixsnsmlheqp", 4},
            {"cybdeufzvntc", 5},
            {"mmtoqiravxdvry", 6},
            {"iyukdjnfoaxxiqyf", 7},
            {"qdujuqtgelyfryqatk", 8},
            {"padlzjhbhsccxpcyryee", 9}
        },
        rbox::string_key_fixed_map_options{
            .ascii_case_insensitive = true,
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_10_wchar_t(const wchar_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const wchar_t*, int>>{
            {L"QEJ", 0},
            {L"kqHMB", 1},
            {L"nIWUSmT", 2},
            {L"tzQPxCHCh", 3},
            {L"poevbLJoLoa", 4},
            {L"eTOdoecveGpr", 5},
            {L"QFnIiUKKEpYEZA", 6},
            {L"mggQBwBADUdRPPgd", 7},
            {L"zUvZgpmmICiBlrDpeC", 8},
            {L"ZJgdPIafWpkAFEnzdkya", 9}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_10_ci_wchar_t(const wchar_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const wchar_t*, int>>{
            {L"miz", 0},
            {L"zojnw", 1},
            {L"xzrvwpe", 2},
            {L"gjgbsxrbx", 3},
            {L"kbbspqqfbqc", 4},
            {L"fctcvhmdshst", 5},
            {L"btcnvssqkigvwk", 6},
            {L"himevujokycaotsd", 7},
            {L"crgqielchljforwjtz", 8},
            {L"uqavrjvdeiddxreijtgw", 9}
        },
        rbox::string_key_fixed_map_options{
            .ascii_case_insensitive = true,
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_10_char8_t(const char8_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char8_t*, int>>{
            {u8"vnR", 0},
            {u8"OqGFq", 1},
            {u8"dfOBrca", 2},
            {u8"vXiOqkVCJ", 3},
            {u8"TBJaheSjIcx", 4},
            {u8"LJjBictxYcwn", 5},
            {u8"RpQgwXJANVjpkZ", 6},
            {u8"ZlAblVvYAZQVZprk", 7},
            {u8"YSgycEomDwtYoobQmz", 8},
            {u8"vreXrwPGzRIvbhqlLqcg", 9}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_10_ci_char8_t(const char8_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char8_t*, int>>{
            {u8"tnl", 0},
            {u8"xzknt", 1},
            {u8"qdmsgib", 2},
            {u8"wnaqzrvxx", 3},
            {u8"xvglncvktkv", 4},
            {u8"dxjqjvnkmwjr", 5},
            {u8"egnvmvxftsjmra", 6},
            {u8"jjgnzstukooovgqp", 7},
            {u8"zzxfvcjqvutkcyhvjh", 8},
            {u8"zgeabhptyconusgwwmpm", 9}
        },
        rbox::string_key_fixed_map_options{
            .ascii_case_insensitive = true,
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_10_char16_t(const char16_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char16_t*, int>>{
            {u"pjP", 0},
            {u"SaWXg", 1},
            {u"XBolZSH", 2},
            {u"DdJphDiZD", 3},
            {u"QHJMuWCNUGB", 4},
            {u"JCkVECqWpOrX", 5},
            {u"XHFOprCeTsprvu", 6},
            {u"IfijoySjTneAAvID", 7},
            {u"AdnAyXLSbWKyEawtWy", 8},
            {u"AIVVIZMoForBFbyvQRZz", 9}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_10_ci_char16_t(const char16_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char16_t*, int>>{
            {u"xfo", 0},
            {u"etram", 1},
            {u"ssvacun", 2},
            {u"eofbimkgo", 3},
            {u"kkymiynicpa", 4},
            {u"xrblhucyubya", 5},
            {u"hgatehepvdsgow", 6},
            {u"iylfttxwdyfjdsaj", 7},
            {u"svmmwgcswuhdwyjvtz", 8},
            {u"dzszblrnvlcqukanpdnl", 9}
        },
        rbox::string_key_fixed_map_options{
            .ascii_case_insensitive = true,
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_10_char32_t(const char32_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char32_t*, int>>{
            {U"ODT", 0},
            {U"jBlUH", 1},
            {U"PrNZIXE", 2},
            {U"DBULrupfr", 3},
            {U"CpWDKNQyvbF", 4},
            {U"ulFnwZqvrMSr", 5},
            {U"JaHmfpUAFJWpSE", 6},
            {U"PTFCYbfsozSptQLx", 7},
            {U"EJHwBVJvwSDrtqohUm", 8},
            {U"uhVIWSlmnVErULWHMsgm", 9}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_10_ci_char32_t(const char32_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char32_t*, int>>{
            {U"jhl", 0},
            {U"fjawr", 1},
            {U"eibbrjw", 2},
            {U"euypdasjp", 3},
            {U"pokfbipdcmp", 4},
            {U"csuvbeezsjch", 5},
            {U"drynttzthyqmoo", 6},
            {U"jsnjstbtxdygugiv", 7},
            {U"cfhfrcfanowtpjbhjw", 8},
            {U"jwocvhizzusvzgndrhue", 9}
        },
        rbox::string_key_fixed_map_options{
            .ascii_case_insensitive = true,
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

}  // extern "C"

