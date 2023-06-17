#include "memory/heap.hpp"

#include <cstddef>

void *operator new(size_t bytes)
{
  return memory::heap::allocate(bytes);
}

void *operator new[](size_t bytes)
{
  return memory::heap::allocate(bytes);
}

void operator delete(void *block) noexcept
{
  memory::heap::deallocate(block);
}

void operator delete[](void *block) noexcept
{
  memory::heap::deallocate(block);
}
