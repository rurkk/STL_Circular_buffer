template<typename E, typename T>
class iterator {
 public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;
  using reference = T&;
  using pointer = T*;

  // Constructor
  iterator(T* ptr, E* cirtucal_buffer) : ptr_(ptr) {
    cirtucal_buffer_ = cirtucal_buffer;
  }
  // Operators
  iterator& operator+=(ptrdiff_t n) {
    int index;
    index = ptr_ - cirtucal_buffer_->data_ + n;
    ptr_ = cirtucal_buffer_->data_ + (((index % (int) cirtucal_buffer_->capacity_) +
        (int) cirtucal_buffer_->capacity_) % (int) cirtucal_buffer_->capacity_);

    return *this;
  }
  iterator operator+(ptrdiff_t n) const {
    int index;
    index = ptr_ - cirtucal_buffer_->data_ + n;
    T* new_ptr;
    new_ptr = cirtucal_buffer_->data_ + (((index % (int) cirtucal_buffer_->capacity_) +
        (int) cirtucal_buffer_->capacity_) % (int) cirtucal_buffer_->capacity_);

    return iterator(new_ptr, cirtucal_buffer_);
  }
  iterator& operator-=(ptrdiff_t n) {
    int index;
    index = ptr_ - cirtucal_buffer_->data_ - n;
    ptr_ = cirtucal_buffer_->data_ + (((index % (int) cirtucal_buffer_->capacity_) +
        (int) cirtucal_buffer_->capacity_) % (int) cirtucal_buffer_->capacity_);

    return *this;
  }
  iterator operator-(ptrdiff_t n) const {
    int index;
    index = ptr_ - cirtucal_buffer_->data_ - n;
    T* new_ptr;
    new_ptr = cirtucal_buffer_->data_ + (((index % (int) cirtucal_buffer_->capacity_) +
        (int) cirtucal_buffer_->capacity_) % (int) cirtucal_buffer_->capacity_);

    return iterator(new_ptr, cirtucal_buffer_);
  }
  ptrdiff_t operator-(const iterator& other) const {
    if (ptr_ >= other.ptr_) {
      return ptr_ - other.ptr_;
    }

    return cirtucal_buffer_->capacity_ - (other.ptr_ - ptr_);
  }
  iterator& operator++() {
    ptr_++;
    if (ptr_ == cirtucal_buffer_->data_ + cirtucal_buffer_->capacity_) {
      ptr_ = cirtucal_buffer_->data_;
    }

    return *this;
  }
  iterator operator++(int) {
    iterator temp = *this;
    ++(*this);

    return temp;
  }
  iterator& operator--() {
    ptr_--;
    if (ptr_ == cirtucal_buffer_->data_ - 1) {
      ptr_ = cirtucal_buffer_->data_ + cirtucal_buffer_->capacity_ - 1;
    }

    return *this;
  }
  iterator operator--(int) {
    iterator temp = *this;
    --(*this);

    return temp;
  }
  bool operator<(const iterator& other) const {
    ptrdiff_t distance = other - *this;

    return distance > 0;
  }
  bool operator<=(const iterator& other) const {
    ptrdiff_t distance = other - *this;

    return distance >= 0;
  }
  bool operator>(const iterator& other) const {
    ptrdiff_t distance = other - *this;

    return distance < 0;
  }
  bool operator>=(const iterator& other) const {
    ptrdiff_t distance = other - *this;

    return distance <= 0;
  }
  reference operator*() const {
    return *ptr_;
  }
  pointer operator->() const {
    return ptr_;
  }
  reference operator[](ptrdiff_t n) const {
    return *(ptr_ + n);
  }
  bool operator==(const iterator& other) const {
    return ptr_ == other.ptr_;
  }
  bool operator!=(const iterator& other) const {
    return ptr_ != other.ptr_;
  }
  T* ptr() const {
    return ptr_;
  }

 private:
  T* ptr_;
  E* cirtucal_buffer_;
};