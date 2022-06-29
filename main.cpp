#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "tailsort.h"
#include "timsort.h"

struct Nbr {
    uint32_t neighbor;
    int data;
};

inline bool operator<(const Nbr& lhs, const Nbr& rhs) {
    return lhs.neighbor < rhs.neighbor;
}

inline bool operator==(const Nbr& lhs, const Nbr& rhs) {
    return lhs.neighbor == rhs.neighbor;
}

inline bool operator!=(const Nbr& lhs, const Nbr& rhs) {
    return lhs.neighbor != rhs.neighbor;
}

void init_edge_list(uint32_t vnum, int sorted_degree, double rate, std::vector<Nbr>& edges) {
    int degree = sorted_degree * (1 + rate);
    edges.clear();
    edges.resize(vnum * degree);
    std::vector<uint32_t> dst_list(degree);

    for (int i = 0; i < degree; ++i) {
        dst_list[i] = i;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    Nbr* ptr = edges.data();
    for (uint32_t i = 0; i < vnum; ++i) {
        std::shuffle(dst_list.begin(), dst_list.end(), gen);
        std::sort(dst_list.begin(), dst_list.begin() + sorted_degree);
        for (auto dst : dst_list) {
            ptr->neighbor = dst + i;
            ++ptr;
        }
    }
}

void init_dup_edge_list(uint32_t vnum, int degree0, int degree1, int degree2, std::vector<Nbr>& edges) {
    int total_degree = degree0 + degree1 + degree2;
    edges.clear();
    edges.resize(vnum * total_degree);
    std::vector<uint32_t> dst_list(total_degree);
    int dedup_degree = degree0 + degree1;

    std::random_device rd;
    std::mt19937 gen(rd());

    Nbr* ptr = edges.data();
    for (uint32_t i = 0; i < vnum; ++i) {
        for (int k = 0; k < dedup_degree; ++k) {
            dst_list[k] = i + k;
        }
        std::shuffle(dst_list.begin(), dst_list.begin() + dedup_degree, gen);
        std::sort(dst_list.begin(), dst_list.begin() + degree0);
        for (int k = dedup_degree; k < total_degree; ++k) {
            dst_list[k] = dst_list[k - dedup_degree];
        }
        std::shuffle(dst_list.begin() + dedup_degree, dst_list.end(), gen);

        for (auto dst : dst_list) {
            ptr->neighbor = dst + i;
            ++ptr;
        }
    }
}

void dedup_case0(uint32_t vnum, int sorted_degree, int unsort, std::vector<Nbr>& edges, int dedup_degree) {
    Nbr* ptr = edges.data();
    int total_degree = sorted_degree + unsort;
    for (uint32_t i = 0; i < vnum; ++i) {
        std::sort(ptr, ptr + total_degree);
        Nbr* last = sorted_dedup(ptr, ptr + total_degree);
        if (ptr + dedup_degree != last) {
            std::cout << "error in case0, " << (last - ptr) << " v.s. " << dedup_degree << std::endl;
        }
        ptr += total_degree;
    }
}

void dedup_case1(uint32_t vnum, int sorted_degree, int unsort, std::vector<Nbr>& edges, int dedup_degree) {
    Nbr* ptr = edges.data();
    int total_degree = sorted_degree + unsort;
    for (uint32_t i = 0; i < vnum; ++i) {
        gfx::timsort(ptr, ptr + total_degree);
        Nbr* last = sorted_dedup(ptr, ptr + total_degree);
        if (ptr + dedup_degree != last) {
            std::cout << "error in case1" << std::endl;
        }
        ptr += total_degree;
    }
}

void dedup_case2(uint32_t vnum, int sorted_degree, int unsort, std::vector<Nbr>& edges, int dedup_degree) {
    Nbr* ptr = edges.data();
    int total_degree = sorted_degree + unsort;
    for (uint32_t i = 0; i < vnum; ++i) {
        gfx::timmerge(ptr, ptr + sorted_degree, ptr + total_degree);
        Nbr* last = sorted_dedup(ptr, ptr + total_degree);
        if (ptr + dedup_degree != last) {
            std::cout << "error in case2" << std::endl;
        }
        ptr += total_degree;
    }
}

void dedup_case3(uint32_t vnum, int sorted_degree, int unsort, std::vector<Nbr>& edges, int dedup_degree) {
    Nbr* ptr = edges.data();
    int total_degree = sorted_degree + unsort;
    std::vector<Nbr> buffer;
    for (uint32_t i = 0; i < vnum; ++i) {
        tailsort(ptr, ptr + total_degree, unsort, buffer);
        Nbr* last = sorted_dedup(ptr, ptr + total_degree);
        if (ptr + dedup_degree != last) {
            std::cout << "error in case3" << std::endl;
        }
        ptr += total_degree;
    }
}

void dedup_case4(uint32_t vnum, int sorted_degree, int unsort, std::vector<Nbr>& edges, int dedup_degree) {
    Nbr* ptr = edges.data();
    int total_degree = sorted_degree + unsort;
    std::vector<Nbr> buffer;
    std::vector<Nbr*> loc;
    for (uint32_t i = 0; i < vnum; ++i) {
        Nbr* last = tailsort_dedup(ptr, ptr + total_degree, unsort, buffer, loc);
        if (ptr + total_degree - dedup_degree != last) {
            std::cout << "error in case4, " << (last - ptr) << " v.s. " << dedup_degree << std::endl;
        }
        ptr += total_degree;
    }
}

void std_case(uint32_t vnum, int sorted_degree, int unsort, std::vector<Nbr>& edges) {
    Nbr* ptr = edges.data();
    int total_degree = sorted_degree + unsort;
    for (uint32_t i = 0; i < vnum; ++i) {
        std::sort(ptr, ptr + total_degree);
        ptr += total_degree;
    }
}

void tail_case(uint32_t vnum, int sorted_degree, int unsort, std::vector<Nbr>& edges) {
    Nbr* ptr = edges.data();
    int total_degree = sorted_degree + unsort;
    std::vector<Nbr> buffer;
    for (uint32_t i = 0; i < vnum; ++i) {
        tailsort(ptr, ptr + total_degree, unsort, buffer);
        ptr += total_degree;
    }
}

void tim_case(uint32_t vnum, int sorted_degree, int unsort, std::vector<Nbr>& edges) {
    Nbr* ptr = edges.data();
    int total_degree = sorted_degree + unsort;
    for (uint32_t i = 0; i < vnum; ++i) {
        Nbr* mid = ptr + sorted_degree;
        Nbr* end = mid + unsort;
        std::sort(mid, end);
        gfx::timmerge(ptr, mid, end);
        // gfx::timsort(ptr, ptr + total_degree);
        ptr += total_degree;
    }
}

template <typename T>
size_t check_correctness(const std::vector<T>& lhs, const std::vector<T>& rhs) {
    if (lhs.size() != rhs.size()) {
        std::cout << "size not same" << std::endl;
        return 0;
    }
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) {
            std::cout << "elem-" << i << " not same" << std::endl;
            return i;
        }
    }
    // std::cout << "same" << std::endl;
    return 0;
}

template <typename T>
void check_correctness(const std::vector<T>& lhs, const std::vector<T>& rhs, int chunk, int load) {
    if (lhs.size() != rhs.size()) {
        std::cout << "size not same" << std::endl;
        return ;
    }
    size_t chunk_num = lhs.size() / chunk;
    const T* src_ptr = lhs.data();
    const T* dst_ptr = rhs.data();
    for (size_t i = 0; i < chunk_num; ++i) {
        for (int j = 0; j < load; ++j) {
            if (src_ptr[j] != dst_ptr[j]) {
                std::cout << "error" << std::endl;
                for (int k = 0; k < load; ++k) {
                    std::cout << src_ptr[k].neighbor << " v.s. " << dst_ptr[k].neighbor << std::endl;
                }
                return ;
            }
        }
        src_ptr += chunk;
        dst_ptr += chunk;
    }
    // std::cout << "same" << std::endl;
}

void benchmark1(uint32_t vnum, int sorted_degree, double rate) {
    std::vector<Nbr> edges1;
    init_edge_list(vnum, sorted_degree, rate, edges1);
    std::vector<Nbr> edges2(edges1);
    std::vector<Nbr> edges3(edges1);

    int total_degree = sorted_degree * (1 + rate);

    auto t0 = std::chrono::system_clock::now();
    std_case(vnum, sorted_degree, total_degree - sorted_degree, edges1);
    auto t1 = std::chrono::system_clock::now();
    tail_case(vnum, sorted_degree, total_degree - sorted_degree, edges2);
    auto t2 = std::chrono::system_clock::now();
    tim_case(vnum, sorted_degree, total_degree - sorted_degree, edges3);
    auto t3 = std::chrono::system_clock::now();

    auto duration0 = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2);

    double time0 = static_cast<double>(duration0.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
    double time1 = static_cast<double>(duration1.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
    double time2 = static_cast<double>(duration2.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

    std::cout << "degree = " << sorted_degree << " + " << (total_degree - sorted_degree) << std::endl;
    std::cout << "std: " << time0 << " (s), tail: " << time1 << " (s), tim: " << time2 << std::endl;

    size_t loc = check_correctness(edges1, edges2);
    if (loc != 0) {
        size_t begin = (loc / total_degree) * total_degree;
        for (int i = 0; i < total_degree; ++i) {
            std::cout << edges1[i + begin].neighbor << " v.s. " << edges2[i + begin].neighbor << std::endl;
        }
    }
}

void benchmark2(uint32_t vnum, int degree0, int degree1, int degree2) {
    std::vector<Nbr> edges;
    init_dup_edge_list(vnum, degree0, degree1, degree2, edges);

    std::cout << "degree = " << degree0 << " + " << degree1 << " + " << degree2 << std::endl;

    auto t0 = std::chrono::system_clock::now();
    std::vector<Nbr> edges1(edges);
    dedup_case0(vnum, degree0, degree1 + degree2, edges1, degree0 + degree1);
    auto t1 = std::chrono::system_clock::now();
    auto duration0 = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
    double time0 = static_cast<double>(duration0.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
    std::cout << "std: " << time0 << " (s)" << std::endl;

    {
        auto t2 = std::chrono::system_clock::now();
        std::vector<Nbr> edges2(edges);
        dedup_case1(vnum, degree0, degree1 + degree2, edges2, degree0 + degree1);
        check_correctness(edges1, edges2, degree0 + degree1 + degree2, degree0 + degree1);
        auto t3 = std::chrono::system_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2);
        double time2 = static_cast<double>(duration2.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

        std::cout << "timsort: " << time2 << " (s)" << std::endl;
    }

    // {
    //     auto t2 = std::chrono::system_clock::now();
    //     std::vector<Nbr> edges2(edges);
    //     dedup_case2(vnum, degree0, degree1 + degree2, edges2, degree0 + degree1);
    //     check_correctness(edges1, edges2, degree0 + degree1 + degree2, degree0 + degree1);
    //     auto t3 = std::chrono::system_clock::now();
    //     auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2);
    //     double time2 = static_cast<double>(duration2.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

    //     std::cout << "timmerge: " << time2 << " (s)" << std::endl;
    // }

    {
        auto t2 = std::chrono::system_clock::now();
        std::vector<Nbr> edges2(edges);
        dedup_case3(vnum, degree0, degree1 + degree2, edges2, degree0 + degree1);
        check_correctness(edges1, edges2, degree0 + degree1 + degree2, degree0 + degree1);
        auto t3 = std::chrono::system_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2);
        double time2 = static_cast<double>(duration2.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

        std::cout << "tailsort: " << time2 << " (s)" << std::endl;
    }

    {
        auto t2 = std::chrono::system_clock::now();
        std::vector<Nbr> edges2(edges);
        dedup_case4(vnum, degree0, degree1 + degree2, edges2, degree0 + degree1);
        // check_correctness(edges1, edges2, degree0 + degree1 + degree2, degree0 + degree1);
        auto t3 = std::chrono::system_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2);
        double time2 = static_cast<double>(duration2.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

        std::cout << "tailsort_dedup: " << time2 << " (s)" << std::endl;
    }
}

#if 0
void benchmark2(uint32_t vnum, int sorted_degree, double rate) {
    std::vector<Nbr> edges1;
    init_edge_list(vnum, sorted_degree, rate, edges1);
    std::vector<Nbr> edges2(edges1);

    int total_degree = sorted_degree * (1 + rate);

    auto t0 = std::chrono::system_clock::now();
    tail_case(vnum, sorted_degree, total_degree - sorted_degree, edges2);
    auto t1 = std::chrono::system_clock::now();
    std_case(vnum, sorted_degree, total_degree - sorted_degree, edges1);
    auto t2 = std::chrono::system_clock::now();

    auto duration0 = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

    double time0 = static_cast<double>(duration0.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
    double time1 = static_cast<double>(duration1.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

    std::cout << "std: " << time1 << " (s), tail: " << time0 << " (s)" << std::endl;

    check_correctness(edges1, edges2);

    size_t loc = check_correctness(edges1, edges2);
    if (loc != 0) {
        size_t begin = (loc / total_degree) * total_degree;
        for (int i = 0; i < total_degree; ++i) {
            std::cout << edges2[i + begin].neighbor << " v.s. " << edges1[i + begin].neighbor << std::endl;
        }
    }
}
#endif

int main() {
#if 0
    std::vector<int> vec = {1, 2, 3, 4, 8, 9, 10, 0, 5, 6, 7};
    std::vector<int> buf;

    tailsort(vec.data(), vec.data() + vec.size(), 4, buf);

    for (auto v : vec) {
        std::cout << v << std::endl;
    }
#else
    // benchmark1(1000000, 64, 0.125);
    // benchmark1(1000000, 64, 0.25);
    // benchmark1(1000000, 64, 0.375);
    // benchmark1(1000000, 64, 0.5);
    // benchmark1(1000000, 64, 0.625);
    // benchmark1(1000000, 64, 0.75);
    // benchmark1(1000000, 64, 0.875);
    benchmark1(1000000, 64, 1.0);
    // benchmark1(1000000, 64, 1.125);
    // benchmark1(1000000, 64, 1.25);
    // benchmark1(1000000, 64, 1.375);
    // benchmark1(1000000, 64, 1.5);
    // benchmark1(1000000, 64, 1.625);
    // benchmark1(1000000, 64, 1.75);
    // benchmark1(1000000, 64, 1.875);
    benchmark1(1000000, 64, 2.0);
    benchmark1(1000000, 64, 3.0);
    benchmark1(1000000, 64, 4.0);
    // benchmark1(1000000, 128, 0.125);
    // benchmark1(1000000, 128, 0.25);
    // benchmark1(1000000, 128, 0.375);
    // benchmark1(1000000, 128, 0.5);
    // benchmark1(1000000, 128, 0.625);
    // benchmark1(1000000, 128, 0.75);
    // benchmark1(1000000, 128, 0.875);
    // benchmark1(1000000, 128, 1.0);

    // benchmark2(3, 16, 4, 2);
    // benchmark2(1000000, 64, 6, 2);
    // benchmark2(1000000, 64, 12, 4);
    // benchmark2(1000000, 64, 18, 6);
    // benchmark2(1000000, 64, 24, 8);
#endif
    return 0;
}
