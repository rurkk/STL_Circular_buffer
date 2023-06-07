#include <memory>
#include "Iterator.h"

template<typename T, typename A = std::allocator<T>>
class CCirtucalBufferExt {
 public:
  // Types
  using size_type = std::size_t;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using allocator_type = A;
  using CCBE = CCirtucalBufferExt<T>;

  // Methods to return iterators
  iterator<CCBE, T> begin() { return iterator(data_ + head_, this); }
  iterator<CCBE, T> end() { return iterator(data_ + size_, this); }
  iterator<CCBE, T> cbegin() const { return iterator(data_ + head_, this); }
  iterator<CCBE, T> cend() const { return iterator(data_ + size_, this); }

  // Constructors and destructor
  CCirtucalBufferExt() : data_(nullptr), size_(0), capacity_(0) {}
  explicit CCirtucalBufferExt(size_type n) : data_(allocator_type().allocate(n)), size_(0), capacity_(n) {}
  CCirtucalBufferExt(size_type n, const T& value) : CCirtucalBufferExt(n) {
    std::fill(begin(), end() + n, value);
    size_ = n;
    tail_ = n - 1;
  }
  CCirtucalBufferExt(const CCirtucalBufferExt& other) : CCirtucalBufferExt(other.size_) {
    std::copy(other.begin(), other.end(), begin());
    size_ = other.size_;
    tail_ = other.tail_;
  }
  CCirtucalBufferExt(CCirtucalBufferExt&& other) noexcept
      : data_(other.data_), size_(other.size_), capacity_(other.capacity_), tail_(other.tail_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    other.tail_ = 0;
  }
  ~CCirtucalBufferExt() {
    allocator_type().deallocate(data_, capacity_);
  }

  // Assignment operators
  CCirtucalBufferExt& operator=(const CCirtucalBufferExt& other) {
    if (this != &other) {
      CCirtucalBufferExt tmp(other);
      swap(tmp);
    }
    return *this;
  }
  CCirtucalBufferExt& operator=(CCirtucalBufferExt&& other) noexcept {
    swap(other);
    return *this;
  }

  // Element access
  reference operator[](int index) {
    return data_[((index % static_cast<int>(size_)) + static_cast<int>(size_)) % static_cast<int>(size_)];
  }
  const_reference operator[](int index) const {
    return data_[((index % static_cast<int>(size_)) + static_cast<int>(size_)) % static_cast<int>(size_)];
  }

  // Modifiers and Methods
  [[nodiscard]] bool empty() const { return size_ == 0; }
  [[nodiscard]] size_type size() const { return size_; }
  [[nodiscard]] size_type capacity() const { return capacity_; }
  void reserve(size_type new_capacity) {
    if (new_capacity > capacity_) {
      pointer new_data = allocator_type().allocate(new_capacity);
      std::copy(begin(), end(), new_data);
      allocator_type().deallocate(data_, capacity_);
      data_ = new_data;
      capacity_ = new_capacity;
    }
  }

  void resize(size_type new_size) {
    pointer new_data = allocator_type().allocate(new_size);
    if (new_size < size_) {
      std::copy(begin(), end() - (size_ - new_size), new_data);
    } else {
      std::copy(begin(), end(), new_data);
    }
    allocator_type().deallocate(data_, capacity_);
    data_ = new_data;
    size_ = new_size;
    capacity_ = new_size;
  }

  template<typename... Args>
  iterator<CCBE, T> emplace(iterator<CCBE, T> position, Args&& ... args) {
    size_t index = position - begin();
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    for (size_t i = size_; i > index; --i) {
      data_[i] = std::move(data_[i - 1]);
    }
    new(&data_[index]) T(std::forward<Args>(args)...);
    size_++;
    tail_++;

    return begin() + index;
  }

  iterator<CCBE, T> insert(iterator<CCBE, T> position, const T& value) {
    return emplace(position, value);
  }

  iterator<CCBE, T> insert(iterator<CCBE, T> position, int n, const T& value) {
    size_t index = position - begin();
    if (size_ + n > capacity_) {
      reserve(size_ + n);
    }
    for (size_t i = size_ + n - 1; i > index + n - 1; --i) {
      data_[i] = std::move(data_[i - n]);
    }
    for (int i = 0; i < n; i++) {
      data_[index + i] = value;
    }
    size_ += n;
    tail_ += n;

    return begin() + index;
  }

  iterator<CCBE, T> insert(iterator<CCBE, T> position, iterator<CCBE, T> iposition, iterator<CCBE, T> jposition) {
    size_t index = position - begin();
    int n = jposition - iposition;
    if (size_ + n > capacity_) {
      reserve(size_ + n);
    }
    for (size_t i = size_ + n - 1; i > index + n - 1; --i) {
      data_[i] = std::move(data_[i - n]);
    }
    for (int i = 0; i < n; i++) {
      data_[index + i] = *(iposition + i);
    }
    size_ += n;
    tail_ += n;

    return position + n;
  }

  iterator<CCBE, T> insert(iterator<CCBE, T> position, std::initializer_list<T> il) {
    for (int i = 0; i < il.end() - il.begin(); i++) {
      insert(position, *(il.begin() + i));
      position += 1;
    }
    position -= 1;
    size_ += il.end() - il.begin();
    tail_ += il.end() - il.begin();

    return position;
  }

  iterator<CCBE, T> erase(iterator<CCBE, T> position) {
    int index;
    index = position - begin();
    for (int i = index; i < ((int) size_) - 1; i++) {
      data_[i] = std::move(data_[i + 1]);
    }
    size_--;

    return position;
  }

  iterator<CCBE, T> erase(iterator<CCBE, T> iposition, iterator<CCBE, T> jposition) {
    int index;
    index = iposition - begin();
    for (int i = index; i < ((int) size_) - (jposition - iposition); i++) {
      data_[i] = std::move(data_[i + (jposition - iposition)]);
    }
    size_ -= jposition - iposition;

    return iposition;
  }

  void pop_front() {
    erase(begin());
  }

  void clear() {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    head_ = 0;
    tail_ = 0;
  }

  void assign(iterator<CCBE, T> iposition, iterator<CCBE, T> jposition) {
    int n;
    n = jposition - iposition;
    if (n > (int) capacity_) {
      reserve(n);
    }
    for (int i = 0; i < n; i++) {
      data_[i] = *(iposition + i);
    }
    size_ = n;
  }

  void assign(std::initializer_list<T> il) {
    assign(il.begin(), il.end());
  }

  void assign(size_type n, const T& value) {
    if (n > capacity_) {
      reserve(n);
    }
    std::fill(begin(), begin() + n, value);
    size_ = n;
  }

  void swap(CCirtucalBufferExt& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  void push_back(const T& value) {
    insert(end(), value);
  }

  void push_back(T&& value) {
    insert(end(), value);
  }

  friend iterator<CCirtucalBufferExt<T>, T>;

 private:
  pointer data_;
  size_type size_{};
  size_type capacity_{};
  size_type head_{};
  size_type tail_{};
};

