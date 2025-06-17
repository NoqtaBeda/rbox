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

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <print>
#include <rbox/enum/enum_count.hpp>
#include <rbox/enum/enum_meta_entries.hpp>
#include <rbox/enum/enum_name.hpp>
#include <rbox/enum/enum_values.hpp>
#include <rbox/utils/config.hpp>

#include "./test_cases.hpp"

template <class E>
RBOX_ALWAYS_INLINE inline auto rbox_enum_name(E e) -> std::string_view
{
    return rbox::enum_name(e);
}

template <class E>
RBOX_ALWAYS_INLINE inline auto naive_enum_name(E e) -> std::string_view
{
    template for (constexpr auto entry : rbox::enum_meta_entries_v<E>)
    {
        if (e == [:entry:]) {
            return std::meta::identifier_of(entry);
        }
    }
    return "";
}

// Prevent optimization
RBOX_NO_INLINE void consume_output(std::string_view);

template <class E, auto Impl>
RBOX_NO_INLINE double run_benchmark(size_t n, unsigned seed)
{
    auto inputs = std::vector<E>(n);
    std::srand(seed);
    for (auto i = 0zU; i < n; i++) {
        inputs[i] = rbox::enum_values_v<E>[std::rand() % rbox::enum_count_v<E>];
    }

    auto start = std::chrono::steady_clock::now();
    for (auto i = 0zU; i < n; i++) {
        consume_output(Impl(inputs[i]));
    }
    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration<double>(end - start).count();
}

template <class E>
RBOX_NO_INLINE void run_and_report(std::string_view name, size_t n, unsigned seed)
{
    std::println("--- {} ({} entries) ---", name, rbox::enum_count_v<E>);

    auto t1 = run_benchmark<E, rbox_enum_name<E>>(n, seed);
    std::println("  rbox::enum_name: {:8.4f} s", t1);

    auto t2 = run_benchmark<E, naive_enum_name<E>>(n, seed);
    std::println("  naive enum_name: {:8.4f} s", t2);

    std::println("  Speedup:           {:5.2f}x", t2 / t1);
}

int main()
{
    constexpr size_t n = 100'000'000zU;
    std::println("Benchmark: enum_name ({} iterations)", n);
    std::println("==============================================================");

    srand(time(nullptr));
    unsigned seed = rand();

    run_and_report<small_continuous_t>("small_continuous_t", n, seed);
    run_and_report<small_almost_continuous_t>("small_almost_continuous_t", n, seed);
    run_and_report<small_sparse_t>("small_sparse_t", n, seed);

    run_and_report<continuous_t>("continuous_t", n, seed);
    run_and_report<almost_continuous_t>("almost_continuous_t", n, seed);
    run_and_report<sparse_t>("sparse_t", n, seed);

    run_and_report<huge_continuous_t>("huge_continuous_t", n, seed);
    run_and_report<huge_almost_continuous_t>("huge_almost_continuous_t", n, seed);
    run_and_report<huge_sparse_t>("huge_sparse_t", n, seed);
}
