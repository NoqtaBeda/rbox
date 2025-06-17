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

#include <limits>
#include <rbox/utils/string_builder.hpp>

#include "tests/test_options.hpp"

TEST(UtilsStringBuilder, DefaultConstructor)
{
    rbox::string_builder builder;
    EXPECT_EQ(builder.size(), 0);
    EXPECT_EQ(builder.strview(), "");
}

TEST(UtilsStringBuilder, InitialSizeConstructor)
{
    rbox::string_builder builder(100);
    EXPECT_EQ(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendChar)
{
    rbox::string_builder builder;
    builder.append_char('a');
    builder.append_char('b');
    builder.append_char('c');
    EXPECT_EQ(builder.size(), 3);
    EXPECT_EQ(builder.strview(), "abc");
}

TEST(UtilsStringBuilder, AppendCharUtf8)
{
    rbox::u8string_builder builder;
    builder.append_char(u8'a');
    builder.append_char(u8'b');
    EXPECT_EQ(builder.size(), 2);
    EXPECT_EQ(builder.strview(), u8"ab");
}

TEST(UtilsStringBuilder, AppendCharUtf16)
{
    rbox::u16string_builder builder;
    builder.append_char(u'a');
    builder.append_char(u'b');
    EXPECT_EQ(builder.size(), 2);
    EXPECT_EQ(builder.strview(), u"ab");
}

TEST(UtilsStringBuilder, AppendUtfCodePointAscii)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point(U'A');
    builder.append_utf_code_point(U'B');
    builder.append_utf_code_point(U'C');
    EXPECT_EQ(builder.size(), 3);
    EXPECT_EQ(builder.strview(), u8"ABC");
}

TEST(UtilsStringBuilder, AppendUtfCodePointMultiByte)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point(U'\u20AC');  // Euro sign
    EXPECT_EQ(builder.size(), 3);
    EXPECT_EQ(builder.strview(), u8"\u20AC");
}

TEST(UtilsStringBuilder, AppendUtfCodePointEmoji)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point(U'\U0001F600');  // Grinning face
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), u8"\U0001F600");
}

TEST(UtilsStringBuilder, AppendUtfCodePointUtf16)
{
    rbox::u16string_builder builder;
    builder.append_utf_code_point(U'\u20AC');  // Euro sign
    EXPECT_EQ(builder.size(), 1);
    EXPECT_EQ(builder.strview(), u"\u20AC");
}

TEST(UtilsStringBuilder, AppendUtfCodePointUtf16Surrogate)
{
    rbox::u16string_builder builder;
    builder.append_utf_code_point(U'\U0001F600');  // Grinning face (surrogate pair)
    EXPECT_EQ(builder.size(), 2);
    EXPECT_EQ(builder.strview(), u"\U0001F600");
}

TEST(UtilsStringBuilder, AppendStringPtrWithEnd)
{
    rbox::string_builder builder;
    const char* str = "hello";
    builder.append_string(str, str + 5);
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), "hello");
}

TEST(UtilsStringBuilder, AppendStringNullTerminated)
{
    rbox::string_builder builder;
    builder.append_string("world");
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), "world");
}

TEST(UtilsStringBuilder, AppendStringView)
{
    rbox::string_builder builder;
    std::string_view sv = "test";
    builder.append_string(sv);
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), "test");
}

TEST(UtilsStringBuilder, AppendStringViewUtf8)
{
    rbox::u8string_builder builder;
    std::u8string_view sv = u8"hello";
    builder.append_string(sv);
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u8"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf8ToUtf8)
{
    rbox::u8string_builder builder;
    builder.append_string(u8"hello");
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u8"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf8ToUtf16)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(u8"hello");
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf16ToUtf8)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(u"hello");
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u8"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf16ToUtf16)
{
    rbox::u16string_builder builder;
    builder.append_string(u"hello");
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringWithInvalidSequence)
{
    rbox::u16string_builder builder;
    const char8_t invalid[] = u8"hello\xFF\xFEworld";
    builder.append_utf_string(invalid, invalid + sizeof(invalid) - 1);
    EXPECT_EQ(builder.strview(), u"hello\uFFFDworld");
}

TEST(UtilsStringBuilder, AppendUtfStringView)
{
    rbox::u8string_builder builder;
    std::u8string_view sv = u8"test";
    builder.append_string(sv);
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), u8"test");
}

TEST(UtilsStringBuilder, AppendUtfStringViewUtf16ToUtf8)
{
    rbox::u8string_builder builder;
    std::u16string_view sv = u"test";
    builder.append_utf_string(sv);
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), u8"test");
}

TEST(UtilsStringBuilder, AppendUtfStringMixed)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(u8"Hello, ");
    builder.append_string(u"世界!");
    EXPECT_EQ(builder.strview(), u"Hello, 世界!");
}

TEST(UtilsStringBuilder, AppendBoolTrue)
{
    rbox::string_builder builder;
    builder.append_bool(true);
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), "true");
}

TEST(UtilsStringBuilder, AppendBoolFalse)
{
    rbox::string_builder builder;
    builder.append_bool(false);
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), "false");
}

TEST(UtilsStringBuilder, AppendIntegerDecimal)
{
    rbox::string_builder builder;
    builder.append_integer(12345);
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), "12345");
}

TEST(UtilsStringBuilder, AppendIntegerNegative)
{
    rbox::string_builder builder;
    builder.append_integer(-6789);
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), "-6789");
}

TEST(UtilsStringBuilder, AppendIntegerHex)
{
    rbox::string_builder builder;
    builder.append_integer(255, 16);
    EXPECT_EQ(builder.size(), 2);
    EXPECT_EQ(builder.strview(), "ff");
}

TEST(UtilsStringBuilder, AppendIntegerOctal)
{
    rbox::string_builder builder;
    builder.append_integer(64, 8);
    EXPECT_EQ(builder.size(), 3);
    EXPECT_EQ(builder.strview(), "100");
}

TEST(UtilsStringBuilder, AppendIntegerBinary)
{
    rbox::string_builder builder;
    builder.append_integer(10, 2);
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), "1010");
}

TEST(UtilsStringBuilder, AppendIntegerUint8)
{
    rbox::string_builder builder;
    uint8_t val = 42;
    builder.append_integer(val);
    EXPECT_EQ(builder.strview(), "42");
}

TEST(UtilsStringBuilder, AppendIntegerInt8)
{
    rbox::string_builder builder;
    int8_t val = -10;
    builder.append_integer(val);
    EXPECT_EQ(builder.strview(), "-10");
}

TEST(UtilsStringBuilder, AppendIntegerInt64)
{
    rbox::string_builder builder;
    int64_t val = 9223372036854775807LL;
    builder.append_integer(val);
    EXPECT_EQ(builder.strview(), "9223372036854775807");
}

TEST(UtilsStringBuilder, AppendFloatingPointFloat)
{
    rbox::string_builder builder;
    builder.append_floating_point(3.14f);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find("3.14") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointDouble)
{
    rbox::string_builder builder;
    builder.append_floating_point(2.718281828);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find("2.718281828") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointLongDouble)
{
    rbox::string_builder builder;
    builder.append_floating_point(1.5L);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find("1.5") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointScientific)
{
    rbox::string_builder builder;
    builder.append_floating_point(1.23e10, std::chars_format::scientific);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find("1.23") != std::string_view::npos);
    EXPECT_TRUE(sv.find("e") != std::string_view::npos || sv.find("E") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendIntegerWithU8StringBuilder)
{
    rbox::u8string_builder builder;
    builder.append_integer(12345);
    EXPECT_EQ(builder.strview(), u8"12345");
}

TEST(UtilsStringBuilder, AppendIntegerWithU16StringBuilder)
{
    rbox::u16string_builder builder;
    builder.append_integer(12345);
    EXPECT_EQ(builder.strview(), u"12345");
}

TEST(UtilsStringBuilder, AppendIntegerWithU32StringBuilder)
{
    rbox::u32string_builder builder;
    builder.append_integer(12345);
    EXPECT_EQ(builder.size(), 5);
}

TEST(UtilsStringBuilder, AppendIntegerNegativeWithU8StringBuilder)
{
    rbox::u8string_builder builder;
    builder.append_integer(-6789);
    EXPECT_EQ(builder.strview(), u8"-6789");
}

TEST(UtilsStringBuilder, AppendIntegerHexWithU8StringBuilder)
{
    rbox::u8string_builder builder;
    builder.append_integer(255, 16);
    EXPECT_EQ(builder.strview(), u8"ff");
}

TEST(UtilsStringBuilder, AppendBoolWithU8StringBuilder)
{
    rbox::u8string_builder builder;
    builder.append_bool(true);
    EXPECT_EQ(builder.strview(), u8"true");
}

TEST(UtilsStringBuilder, AppendBoolWithU16StringBuilder)
{
    rbox::u16string_builder builder;
    builder.append_bool(false);
    EXPECT_EQ(builder.strview(), u"false");
}

TEST(UtilsStringBuilder, AppendFloatingPointWithU8StringBuilder)
{
    rbox::u8string_builder builder;
    builder.append_floating_point(3.14159f);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find(u8"3.14") != std::u8string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointWithU16StringBuilder)
{
    rbox::u16string_builder builder;
    builder.append_floating_point(2.718281828);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find(u"2.718") != std::u16string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointWithPrecisionFloat)
{
    rbox::string_builder builder;
    builder.append_floating_point(3.14159f, std::chars_format::fixed, 2);
    EXPECT_EQ(builder.strview(), "3.14");
}

TEST(UtilsStringBuilder, AppendFloatingPointWithPrecisionDouble)
{
    rbox::string_builder builder;
    builder.append_floating_point(3.14159265358979, std::chars_format::fixed, 4);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find("3.141") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointWithPrecisionDoubleScientific)
{
    rbox::string_builder builder;
    builder.append_floating_point(1.23456789e10, std::chars_format::scientific, 2);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find("1.23") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointWithPrecisionDoubleHex)
{
    rbox::string_builder builder;
    builder.append_floating_point(255.0, std::chars_format::hex, 2);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find("fep") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointWithPrecisionU8StringBuilder)
{
    rbox::u8string_builder builder;
    builder.append_floating_point(3.14159f, std::chars_format::fixed, 2);
    EXPECT_EQ(builder.strview(), u8"3.14");
}

TEST(UtilsStringBuilder, AppendFloatingPointWithPrecisionU16StringBuilder)
{
    rbox::u16string_builder builder;
    builder.append_floating_point(3.14159, std::chars_format::fixed, 3);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find(u"3.14") != std::u16string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointWithPrecisionU32StringBuilder)
{
    rbox::u32string_builder builder;
    builder.append_floating_point(3.14159L, std::chars_format::fixed, 4);
    EXPECT_EQ(builder.size(), 6);
}

TEST(UtilsStringBuilder, AppendFloatingPointWithHighPrecision)
{
    rbox::string_builder builder;
    builder.append_floating_point(3.14159265358979, std::chars_format::fixed, 30);
    auto sv = builder.strview();
    EXPECT_GT(sv.size(), 30);
    EXPECT_TRUE(sv.find("3.14") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointWithVeryHighPrecision)
{
    rbox::string_builder builder;
    const double pi = 3.14159265358979;
    builder.append_floating_point(pi, std::chars_format::fixed, 50);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.starts_with("3.1415926"));
    EXPECT_GT(sv.size(), 50);
}

TEST(UtilsStringBuilder, AppendFloatingPointWithExtremelyHighPrecision)
{
    rbox::string_builder builder(256);
    const double pi = 3.141592653589;
    builder.append_floating_point(pi, std::chars_format::fixed, 200000);
    auto sv = builder.strview();
    EXPECT_GE(sv.size(), 200000);
    EXPECT_TRUE(sv.find("3.14") != std::string_view::npos);
}

TEST(UtilsStringBuilder, ChainedOperations)
{
    rbox::string_builder builder;
    builder.append_string("Hello")
        .append_char(' ')
        .append_string("World")
        .append_char('!')
        .append_integer(42);
    EXPECT_EQ(builder.strview(), "Hello World!42");
}

TEST(UtilsStringBuilder, ChainedOperationsUtf)
{
    rbox::u8string_builder builder;
    builder.append_string(u8"Hello")
        .append_string(u8"世界")
        .append_utf_code_point(U'!')
        .append_utf_code_point(U'\n');
    EXPECT_EQ(builder.strview(), u8"Hello世界!\n");
}

TEST(UtilsStringBuilder, AutoResize)
{
    rbox::string_builder builder(4);
    builder.append_string("12345678901234567890");
    EXPECT_EQ(builder.size(), 20);
    EXPECT_EQ(builder.strview(), "12345678901234567890");
}

TEST(UtilsStringBuilder, AppendToUtf16FromUtf8)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(u8"ABC");
    EXPECT_EQ(builder.size(), 3);
    EXPECT_EQ(builder.strview(), u"ABC");
}

TEST(UtilsStringBuilder, AppendToUtf32FromUtf8)
{
    rbox::u32string_builder builder;
    builder.append_utf_string(u8"hello");
    EXPECT_EQ(builder.size(), 5);
}

TEST(UtilsStringBuilder, StringBuilderAlias)
{
    rbox::string_builder sb;
    sb.append_string("test");
    EXPECT_EQ(sb.strview(), "test");
}

TEST(UtilsStringBuilder, U8StringBuilderAlias)
{
    rbox::u8string_builder sb;
    sb.append_string(u8"test");
    EXPECT_EQ(sb.strview(), u8"test");
}

TEST(UtilsStringBuilder, U16StringBuilderAlias)
{
    rbox::u16string_builder sb;
    sb.append_string(u"test");
    EXPECT_EQ(sb.strview(), u"test");
}

TEST(UtilsStringBuilder, U32StringBuilderAlias)
{
    rbox::u32string_builder sb;
    sb.append_utf_code_point(U'A');
    sb.append_utf_code_point(U'B');
    EXPECT_EQ(sb.size(), 2);
}

TEST(UtilsStringBuilder, WStringBuilderAlias)
{
    rbox::wstring_builder sb;
    sb.append_char(L't');
    sb.append_char(L'e');
    sb.append_char(L's');
    sb.append_char(L't');
    EXPECT_EQ(sb.size(), 4);
}

TEST(UtilsStringBuilder, AppendUtfStringBufferGrowth)
{
    rbox::u8string_builder builder(4);
    std::u8string_view long_str = u8"This is a very long string that should cause buffer growth";
    builder.append_string(long_str);
    EXPECT_EQ(builder.size(), long_str.size());
    EXPECT_EQ(builder.strview(), long_str);
}

TEST(UtilsStringBuilder, AppendUtfCodePointZero)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point(U'\0');
    EXPECT_EQ(builder.size(), 1);
}

TEST(UtilsStringBuilder, AppendUtfStringEmoji)
{
    rbox::u8string_builder builder;
    builder.append_string(u8"😀");
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), u8"😀");
}

TEST(UtilsStringBuilder, AppendUtfStringEmojiToUtf16)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(u8"😀");
    EXPECT_EQ(builder.size(), 2);
    EXPECT_EQ(builder.strview(), u"😀");
}

TEST(UtilsStringBuilder, ConstructorWithAllocator)
{
    rbox::string_builder builder(std::allocator<char>{});
    EXPECT_EQ(builder.size(), 0);
}

TEST(UtilsStringBuilder, InitialSizeWithAllocator)
{
    rbox::string_builder builder(100, std::allocator<char>{});
    EXPECT_EQ(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf8ToUtf32)
{
    rbox::u32string_builder builder;
    builder.append_utf_string(u8"Hello");
    EXPECT_EQ(builder.size(), 5);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf16ToUtf32)
{
    rbox::u32string_builder builder;
    builder.append_utf_string(u"Hi");
    EXPECT_EQ(builder.size(), 2);
}

TEST(UtilsStringBuilder, AppendEmptyString)
{
    rbox::string_builder builder;
    builder.append_string("");
    EXPECT_EQ(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendEmptyStringView)
{
    rbox::string_builder builder;
    std::string_view sv = "";
    builder.append_string(sv);
    EXPECT_EQ(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendIntegerZero)
{
    rbox::string_builder builder;
    builder.append_integer(0);
    EXPECT_EQ(builder.strview(), "0");
}

TEST(UtilsStringBuilder, AppendIntegerNegativeZero)
{
    rbox::string_builder builder;
    builder.append_integer(-0);
    EXPECT_EQ(builder.strview(), "0");
}

TEST(UtilsStringBuilder, AppendIntegerMaxValue)
{
    rbox::string_builder builder;
    builder.append_integer(std::numeric_limits<int64_t>::max());
    auto sv = builder.strview();
    EXPECT_GT(sv.size(), 0);
}

TEST(UtilsStringBuilder, AppendIntegerMinValue)
{
    rbox::string_builder builder;
    builder.append_integer(std::numeric_limits<int64_t>::min());
    auto sv = builder.strview();
    EXPECT_GT(sv.size(), 0);
}

TEST(UtilsStringBuilder, AppendIntegerUnsignedMax)
{
    rbox::string_builder builder;
    builder.append_integer(std::numeric_limits<uint64_t>::max());
    auto sv = builder.strview();
    EXPECT_GT(sv.size(), 0);
}

TEST(UtilsStringBuilder, AppendUtfCodePointMax)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point(U'\U0010FFFF');  // Max Unicode code point
    EXPECT_GT(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendUtfCodePointBmpBoundary)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point(U'\uFFFF');  // Max BMP code point
    EXPECT_EQ(builder.size(), 3);
}

TEST(UtilsStringBuilder, AppendUtfCodePointToUtf32)
{
    rbox::u32string_builder builder;
    builder.append_utf_code_point(U'\U0001F600');
    EXPECT_EQ(builder.size(), 1);
}

TEST(UtilsStringBuilder, AppendBoolToU8StringBuilder)
{
    rbox::u8string_builder builder;
    builder.append_bool(true);
    builder.append_bool(false);
    EXPECT_EQ(builder.strview(), u8"truefalse");
}

TEST(UtilsStringBuilder, AppendBoolToU16StringBuilder)
{
    rbox::u16string_builder builder;
    builder.append_bool(true);
    builder.append_bool(false);
    EXPECT_EQ(builder.strview(), u"truefalse");
}

TEST(UtilsStringBuilder, AppendBoolToU32StringBuilder)
{
    rbox::u32string_builder builder;
    builder.append_bool(true);
    EXPECT_EQ(builder.size(), 4);  // "true" has 4 code points in UTF-32
}

TEST(UtilsStringBuilder, AppendIntegerWithU32StringBuilderDecimal)
{
    rbox::u32string_builder builder;
    builder.append_integer(42, 10);
    EXPECT_EQ(builder.size(), 2);
}

TEST(UtilsStringBuilder, AppendFloatingPointNaN)
{
    rbox::string_builder builder;
    builder.append_floating_point(std::numeric_limits<double>::quiet_NaN());
    auto sv = builder.strview();
    EXPECT_TRUE(
        sv.find("nan") != std::string_view::npos || sv.find("NaN") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointInfinity)
{
    rbox::string_builder builder;
    builder.append_floating_point(std::numeric_limits<double>::infinity());
    auto sv = builder.strview();
    EXPECT_TRUE(
        sv.find("inf") != std::string_view::npos || sv.find("Inf") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointNegativeInfinity)
{
    rbox::string_builder builder;
    builder.append_floating_point(-std::numeric_limits<double>::infinity());
    auto sv = builder.strview();
    EXPECT_TRUE(
        sv.find("-inf") != std::string_view::npos || sv.find("-Inf") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointNegative)
{
    rbox::string_builder builder;
    builder.append_floating_point(-3.14);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find("-3.14") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendFloatingPointZero)
{
    rbox::string_builder builder;
    builder.append_floating_point(0.0);
    EXPECT_EQ(builder.strview(), "0");
}

TEST(UtilsStringBuilder, AppendFloatingPointNegativeZero)
{
    rbox::string_builder builder;
    builder.append_floating_point(-0.0);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find("0") != std::string_view::npos);
}

TEST(UtilsStringBuilder, AppendStringWithWString)
{
    rbox::wstring_builder builder;
    builder.append_string(L"hello");
    EXPECT_EQ(builder.size(), 5);
}

TEST(UtilsStringBuilder, AppendStringViewWithWString)
{
    rbox::wstring_builder builder;
    std::wstring_view sv = L"world";
    builder.append_string(sv);
    EXPECT_EQ(builder.size(), 5);
}

TEST(UtilsStringBuilder, AppendCharWithWString)
{
    rbox::wstring_builder builder;
    builder.append_char(L't');
    builder.append_char(L'e');
    builder.append_char(L's');
    builder.append_char(L't');
    EXPECT_EQ(builder.size(), 4);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf8ToWString)
{
    rbox::wstring_builder builder;
    builder.append_string(L"hello");
    EXPECT_EQ(builder.size(), 5);
}

TEST(UtilsStringBuilder, ComplexChainedOperations)
{
    rbox::string_builder builder;
    builder.append_string("Name: ")
        .append_string("John")
        .append_char(' ')
        .append_integer(30)
        .append_string(", Score: ")
        .append_floating_point(98.5)
        .append_bool(true);
    auto sv = builder.strview();
    EXPECT_TRUE(sv.find("Name: John 30, Score: 98.5true") != std::string_view::npos);
}

TEST(UtilsStringBuilder, ComplexChainedOperationsUtf)
{
    rbox::u8string_builder builder;
    builder.append_string(u8"Hello ")
        .append_string(u8"世界")
        .append_utf_code_point(U' ')
        .append_string(u8"😀")
        .append_utf_code_point(U':')
        .append_integer(100);
    EXPECT_GT(builder.size(), 0);
}

TEST(UtilsStringBuilder, BufferGrowthMultipleTimes)
{
    rbox::string_builder builder(4);
    for (int i = 0; i < 100; ++i) {
        builder.append_char('a');
    }
    EXPECT_EQ(builder.size(), 100);
    EXPECT_EQ(builder.strview(), std::string(100, 'a'));
}

TEST(UtilsStringBuilder, MultipleResizeEvents)
{
    rbox::string_builder builder(2);
    builder.append_string("abc");
    builder.append_string("defghijklmnop");
    builder.append_string("qrstuvwxyz");
    EXPECT_EQ(builder.size(), 26);
}

TEST(UtilsStringBuilder, AppendUtfStringBufferResize)
{
    rbox::u8string_builder builder(4);
    std::u8string_view sv1 = u8"12345678901234567890";
    std::u8string_view sv2 = u8"abcdefghijklmnop";
    builder.append_string(sv1);
    builder.append_string(sv2);
    EXPECT_EQ(builder.size(), sv1.size() + sv2.size());
}

TEST(UtilsStringBuilder, AppendUtfStringSameTypeUtf8ToUtf8)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(u8"hello");
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u8"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringSameTypeUtf16ToUtf16)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(u"hello");
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringSameTypeUtf32ToUtf32)
{
    rbox::u32string_builder builder;
    builder.append_utf_string(U"hello");
    EXPECT_EQ(builder.size(), 5);
}

TEST(UtilsStringBuilder, AppendUtfStringSameTypeUtf8ToUtf8WithEmoji)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(u8"😀🎉");
    EXPECT_EQ(builder.size(), 8);
    EXPECT_EQ(builder.strview(), u8"😀🎉");
}

TEST(UtilsStringBuilder, AppendUtfStringSameTypeUtf16ToUtf16WithEmoji)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(u"😀🎉");
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), u"😀🎉");
}

TEST(UtilsStringBuilder, AppendUtfStringSameTypeWithPtrAndEnd)
{
    rbox::u8string_builder builder;
    const char8_t* str = u8"world";
    builder.append_utf_string(str, str + 5);
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u8"world");
}

TEST(UtilsStringBuilder, AppendUtfStringSameTypeEmpty)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(u8"");
    EXPECT_EQ(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendUtfStringSameTypeWithInvalidSequence)
{
    rbox::u8string_builder builder;
    const char8_t invalid[] = u8"hello\xFF\xFEworld";
    builder.append_utf_string(invalid, invalid + sizeof(invalid) - 1);
    // Invalid sequence \xFF\xFE -> '�' -> 3 bytes in UTF-8 encoding
    EXPECT_EQ(builder.size(), 13);
}

TEST(UtilsStringBuilder, AppendUtfStringSameTypeBufferResize)
{
    rbox::u8string_builder builder(4);
    std::u8string_view long_str = u8"This is a very long string that should cause buffer growth";
    builder.append_utf_string(long_str);
    EXPECT_EQ(builder.size(), long_str.size());
    EXPECT_EQ(builder.strview(), long_str);
}

TEST(UtilsStringBuilder, AppendUtfStringSameTypeChained)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(u8"Hello").append_utf_string(u8" ").append_utf_string(u8"World");
    EXPECT_EQ(builder.size(), 11);
    EXPECT_EQ(builder.strview(), u8"Hello World");
}

TEST(UtilsStringBuilder, AppendUtfStringLargeUtf8ToUtf16)
{
    rbox::u16string_builder builder;
    std::u8string large_str;
    large_str.reserve(120000);
    for (int i = 0; i < 10000; ++i) {
        large_str += u8"Hello World ";
    }
    builder.append_utf_string(std::u8string_view(large_str));
    EXPECT_EQ(builder.size(), 120000);
}

TEST(UtilsStringBuilder, AppendUtfStringLargeUtf8ToUtf32)
{
    rbox::u32string_builder builder;
    std::u8string large_str;
    large_str.reserve(120000);
    for (int i = 0; i < 10000; ++i) {
        large_str += u8"Hello World ";
    }
    builder.append_utf_string(std::u8string_view(large_str));
    EXPECT_EQ(builder.size(), 120000);
}

TEST(UtilsStringBuilder, AppendUtfStringLargeUtf16ToUtf8)
{
    rbox::u8string_builder builder;
    std::u16string large_str;
    large_str.reserve(120000);
    for (int i = 0; i < 10000; ++i) {
        large_str += u"Hello World ";
    }
    builder.append_utf_string(std::u16string_view(large_str));
    EXPECT_EQ(builder.size(), 120000);
}

TEST(UtilsStringBuilder, AppendUtfStringLargeUtf8ToUtf8WithSmallBuffer)
{
    rbox::u8string_builder builder(8);
    std::u8string large_str;
    large_str.reserve(8000);
    for (int i = 0; i < 1000; ++i) {
        large_str += u8"ABCDEFGH";
    }
    builder.append_utf_string(std::u8string_view(large_str));
    EXPECT_EQ(builder.size(), 8000);
}

TEST(UtilsStringBuilder, AppendUtfStringLargeUtf16ToUtf16WithSmallBuffer)
{
    rbox::u16string_builder builder(8);
    std::u16string large_str;
    large_str.reserve(8000);
    for (int i = 0; i < 1000; ++i) {
        large_str += u"ABCDEFGH";
    }
    builder.append_utf_string(std::u16string_view(large_str));
    EXPECT_EQ(builder.size(), 8000);
}

TEST(UtilsStringBuilder, AppendUtfStringLargeUtf32ToUtf32WithSmallBuffer)
{
    rbox::u32string_builder builder(8);
    std::u32string large_str;
    large_str.reserve(4000);
    for (int i = 0; i < 1000; ++i) {
        large_str += U"ABCD";
    }
    builder.append_utf_string(std::u32string_view(large_str));
    EXPECT_EQ(builder.size(), 4000);
}

TEST(UtilsStringBuilder, AppendUtfStringVeryLargeSingleAppend)
{
    rbox::u8string_builder builder;
    std::u8string large_str;
    large_str.reserve(100000);
    for (int i = 0; i < 10000; ++i) {
        large_str += u8"abcdefghij";
    }
    builder.append_utf_string(std::u8string_view(large_str));
    EXPECT_EQ(builder.size(), 100000);
}

TEST(UtilsStringBuilder, AppendUtfStringLargeMixedEncodings)
{
    rbox::u8string_builder builder;
    std::u8string utf8_part;
    utf8_part.reserve(6000);
    for (int i = 0; i < 1000; ++i) {
        utf8_part += u8"Hello ";
    }
    std::u16string utf16_part;
    utf16_part.reserve(6000);
    for (int i = 0; i < 1000; ++i) {
        utf16_part += u"World ";
    }
    builder.append_utf_string(std::u8string_view(utf8_part));
    builder.append_utf_string(std::u16string_view(utf16_part));
    EXPECT_EQ(builder.size(), 12000);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf8)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(U"hello");
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u8"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf8WithMultiByte)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(U"\u20AC");  // Euro sign
    EXPECT_EQ(builder.size(), 3);
    EXPECT_EQ(builder.strview(), u8"\u20AC");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf8WithEmoji)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(U"\U0001F600");  // Grinning face
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), u8"\U0001F600");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf16)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(U"hello");
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf16WithMultiByte)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(U"\u20AC");  // Euro sign (BMP)
    EXPECT_EQ(builder.size(), 1);
    EXPECT_EQ(builder.strview(), u"\u20AC");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf16WithSurrogate)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(U"\U0001F600");  // Grinning face (needs surrogate pair)
    EXPECT_EQ(builder.size(), 2);
    EXPECT_EQ(builder.strview(), u"\U0001F600");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf8WithView)
{
    rbox::u8string_builder builder;
    std::u32string_view sv = U"test";
    builder.append_utf_string(sv);
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), u8"test");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf16WithView)
{
    rbox::u16string_builder builder;
    std::u32string_view sv = U"test";
    builder.append_utf_string(sv);
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), u"test");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf8WithPtrAndEnd)
{
    rbox::u8string_builder builder;
    const char32_t* str = U"hello";
    builder.append_utf_string(str, str + 5);
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u8"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf16WithPtrAndEnd)
{
    rbox::u16string_builder builder;
    const char32_t* str = U"hello";
    builder.append_utf_string(str, str + 5);
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), u"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf8Empty)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(U"");
    EXPECT_EQ(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf16Empty)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(U"");
    EXPECT_EQ(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf8Mixed)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(U"Hello");
    builder.append_utf_string(u8" World");
    builder.append_utf_string(U" \U0001F600");
    EXPECT_EQ(builder.size(), 16);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf16Mixed)
{
    rbox::u16string_builder builder;
    builder.append_utf_string(U"Hello");
    builder.append_string(u" World");
    builder.append_utf_string(U" \U0001F600");
    EXPECT_EQ(builder.size(), 14);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf8Large)
{
    rbox::u8string_builder builder;
    std::u32string large_str;
    large_str.reserve(30000);
    for (int i = 0; i < 10000; ++i) {
        large_str += U"ABC";
    }
    builder.append_utf_string(std::u32string_view(large_str));
    EXPECT_EQ(builder.size(), 30000);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf16Large)
{
    rbox::u16string_builder builder;
    std::u32string large_str;
    large_str.reserve(30000);
    for (int i = 0; i < 10000; ++i) {
        large_str += U"ABC";
    }
    builder.append_utf_string(std::u32string_view(large_str));
    EXPECT_EQ(builder.size(), 30000);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf8SmallBuffer)
{
    rbox::u8string_builder builder(4);
    std::u32string_view sv = U"Hello World";
    builder.append_utf_string(sv);
    EXPECT_EQ(builder.size(), 11);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32ToUtf16SmallBuffer)
{
    rbox::u16string_builder builder(4);
    std::u32string_view sv = U"Hello World";
    builder.append_utf_string(sv);
    EXPECT_EQ(builder.size(), 11);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32MaxCodePoint)
{
    rbox::u8string_builder builder;
    builder.append_utf_string(U"\U0010FFFF");  // Max Unicode
    EXPECT_EQ(builder.size(), 4);
    rbox::u16string_builder builder16;
    builder16.append_utf_string(U"\U0010FFFF");
    EXPECT_EQ(builder16.size(), 2);
}

TEST(UtilsStringBuilder, AppendUtfStringUtf32InvalidSurrogate)
{
    rbox::u8string_builder builder;
    char32_t invalid[] = U"hello\xD800world";  // Invalid surrogate in UTF-32
    builder.append_utf_string(invalid, invalid + sizeof(invalid) / sizeof(char32_t) - 1);
    EXPECT_GT(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendCharWithCount)
{
    rbox::string_builder builder;
    builder.append_char('a', 5);
    EXPECT_EQ(builder.size(), 5);
    EXPECT_EQ(builder.strview(), "aaaaa");
}

TEST(UtilsStringBuilder, AppendCharWithCountUtf8)
{
    rbox::u8string_builder builder;
    builder.append_char(u8'x', 3);
    EXPECT_EQ(builder.size(), 3);
    EXPECT_TRUE(builder.strview() == u8"xxx");
}

TEST(UtilsStringBuilder, AppendCharWithCountUtf16)
{
    rbox::u16string_builder builder;
    builder.append_char(u'y', 4);
    EXPECT_EQ(builder.size(), 4);
    EXPECT_EQ(builder.strview(), u"yyyy");
}

TEST(UtilsStringBuilder, AppendCharWithCountZero)
{
    rbox::string_builder builder;
    builder.append_char('a', 0);
    EXPECT_EQ(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendCharWithCountBufferGrowth)
{
    rbox::string_builder builder(2);
    builder.append_char('b', 100);
    EXPECT_EQ(builder.size(), 100);
    EXPECT_EQ(builder.strview(), std::string(100, 'b'));
}

TEST(UtilsStringBuilder, AppendCharJsonEscapedBasic)
{
    rbox::string_builder builder;
    builder.append_char_by<rbox::escaping_mode::json>('"');
    EXPECT_EQ(builder.strview(), "\\\"");
}

TEST(UtilsStringBuilder, AppendCharJsonEscapedBackslash)
{
    rbox::string_builder builder;
    builder.append_char_by<rbox::escaping_mode::json>('\\');
    EXPECT_EQ(builder.strview(), "\\\\");
}

TEST(UtilsStringBuilder, AppendCharJsonEscapedNewline)
{
    rbox::string_builder builder;
    builder.append_char_by<rbox::escaping_mode::json>('\n');
    EXPECT_EQ(builder.strview(), "\\n");
}

TEST(UtilsStringBuilder, AppendCharJsonEscapedTab)
{
    rbox::string_builder builder;
    builder.append_char_by<rbox::escaping_mode::json>('\t');
    EXPECT_EQ(builder.strview(), "\\t");
}

TEST(UtilsStringBuilder, AppendCharJsonEscapedNoEscape)
{
    rbox::string_builder builder;
    builder.append_char_by<rbox::escaping_mode::json>('a');
    EXPECT_EQ(builder.strview(), "a");
}

TEST(UtilsStringBuilder, AppendCharJsonEscapedUtf8)
{
    rbox::u8string_builder builder;
    builder.append_char_by<rbox::escaping_mode::json>(u8'"');
    EXPECT_TRUE(builder.strview() == u8"\\\"");
}

TEST(UtilsStringBuilder, AppendCharJsonEscapedControlChar)
{
    rbox::string_builder builder;
    builder.append_char_by<rbox::escaping_mode::json>('\x00');
    EXPECT_EQ(builder.strview(), "\\u0000");
}

TEST(UtilsStringBuilder, AppendUtfCodePointJsonEscapedBasic)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point_by<rbox::escaping_mode::json>(U'A');
    EXPECT_TRUE(builder.strview() == u8"A");
}

TEST(UtilsStringBuilder, AppendUtfCodePointJsonEscapedDoubleQuote)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point_by<rbox::escaping_mode::json>(U'"');
    EXPECT_TRUE(builder.strview() == u8"\\\"");
}

TEST(UtilsStringBuilder, AppendUtfCodePointJsonEscapedBackslash)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point_by<rbox::escaping_mode::json>(U'\\');
    EXPECT_TRUE(builder.strview() == u8"\\\\");
}

TEST(UtilsStringBuilder, AppendUtfCodePointJsonEscapedNewline)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point_by<rbox::escaping_mode::json>(U'\n');
    EXPECT_TRUE(builder.strview() == u8"\\n");
}

TEST(UtilsStringBuilder, AppendUtfCodePointJsonEscapedEmoji)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point_by<rbox::escaping_mode::json>(U'\U0001F600');
    EXPECT_TRUE(builder.strview() == u8"\U0001F600");
}

TEST(UtilsStringBuilder, AppendUtfCodePointJsonEscapedInvalid)
{
    rbox::u8string_builder builder;
    builder.append_utf_code_point_by<rbox::escaping_mode::json>(static_cast<char32_t>(0xFFFFFFFF));
    EXPECT_TRUE(builder.strview() == u8"\uFFFD");
}

TEST(UtilsStringBuilder, AppendUtfCodePointJsonEscapedUtf16)
{
    rbox::u16string_builder builder;
    builder.append_utf_code_point_by<rbox::escaping_mode::json>(U'"');
    EXPECT_EQ(builder.strview(), u"\\\"");
}

TEST(UtilsStringBuilder, AppendStringJsonEscapedBasic)
{
    rbox::string_builder builder;
    builder.append_string_by<rbox::escaping_mode::json>("hello");
    EXPECT_EQ(builder.strview(), "hello");
}

TEST(UtilsStringBuilder, AppendStringJsonEscapedWithSpecialChars)
{
    rbox::string_builder builder;
    builder.append_string_by<rbox::escaping_mode::json>("line1\nline2\ttab");
    EXPECT_EQ(builder.strview(), "line1\\nline2\\ttab");
}

TEST(UtilsStringBuilder, AppendStringJsonEscapedQuotes)
{
    rbox::string_builder builder;
    builder.append_string_by<rbox::escaping_mode::json>("say \"hello\"");
    EXPECT_EQ(builder.strview(), "say \\\"hello\\\"");
}

TEST(UtilsStringBuilder, AppendStringJsonEscapedBackslash)
{
    rbox::string_builder builder;
    builder.append_string_by<rbox::escaping_mode::json>("path\\to\\file");
    EXPECT_EQ(builder.strview(), "path\\\\to\\\\file");
}

TEST(UtilsStringBuilder, AppendStringJsonEscapedPtrWithEnd)
{
    rbox::string_builder builder;
    const char* str = "test\nvalue";
    builder.append_string_by<rbox::escaping_mode::json>(str, str + 10);
    EXPECT_EQ(builder.strview(), "test\\nvalue");
}

TEST(UtilsStringBuilder, AppendStringJsonEscapedView)
{
    rbox::string_builder builder;
    std::string_view sv = "escape\tthis";
    builder.append_string_by<rbox::escaping_mode::json>(sv);
    EXPECT_EQ(builder.strview(), "escape\\tthis");
}

TEST(UtilsStringBuilder, AppendStringJsonEscapedUtf8)
{
    rbox::u8string_builder builder;
    builder.append_string_by<rbox::escaping_mode::json>(u8"hello\nworld");
    EXPECT_TRUE(builder.strview() == u8"hello\\nworld");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedUtf8ToUtf8)
{
    rbox::u8string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(u8"hello");
    EXPECT_TRUE(builder.strview() == u8"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedUtf8ToUtf16)
{
    rbox::u16string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(u8"hello\nworld");
    EXPECT_EQ(builder.strview(), u"hello\\nworld");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedUtf8ToUtf32)
{
    rbox::u32string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(u8"test\nvalue");
    EXPECT_TRUE(builder.strview() == U"test\\nvalue");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedUtf16ToUtf8)
{
    rbox::u8string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(u"quote\"here");
    EXPECT_TRUE(builder.strview() == u8"quote\\\"here");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedUtf16ToUtf16)
{
    rbox::u16string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(u"escape\\here");
    EXPECT_EQ(builder.strview(), u"escape\\\\here");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedUtf32ToUtf8)
{
    rbox::u8string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(U"tab\there");
    EXPECT_TRUE(builder.strview() == u8"tab\\there");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedUtf32ToUtf16)
{
    rbox::u16string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(U"newline\n");
    EXPECT_EQ(builder.strview(), u"newline\\n");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedWithEmoji)
{
    rbox::u8string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(u8"Hello 😀!");
    EXPECT_TRUE(builder.strview() == u8"Hello 😀!");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedWithControlChar)
{
    rbox::string_builder builder;
    constexpr auto str = u8"before\0after";
    builder.append_utf_string_by<rbox::escaping_mode::json>(str, str + 12);
    EXPECT_EQ("before\\u0000after", builder.strview());
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedPtrWithEnd)
{
    rbox::u8string_builder builder;
    const char8_t* str = u8"test\nvalue";
    builder.append_utf_string_by<rbox::escaping_mode::json>(str, str + 10);
    EXPECT_TRUE(builder.strview() == u8"test\\nvalue");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedView)
{
    rbox::u8string_builder builder;
    std::u8string_view sv = u8"tab\there";
    builder.append_utf_string_by<rbox::escaping_mode::json>(sv);
    EXPECT_TRUE(builder.strview() == u8"tab\\there");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedEmpty)
{
    rbox::u8string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(u8"");
    EXPECT_EQ(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedSameTypeUtf8)
{
    rbox::u8string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(u8"quote\"");
    EXPECT_TRUE(builder.strview() == u8"quote\\\"");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedSameTypeUtf16)
{
    rbox::u16string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(u"tab\t");
    EXPECT_EQ(builder.strview(), u"tab\\t");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedSameTypeUtf32)
{
    rbox::u32string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(U"newline\n");
    EXPECT_TRUE(builder.strview() == U"newline\\n");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedInvalidSequence)
{
    rbox::u8string_builder builder;
    const char8_t invalid[] = u8"hello\xFFworld";
    builder.append_utf_string_by<rbox::escaping_mode::json>(invalid, invalid + sizeof(invalid) - 1);
    EXPECT_GT(builder.size(), 0);
}

TEST(UtilsStringBuilder, AppendUtfStringWithBasicString)
{
    rbox::u8string_builder builder;
    std::string str = "hello";
    builder.append_utf_string(str);
    EXPECT_TRUE(builder.strview() == u8"hello");
}

TEST(UtilsStringBuilder, AppendUtfStringWithBasicStringUtf16ToUtf8)
{
    rbox::u8string_builder builder;
    std::u16string str = u"world";
    builder.append_utf_string(str);
    EXPECT_TRUE(builder.strview() == u8"world");
}

TEST(UtilsStringBuilder, AppendUtfStringWithBasicStringUtf8ToUtf16)
{
    rbox::u16string_builder builder;
    std::string str = "test";
    builder.append_utf_string(str);
    EXPECT_EQ(builder.strview(), u"test");
}

TEST(UtilsStringBuilder, AppendUtfStringWithBasicStringUtf32ToUtf8)
{
    rbox::u8string_builder builder;
    std::u32string str = U"unicode";
    builder.append_utf_string(str);
    EXPECT_TRUE(builder.strview() == u8"unicode");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedWithBasicString)
{
    rbox::u8string_builder builder;
    std::string str = "line1\nline2";
    builder.append_utf_string_by<rbox::escaping_mode::json>(str);
    EXPECT_TRUE(builder.strview() == u8"line1\\nline2");
}

TEST(UtilsStringBuilder, AppendUtfStringJsonEscapedWithBasicStringUtf16)
{
    rbox::u16string_builder builder;
    std::u16string str = u"quote\"here";
    builder.append_utf_string_by<rbox::escaping_mode::json>(str);
    EXPECT_EQ(builder.strview(), u"quote\\\"here");
}

TEST(UtilsStringBuilder, StrMethod)
{
    rbox::string_builder builder;
    builder.append_string("hello");
    auto str = builder.str();
    EXPECT_EQ(str, "hello");
    EXPECT_EQ(str.size(), 5);
}

TEST(UtilsStringBuilder, StrMethodUtf8)
{
    rbox::u8string_builder builder;
    builder.append_string(u8"world");
    auto str = builder.str();
    EXPECT_TRUE(str == u8"world");
}

TEST(UtilsStringBuilder, StrMethodEmpty)
{
    rbox::string_builder builder;
    auto str = builder.str();
    EXPECT_EQ(str, "");
}

TEST(UtilsStringBuilder, StrMethodAfterModification)
{
    rbox::string_builder builder;
    builder.append_string("hello");
    auto str1 = builder.str();
    builder.append_string(" world");
    auto str2 = builder.str();
    EXPECT_EQ(str1, "hello");
    EXPECT_EQ(str2, "hello world");
}

TEST(UtilsStringBuilder, PmrStringBuilder)
{
    rbox::pmr_string_builder builder;
    builder.append_string("test");
    EXPECT_EQ(builder.strview(), "test");
}

TEST(UtilsStringBuilder, PmrU8StringBuilder)
{
    rbox::pmr_u8string_builder builder;
    builder.append_string(u8"hello");
    EXPECT_TRUE(builder.strview() == u8"hello");
}

TEST(UtilsStringBuilder, PmrU16StringBuilder)
{
    rbox::pmr_u16string_builder builder;
    builder.append_string(u"world");
    EXPECT_EQ(builder.strview(), u"world");
}

TEST(UtilsStringBuilder, PmrU32StringBuilder)
{
    rbox::pmr_u32string_builder builder;
    builder.append_utf_code_point(U'A');
    EXPECT_EQ(builder.size(), 1);
}

TEST(UtilsStringBuilder, ChainedJsonEscaped)
{
    rbox::string_builder builder;
    builder.append_string("Message: ")
        .append_string_by<rbox::escaping_mode::json>("Hello\nWorld")
        .append_string(" - Done");
    EXPECT_EQ(builder.strview(), "Message: Hello\\nWorld - Done");
}

TEST(UtilsStringBuilder, ChainedUtfJsonEscaped)
{
    rbox::u8string_builder builder;
    builder.append_utf_string_by<rbox::escaping_mode::json>(u8"line1\n")
        .append_utf_string_by<rbox::escaping_mode::json>(u8"line2\tindented");
    EXPECT_TRUE(builder.strview() == u8"line1\\nline2\\tindented");
}
