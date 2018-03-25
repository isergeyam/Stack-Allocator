template <typename T> class StackAllocator {
private:
  typedef unsigned int size_t;
  size_t &allocated_memory_;
  char *&memory_;
  char *&head_;
  size_t &alloc_num_;

public:
  typedef T value_type;
  typedef T &reference;
  typedef size_t pointer;
  typedef const T &const_reference;
  typedef const size_t const_pointer;
  template <class U> struct rebind { typedef StackAllocator<U> other; };
  StackAllocator()
      : allocated_memory_(*(new size_t)), memory_(*(new char *)),
        head_(*(new char *)), alloc_num_(*(new size_t(1))) {
    allocated_memory_ = (sizeof(T) > 10000) ? 2 * sizeof(T) : 10000;
    memory_ = new char[allocated_memory_];
    head_ = memory_;
  }
  StackAllocator(const StackAllocator &other) noexcept
      : allocated_memory_(other.allocated_memory_),
        memory_(other.memory_),
        head_(other.head_),
        alloc_num_(alloc_num_) {
    ++alloc_num_;
  }
  pointer allocate(size_t num_) noexcept {
    void *ans = static_cast<void *>(head_);
    char *new_head = head_ + num_ * sizeof(T);
    if (new_head > memory_ + allocated_memory_) {
      char *new_memory_ = new char[allocated_memory_ * 2];
      for (size_t i = 0; i < allocated_memory_; ++i) {
        new_memory_[i] = memory_[i];
      }
      delete[] memory_;
      memory_ = new_memory_;
    }
    head_ = new_head;
    return static_cast<T *>(ans) - static_cast<T *>((void *)memory_);
  }
  void construct(pointer pp, const_reference val) noexcept {
    T *p = static_cast<T *>((void *)(memory_ + pp));
    p = new ((void *)p) T(val);
    return;
  }
  void destroy(pointer pp) noexcept {
    T *p = static_cast<T *>((void *)(memory_ + pp));
    ((T *)p)->~T();
    return;
  }
  void deallocate(T *, size_t) noexcept {}
  ~StackAllocator() {
    --alloc_num_;
    if (alloc_num_ == 0) {
      delete[] memory_;
      delete &memory_;
      delete &allocated_memory_;
      delete &head_;
      delete &alloc_num_;
    }
  }
};
