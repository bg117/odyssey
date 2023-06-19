#include "graphics/framebuffer.hpp"
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

void pf_handler(low_level::isr::interrupt_info *int_info)
{
  LOG("Page fault: 0x%hhX", int_info->vector);
  graphics::framebuffer::printf("rax: 0x%016lX\n", int_info->registers.rax);
  graphics::framebuffer::printf("rbx: 0x%016lX\n", int_info->registers.rbx);
  graphics::framebuffer::printf("rcx: 0x%016lX\n", int_info->registers.rcx);
  graphics::framebuffer::printf("rdx: 0x%016lX\n", int_info->registers.rdx);
  graphics::framebuffer::printf("rsp: 0x%016lX\n", int_info->registers.rsp);
  graphics::framebuffer::printf("rbp: 0x%016lX\n", int_info->registers.rbp);
  graphics::framebuffer::printf("rsi: 0x%016lX\n", int_info->registers.rsi);
  graphics::framebuffer::printf("rdi: 0x%016lX\n", int_info->registers.rdi);
  graphics::framebuffer::printf("r8: 0x%016lX\n", int_info->registers.r8);
  graphics::framebuffer::printf("r9: 0x%016lX\n", int_info->registers.r9);
  graphics::framebuffer::printf("r10: 0x%016lX\n", int_info->registers.r10);
  graphics::framebuffer::printf("r11: 0x%016lX\n", int_info->registers.r11);
  graphics::framebuffer::printf("r12: 0x%016lX\n", int_info->registers.r12);
  graphics::framebuffer::printf("r13: 0x%016lX\n", int_info->registers.r13);
  graphics::framebuffer::printf("r14: 0x%016lX\n", int_info->registers.r14);
  graphics::framebuffer::printf("r15: 0x%016lX\n", int_info->registers.r15);

  asm volatile("cli; hlt;");
}

void kmain()
{
  graphics::framebuffer::initialize();
  low_level::gdt::load(gdt_desc, 0x28, 0x30);
  low_level::isr::initialize(idt);
  low_level::idt::load(idt_desc);
  low_level::isr::set_handler(0x0e, pf_handler);

  volatile char a = *(volatile char *volatile)0x0;

  memory::pmm::initialize();
  memory::vmm::initialize();
  memory::heap::initialize();
}
