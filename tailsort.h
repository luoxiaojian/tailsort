#ifndef TAILSORT_TAILSORT_H
#define TAILSORT_TAILSORT_H

#include <algorithm>

template <typename T>
void shift(T* begin, T* end, int width) {
#if 1
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
    buffer.resize(unsorted);
    T* last = end - unsorted;
    std::move(last, end, buffer.data());
    std::sort(buffer.begin(), buffer.end());

#if 0
    for (int i = 0; i < unsorted; ++i) {
        T* loc = std::lower_bound(begin, last, buffer[unsorted - i - 1]);
        shift(loc, last, unsorted - i);
        loc[unsorted - i - 1] = std::move(buffer[unsorted - i - 1]);
        last = loc;
    }
#else
    T* src = last - 1;
    for (int i = unsorted - 1; i >= 0; --i) {
        auto& cur = buffer[i];
        while (cur < *src && src >= begin) {
            *(--end) = std::move(*(src--));
        }
        *(--end) = std::move(cur);
    }
#endif
}

#endif //TAILSORT_TAILSORT_H
