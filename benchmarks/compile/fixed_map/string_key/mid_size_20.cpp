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
int test_mid_size_20_char(const char* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char*, int>>{
            {"rje", 0},
            {"dkYt", 1},
            {"MVKsC", 2},
            {"hDStSz", 3},
            {"rGIFCfM", 4},
            {"cBVuMqb", 5},
            {"foRKLbWR", 6},
            {"rKcWWlEHP", 7},
            {"CrlLBOFfEw", 8},
            {"AvuQgkvASFs", 9},
            {"QzWJcDfuquhX", 10},
            {"zGaQIDAdmHxNW", 11},
            {"FOCWdnrJisCSFh", 12},
            {"bOMZpTktJaJAfoh", 13},
            {"DhPjFTsGTrAEEpDJ", 14},
            {"jymMGVierXYAvYGr", 15},
            {"asUtLLQFjCIEwvJWI", 16},
            {"yDumSpKyoXAcuVETZy", 17},
            {"yQYPjFciGLvgCgHDaUj", 18},
            {"APjeEYqvNSzPfvRIyuOT", 19}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_20_ci_char(const char* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char*, int>>{
            {"ypr", 0},
            {"btch", 1},
            {"uvjhx", 2},
            {"cndyuw", 3},
            {"dofwjab", 4},
            {"kzbjlln", 5},
            {"ehqnsvzf", 6},
            {"ffctmtvhp", 7},
            {"sehouioiva", 8},
            {"zojvrfcolsj", 9},
            {"unwiojgmpdhm", 10},
            {"slsjwjavmiasv", 11},
            {"yxbtxpjyzhtzlh", 12},
            {"ugtivyxyvveuduu", 13},
            {"bjzobslxecjkxnfg", 14},
            {"ezrlqqifipzjxkzd", 15},
            {"oceyhvxvmzrlczmai", 16},
            {"rdolvxvismuldvhpat", 17},
            {"rkthucuowjundebbjpd", 18},
            {"dhremolvxwrnsxxenudp", 19}
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
int test_mid_size_20_wchar_t(const wchar_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const wchar_t*, int>>{
            {L"NAr", 0},
            {L"cSxn", 1},
            {L"CCpxg", 2},
            {L"RxIPwd", 3},
            {L"zrmhDfQ", 4},
            {L"nPOMbdY", 5},
            {L"vpiYKneW", 6},
            {L"JnLnovXjY", 7},
            {L"MarjiIqZlh", 8},
            {L"QbiawYYpLub", 9},
            {L"lqdiVAHhVeEC", 10},
            {L"XxGLgCGoNcUYQ", 11},
            {L"HtDPbdEzBRgFTC", 12},
            {L"efuMjeirNOLJTuy", 13},
            {L"MHsDGMBgYShPPXJU", 14},
            {L"nBCoAvDzAUguBuQq", 15},
            {L"xjREefffBgVVxZiJd", 16},
            {L"LJJvQhAwQWBUdsMtwg", 17},
            {L"KGnjQEogwJxhWrKoZBJ", 18},
            {L"XNNRPJbMQSrblrSWtvwa", 19}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_20_ci_wchar_t(const wchar_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const wchar_t*, int>>{
            {L"fes", 0},
            {L"vmce", 1},
            {L"xuacx", 2},
            {L"qgmnok", 3},
            {L"fljxkys", 4},
            {L"tcbefyt", 5},
            {L"bvciovnp", 6},
            {L"tonigyqdl", 7},
            {L"ndjvvspqvj", 8},
            {L"bhmtbagjgye", 9},
            {L"yijkdapxnfem", 10},
            {L"rwhqrvzlcmxbn", 11},
            {L"aocksnsmwunjdm", 12},
            {L"akfztowlcndhnsm", 13},
            {L"qcmjxkhkyfcqudqq", 14},
            {L"gyllxuehdeigftey", 15},
            {L"yucfyupoysysovsuu", 16},
            {L"tkukeocpoujzisblqc", 17},
            {L"joobbljcucttqmosrzx", 18},
            {L"bozsugktpqebodzkwcqu", 19}
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
int test_mid_size_20_char8_t(const char8_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char8_t*, int>>{
            {u8"lcp", 0},
            {u8"TCCH", 1},
            {u8"HNkxx", 2},
            {u8"syAXvR", 3},
            {u8"MdYOPve", 4},
            {u8"vgJRysq", 5},
            {u8"UQMjvfLQ", 6},
            {u8"jwtPSQziM", 7},
            {u8"TftJyPYviQ", 8},
            {u8"SVRHfPQBGxb", 9},
            {u8"xtlnvXFmoije", 10},
            {u8"sYgGXIVHcQvXN", 11},
            {u8"iMyjklSXNZkUCc", 12},
            {u8"AxRUpCNsWVYCoIp", 13},
            {u8"tZYEmxRKCDXsXyGH", 14},
            {u8"AkmZMiqdPExJgTHh", 15},
            {u8"sfWkrCGjBfoCwbAdz", 16},
            {u8"GxpyfxobugTPvYjics", 17},
            {u8"ESiWTECNafbqnjJUMHB", 18},
            {u8"hXspthdpAOYNDehFMIbO", 19}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_20_ci_char8_t(const char8_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char8_t*, int>>{
            {u8"qsh", 0},
            {u8"wejn", 1},
            {u8"atlhs", 2},
            {u8"nfvvcq", 3},
            {u8"lcqrqzq", 4},
            {u8"rampbum", 5},
            {u8"lixalzcl", 6},
            {u8"hxvudysxy", 7},
            {u8"keblrkufyv", 8},
            {u8"owpufzecwyo", 9},
            {u8"bjgbzgbkjqmr", 10},
            {u8"pibyugjlybuki", 11},
            {u8"dzlnmxomkfpwpl", 12},
            {u8"zqizcxncntfrjkd", 13},
            {u8"ckvjjotwnfwolobx", 14},
            {u8"ltniuzbcvumlqzxv", 15},
            {u8"faetvzobechyullms", 16},
            {u8"btevolloycseqlmkui", 17},
            {u8"hdaxfpqmrdiyiwogtjw", 18},
            {u8"pgdecofwoczvkvlwcrrj", 19}
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
int test_mid_size_20_char16_t(const char16_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char16_t*, int>>{
            {u"tkT", 0},
            {u"TSOl", 1},
            {u"YxGoh", 2},
            {u"mYipYF", 3},
            {u"bxJKxDZ", 4},
            {u"JiNfetz", 5},
            {u"TUEHAXAK", 6},
            {u"eiuPeCDRH", 7},
            {u"wiXJOLlXiB", 8},
            {u"GdhHjtkkuTo", 9},
            {u"wHsfqmOJriOt", 10},
            {u"NIfGPkLLjkQNU", 11},
            {u"MvKcbfcPXKqPnX", 12},
            {u"KANObFOIsPtEpZZ", 13},
            {u"RztDeSdkCAEDnvMj", 14},
            {u"uTuUwziWxGJgupDh", 15},
            {u"rCpjgdsyNApkuKUum", 16},
            {u"WkFGDFtFbfzGDpnLwd", 17},
            {u"dsFMPREsIagBiqUxWzx", 18},
            {u"czdKJmxJseyGCWJrNRNh", 19}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_20_ci_char16_t(const char16_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char16_t*, int>>{
            {u"edm", 0},
            {u"lzkr", 1},
            {u"lyegt", 2},
            {u"qmqbbb", 3},
            {u"ewkzpqo", 4},
            {u"etqektk", 5},
            {u"fmtxjskq", 6},
            {u"qrpwsjpal", 7},
            {u"kvdnsjzxwu", 8},
            {u"atpiuzysshx", 9},
            {u"bspfquxtymev", 10},
            {u"hbswdgaoknenw", 11},
            {u"gnqytpxxbweqgr", 12},
            {u"kvpqmkforkrlvyx", 13},
            {u"vuzwitpghirjhjyj", 14},
            {u"wgwwpkplrzxijdsv", 15},
            {u"rmmlyzejbjwclouix", 16},
            {u"pggrirwiedtxshhbvq", 17},
            {u"huhbdnkwpdvyearfnup", 18},
            {u"pugyjkjubycushrxxbfn", 19}
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
int test_mid_size_20_char32_t(const char32_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char32_t*, int>>{
            {U"lcz", 0},
            {U"YFlV", 1},
            {U"scatK", 2},
            {U"MgvsDP", 3},
            {U"IHFiGDr", 4},
            {U"mZhvkUD", 5},
            {U"PqTlaVvY", 6},
            {U"sKRWmlNOz", 7},
            {U"BGufzQgliE", 8},
            {U"upaqypCWrvt", 9},
            {U"LUKaqPxSpdQh", 10},
            {U"DtkzRGTXtShOs", 11},
            {U"xNooiEjDVMxASJ", 12},
            {U"EWIZQnWpRWMYfHC", 13},
            {U"HTxeKhdJGmKIjkuH", 14},
            {U"ChRnTLFfGCZdDiGA", 15},
            {U"DKdJDRZtUbzqaVnLe", 16},
            {U"cBwSeIdeEcsAlXiXPU", 17},
            {U"PAxyCyyfRQIiPwhlIzH", 18},
            {U"iUoaWbtDRUIBIyopDwjr", 19}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_20_ci_char32_t(const char32_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char32_t*, int>>{
            {U"gxy", 0},
            {U"dbzv", 1},
            {U"ntyah", 2},
            {U"gcdtbo", 3},
            {U"tvwbhxb", 4},
            {U"mohrgyy", 5},
            {U"beqjhxsk", 6},
            {U"styvkhjev", 7},
            {U"qhnjibrsxf", 8},
            {U"uvnrpbluvmz", 9},
            {U"qkwnnejmfyrp", 10},
            {U"hhjwezobrnnrq", 11},
            {U"emhigkucolcrxg", 12},
            {U"bimvttbcgzysxvr", 13},
            {U"gpgkjaggxdxyphwt", 14},
            {U"wgmhrkyjmoruljil", 15},
            {U"qpodzxpykglknbshx", 16},
            {U"eairssxnjegkhmshpr", 17},
            {U"uvkiypxuxpofxzlfexr", 18},
            {U"pfrubqbcvbyaneuhcwea", 19}
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

