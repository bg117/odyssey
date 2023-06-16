#include "memory/heap.hpp"

#include "memory/vmm.hpp"
#include "misc/log.hpp"

struct __attribute__((aligned(alignof(max_align_t)))) block_info
{
  void *address;
  size_t size;
  block_info *next, *prev;
};

namespace
{
block_info *head, *tail;
}

namespace memory
{
namespace heap
{
void initialize()
{
  head = tail = nullptr;
}

void *allocate(size_t bytes)
{
  LOG("allocate %lu bytes", bytes);
  return nullptr;
}

void deallocate(void *block)
{
  LOG("deallocate %p", block);
}
} // namespace heap
} // namespace memory
