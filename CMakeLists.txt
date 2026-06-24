# Copyright (c) 2026 NoqtaBeda (noqtabeda@163.com)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.25)
project(rbox VERSION 1.1.0 LANGUAGES CXX)

# =============================================================================
# Options
# =============================================================================
option(RBOX_STATIC_TEST "Enable static assertion tests" OFF)

# =============================================================================
# C++ Standard
# =============================================================================
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =============================================================================
# Compiler flags (GCC with reflection)
# =============================================================================
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-freflection)
endif()

# =============================================================================
# rbox header-only library target
# =============================================================================
add_library(rbox INTERFACE)
target_include_directories(rbox INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# =============================================================================
# Tests, examples, benchmarks — only when rbox is the top-level project
# =============================================================================
if(PROJECT_IS_TOP_LEVEL)
    # =============================================================================
    # Google Test (FetchContent)
    # =============================================================================
    include(FetchContent)
    fetchcontent_declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    fetchcontent_makeavailable(googletest)

    # =============================================================================
    # Helper: create test target
    # =============================================================================
    function(make_test test_path)
        string(REPLACE "/" "-" target_name "${test_path}")
        set(target_name "tests-${target_name}")
        set(cpp_file "${CMAKE_CURRENT_SOURCE_DIR}/tests/${test_path}.cpp")

        add_executable(${target_name} "${cpp_file}")
        target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
        target_link_libraries(${target_name} PRIVATE rbox gtest_main gmock_main)

        if(RBOX_STATIC_TEST)
            target_compile_definitions(${target_name} PRIVATE ENABLE_STATIC_TEST)
        endif()

        add_test(NAME ${target_name} COMMAND ${target_name})
    endfunction()

    # =============================================================================
    # Helper: create example target
    # =============================================================================
    function(make_example example_path)
        string(REPLACE "/" "-" target_name "${example_path}")
        set(target_name "examples-${target_name}")
        set(cpp_file "${CMAKE_CURRENT_SOURCE_DIR}/examples/${example_path}.cpp")

        add_executable(${target_name} "${cpp_file}")
        target_link_libraries(${target_name} PRIVATE rbox)
    endfunction()

    # =============================================================================
    # Helper: create compile-time benchmark (shared library)
    # =============================================================================
    function(make_compile_benchmark benchmark_path)
        string(REPLACE "/" "-" bench_name "${benchmark_path}")
        set(target_name "benchmarks-compile-${bench_name}")
        set(cpp_file "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/compile/${benchmark_path}.cpp")

        add_library(${target_name} SHARED "${cpp_file}")
        target_link_libraries(${target_name} PRIVATE rbox)
        set_target_properties(${target_name} PROPERTIES
            CXX_VISIBILITY_PRESET hidden
        )
    endfunction()

    # =============================================================================
    # UT Blacklist
    # =============================================================================
    set(TEST_BLACKLIST)

    # Still fails due to internal bug (with GCC trunk at 2026.6.22)
    list(APPEND TEST_BLACKLIST
        lookup/class_member/string_key/test_special_members
    )
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # GCC 16.1 fails to work due to internal bug
        # (with reflection of functions when the return type is reference)
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS_EQUAL 16.1)
            list(APPEND TEST_BLACKLIST
                lookup/class_member/enum_key/test_custom_filter
                lookup/class_member/string_key/test_access_context_nsdm_1
                lookup/class_member/string_key/test_access_context_nsdm_2
            )
        endif()
    endif()

    # =============================================================================
    # Discover and create all test targets
    # =============================================================================
    file(GLOB_RECURSE test_sources
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    )
    foreach(test_cpp ${test_sources})
        file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}/tests" "${test_cpp}")
        string(REGEX REPLACE "\\.cpp$" "" test_path "${rel_path}")

        list(FIND TEST_BLACKLIST "${test_path}" _idx)
        if(_idx GREATER_EQUAL 0)
            message(STATUS "Skipping blacklisted test: ${test_path}")
            continue()
        endif()

        make_test("${test_path}")
    endforeach()

    # =============================================================================
    # Discover and create all example targets
    # =============================================================================
    file(GLOB_RECURSE example_sources
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp"
    )
    foreach(example_cpp ${example_sources})
        file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}/examples" "${example_cpp}")
        string(REGEX REPLACE "\\.cpp$" "" example_path "${rel_path}")

        make_example("${example_path}")
    endforeach()

    # =============================================================================
    # Benchmarks
    # =============================================================================
    add_executable(benchmarks-enum-enum_name
        benchmarks/enum/enum_name.cpp
        benchmarks/enum/helpers/consume_output.cpp
    )
    target_link_libraries(benchmarks-enum-enum_name PRIVATE rbox)

    add_executable(benchmarks-enum-enum_cast
        benchmarks/enum/enum_cast.cpp
        benchmarks/enum/helpers/consume_output.cpp
    )
    target_link_libraries(benchmarks-enum-enum_cast PRIVATE rbox)

    file(GLOB_RECURSE compile_bench_sources
        "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/compile/fixed_map/*.cpp"
    )
    foreach(bench_cpp ${compile_bench_sources})
        file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/compile" "${bench_cpp}")
        string(REGEX REPLACE "\\.cpp$" "" benchmark_path "${rel_path}")

        make_compile_benchmark("${benchmark_path}")
    endforeach()

    enable_testing()
endif()
