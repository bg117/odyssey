#pragma once

namespace memory
{
namespace vmm
{
void initialize();

void *allocate();
void deallocate(void *page);
}
} // namespace memory