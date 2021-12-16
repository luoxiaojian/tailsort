#ifndef TAILSORT_TAILSORT_H
#define TAILSORT_TAILSORT_H

#include <algorithm>

template <typename T>
void shift(T* begin, T* end, int width) {
#if 0
    std::move(begin, end, begin + width);
#else
    T* new_begin = begin + width;
    int last_index = end - begin - 1;
    while (last_index >= 0) {
        new_begin[last_index] = std::move(begin[last_index]);
        --last_index;
    }
#endif
}

template <typename T>
void tailsort(T* begin, T* end, int unsorted, std::vector<T>& buffer) {
    buffer.clear();
    T* last = end - unsorted;
    for (int i = 0; i < unsorted; ++i) {
        buffer.emplace_back(last[i]);
    }
    std::sort(buffer.begin(), buffer.end());

    for (int i = 0; i < unsorted; ++i) {
        T* loc = std::lower_bound(begin, last, buffer[unsorted - i - 1]);
        shift(loc, last, unsorted - i);
        loc[unsorted - i - 1] = std::move(buffer[unsorted - i - 1]);
        last = loc;
    }
}

#endif //TAILSORT_TAILSORT_H
