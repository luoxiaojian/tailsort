//
// Created by luoxiaojian on 2021/12/30.
//

#include <stdlib.h>

#include <algorithm>
#include <memory>
#include <iostream>
#include <vector>
#include <type_traits>

template <typename T>
static typename std::enable_if<!std::is_trivially_move_constructible<T>::value, void>::type
construct_backward(T* begin1, T* end1, T* begin2) {
    while (begin1 != end1) {
        new(begin2++) T(std::move(*(begin1++)));
    }
}

template <typename T>
static typename std::enable_if<std::is_trivially_move_constructible<T>::value, void>::type
construct_backward(T* begin1, T* end1, T* begin2) {
    ptrdiff_t n = end1 - begin1;
    if (n > 0) {
        memcpy(begin2, begin1, n * sizeof(T));
    }
}

template <typename T>
static typename std::enable_if<!std::is_trivially_destructible<T>::value, void>::type
destroy(T* begin, T* end) {

}

template <typename T>
static typename std::enable_if<std::is_trivially_destructible<T>::value, void>::type
destroy(T* begin, T* end) {

}

class Foo {
public:
    Foo() {
        std::cout << "new Foo: " << reinterpret_cast<uint64_t>(this) << std::endl;
    }

    Foo(const Foo& rhs) {
        std::cout << "new Foo from &" << std::endl;
    }

    Foo(Foo&& rhs) {
        std::cout << "new Foo from &&" << std::endl;
    }

    ~Foo() {
        std::cout << "delete Foo: " << reinterpret_cast<uint64_t>(this) << std::endl;
    }
};

int main() {
    Foo* foos = static_cast<Foo*>(malloc(sizeof(Foo) * 3));
    for (int i = 0; i < 3; ++i) {
        new(&foos[i]) Foo;
    }

    Foo* array = static_cast<Foo*>(malloc(sizeof(Foo) * 3));
    std::cout << "before move" << std::endl;
    std::uninitialized_move(foos, foos + 3, array);
    // std::move(foos.data(), foos.data() + foos.size(), array);
    std::cout << "after move" << std::endl;

    return 0;
}