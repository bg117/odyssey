#pragma once

#include <cstddef>

namespace memory
{
namespace heap
{
void initialize();

void *allocate(size_t bytes);
void deallocate(void *block);
}
} // namespace memory
