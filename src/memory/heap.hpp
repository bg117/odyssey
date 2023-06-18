#pragma once

#include <cstdint>

namespace memory
{
namespace heap
{
void initialize();

void *allocate(uint64_t bytes);
void deallocate(void *block);
} // namespace heap
} // namespace memory
