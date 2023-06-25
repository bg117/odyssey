#include "graphics/framebuffer.hpp"
#include "kernel/cpu_exception.hpp"
#include "low_level/gdt.hpp"
#include "low_level/idt.hpp"
#include "low_level/isr.hpp"
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

low_level::idt::entry idt[256]      = {};
low_level::idt::descriptor idt_desc = {.limit = sizeof(idt) - 1, .base = idt};
} // namespace

void kmain()
{
  graphics::framebuffer::initialize();
  load(gdt_desc, 0x28, 0x30);

  low_level::isr::initialize(idt);
  load(idt_desc);

  kernel::cpu_exception::initialize();

  memory::pmm::initialize();
  memory::vmm::initialize();
  memory::heap::initialize();

  graphics::framebuffer::print("Done initializing.\n");
  graphics::framebuffer::print("---\n");
  graphics::framebuffer::printf(
      "ODYSSEY operating system, compiled at %s on %s\n", __DATE__,
      __TIMESTAMP__);
}
