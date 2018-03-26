#include "StackAllocator_fancy.hpp"
int main() {
  StackAllocator<int>::pointer ptr;
  typename StackAllocator<int>::const_pointer const_ptr(ptr);
}
