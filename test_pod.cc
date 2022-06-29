#include <type_traits>
#include <iostream>
#include <string>

struct Foo {
    Foo() = default;
    Foo(int len) : size(len) {
    }
    ~Foo() {}

    int* buffer;
    int size;
};


template <typename T>
class Vertex {
public:
    Vertex() = default;
    ~Vertex() = default;
    explicit Vertex(const T& value) noexcept : value_(value) {}
    // Vertex(const Vertex& rhs) noexcept : value_(rhs.value_) {}


    // inline Vertex& operator=(const Vertex& rhs) noexcept {
    //      value_ = rhs.value_;
    //      return *this;
    // }

    inline Vertex& operator=(const T& value) noexcept {
        value_ = value;
        return *this;
    }

    // inline Vertex& operator++() noexcept {
    //     value_++;
    //     return *this;
    // }

    // inline Vertex operator++(int) noexcept {
    //     Vertex res(value_);
    //     value_++;
    //     return res;
    // }

    // inline Vertex& operator--() noexcept {
    //     value_--;
    //     return *this;
    // }

    // inline Vertex operator--(int) noexcept {
    //     Vertex res(value_);
    //     value_--;
    //     return res;
    // }
    // inline bool operator==(const Vertex& rhs) const {
    //     return value_ == rhs.value_;
    // }

    // inline bool operator!=(const Vertex& rhs) const {
    //     return value_ != rhs.value_;
    // }

    // void Swap(Vertex& rhs) { std::swap(value_, rhs.value_); }

    // inline bool operator<(const Vertex& rhs) const { return value_ < rhs.value_; }

    inline T GetValue() const { return value_; }

    inline void SetValue(T value) { value_ = value; }
private:
    T value_;
};

int main() {
    std::cout << std::is_pod<int>::value << std::endl;
    std::cout << std::is_pod<Foo>::value << std::endl;
    std::cout << std::is_pod<std::string>::value << std::endl;
    std::cout << std::is_pod<Vertex<uint32_t>>::value << std::endl;
    return 0;
}