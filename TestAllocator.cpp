#include "StackAllocator.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <iterator>
#include <list>
#include <random>
#include <vector>
template <typename T>
std::list<int, T> process_operations(size_t n1, size_t n2,
                                     const std::vector<int> &mvec) {
  std::list<int, T> mlist;
  auto it = mvec.begin();
  for (size_t i = 0; i < n1; ++i) {
    mlist.push_back(*it);
    ++it;
  }
  for (size_t i = 0; i < n2; ++i) {
    bool del = *it % 2;
    ++it;
    if (del && !mlist.empty())
      mlist.pop_back();
    else {
      mlist.push_back(*it);
      ++it;
    }
  }
  return mlist;
}
int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distr;
  // StackAllocator<int> myalloc;
  // int *beg = myalloc.allocate(1);
  // myalloc.construct(beg, 1);
  size_t n1, n2;
  std::cin >> n1 >> n2;
  std::vector<int> mvec(n1 + 2 * n2);
  std::generate(mvec.begin(), mvec.end(),
                [&distr, &gen]() { return distr(gen); });
  auto begin = std::chrono::steady_clock::now();
  auto l1 = process_operations<std::allocator<int>>(n1, n2, mvec);
  auto end = std::chrono::steady_clock::now();
  std::cout << "Standard allocator:\t"
            << std::chrono::duration<double, std::milli>(end - begin).count()
            << " ms\n"
            << std::endl;
  begin = std::chrono::steady_clock::now();
  auto l2 = process_operations<StackAllocator<int>>(n1, n2, mvec);
  end = std::chrono::steady_clock::now();
  std::cout << "Stack allocator:\t"
            << std::chrono::duration<double, std::milli>(end - begin).count()
            << " ms\n"
            << std::endl;
  /*std::copy(l1.begin(), l1.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  std::copy(l2.begin(), l2.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;*/
  assert(std::equal(l1.begin(), l1.end(), l2.begin()));
  return 0;
}
