#include "memory/heap.hpp"

#include <cstddef>

void *operator new(const uint64_t bytes)
{
  return memory::heap::allocate(bytes);
}

void *operator new[](const uint64_t bytes)
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

void operator delete(void *block, uint64_t) noexcept
{
  memory::heap::deallocate(block);
}

void operator delete[](void *block, uint64_t) noexcept
{
  memory::heap::deallocate(block);
}
