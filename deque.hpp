#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {

template <class T> class deque {
public:
  class const_iterator;
  class iterator {
  private:
    deque* dq;
    size_t idx;
  public:
    iterator(deque* dq = nullptr, size_t idx = 0) : dq(dq), idx(idx) {}
    
    iterator operator+(const int &n) const {
        return iterator(dq, idx + n);
    }
    iterator operator-(const int &n) const {
        return iterator(dq, idx - n);
    }
    int operator-(const iterator &rhs) const {
        if (dq != rhs.dq) throw invalid_iterator();
        return idx - rhs.idx;
    }
    iterator &operator+=(const int &n) {
        idx += n;
        return *this;
    }
    iterator &operator-=(const int &n) {
        idx -= n;
        return *this;
    }
    iterator operator++(int) {
        iterator tmp = *this;
        idx++;
        return tmp;
    }
    iterator &operator++() {
        idx++;
        return *this;
    }
    iterator operator--(int) {
        iterator tmp = *this;
        idx--;
        return tmp;
    }
    iterator &operator--() {
        idx--;
        return *this;
    }
    T &operator*() const {
        if (!dq || idx >= dq->sz) throw invalid_iterator();
        return dq->data[(dq->head + idx) % dq->cap];
    }
    T *operator->() const noexcept {
        return &(dq->data[(dq->head + idx) % dq->cap]);
    }
    bool operator==(const iterator &rhs) const {
        return dq == rhs.dq && idx == rhs.idx;
    }
    bool operator==(const const_iterator &rhs) const {
        return dq == rhs.dq && idx == rhs.idx;
    }
    bool operator!=(const iterator &rhs) const {
        return !(*this == rhs);
    }
    bool operator!=(const const_iterator &rhs) const {
        return !(*this == rhs);
    }
    friend class const_iterator;
    friend class deque;
  };

  class const_iterator {
  private:
    const deque* dq;
    size_t idx;
  public:
    const_iterator(const deque* dq = nullptr, size_t idx = 0) : dq(dq), idx(idx) {}
    const_iterator(const iterator &other) : dq(other.dq), idx(other.idx) {}
    
    const_iterator operator+(const int &n) const {
        return const_iterator(dq, idx + n);
    }
    const_iterator operator-(const int &n) const {
        return const_iterator(dq, idx - n);
    }
    int operator-(const const_iterator &rhs) const {
        if (dq != rhs.dq) throw invalid_iterator();
        return idx - rhs.idx;
    }
    const_iterator &operator+=(const int &n) {
        idx += n;
        return *this;
    }
    const_iterator &operator-=(const int &n) {
        idx -= n;
        return *this;
    }
    const_iterator operator++(int) {
        const_iterator tmp = *this;
        idx++;
        return tmp;
    }
    const_iterator &operator++() {
        idx++;
        return *this;
    }
    const_iterator operator--(int) {
        const_iterator tmp = *this;
        idx--;
        return tmp;
    }
    const_iterator &operator--() {
        idx--;
        return *this;
    }
    const T &operator*() const {
        if (!dq || idx >= dq->sz) throw invalid_iterator();
        return dq->data[(dq->head + idx) % dq->cap];
    }
    const T *operator->() const noexcept {
        return &(dq->data[(dq->head + idx) % dq->cap]);
    }
    bool operator==(const iterator &rhs) const {
        return dq == rhs.dq && idx == rhs.idx;
    }
    bool operator==(const const_iterator &rhs) const {
        return dq == rhs.dq && idx == rhs.idx;
    }
    bool operator!=(const iterator &rhs) const {
        return !(*this == rhs);
    }
    bool operator!=(const const_iterator &rhs) const {
        return !(*this == rhs);
    }
    friend class iterator;
    friend class deque;
  };

private:
    T* data;
    size_t cap;
    size_t head;
    size_t sz;

    void reallocate(size_t new_cap) {
        T* new_data = reinterpret_cast<T*>(new char[new_cap * sizeof(T)]);
        size_t i = 0;
        try {
            for (; i < sz; ++i) {
                new (new_data + i) T(data[(head + i) % cap]);
            }
        } catch (...) {
            for (size_t j = 0; j < i; ++j) {
                new_data[j].~T();
            }
            delete[] reinterpret_cast<char*>(new_data);
            throw;
        }
        for (size_t j = 0; j < sz; ++j) {
            data[(head + j) % cap].~T();
        }
        delete[] reinterpret_cast<char*>(data);
        data = new_data;
        head = 0;
        cap = new_cap;
    }

public:
  deque() : data(nullptr), cap(0), head(0), sz(0) {}
  
  deque(const deque &other) : data(nullptr), cap(other.cap), head(0), sz(other.sz) {
      if (cap > 0) {
          data = reinterpret_cast<T*>(new char[cap * sizeof(T)]);
          size_t i = 0;
          try {
              for (; i < sz; ++i) {
                  new (data + i) T(other.data[(other.head + i) % other.cap]);
              }
          } catch (...) {
              for (size_t j = 0; j < i; ++j) {
                  data[j].~T();
              }
              delete[] reinterpret_cast<char*>(data);
              data = nullptr;
              cap = 0;
              sz = 0;
              throw;
          }
      }
  }

  ~deque() {
      clear();
      delete[] reinterpret_cast<char*>(data);
  }

  deque &operator=(const deque &other) {
      if (this == &other) return *this;
      T* new_data = nullptr;
      if (other.cap > 0) {
          new_data = reinterpret_cast<T*>(new char[other.cap * sizeof(T)]);
          size_t i = 0;
          try {
              for (; i < other.sz; ++i) {
                  new (new_data + i) T(other.data[(other.head + i) % other.cap]);
              }
          } catch (...) {
              for (size_t j = 0; j < i; ++j) {
                  new_data[j].~T();
              }
              delete[] reinterpret_cast<char*>(new_data);
              throw;
          }
      }
      clear();
      delete[] reinterpret_cast<char*>(data);
      data = new_data;
      cap = other.cap;
      sz = other.sz;
      head = 0;
      return *this;
  }

  T &at(const size_t &pos) {
      if (pos >= sz) throw index_out_of_bound();
      return data[(head + pos) % cap];
  }
  const T &at(const size_t &pos) const {
      if (pos >= sz) throw index_out_of_bound();
      return data[(head + pos) % cap];
  }
  T &operator[](const size_t &pos) {
      if (pos >= sz) throw index_out_of_bound();
      return data[(head + pos) % cap];
  }
  const T &operator[](const size_t &pos) const {
      if (pos >= sz) throw index_out_of_bound();
      return data[(head + pos) % cap];
  }

  const T &front() const {
      if (sz == 0) throw container_is_empty();
      return data[head];
  }
  const T &back() const {
      if (sz == 0) throw container_is_empty();
      return data[(head + sz - 1) % cap];
  }

  iterator begin() { return iterator(this, 0); }
  const_iterator cbegin() const { return const_iterator(this, 0); }
  iterator end() { return iterator(this, sz); }
  const_iterator cend() const { return const_iterator(this, sz); }

  bool empty() const { return sz == 0; }
  size_t size() const { return sz; }

  void clear() {
      for (size_t i = 0; i < sz; ++i) {
          data[(head + i) % cap].~T();
      }
      sz = 0;
      head = 0;
  }

  iterator insert(iterator pos, const T &value) {
      if (pos.dq != this || pos.idx > sz) throw invalid_iterator();
      T val_copy = value;
      if (pos.idx == sz) {
          push_back(val_copy);
          return iterator(this, sz - 1);
      }
      if (pos.idx == 0) {
          push_front(val_copy);
          return iterator(this, 0);
      }
      if (sz == cap) {
          size_t idx = pos.idx;
          reallocate(cap == 0 ? 8 : cap * 2);
          pos = iterator(this, idx);
      }
      size_t idx = pos.idx;
      new (data + (head + sz) % cap) T(data[(head + sz - 1) % cap]);
      for (size_t i = sz - 1; i > idx; --i) {
          data[(head + i) % cap] = data[(head + i - 1) % cap];
      }
      data[(head + idx) % cap] = val_copy;
      sz++;
      return iterator(this, idx);
  }

  iterator erase(iterator pos) {
      if (pos.dq != this || pos.idx >= sz) throw invalid_iterator();
      if (pos.idx == 0) {
          pop_front();
          return iterator(this, 0);
      }
      if (pos.idx == sz - 1) {
          pop_back();
          return iterator(this, sz);
      }
      size_t idx = pos.idx;
      for (size_t i = idx; i < sz - 1; ++i) {
          data[(head + i) % cap] = data[(head + i + 1) % cap];
      }
      data[(head + sz - 1) % cap].~T();
      sz--;
      return iterator(this, idx);
  }

  void push_back(const T &value) {
      if (sz == cap) {
          reallocate(cap == 0 ? 8 : cap * 2);
      }
      new (data + (head + sz) % cap) T(value);
      sz++;
  }

  void pop_back() {
      if (sz == 0) throw container_is_empty();
      data[(head + sz - 1) % cap].~T();
      sz--;
  }

  void push_front(const T &value) {
      if (sz == cap) {
          reallocate(cap == 0 ? 8 : cap * 2);
      }
      size_t new_head = (head - 1 + cap) % cap;
      if (cap == 0) new_head = 0; // Should not happen since we reallocate
      new (data + new_head) T(value);
      head = new_head;
      sz++;
  }

  void pop_front() {
      if (sz == 0) throw container_is_empty();
      data[head].~T();
      head = (head + 1) % cap;
      sz--;
  }
};

} // namespace sjtu

#endif