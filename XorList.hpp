#pragma once
#ifndef _XOR_LIST_H_
#define _XOR_LIST_H_
#include <cstddef>
#include <iterator>
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
                             std::ptrdiff_t, _Tp *, _Ref> {
    using base = std::iterator<std::bidirectional_iterator_tag, _Tp,
                               std::ptrdiff_t, _Tp *, _Ref>;
    using typename base::pointer;
    using typename base::reference;
    using typename base::difference_type;
    using typename base::value_type;
    typedef _XorList_iterator _Self;
  };
  // typedef _List_iterator<_Tp>                        iterator;
  // typedef _List_const_iterator<_Tp>                  const_iterator;
  // typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
  // typedef std::reverse_iterator<iterator>            reverse_iterator;
  explicit XorList(const _Alloc &alloc = _Alloc());
  XorList(size_type count, const _Tp &value = _Tp(),
          const _Alloc &alloc = _Alloc());
};
#endif
