#include <iostream>
#include <vector>
#include <string>

struct RefString {
    char* buffer;
    size_t length;
};

template <class T, class Enable = void>
struct CommImpl {
    static void send(const T& value) {
        std::cout << "base" << std::endl;
    }
};

template <class T>
struct CommImpl<T, typename std::enable_if<std::is_pod<T>::value>::type> {
    static void send(const T& value) {
        std::cout << "pod" << std::endl;
    }
};

template <class T>
struct CommImpl<std::vector<T>, typename std::enable_if<std::is_pod<T>::value>::type> {
    static void send(const std::vector<T>& value) {
        std::cout << "pod vector" << std::endl;
    }
};

template <class T>
struct CommImpl<std::vector<T>, typename std::enable_if<!std::is_pod<T>::value>::type> {
    static void send(const std::vector<T>& value) {
        std::cout << "non-pod vector" << std::endl;
    }
};

template <>
struct CommImpl<RefString, void> {
    static void send(const RefString& value) {
        std::cout << "refstring" << std::endl;
    }
};

template <typename T>
void Send(const T& value) {
    CommImpl<T>::send(value);
}

int main() {
    int a = 10;
    Send(a);
    RefString rs;
    Send(rs);
    std::vector<std::string> vecstr;
    Send(vecstr);
    std::vector<int> vecint;
    Send(vecint);

    return 0;
}
