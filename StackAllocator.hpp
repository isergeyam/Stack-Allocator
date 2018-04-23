#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <memory>
template <typename _Tp> static _Tp *m_alloc(_Tp x = _Tp()) {
  void *chunk_ = std::malloc(sizeof(_Tp));
  if (chunk_ == nullptr)
    throw std::bad_alloc();
  return new (chunk_) _Tp(x);
}
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
    std::free(memory_);
    if (prev_alloc_ != nullptr)
      prev_alloc_.get()->~StackAllocator();
    std::free(prev_alloc_);
    std::free(&(allocated_memory_.get()));
    std::free(&(memory_.get()));
    std::free(&(head_.get()));
    std::free(&(prev_alloc_.get()));
    std::free(&(alloc_num_.get()));
  }

public:
  typedef T value_type;
  typedef T &reference;
  typedef T *pointer;
  typedef const T &const_reference;
  typedef const T *const_pointer;
  template <class U> struct rebind { typedef StackAllocator<U> other; };
  StackAllocator()
      : allocated_memory_(*m_alloc<size_t>(0)), memory_(*m_alloc<void *>(nullptr)),
        head_(*m_alloc<void *>(nullptr)), prev_alloc_(*m_alloc<StackAllocator *>(nullptr)),
        alloc_num_(*m_alloc<size_t>(1)) {
    allocated_memory_.get() = std::max(2 * sizeof(T), ALLOC_MEM_);
    memory_.get() = malloc(allocated_memory_);
    if (memory_ == nullptr)
      throw std::bad_alloc();
    head_.get() = memory_;
  }
  StackAllocator(void *memory_, size_t allocated_memory_,
                 StackAllocator *prev_alloc_) noexcept
      : allocated_memory_(*m_alloc(allocated_memory_)),
        memory_(*m_alloc(memory_)),
        head_(*m_alloc<void *>(nullptr)),
        prev_alloc_(*m_alloc(prev_alloc_)),
        alloc_num_(*m_alloc<size_t>(1)) {}
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
  pointer allocate(size_t num_) {
    void *ans = head_;
    size_t offset = sizeof(T) * num_;
    offset += offset % std::max(alignof(T), alignof(std::max_align_t));
    head_.get() = (char *)(head_.get()) + offset;
    if ((char *)(head_.get()) > (char *)(memory_.get()) + allocated_memory_) {
      prev_alloc_.get() = m_alloc(StackAllocator(memory_, allocated_memory_, prev_alloc_));
      memory_.get() = malloc(allocated_memory_);
      if (memory_ == nullptr)
        throw std::bad_alloc();
      head_.get() = (char *)(memory_.get()) + offset;
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
