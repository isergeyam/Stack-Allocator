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
struct StackBlock {
  char *memory_;
  StackBlock *prev_block;
  ~StackBlock() {
    std::free(memory_);
    if (prev_block != nullptr)
      prev_block->~StackBlock();
    std::free(prev_block);
  }
  static constexpr size_t ALLOC_MEM_ = 100000;
  StackBlock(size_t alloc_num = ALLOC_MEM_) : prev_block(nullptr) {
    memory_ = static_cast<char *>(std::malloc(alloc_num));
    if (memory_ == nullptr)
      throw std::bad_alloc();
  }
};
struct StackRealAllocator {
  size_t alloc_num;
  char *head_;
  StackBlock *m_block;
  StackRealAllocator() : alloc_num(1), head_(nullptr), m_block(nullptr) {}
  void insert_block(size_t alloc_num = StackBlock::ALLOC_MEM_) {
    StackBlock *prev_block = m_block;
    m_block = new (std::malloc(sizeof(StackBlock))) StackBlock(alloc_num);
    m_block->prev_block = prev_block;
    head_ = m_block->memory_;
  }
  void *allocate(size_t num_) {
    if (num_>StackBlock::ALLOC_MEM_) {
      auto new_block = new (std::malloc(sizeof(StackBlock))) StackBlock(num_);
      new_block->prev_block = m_block->prev_block;
      m_block->prev_block = new_block;
      return new_block->memory_;
    }
    if (m_block == nullptr) {
      m_block = new (std::malloc(sizeof(StackBlock))) StackBlock();
      head_ = m_block->memory_;
    }
    if (head_ + num_ > m_block->memory_ + StackBlock::ALLOC_MEM_) {
      insert_block();
    }
    char *ans = head_;
    head_ += num_;
    return static_cast<void *>(ans);
  }
  ~StackRealAllocator() {
    --alloc_num;
    if (alloc_num == 0) {
      if (m_block != nullptr)
        m_block->~StackBlock();
      std::free(m_block);
    }
  }
};
template <typename T> class StackAllocator;
template <typename T> class StackAllocator {
private:
  StackRealAllocator *m_alloc;
  void free_variables() {
    if (m_alloc == nullptr)
      return;
    m_alloc->~StackRealAllocator();
    if (m_alloc->alloc_num == 0)
      std::free(m_alloc);
  }

public:
  typedef T value_type;
  typedef T &reference;
  typedef T *pointer;
  typedef const T &const_reference;
  typedef const T *const_pointer;
  template <class U> struct rebind { typedef StackAllocator<U> other; };
  StackAllocator()
      : m_alloc(new (std::malloc(sizeof(StackRealAllocator)))
                    StackRealAllocator()) {}
  template <typename U>
  StackAllocator(const StackAllocator<U> &other) : m_alloc(nullptr) {
    *this = other;
  }
  StackRealAllocator *GetAlloc() const noexcept { return m_alloc; }
  template <typename U>
  StackAllocator &operator=(const StackAllocator<U> &other) {
    free_variables();
    m_alloc = other.GetAlloc();
    ++m_alloc->alloc_num;
    return *this;
  }
  pointer allocate(size_t num_) {
    num_ = sizeof(T) * num_;
    return static_cast<T *>(m_alloc->allocate(
        num_ + num_ % std::max(alignof(T), alignof(std::max_align_t))));
  }
  void construct(pointer p, const_reference val) noexcept {
    p = new ((void *)p) T(val);
    return;
  }
  void destroy(pointer p) noexcept {
    p->~T();
    return;
  }
  void deallocate(T *, size_t) noexcept {}
  ~StackAllocator() { free_variables(); }
};
