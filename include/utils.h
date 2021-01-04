/*
Author: Florian Schwarzl
Date_ 2020-11-23

Kat. Nr: 21
*/

#ifndef UTILS_H
#define UTILS_H

#include <mutex>
#include <iostream>

extern std::recursive_mutex out_mtx;

inline void println() {
    std::lock_guard<std::recursive_mutex> lg{out_mtx};
    std::cout << std::endl;
}

template<typename T, typename... Rest>
inline void println(const T& word, const Rest&... rest) {
    std::lock_guard<std::recursive_mutex> lg{out_mtx};
    std::cout << word << ' ';
    println(rest...);
}

#endif // UTILS_H