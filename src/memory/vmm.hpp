#pragma once

#include <cstdint>

namespace memory
{
namespace vmm
{
void initialize();

void *allocate(uint64_t count = 1, bool higher_half = true);
void *allocate(const void *pre, uint64_t count = 1, bool higher_half = true);
void deallocate(void *page);
} // namespace vmm
} // namespace memory
