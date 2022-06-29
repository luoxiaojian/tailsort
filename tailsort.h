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

template <typename T>
T* sorted_dedup(T* begin, T* end) {
    int num = end - begin;
    int count = 0;
    for (int i = 1; i < num; ++i) {
        if (begin[i] == begin[i - 1]) {
            ++count;
        } else {
            begin[i - count] = std::move(begin[i]);
        }
    }
    return end - count;
}

template <typename T>
T* tailsort_dedup(T* begin, T* end, int unsorted, std::vector<T>& buffer, std::vector<T*>& loc) {
#if 0
    buffer.resize(unsorted);
    T* last = end - unsorted;
    std::move(last, end, buffer.data());
    std::sort(buffer.begin(), buffer.end());

    unsorted = sorted_dedup(buffer.data(), buffer.data() + unsorted) - buffer.data();
    // std::cout << "deduped unsorted = " << unsorted << std::endl;
    loc.clear();
    loc.resize(unsorted);
    // for (auto ptr = begin; ptr != last; ++ptr) {
    //     std::cout << ptr->neighbor << ", ";
    // }
    // std::cout << std::endl;
    T* ptr = begin;
    int shift = 0;
    for (int i = 0; i < unsorted; ++i) {
        auto& cur = buffer[i];
        // std::cout << cur.neighbor << ", ";
        while (*ptr < cur && ptr != last) {
            ++ptr;
        }
        if (*ptr == cur && ptr != last) {
            loc[i] = NULL;
        } else {
            loc[i] = ptr;
            ++shift;
        }
    }
    // std::cout << std::endl;
    // std::cout << "shift = " << shift << std::endl;
    T* ret = last + shift;
    for (int i = unsorted - 1; i >= 0; --i) {
        if (loc[i] == NULL) {
            continue;
        }
        std::move(loc[i], last, loc[i] + shift);
        *(loc[i] + shift - 1) = std::move(buffer[i]);
        last = loc[i];
        --shift;
    }
    return ret;
#else
    buffer.resize(unsorted);
    T* last = end - unsorted;
    std::move(last, end, buffer.data());
    std::sort(buffer.begin(), buffer.end());
    T* src = last - 1;
    for (int i = unsorted - 1; i >= 0; --i) {
        auto& cur = buffer[i];
        while (cur < *src && src >= begin) {
            *(--end) = std::move(*(src--));
        }
        if (cur == *src) {
            --src;
        }
        *(--end) = std::move(cur);
    }
    while (src >= begin) {
        *(--end) = std::move(*src--);
    }
    return end;
#endif
}

#endif //TAILSORT_TAILSORT_H
