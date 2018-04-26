#pragma  once
#include <cstdlib>
class IMemoryManager {
public:
  virtual void *Alloc(size_t size) = 0;
  virtual void Free(void *ptr) = 0;
  virtual ~IMemoryManager() = default;
};
