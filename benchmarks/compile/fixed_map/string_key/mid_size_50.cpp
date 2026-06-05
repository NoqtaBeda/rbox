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
int test_mid_size_50_char(const char* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char*, int>>{
            {"nGD", 0},
            {"xdN", 1},
            {"OQNE", 2},
            {"QFba", 3},
            {"IJAa", 4},
            {"bHUrI", 5},
            {"sbGSR", 6},
            {"BZlgg", 7},
            {"HjpmNH", 8},
            {"qwrYzf", 9},
            {"xzDKpS", 10},
            {"otRfPPW", 11},
            {"cfzyyJE", 12},
            {"dOaSkfFB", 13},
            {"PYvKgHco", 14},
            {"nSKErceR", 15},
            {"rIKQclubn", 16},
            {"LjWTzetkK", 17},
            {"pKyRIvyWV", 18},
            {"iYSUfGVwdg", 19},
            {"BoevMXNMzX", 20},
            {"ubOrYCFowJ", 21},
            {"yBlRLQyfXNs", 22},
            {"ZpTefrjyTYO", 23},
            {"jVyuxgfatCx", 24},
            {"WrgiflBCJJGA", 25},
            {"gbfwJfMMYuya", 26},
            {"sAyXfUJpKHkR", 27},
            {"ykirtrFjekBrA", 28},
            {"tEYexqpUOFMNm", 29},
            {"DgitazvNyZvCv", 30},
            {"BPMituMSmEulzu", 31},
            {"sVSOFKYpuyrYzx", 32},
            {"hKmLIlRXJbUDTn", 33},
            {"CZsSeYYARFiOtpq", 34},
            {"QjTBYyeCMELzIGS", 35},
            {"AIcYxSZIMOfgXpQ", 36},
            {"QwkPNcKPRPzWvYBg", 37},
            {"agqoGVHJLSKoCxzD", 38},
            {"XRLSGjwbEgsAfhUjw", 39},
            {"tvDYnHEwEgCUSCuet", 40},
            {"cZThbvPgRZkVpHlJk", 41},
            {"vJBDoZzOllOQBzbVNm", 42},
            {"CLByaTnnrWTZYeKgZI", 43},
            {"lxumDhqQFHOuMyNzLh", 44},
            {"wwDNlRTZtNLfRiuhpth", 45},
            {"lxSjGyAMiKyBlFOISPl", 46},
            {"JkFsiluCNdwaFimyJGP", 47},
            {"FARFATCFkfKbYWoscroI", 48},
            {"skXDKVXXFJGhKhrXIxIW", 49}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_50_ci_char(const char* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char*, int>>{
            {"byx", 0},
            {"org", 1},
            {"ndxu", 2},
            {"yhjb", 3},
            {"oilc", 4},
            {"odyzh", 5},
            {"gzxsw", 6},
            {"lwtun", 7},
            {"ftezgg", 8},
            {"zbslri", 9},
            {"trfkwj", 10},
            {"jsiqvde", 11},
            {"yznbiue", 12},
            {"wehewkhy", 13},
            {"vmpesuiu", 14},
            {"nmocuzyc", 15},
            {"mqxiwlopk", 16},
            {"sayxcxouv", 17},
            {"wlczrmgng", 18},
            {"pikjkrsesp", 19},
            {"zkvybbduzo", 20},
            {"adfooangwe", 21},
            {"ujficuliclv", 22},
            {"ufbmujxwyhn", 23},
            {"ucwdagpcesh", 24},
            {"qvoaawkdnwep", 25},
            {"chmcxzddklje", 26},
            {"myyeuvecqsat", 27},
            {"ufolxguxentvo", 28},
            {"gcdeydsxmlnkz", 29},
            {"ehiuchrttxtjy", 30},
            {"wavjgqtqgxmjub", 31},
            {"zzhpmdhputcqal", 32},
            {"kemsnlrvfypyca", 33},
            {"scaigbbzmqjuwqy", 34},
            {"nnwmcurrteicjcq", 35},
            {"gzerkmsuyuvucjw", 36},
            {"nxhbhcndottbjvxv", 37},
            {"xfdawewafplqqzxi", 38},
            {"flexyixdyakzniqci", 39},
            {"wsucpoqlbpsflfiyd", 40},
            {"svxdhxqabahbplmef", 41},
            {"brzwxunhkhnxkicsld", 42},
            {"qvbfhzqbmcojyjkcro", 43},
            {"algjszjxtholspyygx", 44},
            {"ryzheanahrluwakayvm", 45},
            {"xxjdgqhnpbewicbhqnw", 46},
            {"loxcsdqeumcssbnvehj", 47},
            {"ikzmwxkkoihcgeyysded", 48},
            {"fookndrlygojoidcfzvj", 49}
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
int test_mid_size_50_wchar_t(const wchar_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const wchar_t*, int>>{
            {L"TSM", 0},
            {L"cnu", 1},
            {L"jfTp", 2},
            {L"wzGd", 3},
            {L"Rtql", 4},
            {L"bzCJV", 5},
            {L"JpgDg", 6},
            {L"ZYiha", 7},
            {L"dXoimz", 8},
            {L"xROPfL", 9},
            {L"Lqebem", 10},
            {L"PCZifZv", 11},
            {L"hcDdkKB", 12},
            {L"UzFbyRBl", 13},
            {L"wnlrrCjc", 14},
            {L"NNNpPsFA", 15},
            {L"JEdfryiAm", 16},
            {L"PZHpOZIYb", 17},
            {L"yYTwEIYFw", 18},
            {L"KGuyrlbuMo", 19},
            {L"bZXrdZEHwX", 20},
            {L"LokgpQprIY", 21},
            {L"VdWoKZywllp", 22},
            {L"LuZVSwLbTSw", 23},
            {L"KKjKmZFItlF", 24},
            {L"cfdoMobHEavN", 25},
            {L"mZivTlZudbjL", 26},
            {L"TjXhHxexTQzL", 27},
            {L"gvtuikUBYOFPu", 28},
            {L"lTJSNYwoQLlyt", 29},
            {L"USsilUaSKzXKc", 30},
            {L"lMuZNoOKgFjvVe", 31},
            {L"pwukOfTQOUvCaq", 32},
            {L"npSewqYgUadyCU", 33},
            {L"AkAFywIygEYKPWR", 34},
            {L"okCeZcsbuqgevky", 35},
            {L"kUeJfvMNETbBPkM", 36},
            {L"BkdgvnmASJUUIqQs", 37},
            {L"tpgdzKJFjdxaBfsQ", 38},
            {L"OMEmgbnkOsfEhtYzE", 39},
            {L"FQqfPIylxyxlCcqCD", 40},
            {L"qorKdjWSQgfQwZIAW", 41},
            {L"LoJdyHAIREKpEtfzTc", 42},
            {L"GKHZKRNjhWClknZmic", 43},
            {L"BfRBmOjMquUZefyJzJ", 44},
            {L"urHDaYSNLHYAhAjjKLL", 45},
            {L"WggKXgYsIwAqyPFKMEc", 46},
            {L"lrzjNMRSzczvSpdVErx", 47},
            {L"bvYttrFSRgoitVCXurUA", 48},
            {L"MPfmCnAVFWHxdGYketTG", 49}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_50_ci_wchar_t(const wchar_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const wchar_t*, int>>{
            {L"mey", 0},
            {L"qsa", 1},
            {L"fggb", 2},
            {L"hbob", 3},
            {L"lwgi", 4},
            {L"loqmu", 5},
            {L"dvahl", 6},
            {L"ptofp", 7},
            {L"srllfi", 8},
            {L"xwcjam", 9},
            {L"bfsygh", 10},
            {L"uhvgiun", 11},
            {L"qayyvap", 12},
            {L"euftzahi", 13},
            {L"tzjwwuin", 14},
            {L"mloigojv", 15},
            {L"qtmsdaqvl", 16},
            {L"ruttjjvdp", 17},
            {L"ckiukiiuw", 18},
            {L"ujgeqyhbzz", 19},
            {L"ztmvkvexau", 20},
            {L"pjinmmxbsw", 21},
            {L"sgkxwhvruzp", 22},
            {L"vlqjfvrfjdp", 23},
            {L"exixzryfvku", 24},
            {L"chlhxjnzklis", 25},
            {L"jodpbusstcpg", 26},
            {L"qdvmqjnbeegk", 27},
            {L"nsoekwfycpkuf", 28},
            {L"kubaoigyyfsyf", 29},
            {L"pzyycetnunmnp", 30},
            {L"mabrgxlakyqgav", 31},
            {L"auxhhwljedmqsj", 32},
            {L"fcbjjoxqtqknve", 33},
            {L"kplygfzmahhxegx", 34},
            {L"asqfdlwuzbmuiyr", 35},
            {L"tbtbvduabwdnomd", 36},
            {L"ripwegwvuajnvdvq", 37},
            {L"ittwendqxtdjdzdp", 38},
            {L"gtgyiqglwnjfbrpgw", 39},
            {L"pkhaavcdsvpecyqcx", 40},
            {L"xdiuhojiobdfbjlzv", 41},
            {L"knxddybaevufklotix", 42},
            {L"clkfdmmoimwpznuzfd", 43},
            {L"ewxbfdnspzsvoftmyl", 44},
            {L"tavxeppdnobciukawvq", 45},
            {L"yxsxshvkqqwwtdnvxhp", 46},
            {L"lvvumertbaufqppfcyp", 47},
            {L"khkibqhrummhcoozsocp", 48},
            {L"okdpxuadmnbrraczttuj", 49}
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
int test_mid_size_50_char8_t(const char8_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char8_t*, int>>{
            {u8"GJn", 0},
            {u8"PQD", 1},
            {u8"vxdo", 2},
            {u8"DvJX", 3},
            {u8"NMET", 4},
            {u8"QxTCh", 5},
            {u8"gWQoa", 6},
            {u8"vwOZs", 7},
            {u8"JHxSgc", 8},
            {u8"kFiTVU", 9},
            {u8"AgqNmm", 10},
            {u8"hznDYmT", 11},
            {u8"vgWAcOR", 12},
            {u8"LhXCDQLG", 13},
            {u8"iFaHVdJB", 14},
            {u8"LEGXlSLl", 15},
            {u8"UigyQMMuG", 16},
            {u8"yAMSWprzv", 17},
            {u8"sCZiiAMSV", 18},
            {u8"GtSJuTVJOn", 19},
            {u8"mnNTsRwRiT", 20},
            {u8"PlGISOuThW", 21},
            {u8"wJELKQTARVI", 22},
            {u8"sBZaHgbyjdQ", 23},
            {u8"dmrWYksRqjd", 24},
            {u8"SYsnWIcwCgNR", 25},
            {u8"VJoVPJypGYYZ", 26},
            {u8"SsSQdyyAYRuJ", 27},
            {u8"daVqmNXCoOTTP", 28},
            {u8"xWLIVMmXUmsCl", 29},
            {u8"RellVGhycBrJq", 30},
            {u8"ikLqavDTjcjuMd", 31},
            {u8"XONQtFYKJweYTu", 32},
            {u8"HolHeYGkAIIzfw", 33},
            {u8"onQvvxsnWNHEJWP", 34},
            {u8"ahQwCpPNNpcRuyY", 35},
            {u8"hyqIUsbHXxGZGCF", 36},
            {u8"csPmuGfgkXIIaOen", 37},
            {u8"QOXnRBgnISVXBPeV", 38},
            {u8"RjbDTvcfedlYqJeKo", 39},
            {u8"qAyCOzBubyRhIaPUN", 40},
            {u8"eWVLcSewGgsYRtMfs", 41},
            {u8"WCyzQbEkIoiVzYZIsO", 42},
            {u8"jtRYUPxaJJjhcaKMzI", 43},
            {u8"JftnVVUwnAPGjkloNq", 44},
            {u8"mhlQZKdWJDPJesQeqgm", 45},
            {u8"ULFvwiQPpgsNemuFCvN", 46},
            {u8"QtSLjKKxZuBkaupYoTV", 47},
            {u8"PBrxiRUvEDCwXtFJglPM", 48},
            {u8"friImqUOeUebGObLLzXL", 49}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_50_ci_char8_t(const char8_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char8_t*, int>>{
            {u8"gbr", 0},
            {u8"irr", 1},
            {u8"puye", 2},
            {u8"lmhy", 3},
            {u8"sxje", 4},
            {u8"oqqcm", 5},
            {u8"lqavh", 6},
            {u8"ztvfy", 7},
            {u8"qeoffs", 8},
            {u8"svexxp", 9},
            {u8"lbhyph", 10},
            {u8"cilhbvg", 11},
            {u8"qzlmpob", 12},
            {u8"bukdwqui", 13},
            {u8"xisrsfmu", 14},
            {u8"mlsucqim", 15},
            {u8"hqmllpzpt", 16},
            {u8"aqeofhcqu", 17},
            {u8"igeffltwd", 18},
            {u8"vhnzkxdpvp", 19},
            {u8"pgsfunahbe", 20},
            {u8"ssfeuqbseb", 21},
            {u8"fifqmtsuaxx", 22},
            {u8"vjcgotppfhn", 23},
            {u8"tezsyftvqif", 24},
            {u8"xvkotschmmed", 25},
            {u8"agqqvnetdfpt", 26},
            {u8"tbzrqydcpvet", 27},
            {u8"rayvunqnlbwqn", 28},
            {u8"hzpzymlszxyws", 29},
            {u8"dneipyhcjtrni", 30},
            {u8"gawxzasqdqamug", 31},
            {u8"wzknldenusiirn", 32},
            {u8"sltivmygnrzrpf", 33},
            {u8"lrrpijslywmxevd", 34},
            {u8"xhozwfdgqmzkzrt", 35},
            {u8"wvwbfmuazceptnc", 36},
            {u8"cihlctbxmmtuqjwt", 37},
            {u8"byfupoagvzqjgbzt", 38},
            {u8"qqwhtfvblhuafciym", 39},
            {u8"lvmmsgqaxwccrjxrs", 40},
            {u8"llihtgomsaifnqdrc", 41},
            {u8"qjkdzqirzsiwomspyh", 42},
            {u8"ykyovosapdicmuzhvw", 43},
            {u8"hzknlqrawihjcmvxkc", 44},
            {u8"rfpklgsifxvgpxghxhi", 45},
            {u8"ghghtleyddcvvptayrb", 46},
            {u8"pltkpsipfyqimhysebq", 47},
            {u8"znprbqlmvyfodwqokwaa", 48},
            {u8"vgmsdkltifrihiptklfp", 49}
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
int test_mid_size_50_char16_t(const char16_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char16_t*, int>>{
            {u"BnA", 0},
            {u"SWx", 1},
            {u"xzrC", 2},
            {u"lKiK", 3},
            {u"RogU", 4},
            {u"rZpJB", 5},
            {u"ymjZV", 6},
            {u"jEZal", 7},
            {u"AFiOHI", 8},
            {u"UuYFtq", 9},
            {u"JKdzgS", 10},
            {u"kdnqEOg", 11},
            {u"CVvqxxJ", 12},
            {u"JZqwbAhw", 13},
            {u"RMmKKPPI", 14},
            {u"lXswuRGF", 15},
            {u"XVCfyNsDO", 16},
            {u"jkOZvAONK", 17},
            {u"zefkvouuP", 18},
            {u"sTrMPzrCXx", 19},
            {u"VLHCAklbiU", 20},
            {u"pQqVSVfnkz", 21},
            {u"ggeEJdbXzHW", 22},
            {u"fgqNjfPytop", 23},
            {u"VsCOiiHkbcQ", 24},
            {u"wuEDIrJCiSIl", 25},
            {u"LMDOVOKBHOOI", 26},
            {u"twEIoZfCtxAq", 27},
            {u"jtaaHWiwsEIaS", 28},
            {u"FWEtaXBZsMoYa", 29},
            {u"JxmAJBzpkTVPy", 30},
            {u"yoqfBOpHWZNZSR", 31},
            {u"rsPVIZrAmelish", 32},
            {u"ODNCrSNFmubjdI", 33},
            {u"blgsriTCUaofjaF", 34},
            {u"nvAtYZFxDSYcvfW", 35},
            {u"jdrXzeIElmqyahX", 36},
            {u"TpyXCpVcmDVXgHQn", 37},
            {u"EyRtukOUcVJHhqEO", 38},
            {u"tmxdQnhpOBuakYrLh", 39},
            {u"zYpacLYSESMjxXfpk", 40},
            {u"FfRNvWOddwPjSIYgi", 41},
            {u"FouJYPWwZCUmbSywUi", 42},
            {u"qcFTCJsISFmKlIwuUD", 43},
            {u"rMJkeHjCsqIxDaXOGy", 44},
            {u"UlbNxXNpuUeQIymHRFX", 45},
            {u"JjNsuAPuhgHQecilcmg", 46},
            {u"FmgcKPyQFHNSLqdxoMT", 47},
            {u"eanPFfDZOdxJVAxYTdNr", 48},
            {u"HcUetenHggUoIymmHQpK", 49}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_50_ci_char16_t(const char16_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char16_t*, int>>{
            {u"ufr", 0},
            {u"xrn", 1},
            {u"xdyu", 2},
            {u"slvo", 3},
            {u"wvnd", 4},
            {u"fzfrm", 5},
            {u"qkvfr", 6},
            {u"tolkr", 7},
            {u"klnyun", 8},
            {u"xkthsr", 9},
            {u"gmyrxi", 10},
            {u"loomaxb", 11},
            {u"jtaqeja", 12},
            {u"jhgkaywi", 13},
            {u"rqrssedb", 14},
            {u"oleeilsh", 15},
            {u"hizjcsmsg", 16},
            {u"xcxkzpfpw", 17},
            {u"wsdtujbxz", 18},
            {u"smcybyklnn", 19},
            {u"dyhxutauyu", 20},
            {u"mbjqbkrsek", 21},
            {u"qwhebvxmoyt", 22},
            {u"sqchnjzmeek", 23},
            {u"cpsxbrmicmq", 24},
            {u"nyweyeprgpmu", 25},
            {u"sgrtxqhhciyn", 26},
            {u"kvscdvnnllgk", 27},
            {u"ltuayjfltzote", 28},
            {u"obhgvleyxcdxb", 29},
            {u"htejakwzdjopa", 30},
            {u"kgxrggrqyirptx", 31},
            {u"ewwvamsvqbifbj", 32},
            {u"gzwtibouacmviv", 33},
            {u"vldajumhmqwdkpt", 34},
            {u"rcfgqorwabhqjkp", 35},
            {u"mxfalvllujhqizb", 36},
            {u"uimarihekmvecxmq", 37},
            {u"vtuvhnhsenjtkore", 38},
            {u"guesrvzgejwwqenye", 39},
            {u"usfkcrrxdfkexaykn", 40},
            {u"njoxuxieygdefaytt", 41},
            {u"itsththhvalcwypeqt", 42},
            {u"pwulfmppefghbnyain", 43},
            {u"hxvgygcfotpwkohnmy", 44},
            {u"btxmxkmtxstrquwzbqd", 45},
            {u"oobhqrmxbrmqyubgjom", 46},
            {u"jyjofnjtbwqeouocxqd", 47},
            {u"fyswremnkoafoswgocik", 48},
            {u"wuwzfwxdcdwoulcqwmxz", 49}
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
int test_mid_size_50_char32_t(const char32_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char32_t*, int>>{
            {U"GNk", 0},
            {U"yJy", 1},
            {U"cXkv", 2},
            {U"KQjk", 3},
            {U"jlXT", 4},
            {U"dAttU", 5},
            {U"XCsOl", 6},
            {U"himaN", 7},
            {U"WqaDFF", 8},
            {U"IZaMFp", 9},
            {U"nLQEDA", 10},
            {U"CfMMapJ", 11},
            {U"rNOJndl", 12},
            {U"jdPwcjcQ", 13},
            {U"KMtvfdgA", 14},
            {U"lkRsNQSS", 15},
            {U"MKYJlDVLx", 16},
            {U"cfXtuxyeW", 17},
            {U"BJesEihSr", 18},
            {U"vHAHnSnNdg", 19},
            {U"KUOHfEUSMY", 20},
            {U"TsBMuqHKNw", 21},
            {U"iNKFHUOFFZl", 22},
            {U"NoTsmahbDOY", 23},
            {U"hVnZNAAcvwJ", 24},
            {U"ZOnaOmSsqYet", 25},
            {U"tGJuXahRvvzU", 26},
            {U"KNblmvvREEZc", 27},
            {U"PiEjODIvDYAVd", 28},
            {U"HtKURuJIbnKRv", 29},
            {U"ZYwejrbvyOIkK", 30},
            {U"MylMhYWtTuTcrA", 31},
            {U"fFpxCtnHtlhxYc", 32},
            {U"XmfCGbZEGjmvEU", 33},
            {U"HtXujXgHRinUcDy", 34},
            {U"THhatcRMfvDcgHX", 35},
            {U"QVtbKWtOnummsrI", 36},
            {U"uXCQhrjkrhaNJGgn", 37},
            {U"IwJurjTZsKpketNv", 38},
            {U"ICdibERdgydfBzlTM", 39},
            {U"LOSSCNwtvmTWQZIfW", 40},
            {U"WDKifZSDgDtRPTXDE", 41},
            {U"oojNqxzlSQvYuDFbeE", 42},
            {U"hwEDksXwMKiGgzTYgu", 43},
            {U"JPeYtIDzLApNpJkEye", 44},
            {U"vnSLBYBKYvISplQQeVT", 45},
            {U"KFhLMDuJgCltzeMgRMX", 46},
            {U"yuFPQdfniLWOZaWjQjC", 47},
            {U"IsyNKCmnRutKYRtBTwfL", 48},
            {U"aNGzisdxJdaTJwhsbpBS", 49}
        });

    if (auto opt = map.find(key); opt) {
        return *opt;
    }
    return -1;
}

[[gnu::visibility("default")]]
int test_mid_size_50_ci_char32_t(const char32_t* key)
{
    constexpr auto map = RBOX_STRING_KEY_FIXED_MAP(
        std::initializer_list<std::pair<const char32_t*, int>>{
            {U"smu", 0},
            {U"kwm", 1},
            {U"mmnr", 2},
            {U"gvfp", 3},
            {U"egrm", 4},
            {U"lnejv", 5},
            {U"eeacu", 6},
            {U"aexpv", 7},
            {U"neceuz", 8},
            {U"nozwtn", 9},
            {U"tfoqnr", 10},
            {U"omlhfnt", 11},
            {U"aspxhwq", 12},
            {U"inrslbbs", 13},
            {U"pwldspwc", 14},
            {U"exxqyyio", 15},
            {U"qcqtchpux", 16},
            {U"vvcvurzsj", 17},
            {U"tbkryapoq", 18},
            {U"psxknlzjjf", 19},
            {U"bwtxjxzsib", 20},
            {U"upsfsbvjzx", 21},
            {U"tajpceqczii", 22},
            {U"etiiiaxdtfs", 23},
            {U"xevolzfizwh", 24},
            {U"pfzqbkcetepy", 25},
            {U"nymhvoxufgsx", 26},
            {U"lbuiydbfompj", 27},
            {U"pxyrhmvwmpieq", 28},
            {U"najzomvkqvbdr", 29},
            {U"ulntuikiizdiu", 30},
            {U"smoyvxhxvsygqk", 31},
            {U"vwfkravacandqd", 32},
            {U"volhxjigbatcbt", 33},
            {U"vgtudwdctemvyus", 34},
            {U"zfdwfijovcuoiyn", 35},
            {U"wvyykifyciybmmk", 36},
            {U"biifocpgtmfupder", 37},
            {U"yngtudowidbccsxi", 38},
            {U"yoapeuuabvmiewkms", 39},
            {U"aqwyppnmxyzzcrham", 40},
            {U"wypdsrlkvjmbuuclr", 41},
            {U"kcoyyjugfrbmvgjwmd", 42},
            {U"uwlpcuoxgebgaxshpm", 43},
            {U"czrujprzoabkbkzxbm", 44},
            {U"dgzuvqovrexshcbxepu", 45},
            {U"aptbhwueafubjzdotfs", 46},
            {U"cddptzthczgnkzxvmkd", 47},
            {U"vsrekzgybsfkcdieynfw", 48},
            {U"lrfohsbedzutmyfapcaa", 49}
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

