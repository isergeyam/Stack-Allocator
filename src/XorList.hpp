#pragma once
#ifndef _XOR_LIST_H_
#define _XOR_LIST_H_
#include <cstddef>
#include <iostream>
#include <iterator>
template<typename _Tp, typename _Alloc = std::allocator<_Tp>>
class XorList {
 public:
  using value_type = _Tp;
  using allocator_type = _Alloc;
  using alloc_traits = typename std::allocator_traits<_Alloc>;
  using pointer = typename alloc_traits::pointer;
  using const_pointer = typename alloc_traits::const_pointer;
  using reference = typename allocator_type::reference;
  using const_reference = typename allocator_type::const_reference;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  struct _XorList_node;
  struct _XorList_iterator;
  using iterator = _XorList_iterator;
  using reverse_iterator = std::reverse_iterator<_XorList_iterator>;
  using _Node_alloc_type =
  typename _Alloc::template rebind<_XorList_node>::other;
  using _Node_alloc_traits = std::allocator_traits<_Node_alloc_type>;
  using node_pointer = typename _Node_alloc_traits::pointer;

 private:
  static constexpr node_pointer _M_process_xor(node_pointer __x,
                                               node_pointer __y) {
    return reinterpret_cast<node_pointer>(reinterpret_cast<uintptr_t>(__x) ^
        reinterpret_cast<uintptr_t>(__y));
  }

 public:
  struct _XorList_node {
    using _Self = _XorList_node;
    _Tp _M_data;
    node_pointer _M_xor;
    _XorList_node(_Tp &&_M_data)
        : _M_data(std::move(_M_data)), _M_xor(nullptr) {}
    node_pointer _M_relative(node_pointer _rel) {
      return _M_process_xor(_M_xor, _rel);
    }
    void _M_recalc(node_pointer __p, node_pointer __n) {
      _M_xor = _M_process_xor(_M_xor, _M_process_xor(__p, __n));
    }
  };
  struct _XorList_iterator
      : public std::iterator<std::bidirectional_iterator_tag, _Tp,
                             std::ptrdiff_t, pointer, reference> {
    using base = std::iterator<std::bidirectional_iterator_tag, _Tp,
                               std::ptrdiff_t, pointer, reference>;
    using typename base::difference_type;
    using _Self = _XorList_iterator;

   private:
    node_pointer _M_node;
    node_pointer _M_prev;

   public:
    _XorList_iterator() { _M_node = _M_prev = nullptr; }
    reference operator*() { return _M_node->_M_data; }
    pointer operator->() { return &_M_node->_M_data; }
    _Self &operator++() {
      node_pointer __tmp = _M_node;
      _M_node = _M_node->_M_relative(_M_prev);
      _M_prev = __tmp;
      return *this;
    }
    _Self operator++(int) {
      _Self __tmp = *this;
      operator++();
      return __tmp;
    }
    _Self &operator--() {
      node_pointer __tmp = _M_node;
      _M_node = _M_prev;
      _M_prev = (_M_prev)->_M_relative(__tmp);
      return *this;
    }
    _Self operator--(int) {
      _Self __tmp = *this;
      operator--();
      return __tmp;
    }
    friend class XorList;
    constexpr bool operator==(const _Self &other) const noexcept {
      return other._M_node==_M_node && other._M_prev==_M_prev;
    }
    bool operator!=(const _Self &other) { return !operator==(other); }
  };

 private:
  _Node_alloc_type _M_node_allocator;
  iterator _M_begin;
  iterator _M_end;
  size_type _M_size;
  constexpr node_pointer _M_get_node() {
    return _Node_alloc_traits::allocate(_M_node_allocator, 1);
  }
  template<typename... _Args>
  node_pointer _M_create_node(_Args &&... __args) {
    node_pointer __p = _M_get_node();
    _Node_alloc_traits::construct(
        _M_node_allocator, __p, std::move(_Tp(std::forward<_Args>(__args)...)));
    return __p;
  }
  template<typename... _Args>
  iterator _M_insert(iterator __position, _Args &&... __args) {
    node_pointer __c1 = _M_create_node(std::forward<_Args>(__args)...);
    node_pointer &__p = __position._M_prev;
    node_pointer __c = __position._M_node;
    if (__p!=nullptr)
      (__p)->_M_recalc(__c, __c1);
    __c1->_M_recalc(__p, __c);
    __c->_M_recalc(__p, __c1);
    if (__position==_M_begin) {
      _M_begin._M_prev = __c1;
      --_M_begin;
    }
    if (__position==_M_end)
      _M_end._M_prev = __c1;
    __p = __c1;
    ++_M_size;
    return __position;
  }
  void _M_destroy() {
    while (!empty())
      pop_back();
    _M_free_node(_M_end._M_node);
  }
  void _M_free_node(node_pointer __node) {
    _Node_alloc_traits::destroy(_M_node_allocator, __node);
    _Node_alloc_traits::deallocate(_M_node_allocator, __node, 1);
  }

 public:
  explicit XorList(const _Alloc &alloc = _Alloc())
      : _M_node_allocator(alloc), _M_size(0) {
    _M_begin._M_node = _M_end._M_node = _M_create_node();
    _M_begin._M_prev = _M_end._M_prev = nullptr;
  }
  iterator insert_before(iterator __position, const _Tp &__x) {
    __position = _M_insert(__position, __x);
    return __position;
  }
  iterator insert_before(iterator __position, _Tp &&__x) {
    __position = _M_insert(__position, std::move(__x));
    return __position;
  }
  iterator insert_after(iterator __position, const _Tp &__x) {
    ++__position;
    __position = _M_insert(__position, __x);
    return --__position;
  }
  iterator insert_after(iterator __position, _Tp &&__x) {
    ++__position;
    __position = _M_insert(__position, std::move(__x));
    return --__position;
  }
  constexpr bool empty() const noexcept { return _M_begin==_M_end; }
  void push_front(const _Tp &__x) { _M_insert(_M_begin, __x); }
  void push_front(_Tp &&__x) { _M_insert(_M_begin, std::move(__x)); }
  void push_back(const _Tp &__x) { _M_insert(_M_end, __x); }
  void push_back(_Tp &&__x) { _M_insert(_M_end, std::move(__x)); }
  iterator erase(iterator __pos) {
    iterator __ans = __pos;
    ++__ans;
    if (__pos==_M_begin) {
      ++_M_begin;
      _M_begin._M_prev = nullptr;
    }
    node_pointer p = __pos._M_prev, n = __pos._M_node->_M_relative(p),
        c = __pos._M_node;
    if (p!=nullptr)
      p->_M_recalc(c, n);
    n->_M_recalc(c, p);
    if (n==_M_end._M_node)
      _M_end._M_prev = p;
    _M_free_node(c);
    --_M_size;
    __ans._M_prev = p;
    return __ans;
  }
  void pop_back() { erase(--end()); }
  void pop_front() { erase(begin()); }
  iterator begin() noexcept { return _M_begin; }
  iterator end() noexcept { return _M_end; }
  iterator begin() const noexcept { return _M_begin; }
  iterator end() const noexcept { return _M_end; }
  constexpr size_type size() const noexcept { return _M_size; }
  XorList(size_type count, const _Tp &value, const _Alloc &alloc = _Alloc())
      : XorList(alloc) {
    for (size_type i = 0; i < count; ++i)
      push_back(value);
  }
  XorList(const XorList &other) : XorList() { *this = other; }
  XorList(XorList &&other) { *this = std::move(other); }
  XorList &operator=(const XorList &other) {
    _M_node_allocator = alloc_traits::select_on_container_copy_construction(
        other._M_node_allocator);
    for (auto it = other.begin(); it!=other.end(); ++it)
      push_back(*it);
    return *this;
  }
  XorList &operator=(XorList &&other) {
    _M_node_allocator = other._M_node_allocator;
    _M_begin = other._M_begin;
    _M_end = other._M_end;
    other._M_begin._M_node = other._M_end._M_node = _M_create_node();
    other._M_begin._M_prev = other._M_end._M_prev = nullptr;
    return *this;
  }
  XorList(std::initializer_list<_Tp> init,
          const allocator_type &Alloc = allocator_type())
      : XorList(Alloc) {
    for (auto it = init.begin(); it!=init.end(); ++it)
      push_back(*it);
  }
  ~XorList() { _M_destroy(); }
};
#endif
