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
        size_t pos = dq->head + idx;
        if (pos >= dq->cap) pos -= dq->cap;
        return dq->data[pos];
    }
    T *operator->() const noexcept {
        size_t pos = dq->head + idx;
        if (pos >= dq->cap) pos -= dq->cap;
        return &(dq->data[pos]);
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
        size_t pos = dq->head + idx;
        if (pos >= dq->cap) pos -= dq->cap;
        return dq->data[pos];
    }
    const T *operator->() const noexcept {
        size_t pos = dq->head + idx;
        if (pos >= dq->cap) pos -= dq->cap;
        return &(dq->data[pos]);
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
                size_t p = head + i;
                if (p >= cap) p -= cap;
                new (new_data + i) T(data[p]);
            }
        } catch (...) {
            for (size_t j = 0; j < i; ++j) {
                new_data[j].~T();
            }
            delete[] reinterpret_cast<char*>(new_data);
            throw;
        }
        for (size_t j = 0; j < sz; ++j) {
            size_t p = head + j;
            if (p >= cap) p -= cap;
            data[p].~T();
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
                  size_t p = other.head + i;
                  if (p >= other.cap) p -= other.cap;
                  new (data + i) T(other.data[p]);
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
                  size_t p = other.head + i;
                  if (p >= other.cap) p -= other.cap;
                  new (new_data + i) T(other.data[p]);
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
      size_t p = head + pos;
      if (p >= cap) p -= cap;
      return data[p];
  }
  const T &at(const size_t &pos) const {
      if (pos >= sz) throw index_out_of_bound();
      size_t p = head + pos;
      if (p >= cap) p -= cap;
      return data[p];
  }
  T &operator[](const size_t &pos) {
      if (pos >= sz) throw index_out_of_bound();
      size_t p = head + pos;
      if (p >= cap) p -= cap;
      return data[p];
  }
  const T &operator[](const size_t &pos) const {
      if (pos >= sz) throw index_out_of_bound();
      size_t p = head + pos;
      if (p >= cap) p -= cap;
      return data[p];
  }

  const T &front() const {
      if (sz == 0) throw container_is_empty();
      return data[head];
  }
  const T &back() const {
      if (sz == 0) throw container_is_empty();
      size_t p = head + sz - 1;
      if (p >= cap) p -= cap;
      return data[p];
  }

  iterator begin() { return iterator(this, 0); }
  const_iterator cbegin() const { return const_iterator(this, 0); }
  iterator end() { return iterator(this, sz); }
  const_iterator cend() const { return const_iterator(this, sz); }

  bool empty() const { return sz == 0; }
  size_t size() const { return sz; }

  void clear() {
      for (size_t i = 0; i < sz; ++i) {
          size_t p = head + i;
          if (p >= cap) p -= cap;
          data[p].~T();
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
      size_t p_new = head + sz;
      if (p_new >= cap) p_new -= cap;
      size_t p_old = head + sz - 1;
      if (p_old >= cap) p_old -= cap;
      new (data + p_new) T(data[p_old]);
      for (size_t i = sz - 1; i > idx; --i) {
          size_t p1 = head + i;
          if (p1 >= cap) p1 -= cap;
          size_t p2 = head + i - 1;
          if (p2 >= cap) p2 -= cap;
          data[p1] = data[p2];
      }
      size_t p_idx = head + idx;
      if (p_idx >= cap) p_idx -= cap;
      data[p_idx] = val_copy;
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
          size_t p1 = head + i;
          if (p1 >= cap) p1 -= cap;
          size_t p2 = head + i + 1;
          if (p2 >= cap) p2 -= cap;
          data[p1] = data[p2];
      }
      size_t p_last = head + sz - 1;
      if (p_last >= cap) p_last -= cap;
      data[p_last].~T();
      sz--;
      return iterator(this, idx);
  }

  void push_back(const T &value) {
      if (sz == cap) {
          reallocate(cap == 0 ? 8 : cap * 2);
      }
      size_t p = head + sz;
      if (p >= cap) p -= cap;
      new (data + p) T(value);
      sz++;
  }

  void pop_back() {
      if (sz == 0) throw container_is_empty();
      size_t p = head + sz - 1;
      if (p >= cap) p -= cap;
      data[p].~T();
      sz--;
  }

  void push_front(const T &value) {
      if (sz == cap) {
          reallocate(cap == 0 ? 8 : cap * 2);
      }
      size_t new_head = (head == 0) ? cap - 1 : head - 1;
      new (data + new_head) T(value);
      head = new_head;
      sz++;
  }

  void pop_front() {
      if (sz == 0) throw container_is_empty();
      data[head].~T();
      head++;
      if (head == cap) head = 0;
      sz--;
  }
};

} // namespace sjtu

#endif