#include "StackAllocator.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <random>
static std::random_device rd;
static std::mt19937 gen(rd());
std::uniform_int_distribution<int> distr;
template <typename T> void process_operations(size_t n1, size_t n2) {
  std::list<int, T> mlist;
  for (size_t i = 0; i < n1; ++i)
    mlist.push_back(distr(gen));
  for (size_t i = 0; i < n2; ++i) {
    bool del = distr(gen) % 2;
    if (del && !mlist.empty())
      mlist.pop_back();
    else
      mlist.push_back(distr(gen));
  }
  return;
}
int main() {
  // StackAllocator<int> myalloc;
  // int *beg = myalloc.allocate(1);
  // myalloc.construct(beg, 1);
  size_t n1, n2;
  std::cin >> n1 >> n2;
  auto begin = std::chrono::steady_clock::now();
  process_operations<std::allocator<int>>(n1, n2);
  auto end = std::chrono::steady_clock::now();
  std::cout << "Standard allocator:\t"
            << std::chrono::duration<double, std::milli>(end - begin).count()
            << " ms\n"
            << std::endl;
  begin = std::chrono::steady_clock::now();
  process_operations<StackAllocator<int>>(n1, n2);
  end = std::chrono::steady_clock::now();
  std::cout << "Stack allocator:\t"
            << std::chrono::duration<double, std::milli>(end - begin).count()
            << " ms\n"
            << std::endl;
}
