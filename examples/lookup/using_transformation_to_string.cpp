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

#include <print>
#include <rbox/lookup.hpp>
#include <rbox/utils/identifier_naming.hpp>
#include <rbox/utils/meta_utility.hpp>  // all_direct_nonstatic_data_members_v
#include <string>

struct RequestArgs {
    std::string category;
    std::string nameRegex;
    std::string startDateTime;
    std::string endDateTime;

    void DumpFields() const
    {
        template for (constexpr auto field : rbox::all_direct_nonstatic_data_members_v<RequestArgs>)
        {
            std::println("Field '{}' is set to {:?}", identifier_of(field), this->[:field:]);
        }
    }
};

int main()
{
    // Invokes overload (2.2):
    // rbox::make_public_nonstatic_data_member_fixed_map(
    //    T, pattern, transform_fn, options, ctx)
    constexpr auto kFieldsTable = RBOX_PUBLIC_NONSTATIC_DATA_MEMBER_FIXED_MAP(
        // 1st arg: The class whose members are to be traversed
        RequestArgs,
        // 2nd arg: The name pattern to be matched.
        // If a member has identifier which matches this pattern,
        // the matched part is passed to the 3rd arg.
        // Otherwise, the member is filtered out.
        // In this example, we use the trivial pattern "*" to match every non-static data member.
        "*",
        // 3rd arg: Name transformer function
        // In this example, the transformer is a function object defined in
        // <rbox/utils/identifier_naming.hpp>
        // with call signature: (std::string_view) -> std::optional<std::string>
        // This function object converts the names to kebab-case.
        rbox::to_kebab_case_opt
        // 4th arg (optional): Configuration of the underlying fixed map.

        // 5th arg (optional): Access context, std::meta::access_context::current() by default.
    );

    // Use decltype(kFieldsTable)::value_type to get the value type.
    static_assert(std::is_same_v<std::string RequestArgs::*, decltype(kFieldsTable)::value_type>);
    // Use size() member function to get the size, i.e. how many entries in the table.
    static_assert(kFieldsTable.size() == 4);

    auto args = RequestArgs{};
    // Accesses the table with operator[]. Null check is omitted in this example.
    args.*kFieldsTable["category"] = "Cats";
    args.*kFieldsTable["name-regex"] = "[A-Za-z]+";
    args.*kFieldsTable["start-date-time"] = "2026-01-01T00:00:00+08:00";
    args.*kFieldsTable["end-date-time"] = "2026-01-15T00:00:00+08:00";

    // Member functions are filtered out.
    static_assert(kFieldsTable["DumpFields"] == nullptr);
    static_assert(kFieldsTable["dump-fields"] == nullptr);

    // Prints all the fields of args.
    args.DumpFields();

    return 0;
}

// Expected output:
// Field 'category' is set to "Cats"
// Field 'nameRegex' is set to "[A-Za-z]+"
// Field 'startDateTime' is set to "2026-01-01T00:00:00+08:00"
// Field 'endDateTime' is set to "2026-01-15T00:00:00+08:00"
