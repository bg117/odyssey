#pragma once

#include <cstdint>

namespace memory
{
namespace vmm
{
void initialize();

void *allocate(uint64_t count = 1);
void deallocate(void *page);
}
} // namespace memory
