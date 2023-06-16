#include "graphics/framebuffer.hpp"
#include "low_level/gdt.hpp"
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

  auto p1 = memory::vmm::allocate(35);
  auto p2 = memory::vmm::allocate(16);
  memory::vmm::deallocate(p1);
  auto p3 = memory::vmm::allocate(18);
  auto p4 = memory::vmm::allocate(18);

  LOG("%p, %p, %p, %p", p1, p2, p3, p4);

  int *a = new int[10];
  delete[] a;
}
