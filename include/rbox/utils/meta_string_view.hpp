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

#ifndef RBOX_UTILS_META_STRING_VIEW_HPP
#define RBOX_UTILS_META_STRING_VIEW_HPP

#include <array>
#include <compare>
#include <rbox/utils/config.hpp>
#include <string_view>

namespace rbox {
template <class CharT>
struct meta_basic_string_view {
    using value_type = CharT;
    using pointer = CharT*;
    using const_pointer = const CharT*;
    using reference = CharT&;
    using const_reference = const CharT&;
    using iterator = const_pointer;
    using const_iterator = const_pointer;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    const CharT* head;
    size_t n;

    // Trivial construction
    constexpr meta_basic_string_view() = default;

    consteval meta_basic_string_view(const CharT* literal) : head(literal)
    {
        if (literal == nullptr) {
            compile_error("String literal must not be null pointer.");
        }
        const auto* tail = literal;
        while (*tail != '\0') ++tail;
        n = static_cast<size_t>(tail - literal);
    }

    template <size_t N>
    consteval meta_basic_string_view(const CharT (&literal)[N])
        : meta_basic_string_view(static_cast<const CharT*>(literal))
    {
        if (literal[N - 1] != '\0') {
            compile_error("String literal must be null-terminated.");
        }
    }

    template <size_t N>
    consteval meta_basic_string_view(const std::array<CharT, N>& arr)
        : meta_basic_string_view(arr.data())
    {
        if (arr[N - 1] != '\0') {
            compile_error("String literal must be null-terminated.");
        }
    }

    consteval meta_basic_string_view(std::basic_string_view<CharT> sv)
        : meta_basic_string_view(sv.data())
    {
        if (*sv.end() != '\0') {
            compile_error("String literal must be null-terminated.");
        }
    }

    // Implicit conversion to std::string_view
    constexpr operator std::basic_string_view<CharT>() const
    {
        return {head, n};
    }

    constexpr auto operator[](size_t index) const -> CharT
    {
        return head[index];
    }

    constexpr auto front() const -> CharT
    {
        return head[0];
    }

    constexpr auto back() const -> CharT
    {
        return head[n - 1];
    }

    constexpr auto size() const -> size_t
    {
        return n;
    }

    constexpr auto length() const -> size_t
    {
        return n;
    }

    constexpr auto empty() const -> bool
    {
        return n == 0;
    }

    constexpr auto data() const -> const CharT*
    {
        return head;
    }

    constexpr auto begin() const -> const CharT*
    {
        return head;
    }

    constexpr auto end() const -> const CharT*
    {
        return head + n;
    }

    constexpr auto cbegin() const -> const CharT*
    {
        return head;
    }

    constexpr auto cend() const -> const CharT*
    {
        return head + n;
    }

    // Note: remove_suffix(n) is not provided since we need to ensure
    // the constraint head[n] == '\0' is always satisfied.
    constexpr auto remove_prefix(size_t n) const -> meta_basic_string_view
    {
        auto res = meta_basic_string_view{};
        res.head = this->head + n;
        res.n = this->n - n;
        return res;
    }

private:
    constexpr auto compare(const CharT* rhs_head, size_t rhs_n) const -> std::strong_ordering
    {
        for (size_t i = 0; i < n && i < rhs_n; ++i) {
            if (auto cmp = head[i] <=> rhs_head[i]; cmp != 0) {
                return cmp;
            }
        }
        return n <=> rhs_n;
    }

    constexpr auto equals(const CharT* rhs_head, size_t rhs_n) const -> bool
    {
        if (n != rhs_n) {
            return false;
        }
        for (size_t i = 0; i < n; ++i) {
            if (head[i] != rhs_head[i]) {
                return false;
            }
        }
        return true;
    }

public:
    constexpr auto operator<=>(std::basic_string_view<CharT> rhs) const -> std::strong_ordering
    {
        return compare(rhs.data(), rhs.length());
    }

    constexpr bool operator==(std::basic_string_view<CharT> rhs) const
    {
        return equals(rhs.data(), rhs.length());
    }

    constexpr auto operator<=>(meta_basic_string_view<CharT> rhs) const -> std::strong_ordering
    {
        return compare(rhs.head, rhs.n);
    }

    constexpr bool operator==(meta_basic_string_view<CharT> rhs) const
    {
        return equals(rhs.head, rhs.n);
    }

    constexpr auto operator<=>(const CharT* rhs) const -> std::strong_ordering
    {
        if (rhs == nullptr) [[unlikely]] {
            return size() <=> 0;  // nullptr as empty string
        }
        if (head == nullptr) [[unlikely]] {
            return (*rhs == '\0') ? std::strong_ordering::equal : std::strong_ordering::less;
        }
        for (size_t i = 0; i < n; ++i) {
            if (rhs[i] == '\0') {
                return std::strong_ordering::greater;
            }
            if (auto cmp = head[i] <=> rhs[i]; cmp != 0) {
                return cmp;
            }
        }
        return rhs[n] == '\0' ? std::strong_ordering::equal : std::strong_ordering::less;
    }

    constexpr bool operator==(const CharT* rhs) const
    {
        if (rhs == nullptr) [[unlikely]] {
            return size() == 0;  // nullptr as empty string
        }
        for (size_t i = 0; i < n; ++i) {
            if (head[i] != rhs[i]) {
                return false;
            }
        }
        return rhs[n] == '\0';
    }
};

using meta_string_view = meta_basic_string_view<char>;
using meta_wstring_view = meta_basic_string_view<wchar_t>;
using meta_u8string_view = meta_basic_string_view<char8_t>;
using meta_u16string_view = meta_basic_string_view<char16_t>;
using meta_u32string_view = meta_basic_string_view<char32_t>;

}  // namespace rbox

// Please include <rbox/format.hpp> for std::format and fmt::format support

#endif  // RBOX_UTILS_META_STRING_VIEW_HPP
