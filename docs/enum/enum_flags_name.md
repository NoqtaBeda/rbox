## Conversion from Enum Flag Value to String

Defined in header `<rbox/enum/enum_flags_name.hpp>`.

```cpp
namespace rbox {

enum class to_string_status {
    done = 0,
    invalid_input = 1,
    buffer_run_out = 2,
};

template <class Iter>
struct enum_flags_name_to_result {
    to_string_status status;
    Iter out;
};

struct enum_flags_name_t {
    // (1.1)
    template <enum_type E>
    static constexpr auto operator()(E e, char delim = '|') -> std::optional<std::string>;
    // (1.2)
    template <enum_type E>
    static constexpr auto operator()(E e, std::string_view delim) -> std::optional<std::string>;
};

struct enum_flags_name_to_t {
    // (2.1)
    template <std::output_iterator<char> Iter, std::sentinel_for<Iter> Sentinel, enum_type E>
    static constexpr auto operator()(Iter iter, Sentinel sentinel, E e, char delim = '|')
        -> enum_flags_name_to_result<Iter>;
    // (2.2)
    template <std::output_iterator<char> Iter, std::sentinel_for<Iter> Sentinel, enum_type E>
    static constexpr auto operator()(Iter iter, Sentinel sentinel, E e, std::string_view delim)
        -> enum_flags_name_to_result<Iter>;
    // (2.3)
    template <enum_type E>
    static constexpr auto operator()(std::ostream& out, E e, char delim = '|') -> to_string_status;
    // (2.4)
    template <enum_type E>
    static constexpr auto operator()(std::ostream& out, E e, std::string_view delim)
        -> to_string_status;
    // (2.5)
    template <enum_type E>
    static constexpr auto operator()(string_builder& dest, E e, char delim = '|')
        -> to_string_status;
    // (2.6)
    template <enum_type E>
    static constexpr auto operator()(string_builder& dest, E e, std::string_view delim)
        -> to_string_status;
};

constexpr auto enum_flags_name = enum_flags_name_t{};
constexpr auto enum_flags_name_opt = enum_flags_name_t{};  // Alias of enum_flags_name
constexpr auto enum_flags_name_to = enum_flags_name_to_t{};

}  // namespace rbox
```

- (1.1, 1.2) `enum_flags_name(e, delim)` gets the string representation of given enum flags value, entries separated by given delimiter (which is `'|'` by default). If `e` cannot be decomposed into a disjunction of defined enum entries, `std::nullopt` is returned;
- (2.1, 2.2) `enum_flags_name_to(iter, sentinel, e, delim)` writes the string representation of given enum flags value to buffer `[iter, sentinel)`, entries separated by given delimiter (which is `'|'` by default). Return value contains two fields `{status, out}` by the following rules:
  - If `e` can be decomposed into a disjunction of defined enum entries, and given buffer `[iter, sentinel)` is enough to hold the result string, then `status = to_string_status::done` and `out` points to end position of the string written, i.e. `[iter, out)` contains the result string;
  - If `e` can be decomposed into a disjunction of defined enum entries, but given buffer `[iter, sentinel)` cannot hold the result string, then `status = to_string_status::buffer_run_out` and `out` (along with the partial string written to `[iter, out)`) is unspecified;
  - If `e` cannot be decomposed into a disjunction of defined enum entries, then `status = to_string_status::invalid_input` and `out = iter`, i.e. nothing will be written.
- (2.3, 2.4) `enum_flags_name_to(out, e, delim)` writes the string representation of given enum flags value to given output stream, entries separated by given delimiter (which is `'|'` by default). Return value is an error code by the following rules:
  - If `e` can be decomposed into a disjunction of defined enum entries, then `to_string_status::done` is returned;
  - Otherwise, `to_string_status::invalid_input` is returned and nothing is written to the given output stream.
- (2.5, 2.6) `enum_flags_name_to(dest, e, delim)` writes the string representation of given enum flags value to a `string_builder`, entries separated by given delimiter (which is `'|'` by default). The builder handles buffer management internally (reserves space before writing via `out_unsafe()`). Return value is `to_string_status` by the same rules as (2.3, 2.4).

For overload (2.1) and (2.2), [`std::unreachable_sentinel`](https://en.cppreference.com/w/cpp/iterator/unreachable_sentinel_t) can be used as the second argument for better performance (boundary checks eliminated) when it's ensured that the destination buffer is large enough to hold all possible values.

Example:

```cpp
enum class permissions {
    read = 1,
    write = 2,
    execute = 4,
};

// Possibly "write|read"
std::println("{}", *rbox::enum_flags_name(static_cast<permissions>(3)));
// Possibly "execute, read"
std::println("{}", *rbox::enum_flags_name(static_cast<permissions>(5), ", "));
// "Empty set" is still considered a valid value. Empty string is returned in this case.
static_assert(rbox::enum_flags_name(static_cast<permissions>(0)) == "");
// Invalid value
static_assert(rbox::enum_flags_name(static_cast<permissions>(8)) == std::nullopt);

char buffer[20] = {};
auto [status1, out1] =
    rbox::enum_flags_name_to(buffer, buffer + 16, static_cast<permissions>(6), " | ");
assert(status1 == rbox::to_string_status::done);
assert(out1 == buffer + 15);
printf("%s\n", buffer);  // Possibly "execute | write"

std::ranges::fill(buffer, '\0');
auto [status2, _] = rbox::enum_flags_name_to(buffer, buffer + 16, static_cast<permissions>(7));
assert(status2 == rbox::to_string_status::buffer_run_out);

std::ranges::fill(buffer, '\0');
auto [status3, _] = rbox::enum_flags_name_to(buffer, buffer + 16, static_cast<permissions>(-1));
assert(status3 == rbox::to_string_status::invalid_input);

// Possibly writes "execute | write | read" to stdout
auto status4 = rbox::enum_flags_name_to(std::cout, static_cast<permissions>(7), ", ");
assert(status4 == rbox::to_string_status::done);
// Nothing will be written to stdout
auto status5 = rbox::enum_flags_name_to(std::cout, static_cast<permissions>(15));
assert(status5 == rbox::to_string_status::invalid_input);

auto builder = rbox::string_builder{};
auto status6 = rbox::enum_flags_name_to(builder, static_cast<permissions>(3));
assert(status6 == rbox::to_string_status::done);
printf("%s\n", builder.strview().data());  // Possibly "write|read"

bool write_file_permissions_config(std::string_view file, std::string_view perm_string);
// Example of monadic operations
bool set_file_permissions(std::string_view file, permissions perms)
{
    return rbox::enum_flags_name(perms)
        .transform([file](std::string perms_string) {
            return write_file_permissions_config(file, perms_string);
        })
        .value_or(false);
}
```
