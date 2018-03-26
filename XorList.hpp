#pragma once
#ifndef _XOR_LIST_H_
#define _XOR_LIST_H_
#include <cstddef>
#include <iterator>
namespace __sg_lib {
template <typename _Tp, typename _Alloc> class XorList {
public:
  typedef _Tp value_type;
  typedef typename _Alloc::pointer pointer;
  typedef typename _Alloc::const_pointer const_pointer;
  typedef typename _Alloc::reference reference;
  typedef typename _Alloc::const_reference const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Alloc allocator_type;
  template <typename _Ref, typename _Ptr>
  struct _XorList_iterator
      : public std::iterator<std::bidirectional_iterator_tag, _Tp,
                             std::ptrdiff_t, _Ptr, _Ref> {
    using base = std::iterator<std::bidirectional_iterator_tag, _Tp,
                               std::ptrdiff_t, _Ptr, _Ref>;
    using typename base::pointer;
    using typename base::reference;
    using typename base::difference_type;
    using typename base::value_type;
    typedef _XorList_iterator _Self;
  };
  typedef _XorList_iterator<reference, pointer> iterator;
  typedef _XorList_iterator<const_reference, const_pointer> const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;

private:
  allocator_type _M_allocator;
  iterator _M_begin;
  iterator _M_end;

public:
  explicit XorList(const _Alloc &alloc = _Alloc()) : _M_allocator(alloc) {}
  XorList(size_type count, const _Tp &value = _Tp(),
          const _Alloc &alloc = _Alloc());
};
}
#endif
