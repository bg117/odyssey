#include "graphics/framebuffer.hpp"
#include "low_level/gdt.hpp"
#include "memory/heap.hpp"
#include "memory/pmm.hpp"
#include "memory/vmm.hpp"
#include "misc/log.hpp"

namespace
{
low_level::gdt::entry gdt[] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0, 0, 0x9a, 0, 0x8, 0},
    {0xffff, 0, 0, 0x92, 0, 0x8, 0},
    {0xffff, 0, 0, 0x9a, 0xf, 0xc, 0},
    {0xffff, 0, 0, 0x92, 0xf, 0xc, 0},
    {0, 0, 0, 0x9a, 0x2, 0xa, 0},
    {0, 0, 0, 0x92, 0, 0xa, 0},
    {0, 0, 0, 0xf2, 0, 0, 0},
    {0, 0, 0, 0xfa, 0, 2, 0},
};
low_level::gdt::descriptor gdt_desc;
} // namespace

void kmain()
{
  graphics::framebuffer::initialize();

  low_level::gdt::make_descriptor(gdt_desc, gdt, 9);
  low_level::gdt::load(gdt_desc, 0x28, 0x30);

  memory::pmm::initialize();
  memory::vmm::initialize();
  memory::heap::initialize();

  for (int i = 1; i <= 20; i++)
  {
    void *alloc =  memory::heap::allocate(i * 2 + 1000);
    LOG("test heap alloc[%d]: %p", i, alloc);
  }
  for (int i = 21; i <= 30; i++)
  {
    void *alloc =  memory::heap::allocate(i * 2);
    LOG("test heap alloc[%d]: %p", i, alloc);
  }
}
