//
// Created by luoxiaojian on 2021/12/31.
//

#include <vector>
#include <iostream>
#include <algorithm>

struct Nbr {
    Nbr(int n) : neighbor(n) {}

    int neighbor;
};

template <typename NBR_T>
struct Op {
    static bool lt(const NBR_T& lhs, const NBR_T& rhs) {
        return lhs.neighbor < rhs.neighbor;
    }

    static bool gt(const NBR_T& lhs, const NBR_T& rhs) {
        return lhs.neighbor > rhs.neighbor;
    }
};

int main() {

    std::vector<Nbr> vec;
    vec.emplace_back(3);
    vec.emplace_back(1);
    vec.emplace_back(2);

    std::sort(vec.begin(), vec.end(), Op<Nbr>::lt);
    for (auto& v : vec) {
        std::cout << v.neighbor << std::endl;
    }

    std::sort(vec.begin(), vec.end(), Op<Nbr>::gt);
    for (auto& v : vec) {
        std::cout << v.neighbor << std::endl;
    }
    return 0;
}

