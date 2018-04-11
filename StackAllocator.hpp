#include <algorithm>
#include <cstdlib>
#include <functional>
template <typename T> class StackAllocator;
template <typename T> class StackAllocator {
private:
  std::reference_wrapper<size_t> allocated_memory_;
  std::reference_wrapper<void *> memory_;
  std::reference_wrapper<void *> head_;
  std::reference_wrapper<StackAllocator *> prev_alloc_;
  std::reference_wrapper<size_t> alloc_num_;
  static const size_t ALLOC_MEM_;
  inline void free_variables() {
    if (alloc_num_ != 1) {
      --alloc_num_;
      return;
    }
    free(memory_);
    delete prev_alloc_;
    delete &(allocated_memory_.get());
    delete &(memory_.get());
    delete &(head_.get());
    delete &(prev_alloc_.get());
    delete &(alloc_num_.get());
  }

public:
  typedef T value_type;
  typedef T &reference;
  typedef T *pointer;
  typedef const T &const_reference;
  typedef const T *const_pointer;
  template <class U> struct rebind { typedef StackAllocator<U> other; };
  StackAllocator()
      : allocated_memory_(*(new size_t)), memory_(*(new void *)),
        head_(*(new void *)), prev_alloc_(*(new StackAllocator *(nullptr))),
        alloc_num_(*(new size_t(1))) {
    allocated_memory_.get() = std::max(2 * sizeof(T), ALLOC_MEM_);
    memory_.get() = malloc(allocated_memory_);
    head_.get() = memory_;
  }
  StackAllocator(void *memory_, size_t allocated_memory_,
                 StackAllocator *prev_alloc_) noexcept
      : allocated_memory_(*(new size_t(allocated_memory_))),
        memory_(*(new void *(memory_))),
        head_(*(new void *)),
        prev_alloc_(*(new StackAllocator *(prev_alloc_))),
        alloc_num_(*(new size_t(1))) {}
  StackAllocator(const StackAllocator &other) noexcept
      : allocated_memory_(other.allocated_memory_),
        memory_(other.memory_),
        head_(other.head_),
        prev_alloc_(other.prev_alloc_),
        alloc_num_(other.alloc_num_) {
    ++alloc_num_;
  }
  StackAllocator &operator=(const StackAllocator &other) {
    free_variables();
    allocated_memory_ = other.allocated_memory_;
    memory_ = other.memory_;
    head_ = other.head_;
    prev_alloc_ = other.prev_alloc_;
    alloc_num_ = other.alloc_num_;
    ++alloc_num_;
    return *this;
  }
  pointer allocate(size_t num_) noexcept {
    void *ans = head_;
    head_.get() = (char *)(head_.get()) + sizeof(T) * num_;
    if ((char *)(head_.get()) > (char *)(memory_.get()) + allocated_memory_) {
      prev_alloc_.get() =
          new StackAllocator(memory_, allocated_memory_, prev_alloc_);
      memory_.get() = malloc(allocated_memory_);
      head_.get() = (char *)(memory_.get()) + num_ * sizeof(T);
      ans = memory_;
    }
    return static_cast<T *>(ans);
  }
  void construct(pointer p, const_reference val) noexcept {
    if ((char *)p >= (char *)(memory_.get()) &&
        (char *)p <= (char *)(memory_.get()) + allocated_memory_)
      p = new ((void *)p) T(val);
    else
      prev_alloc_.get()->construct(p, val);
    return;
  }
  void destroy(pointer p) noexcept {
    ((T *)p)->~T();
    return;
  }
  void deallocate(T *, size_t) noexcept {}
  ~StackAllocator() { free_variables(); }
  bool operator==(const StackAllocator &other) const noexcept {
    return memory_ == other.memory_;
  }
  bool operator!=(const StackAllocator &other) const noexcept {
    return !operator==(other);
  }
};
template <typename T> const size_t StackAllocator<T>::ALLOC_MEM_ = 100000;
