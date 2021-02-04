#pragma once

#include <iterator>
#include <numeric>
#include <thread>

template <typename It, typename T, class BinaryOperation>
void concurent_accumulate(It first, It last, T init, BinaryOperation op) {
    constexpr size_t MIN_BLOCK_SIZE = 1000;

    size_t distance = std::distance(first, last);
    size_t threads_number_by_elem_count =
        (distance + MIN_BLOCK_SIZE - 1) / MIN_BLOCK_SIZE;
    size_t threads_number_by_hardware = std::thread::hardware_concurrency();

    size_t threads_number =
        std::min(threads_number_by_elem_count, threads_number_by_hardware);
    size_t block_size = (distance + threads_number - 1) / threads_number;

    for (It first_it = first; first_it < last;) {
        It last_it = std::advance(first_it, block_size);
        std::thread(std::accumulate(first_it, last_it, , op));
    }
}