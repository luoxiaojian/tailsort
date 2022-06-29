//
// Created by luoxiaojian on 2022/1/5.
//

#include <iostream>

template <typename VID_T>
class Vertex {
public:
    Vertex() noexcept {}
    explicit Vertex(const VID_T& value) noexcept : value_(value) {}
    Vertex(const Vertex& rhs) noexcept : value_(rhs.value_) {}
    ~Vertex() {}

    inline Vertex& operator=(const Vertex& rhs) noexcept {
        value_ = rhs.value_;
        return *this;
    }

    inline Vertex& operator++() noexcept {
        value_++;
        return *this;
    }

    inline Vertex operator++(int) noexcept {
        Vertex res(value_);
        value_++;
        return res;
    }

    inline Vertex& operator--() noexcept {
        value_--;
        return *this;
    }

    inline Vertex operator--(int) noexcept {
        Vertex res(value_);
        value_--;
        return res;
    }

    inline Vertex operator+(size_t offset) const noexcept {
        Vertex res(value_ + offset);
        return res;
    }

    inline bool operator==(const Vertex& rhs) const noexcept {
        return value_ == rhs.value_;
    }

    inline bool operator!=(const Vertex& rhs) const noexcept {
        return value_ != rhs.value_;
    }

    void Swap(Vertex& rhs) noexcept { std::swap(value_, rhs.value_); }

    inline bool operator<(const Vertex& rhs) const noexcept
    { return value_ < rhs.value_; }

    inline const VID_T& GetValue() const noexcept { return value_; }

    inline void SetValue(const VID_T& value) noexcept { value_ = value; }

private:
    VID_T value_;
};

template <typename VID_T>
class VertexRange {
public:
    VertexRange() {}
    VertexRange(const VID_T& begin, const VID_T& end) : begin_(begin), end_(end) {}
    VertexRange(const VertexRange& rhs) : begin_(rhs.begin_), end_(rhs.end_) {}

    class iterator {
        using reference_type = Vertex<VID_T>&;
    private:
        Vertex<VID_T> cur_;
    public:
        iterator() noexcept : cur_() {}
        explicit iterator(const VID_T& v) noexcept : cur_(v) {}

        inline reference_type operator*() noexcept { return cur_; }

        inline iterator& operator++() noexcept {
            ++cur_;
            return *this;
        }

        inline iterator operator++(int) noexcept {
            return iterator(cur_.GetValue()++);
        }

        inline iterator& operator--() noexcept {
            --cur_;
            return *this;
        }

        inline iterator operator--(int) noexcept {
            return iterator(cur_.GetValue()--);
        }

        iterator operator+(size_t offset) noexcept {
            return iterator(cur_.GetValue() + offset);
        }

        bool operator==(const iterator& rhs) noexcept {
            return cur_ == rhs.cur_;
        }

        bool operator!=(const iterator& rhs) noexcept {
            return cur_ != rhs.cur_;
        }
    };

    inline iterator begin() const { return iterator(begin_); }

    inline iterator end() const { return iterator(end_); }

private:
    VID_T begin_;
    VID_T end_;
};

template <typename VID_T>
class DualVertexRange {
public:
    DualVertexRange(const VID_T& head_begin, const VID_T& head_end, const VID_T& tail_begin, const VID_T& tail_end)
    : head_begin_(head_begin), head_end_(head_end), tail_begin_(tail_begin), tail_end_(tail_end) {
        head_end_ = std::max(head_begin_, head_end_);
        tail_end_ = std::max(tail_begin_, tail_end_);
        if (head_begin_ > tail_begin_) {
            std::swap(head_begin_, tail_begin_);
            std::swap(head_end_, tail_end_);
        }
        if (head_end_ >= tail_begin_) {
            head_end_ = tail_end_;
            tail_begin_ = tail_end_;
        }
    }

    class iterator {
        using reference_type = const Vertex<VID_T>&;
    private:
        Vertex<VID_T> cur_;
        VID_T head_end_;
        VID_T tail_begin_;

    public:
        iterator() noexcept : cur_() {}
        explicit iterator(const VID_T& v) noexcept : cur_(v) {}
        explicit iterator(const VID_T& v, const VID_T& x, const VID_T& y) noexcept : cur_(v), head_end_(x), tail_begin_(y) {}

        inline reference_type operator*() noexcept { return cur_; }

        inline iterator& operator++() noexcept {
            ++cur_;
            if (cur_.GetValue() == head_end_) {
                cur_.SetValue(tail_begin_);
            }
            return *this;
        }

        inline iterator operator++(int) noexcept {
            VID_T new_value = cur_.GetValue()++;
            if (new_value == head_end_) {
                new_value = tail_begin_;
            }
            return iterator(new_value, head_end_, tail_begin_);
        }

        inline iterator& operator--() noexcept {
            if (cur_.GetValue() == tail_begin_) {
                cur_.SetValue(head_end_);
            }
            --cur_;
            return *this;
        }

        inline iterator operator--(int) noexcept {
            return iterator(cur_.GetValue()--, head_end_, tail_begin_);
        }

        iterator operator+(size_t offset) noexcept {
            VID_T new_value = cur_.GetValue() + offset;
            if (cur_.GetValue() < head_end_ && new_value >= head_end_) {
                new_value = offset - (head_end_ - cur_.GetValue()) + tail_begin_;
            }
            return iterator(new_value, head_end_, tail_begin_);
        }

        bool operator==(const iterator& rhs) noexcept {
            return cur_ == rhs.cur_;
        }

        bool operator!=(const iterator& rhs) noexcept {
            return cur_ != rhs.cur_;
        }
    };

    inline iterator begin() const { return iterator(head_begin_, head_end_, tail_begin_); }

    inline iterator end() const { return iterator(tail_end_); }

private:
    VID_T head_begin_;
    VID_T head_end_;
    VID_T tail_begin_;
    VID_T tail_end_;
};

int main() {
    // VertexRange<uint32_t> range(0, 5);
    // for (auto& v : range) {
    //     std::cout << v.GetValue() << std::endl;
    // }
    // for (auto v : range) {
    //     std::cout << v.GetValue() << std::endl;
    // }

    DualVertexRange<uint32_t> drange(0, 3, 5, 7);
    for (auto& v : drange) {
        std::cout << v.GetValue() << std::endl;
    }
    for (auto v : drange) {
        std::cout << v.GetValue() << std::endl;
    }

    return 0;
}
