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

#include <functional>
#include <print>
#include <rbox/lookup.hpp>
#include <string>

struct dog_t {
    // Identifier "name" does not match the pattern "eat_*". Ignored.
    std::string name;

    // Identifier "eat_apple" matches the pattern "eat_*". The matched part '*' -> "apple".
    void eat_apple(int n) const
    {
        std::println("My dog {} eats {} apple(s).", name, n);
    }

    // Similarly: '*' -> "banana"
    void eat_banana(int n) const
    {
        std::println("My dog {} eats {} banana(s).", name, n);
    }

    // Similarly: '*' -> "orange"
    void eat_orange(int n) const
    {
        std::println("My dog {} eats {} orange(s).", name, n);
    }

    // Identifier "eat" does not match the pattern "eat_*". Ignored.
    void eat(std::string_view type, int n) const
    {
        // How does it work:
        // For each addressable member whose identifier matches pattern "eat_*",
        // the matched part is key and pointer to the member is value.

        // Invokes overload (1):
        // rbox::make_public_nonstatic_data_member_fixed_map(
        //    T, pattern, options, ctx)
        constexpr auto dispatch_table = RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(
            // 1st arg: The class whose members are to be traversed
            dog_t,
            // 2nd arg: The name pattern to be matched.
            // If a member has identifier which matches this pattern, the matched part is the key.
            // Otherwise, the member is filtered out.
            "eat_*"
            // 3rd arg (optional): Configuration of the underlying fixed map.

            // 4th arg (optional): Access context, std::meta::access_context::current() by default.
            // Here we use the default value. Current access context has access to private members
            // of dog_t, thus &dog_t::eat_watermelon can be added to the table.
        );
        // Contents in dispatch_table:
        //   ("apple", &dog_t::eat_apple)
        //   ("banana", &dog_t::eat_banana)
        //   ("orange", &dog_t::eat_orange)
        //   ("watermelon", &dog_t::eat_watermelon)

        // Use decltype(dispatch_table)::value_type to get the value type.
        static_assert(
            std::is_same_v<void (dog_t::*)(int) const, decltype(dispatch_table)::value_type>);
        // Use size() member function to get the size, i.e. how many entries in the table.
        static_assert(dispatch_table.size() == 4);

        // Use operator[] to access the corresponding value, i.e. member function pointer.
        if (auto mptr = dispatch_table[type]; mptr != nullptr) {
            // mptr != nullptr: type is a key in dispatch_table
            std::invoke(mptr, this, n);
        } else {
            // mptr == nullptr: type is not a key in dispatch_table
            std::println("My dog {} can not eat this kind of fruit.", name);
        }
    }

private:
    // '*' -> "watermelon"
    void eat_watermelon(int n) const
    {
        std::println(
            "My dog {} eats {} WATERMELON(S)! OH MY GOD! DON'T TELL OTHER PEOPLE!", name, n);
    }
};

int main()
{
    auto my_dog = dog_t{"Mr. Foobar"};
    my_dog.eat("apple", 3);
    my_dog.eat("banana", 4);
    my_dog.eat("watermelon", 5);
    my_dog.eat("mango", 6);
    return 0;
}

// Expected output:
// My dog Mr. Foobar eats 3 apple(s).
// My dog Mr. Foobar eats 4 banana(s).
// My dog Mr. Foobar eats 5 WATERMELON(S)! OH MY GOD! DON'T TELL OTHER PEOPLE!
// My dog Mr. Foobar can not eat this kind of fruit.
