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
low_level::gdt::descriptor gdt_desc = {.limit = sizeof(gdt) - 1, .base = gdt};
} // namespace

void kmain()
{
  graphics::framebuffer::initialize();
  low_level::gdt::load(gdt_desc, 0x28, 0x30);

  memory::pmm::initialize();
  memory::vmm::initialize();
  memory::heap::initialize();
}
