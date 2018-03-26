#include <algorithm>
#include <cstdint>
#include <cstdlib>
template <typename T> class StackAllocator;
template <typename T> class StackAllocator {
private:
  size_t &allocated_memory_;
  void *&memory_;
  void *&head_;
  static const size_t ALLOC_MEM_;
  inline void free_variables() {
    free(memory_);
    delete &allocated_memory_;
    delete &memory_;
    delete &head_;
  }

public:
  template <typename _Ref, typename _Ptr>
  struct _Fancy_iterator : public std::iterator<std::bidirectional_iterator_tag,
                                                T, std::ptrdiff_t, _Ptr, _Ref> {
    using base = std::iterator<std::bidirectional_iterator_tag, T,
                               std::ptrdiff_t, _Ptr, _Ref>;
    using typename base::pointer;
    using typename base::reference;
    using typename base::difference_type;
    using typename base::value_type;
    using _Self = _Fancy_iterator;
    using _Self_Const =
        _Fancy_iterator<std::remove_const_t<_Ref>, std::remove_const_t<_Ptr>>;

  private:
    intptr_t offset_;
    void *&memory_;
    void optional_free() {
      if (offset_ == -1)
        delete &memory_;
      return;
    }

  public:
    explicit _Fancy_iterator(intptr_t offset_ = -1,
                             void *&memory_ = *(new void *))
        : offset_(offset_), memory_(memory_) {}
    explicit _Fancy_iterator(const _Self_Const &other)
        : offset_(other.offset_), memory_(other.memory_) {}
    /*explicit _Fancy_iterator(
        const _Fancy_iterator<std::add_const_t<_Ref>, std::add_const_t<_Ptr>>
            &other)
        : offset_(other.offset_), memory_(other.memory_) {}*/
    ~_Fancy_iterator() { optional_free(); }
    _Ref operator*() const {
      return *reinterpret_cast<_Ptr>((char *)memory_ + offset_);
    }
    _Ptr operator->() { return get_raw_pointer(); }
    _Ptr get_raw_pointer() const { return &(operator*()); }
    const _Self &operator+=(difference_type x) {
      offset_ += x;
      return *this;
    }
    _Self operator+(difference_type x) { return _Self(offset_ + x, memory_); }
    operator _Ptr() const { return get_raw_pointer(); }
    const _Self &operator=(std::nullptr_t) {
      offset_ = -reinterpret_cast<intptr_t>(memory_);
      return *this;
    }
    /*_Fancy_iterator(const _Self &other)
        : memory_(other.memory_), offset_(other.offset_) {}*/
    const _Self &operator=(const _Self &other) {
      optional_free();
      this->offset_ = other.offset_;
      this->memory_ = other.memory_;
    }
    bool operator==(const _Fancy_iterator &rhs) const {
      return get_raw_pointer() == rhs.get_raw_pointer();
    }
    bool operator!=(const _Fancy_iterator &rhs) const {
      return !operator==(rhs);
    }
    const _Fancy_iterator &operator++() {
      ++offset_;
      return *this;
    }
    const _Fancy_iterator &operator--() {
      --offset_;
      return *this;
    }
    /*bool operator==(std::nullptr_t rhs) { return get_raw_pointer() == rhs; }
    bool operator!=(std::nullptr_t rhs) { return !operator==(rhs); }
                operator std::nullptr_t() {
                        return nullptr;
                }*/
  };
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = _Fancy_iterator<T &, T *>;
  using const_pointer = _Fancy_iterator<const T &, const T *>;
  template <class U> struct rebind { typedef StackAllocator<U> other; };
  StackAllocator()
      : allocated_memory_(*(new size_t)), memory_(*(new void *)),
        head_(*(new void *)) {
    allocated_memory_ = std::max(2 * sizeof(T), static_cast<size_t>(100000));
    memory_ = malloc(allocated_memory_);
    head_ = memory_;
  }
  StackAllocator(const StackAllocator &other) noexcept : StackAllocator() {
    *this = other;
  }
  StackAllocator &operator=(const StackAllocator &other) {
    free_variables();
    allocated_memory_ = other.allocated_memory_;
    memory_ = other.memory_;
    head_ = other.head_;
    return *this;
  }
  pointer allocate(size_t num_) noexcept {
    void *ans = head_;
    head_ = static_cast<void *>((char *)head_ + num_ * sizeof(T));
    if ((char *)head_ > (char *)memory_ + allocated_memory_) {
      /*prev_alloc_ = new StackAllocator(memory_, allocated_memory_,
      prev_alloc_);
      memory_ = malloc(allocated_memory_);
      new_head = static_cast<void *>((char *)memory_ + num_ * sizeof(T));
      ans = memory_;*/
      memory_ = std::realloc(
          memory_,
          std::max(static_cast<size_t>(2 * ((char *)head_ - (char *)memory_)),
                   2 * allocated_memory_));
      head_ = reinterpret_cast<void *>((char *)memory_ +
                                       ((char *)head_ - (char *)memory_) +
                                       num_ * sizeof(T));
    }
    return pointer((char *)ans - (char *)memory_,
                   memory_) /*- static_cast<T *>((void *)memory_)*/;
  }
  void construct(pointer pp, const_reference val) noexcept {
    T *p = pp.get_raw_pointer();
    // T *p = static_cast<T *>((void *)(memory_ + pp));
    p = new ((void *)p) T(val);
    return;
  }
  void destroy(pointer pp) noexcept {
    T *p = pp.get_raw_pointer();
    // T *p = static_cast<T *>((void *)(memory_ + pp));
    ((T *)p)->~T();
    return;
  }
  void deallocate(pointer, size_t) noexcept {}
  ~StackAllocator() { free_variables(); }
};
template <typename T> const size_t StackAllocator<T>::ALLOC_MEM_ = 100000;
/*namespace std {
template <typename T> struct allocator_traits<StackAllocator<T>> {
  using Alloc = StackAllocator<T>;
  using pointer = typename Alloc::pointer;
  using const_pointer = typename Alloc::const_pointer;
  using value_type = typename Alloc::value_type;
  using size_type = size_t;
  using defference_type = ptrdiff_t;
  using construct = typename Alloc::construct;
  using destroy = typename Alloc::destroy;
  size_type max_size() const {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }
  template <typename U>
  using rebind_alloc = typename Alloc::template rebind<U>::other;
  static pointer allocate(Alloc &a, size_type n) { return a.allocate(n); }
  static pointer deallocate(Alloc &a, size_type n) { return a.deallocate(n); }
};
}*/
