#ifndef PYLIST_H
#define PYLIST_H

#include <iostream>
#include <memory>
#include <vector>

class pylist {
public:
    enum class kind_t { is_int, is_list };

private:
    kind_t kind_{kind_t::is_list};
    int int_val_{0};
    std::shared_ptr<std::vector<pylist>> list_;

    void ensure_list_storage() {
        if (list_ == nullptr) list_ = std::make_shared<std::vector<pylist>>();
    }

public:
    pylist() : kind_(kind_t::is_list), int_val_(0), list_(std::make_shared<std::vector<pylist>>()) {}
    pylist(int v) : kind_(kind_t::is_int), int_val_(v), list_(nullptr) {}

    pylist(const pylist&) = default;
    pylist(pylist&&) noexcept = default;
    pylist& operator=(const pylist&) = default;
    pylist& operator=(pylist&&) noexcept = default;

    pylist& operator=(int v) {
        kind_ = kind_t::is_int;
        int_val_ = v;
        list_.reset();
        return *this;
    }

    void append(int v) {
        if (kind_ != kind_t::is_list) {
            kind_ = kind_t::is_list;
            ensure_list_storage();
        }
        ensure_list_storage();
        list_->emplace_back(pylist(v));
    }

    void append(const pylist &x) {
        if (kind_ != kind_t::is_list) {
            kind_ = kind_t::is_list;
            ensure_list_storage();
        }
        ensure_list_storage();
        list_->push_back(x);
    }

    pylist pop() {
        if (kind_ != kind_t::is_list) {
            return pylist();
        }
        ensure_list_storage();
        pylist ret;
        if (!list_->empty()) {
            ret = std::move(list_->back());
            list_->pop_back();
        }
        return ret;
    }

    pylist &operator[](size_t i) {
        if (kind_ != kind_t::is_list) {
            kind_ = kind_t::is_list;
            ensure_list_storage();
        }
        return (*list_)[i];
    }

    const pylist &operator[](size_t i) const {
        return (*list_)[i];
    }

    operator int() const {
        if (kind_ == kind_t::is_int) return int_val_;
        return 0;
    }

    friend std::ostream &operator<<(std::ostream &os, const pylist &ls) {
        if (ls.kind_ == kind_t::is_int) {
            os << ls.int_val_;
            return os;
        }
        os << '[';
        if (ls.list_) {
            for (size_t i = 0; i < ls.list_->size(); ++i) {
                if (i) os << ", ";
                os << (*ls.list_)[i];
            }
        }
        os << ']';
        return os;
    }
};

#endif // PYLIST_H
