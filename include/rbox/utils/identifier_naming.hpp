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

#ifndef RBOX_UTILS_IDENTIFIER_NAMING_HPP
#define RBOX_UTILS_IDENTIFIER_NAMING_HPP

#include <rbox/type_traits/arithmetic_types.hpp>
#include <rbox/type_traits/string_like_types.hpp>
#include <rbox/utils/config.hpp>
#include <rbox/utils/ctype/case_conversion.hpp>
#include <rbox/utils/make_string_view.hpp>
#include <string>
#include <string_view>

namespace rbox {
#define RBOX_IDENTIFIER_NAMING_RULES_FOR_EACH(F) \
    F(snake_case)                                \
    F(all_caps_snake_case)                       \
    F(kebab_case)                                \
    F(all_caps_kebab_case)                       \
    F(lower_camel_case)                          \
    F(upper_camel_case)                          \
    F(lower_camel_snake_case)                    \
    F(upper_camel_snake_case)                    \
    F(http_header_case)

enum class identifier_naming_rule {
#define RBOX_DEFINE_IDENTIFIER_NAMING_RULE(rule) rule,
    RBOX_IDENTIFIER_NAMING_RULES_FOR_EACH(RBOX_DEFINE_IDENTIFIER_NAMING_RULE)
#undef RBOX_DEFINE_IDENTIFIER_NAMING_RULE
};

struct non_alpha_as_lower_tag_t {};
struct non_alpha_as_upper_tag_t {};

constexpr auto non_alpha_as_lower = non_alpha_as_lower_tag_t{};
constexpr auto non_alpha_as_upper = non_alpha_as_upper_tag_t{};

namespace impl {
template <class T>
concept identifier_conversion_tag =
    std::is_same_v<T, non_alpha_as_lower_tag_t> || std::is_same_v<T, non_alpha_as_upper_tag_t>;

enum class identifier_parsing_state_l {
    init,
    lower_or_digit,
    first_upper,
    subsequent_upper,
};

enum class identifier_parsing_state_u {
    init,
    lower,
    first_upper_or_digit,
    subsequent_upper_or_digit,
};

template <class CharT>
constexpr bool is_identifier_delimiter(CharT c)
{
    return c == '_' || c == '-';
}

template <class Policy, class CharT>
constexpr auto visit_identifier_segments_l(
    CharT* buffer, const CharT* input, const CharT* input_end) -> CharT*
{
    using enum identifier_parsing_state_l;

    constexpr auto delimiter = static_cast<CharT>(Policy::delimiter);
    auto state = init;
    auto first = true;
    const auto* head = input;
    const auto* tail = input;

    for (; tail < input_end; ++tail) {
        if (state == init) {
            if (is_identifier_delimiter(*tail)) {
                *buffer++ = delimiter;
                ++head;
            } else if (ascii_isupper(*tail)) {
                state = first_upper;
            } else {
                state = lower_or_digit;
            }
            continue;
        }
        if (is_identifier_delimiter(*tail)) {
            Policy::visit_non_empty_word(&buffer, head, tail, &first);
            state = init;
            head = tail + 1;
            continue;
        }
        if (state == lower_or_digit) {
            if (ascii_isupper(*tail)) {
                Policy::visit_non_empty_word(&buffer, head, tail, &first);
                state = first_upper;
                head = tail;
            }
        } else if (state == first_upper) {
            state = ascii_isupper(*tail) ? subsequent_upper : lower_or_digit;
        } else {
            // state == subsequent_upper
            if (!ascii_isupper(*tail)) {
                Policy::visit_non_empty_word(&buffer, head, tail - 1, &first);
                state = lower_or_digit;
                head = tail - 1;
            }
        }
    }
    if (head < tail) {
        Policy::visit_non_empty_word(&buffer, head, tail, &first);
    } else if (!first) {
        *buffer++ = delimiter;
    }
    return buffer;
}

template <class Policy, class CharT>
constexpr auto visit_identifier_segments_u(
    CharT* buffer, const CharT* input, const CharT* input_end) -> CharT*
{
    using enum identifier_parsing_state_u;

    constexpr auto delimiter = static_cast<CharT>(Policy::delimiter);
    auto state = init;
    auto first = true;
    const auto* head = input;
    const auto* tail = input;

    for (; tail < input_end; ++tail) {
        if (state == init) {
            if (is_identifier_delimiter(*tail)) {
                *buffer++ = delimiter;
                ++head;
            } else if (ascii_islower(*tail)) {
                state = lower;
            } else {
                state = first_upper_or_digit;
            }
            continue;
        }
        if (is_identifier_delimiter(*tail)) {
            Policy::visit_non_empty_word(&buffer, head, tail, &first);
            state = init;
            head = tail + 1;
            continue;
        }
        if (state == lower) {
            if (!ascii_islower(*tail)) {
                Policy::visit_non_empty_word(&buffer, head, tail, &first);
                state = first_upper_or_digit;
                head = tail;
            }
        } else if (state == first_upper_or_digit) {
            state = ascii_islower(*tail) ? lower : subsequent_upper_or_digit;
        } else {
            // state == subsequent_upper_or_digit
            if (ascii_islower(*tail)) {
                Policy::visit_non_empty_word(&buffer, head, tail - 1, &first);
                state = lower;
                head = tail - 1;
            }
        }
    }
    if (head < tail) {
        Policy::visit_non_empty_word(&buffer, head, tail, &first);
    } else if (!first) {
        *buffer++ = delimiter;
    }
    return buffer;
}

template <class CharT>
constexpr void to_lower_case(CharT** buffer, const CharT* word, const CharT* word_end)
{
    for (; word < word_end; ++word) {
        **buffer = ascii_tolower(*word);
        *buffer += 1;
    }
}

template <class CharT>
constexpr void to_upper_case(CharT** buffer, const CharT* word, const CharT* word_end)
{
    for (; word < word_end; ++word) {
        **buffer = ascii_toupper(*word);
        *buffer += 1;
    }
}

template <class CharT>
constexpr void to_upper_camel_case(CharT** buffer, const CharT* word, const CharT* word_end)
{
    **buffer = ascii_toupper(*word++);
    *buffer += 1;
    for (; word < word_end; ++word) {
        **buffer = ascii_tolower(*word);
        *buffer += 1;
    }
}

template <char Delim, bool U1, bool U2>
struct delimited_uniform_word_policy {
    static constexpr auto reserving_scale = 2;
    static constexpr auto delimiter = Delim;

    template <class CharT>
    static constexpr void visit_non_empty_word(
        CharT** buffer, const CharT* word, const CharT* word_end, bool* is_first)
    {
        if (!*is_first) {
            **buffer = static_cast<CharT>(delimiter);
            *buffer += 1;
        } else {
            *is_first = false;
        }
        if constexpr (!U1) {
            to_lower_case(buffer, word, word_end);
        } else if constexpr (!U2) {
            to_upper_camel_case(buffer, word, word_end);
        } else {
            to_upper_case(buffer, word, word_end);
        }
    }
};

struct lower_camel_case_word_policy {
    static constexpr auto reserving_scale = 1;
    static constexpr auto delimiter = '_';

    template <class CharT>
    static constexpr void visit_non_empty_word(
        CharT** buffer, const CharT* word, const CharT* word_end, bool* is_first)
    {
        if (!*is_first) {
            to_upper_camel_case(buffer, word, word_end);
        } else {
            to_lower_case(buffer, word, word_end);
            *is_first = false;
        }
    }
};

struct upper_camel_case_word_policy {
    static constexpr auto reserving_scale = 1;
    static constexpr auto delimiter = '_';

    template <class CharT>
    static constexpr void visit_non_empty_word(
        CharT** buffer, const CharT* word, const CharT* word_end, bool* is_first)
    {
        to_upper_camel_case(buffer, word, word_end);
        *is_first = false;
    }
};

struct lower_camel_snake_case_word_policy {
    static constexpr auto reserving_scale = 2;
    static constexpr auto delimiter = '_';

    template <class CharT>
    static constexpr void visit_non_empty_word(
        CharT** buffer, const CharT* word, const CharT* word_end, bool* is_first)
    {
        if (!*is_first) {
            **buffer = static_cast<CharT>(delimiter);
            *buffer += 1;
            to_upper_camel_case(buffer, word, word_end);
        } else {
            to_lower_case(buffer, word, word_end);
            *is_first = false;
        }
    }
};

template <class Tag, class Policy, class CharT>
constexpr auto convert_identifier_impl(const CharT* input, const CharT* input_end)
    -> std::basic_string<CharT>
{
    if (input >= input_end) [[unlikely]] {
        return {};
    }
    auto res = std::basic_string<CharT>{};
    auto reserved_size = static_cast<size_t>(input_end - input) * Policy::reserving_scale;
    res.resize_and_overwrite(reserved_size, [&](CharT* buffer, size_t) {
        if constexpr (std::is_same_v<Tag, non_alpha_as_lower_tag_t>) {
            return visit_identifier_segments_l<Policy>(buffer, input, input_end) - buffer;
        } else {
            static_assert(std::is_same_v<Tag, non_alpha_as_upper_tag_t>);
            return visit_identifier_segments_u<Policy>(buffer, input, input_end) - buffer;
        }
    });
    return res;
}

template <class Policy>
struct convert_identifier_interface {
    template <identifier_conversion_tag Tag, char_type CharT>
    static constexpr auto operator()(Tag, const CharT* identifier, const CharT* identifier_end)
        -> std::basic_string<CharT>
    {
        return convert_identifier_impl<Tag, Policy>(identifier, identifier_end);
    }

    template <identifier_conversion_tag Tag, string_like StringLike>
    static constexpr auto operator()(Tag, const StringLike& identifier)
    {
        auto sv = make_string_view(identifier);
        return convert_identifier_impl<Tag, Policy>(sv.data(), sv.data() + sv.length());
    }

    template <char_type CharT>
    static constexpr auto operator()(const CharT* identifier, const CharT* identifier_end)
        -> std::basic_string<CharT>
    {
        return operator()(non_alpha_as_lower, identifier, identifier_end);
    }

    template <string_like StringLike>
    static constexpr auto operator()(const StringLike& identifier)
    {
        return operator()(non_alpha_as_lower, identifier);
    }
};
}  // namespace impl

#define RBOX_TO_DELIMITED_UNIFORM_FUNCTOR(name, Delim, U1, U2)                                  \
    using to_##name##_case_t =                                                                  \
        impl::convert_identifier_interface<impl::delimited_uniform_word_policy<Delim, U1, U2>>; \
    constexpr auto to_##name##_case = to_##name##_case_t{};

RBOX_TO_DELIMITED_UNIFORM_FUNCTOR(snake, '_', false, false)
RBOX_TO_DELIMITED_UNIFORM_FUNCTOR(kebab, '-', false, false)
RBOX_TO_DELIMITED_UNIFORM_FUNCTOR(all_caps_snake, '_', true, true)
RBOX_TO_DELIMITED_UNIFORM_FUNCTOR(all_caps_kebab, '-', true, true)
RBOX_TO_DELIMITED_UNIFORM_FUNCTOR(upper_camel_snake, '_', true, false)
RBOX_TO_DELIMITED_UNIFORM_FUNCTOR(http_header, '-', true, false)

#undef RBOX_TO_DELIMITED_UNIFORM_FUNCTOR

using to_lower_camel_case_t =
    impl::convert_identifier_interface<impl::lower_camel_case_word_policy>;
constexpr auto to_lower_camel_case = to_lower_camel_case_t{};

using to_upper_camel_case_t =
    impl::convert_identifier_interface<impl::upper_camel_case_word_policy>;
constexpr auto to_upper_camel_case = to_upper_camel_case_t{};

using to_lower_camel_snake_case_t =
    impl::convert_identifier_interface<impl::lower_camel_snake_case_word_policy>;
constexpr auto to_lower_camel_snake_case = to_lower_camel_snake_case_t{};

struct convert_identifier_t {
    template <impl::identifier_conversion_tag Tag, char_type CharT>
    static constexpr auto operator()(
        Tag tag,
        const CharT* identifier,
        const CharT* identifier_end,
        identifier_naming_rule naming_rule) -> std::basic_string<CharT>
    {
#define RBOX_IDENTIFIER_NAMING_CASE(rule) \
    case identifier_naming_rule::rule:    \
        return to_##rule(tag, identifier, identifier_end);

        switch (naming_rule) {
            RBOX_IDENTIFIER_NAMING_RULES_FOR_EACH(RBOX_IDENTIFIER_NAMING_CASE)
            default:
                RBOX_ERROR_IF_CONSTEVAL("Invalid identifier naming rule");
                return std::basic_string<CharT>{};
        }
#undef RBOX_IDENTIFIER_NAMING_CASE
    }

    template <impl::identifier_conversion_tag Tag, string_like StringLike>
    static constexpr auto operator()(
        Tag tag, const StringLike& identifier, identifier_naming_rule naming_rule)
    {
        auto sv = make_string_view(identifier);
        return operator()(tag, sv.data(), sv.data() + sv.length(), naming_rule);
    }

    template <char_type CharT>
    static constexpr auto operator()(
        const CharT* identifier, const CharT* identifier_end, identifier_naming_rule naming_rule)
        -> std::basic_string<CharT>
    {
        return operator()(non_alpha_as_lower, identifier, identifier_end, naming_rule);
    }

    template <string_like StringLike>
    static constexpr auto operator()(
        const StringLike& identifier, identifier_naming_rule naming_rule)
    {
        return operator()(non_alpha_as_lower, identifier, naming_rule);
    }
};

constexpr auto convert_identifier = convert_identifier_t{};
}  // namespace rbox

#endif  // RBOX_UTILS_IDENTIFIER_NAMING_HPP
