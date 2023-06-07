#include <memory>
#include "Iterator.h"

template<typename T, typename A = std::allocator<T>>
class CCirtucalBuffer {
 public:
  // Types
  using size_type = std::size_t;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using allocator_type = A;
  using CCB = CCirtucalBuffer<T>;

  // Methods to return iterators
  iterator<CCB, T> begin() { return iterator(data_ + head_, this); }
  iterator<CCB, T> end() { return iterator(data_ + tail_, this); }
  iterator<CCB, T> cbegin() const { return iterator(data_ + head_, this); }
  iterator<CCB, T> cend() const { return iterator(data_ + tail_, this); }

  // Constructors and destructor
  CCirtucalBuffer() : data_(nullptr) {}
  explicit CCirtucalBuffer(size_type n) : data_(allocator_type().allocate(n)), size_(0), capacity_(n) {}
  CCirtucalBuffer(size_type n, const T& value)
      : data_(allocator_type().allocate(n)), capacity_(n), size_(n), tail_(n - 1) {
    std::fill(data_, data_ + capacity_, value);
  }
  CCirtucalBuffer(const CCirtucalBuffer& other)
      : data_(allocator_type().allocate(other.capacity_)),
        capacity_(other.capacity_),
        size_(other.size_),
        head_(other.head_),
        tail_(other.tail_) {
    std::copy(other.data_, other.data_ + capacity_, data_);
  }
  ~CCirtucalBuffer() {
    allocator_type().deallocate(data_, capacity_);
  }

  // Assignment operators
  CCirtucalBuffer& operator=(const CCirtucalBuffer& other) {
    if (this != &other) {
      CCirtucalBuffer tmp(other);
      swap(tmp);
    }

    return *this;
  }
  CCirtucalBuffer& operator=(CCirtucalBuffer&& other) noexcept {
    swap(other);

    return *this;
  }

  // Element access
  reference operator[](size_type index) {
    return data_[(((index + head_) % static_cast<int>(capacity_)) + static_cast<int>(capacity_))
        % static_cast<int>(capacity_)];
  }
  const_reference operator[](size_type index) const {
    return data_[(((index + head_) % static_cast<int>(capacity_)) + static_cast<int>(capacity_))
        % static_cast<int>(capacity_)];
  }

  // Modifiers and Methods
  [[nodiscard]] bool empty() const {
    return size_ == 0;
  }

  [[nodiscard]] size_type size() const {
    return size_;
  }

  [[nodiscard]] size_type capacity() const {
    return capacity_;
  }

  void reserve(size_type new_capacity) {
    if (new_capacity > capacity_) {
      T* new_data = allocator_type().allocate(new_capacity);
      if (head_ == 0) {
        std::copy(data_, data_ + capacity_, new_data);
      } else {
        std::copy(data_, data_ + head_, new_data);
        std::copy(data_ + tail_, data_ + capacity_, new_data + new_capacity - (capacity_ - tail_));
        tail_ = tail_ + new_capacity - capacity_;
      }
      allocator_type().deallocate(data_, capacity_);
      data_ = new_data;
      capacity_ = new_capacity;
    }
  }

  void resize(size_type new_size) {
    if (new_size > capacity_) {
      reserve(new_size);
    }
    if (new_size < size_) {
      head_ = capacity_ % (head_ + (size_ - new_size));
    } else {
      tail_ = capacity_ % (tail_ + (new_size - size_));
    }
    size_ = new_size;
  }

  template<typename... Args>
  iterator<CCB, T> emplace(iterator<CCB, T> position, Args&& ... args) {
    int index;
    index = (int(position.ptr() - data_) - int(head_) % ((int) capacity_) + ((int) capacity_)) % ((int) capacity_);
    if (size_ < capacity_) {
      for (int i = size_; i > index; i--) {
        data_[(i + head_) % capacity_] = std::move(data_[(i + head_ - 1) % capacity_]);
      }
      tail_ = (tail_ + 1) % capacity_;
      size_++;
    } else {
      for (int i = 0; i < index; i++) {
        data_[(i + head_) % capacity_] = std::move(data_[(i + head_ + 1) % capacity_]);
      }
      head_ = (head_ + 1) % capacity_;
      tail_ = (tail_ + 1) % capacity_;
    }
    new(&data_[(size_t) (position.ptr() - data_)]) T(std::forward<Args>(args)...);

    return position;
  }

  iterator<CCB, T> insert(iterator<CCB, T> position, const T& value) {
    return emplace(position, value);
  }

  iterator<CCB, T> insert(iterator<CCB, T> position, int n, const T& value) {
    for (int i = 0; i < n; i++) {
      insert(position, value);
      position += 1;
    }
    position -= 1;

    return position;
  }

  iterator<CCB, T> insert(iterator<CCB, T> position, iterator<CCB, T> iposition, iterator<CCB, T> jposition) {
    for (int i = 0; i < jposition - iposition; i++) {
      insert(position, *(iposition + i));
      position += 1;
    }
    position -= 1;

    return position;
  }

  iterator<CCB, T> insert(iterator<CCB, T> position, std::initializer_list<T> il) {
    for (int i = 0; i < il.end() - il.begin(); i++) {
      insert(position, *(il.begin() + i));
      position += 1;
    }
    position -= 1;

    return position;
  }

  void push_back(const T& value) {
    insert(end(), value);
  }

  iterator<CCB, T> erase(iterator<CCB, T> position) {
    int index;
    index = (int(position.ptr() - data_) - int(head_) % ((int) capacity_) + ((int) capacity_)) % ((int) capacity_);
    for (int i = index; i < int(size_) - 1; i++) {
      data_[(i + head_) % capacity_] = std::move(data_[(i + head_ + 1) % capacity_]);
    }
    tail_ = ((((int) tail_) - 1) % ((int) capacity_) + ((int) capacity_)) % capacity_;
    size_--;

    return position;
  }

  iterator<CCB, T> erase(iterator<CCB, T> iposition, iterator<CCB, T> jposition) {
    for (int i = 0; i < jposition - iposition; i++) {
      erase(iposition);
    }

    return iposition;
  }

  void pop_front() {
    erase(begin());
  }

  void clear() {
    allocator_type().deallocate(data_, capacity_);
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    head_ = 0;
    tail_ = 0;
  }

  void assign(iterator<CCB, T> iposition, iterator<CCB, T> jposition) {
    int n;
    n = jposition - iposition;
    iterator position = begin();
    for (int i = 0; i < n; i++) {
      *(position + i) = *(iposition + i);
    }
    size_ = n;
  }

  void assign(std::initializer_list<T> il) {
    assign(il.begin(), il.end());
  }

  void assign(size_type n, const T& value) {
    std::fill(begin(), begin() + n - 1, value);
    *(begin() + n - 1) = value;
    size_ = n;
  }

  void swap(CCirtucalBuffer& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
  }

  friend iterator<CCirtucalBuffer<T>, T>;

 private:
  pointer data_;
  size_type size_{};
  size_type capacity_{};
  size_type head_{};
  size_type tail_{};
};

