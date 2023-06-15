#pragma once

namespace memory
{
namespace pmm
{
void initialize();

void *allocate();
void deallocate(void *page);
} // namespace pmm
} // namespace memory