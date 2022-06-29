//
// Created by luoxiaojian on 2022/1/4.
//

#include <type_traits>
#include <iostream>

template <typename T>
class Foo {
public:
    Foo() {}
    ~Foo() {}

    template <class Q = T>
    typename std::enable_if<std::is_same<Q, int>::value, void>::type foo() {
        std::cout << "is int" << std::endl;
    }

    template <class Q = T>
    typename std::enable_if<std::is_same<Q, float>::value, void>::type foo() {
        std::cout << "is float" << std::endl;
    }

    template <class Q = T>
    void foo() {
        std::cout << "is not" << std::endl;
    }
};

int main() {
    Foo<int> a;
    Foo<float> b;
    a.foo();
    b.foo();

    return 0;
}

