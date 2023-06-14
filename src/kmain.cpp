#include "graphics/framebuffer.hpp"
#include "low_level/gdt.hpp"
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
  graphics::framebuffer::print("Odyssey\n");

  low_level::gdt::make_descriptor(gdt_desc, gdt, 9);
  low_level::gdt::load(gdt_desc, 0x28, 0x30);
}
