-- Copyright (c) 2026 NoqtaBeda (noqtabeda@163.com)
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.

add_rules("mode.debug", "mode.release")

add_requires("gtest", {
    config = { main = true }
})

-- Uses static assertion for supported unit test cases
option("static-test")
  set_default(false)
  set_showmenu(true)
  add_defines("ENABLE_STATIC_TEST")

function parse_test_case_path(path, group_prefix, path_prefix)
  local slash_pos = string.find(path, "/")
  if slash_pos then
    local group = group_prefix .. "/" .. string.sub(path, 1, slash_pos - 1)
    local target = group_prefix .. "-" .. string.gsub(path, "/", "-")
    local cpp_path = path_prefix .. "/" .. path .. ".cpp"
    return group, target, cpp_path
  else
    local group = group_prefix
    local target = group_prefix .. "-" .. path
    local cpp_path = path_prefix .. "/" .. path .. ".cpp"
    return group, target, cpp_path
  end
end

function make_test_case(path)
  local group_name, target_name, cpp_path =
    parse_test_case_path(path, "tests", "tests")

  target(target_name, function ()
    set_kind("binary")
    set_group(group_name)
    add_files(cpp_path)
    set_languages("c++26")
    add_includedirs("include", ".")
    add_packages("gtest")
    add_options("static-test")
  end)
end

function make_example(path)
  local group_name, target_name, cpp_path =
    parse_test_case_path(path, "examples", "examples")

  target(target_name, function ()
    set_kind("binary")
    set_group(group_name)
    add_files(cpp_path)
    set_languages("c++26")
    add_includedirs("include")
  end)
end

-- Test cases: recursively glob all .cpp files under tests/, excluding blacklist.
local test_blacklist = {
    -- GCC 16.1 internal compiler error
    "lookup/class_member/enum_key/test_custom_filter",
    "lookup/class_member/string_key/test_special_members",
}

local function is_blacklisted(path, blacklist)
    for _, bl in ipairs(blacklist) do
        if path == bl then
            return true
        end
    end
    return false
end

local test_files = os.files("tests/**/*.cpp")
for _, file in ipairs(test_files) do
    -- Strip "tests/" prefix and ".cpp" suffix to recover the logical path
    local path = string.sub(file, 7, -5)
    if not is_blacklisted(path, test_blacklist) then
        make_test_case(path)
    end
end

-- Examples: recursively glob all .cpp files under examples/.
local example_files = os.files("examples/**/*.cpp")
for _, file in ipairs(example_files) do
    local path = string.sub(file, 10, -5)  -- strip "examples/" and ".cpp"
    make_example(path)
end

target("benchmarks-enum-enum_name", function ()
  set_kind("binary")
  add_files("benchmarks/enum/enum_name.cpp",
            "benchmarks/enum/helpers/consume_output.cpp")
  set_languages("c++26")
  add_includedirs("include")
end)

target("benchmarks-enum-enum_cast", function ()
  set_kind("binary")
  add_files("benchmarks/enum/enum_cast.cpp",
            "benchmarks/enum/helpers/consume_output.cpp")
  set_languages("c++26")
  add_includedirs("include")
end)
