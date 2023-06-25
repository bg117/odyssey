#include "kernel/cpu_exception.hpp"

#include "graphics/framebuffer.hpp"
#include "low_level/isr.hpp"

namespace
{
__attribute__((noreturn)) void halt();
void abort_handler(const low_level::isr::interrupt_info *int_info);
void page_fault_handler(const low_level::isr::interrupt_info *int_info);
void fault_handler(const low_level::isr::interrupt_info *);

constexpr const char *EXCEPTIONS[] = {
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "Double fault",
    "",
    "",
    "",
    "",
    "General protection fault",
    "",
    "",
    "",
    "",
    "Machine check",
};
} // namespace

namespace kernel
{
namespace cpu_exception
{
void initialize()
{
  set_handler(0, fault_handler);
  set_handler(1, fault_handler);
  set_handler(2, fault_handler);
  set_handler(3, fault_handler);
  set_handler(4, fault_handler);
  set_handler(5, fault_handler);
  set_handler(6, fault_handler);
  set_handler(7, fault_handler);
  set_handler(8, abort_handler);
  set_handler(9, fault_handler);
  set_handler(10, fault_handler);
  set_handler(11, fault_handler);
  set_handler(12, fault_handler);
  set_handler(13, abort_handler);
  set_handler(14, page_fault_handler);
  set_handler(15, fault_handler);
  set_handler(16, fault_handler);
  set_handler(17, fault_handler);
  set_handler(18, abort_handler);
  set_handler(19, fault_handler);
  set_handler(20, fault_handler);
  set_handler(21, fault_handler);
  set_handler(22, fault_handler);
  set_handler(23, fault_handler);
  set_handler(24, fault_handler);
  set_handler(25, fault_handler);
  set_handler(26, fault_handler);
  set_handler(27, fault_handler);
  set_handler(28, fault_handler);
  set_handler(29, fault_handler);
  set_handler(30, fault_handler);
  set_handler(31, fault_handler);
}
} // namespace cpu_exception
} // namespace kernel

namespace
{
__attribute__((noreturn)) void halt()
{
  asm volatile("cli");
  while (true)
  {
    asm volatile("hlt");
  }
}

void abort_handler(const low_level::isr::interrupt_info *int_info)
{
  graphics::framebuffer::printf("ABORT: %s\nError code 0x%02X\nRegister dump:",
                                EXCEPTIONS[int_info->vector],
                                int_info->error_code);
  graphics::framebuffer::printf("General Purpose Registers:\n");
  graphics::framebuffer::printf("RAX:\t0x%016lX\tRBX:\t0x%016lX\n",
                                int_info->registers.rax,
                                int_info->registers.rbx);
  graphics::framebuffer::printf("RCX:\t0x%016lX\tRDX:\t0x%016lX\n",
                                int_info->registers.rcx,
                                int_info->registers.rdx);
  graphics::framebuffer::printf("RSI:\t0x%016lX\tRDI:\t0x%016lX\n",
                                int_info->registers.rsi,
                                int_info->registers.rdi);
  graphics::framebuffer::printf("RBP:\t0x%016lX\tRSP:\t0x%016lX\n",
                                int_info->registers.rbp,
                                int_info->registers.rsp);
  graphics::framebuffer::printf("R8:\t0x%016lX\tR9:\t0x%016lX\n",
                                int_info->registers.r8, int_info->registers.r9);
  graphics::framebuffer::printf("R10:\t0x%016lX\tR11:\t0x%016lX\n",
                                int_info->registers.r10,
                                int_info->registers.r11);
  graphics::framebuffer::printf("R12:\t0x%016lX\tR13:\t0x%016lX\n",
                                int_info->registers.r12,
                                int_info->registers.r13);
  graphics::framebuffer::printf("R14:\t0x%016lX\tR15:\t0x%016lX\n",
                                int_info->registers.r14,
                                int_info->registers.r15);

  graphics::framebuffer::printf("\nControl Registers:\n");
  graphics::framebuffer::printf("CR0:\t0x%016lX\tCR2:\t0x%016lX\n",
                                int_info->registers.cr0,
                                int_info->registers.cr2);
  graphics::framebuffer::printf("CR3:\t0x%016lX\tCR4:\t0x%016lX\n",
                                int_info->registers.cr3,
                                int_info->registers.cr4);
  halt();
}

void page_fault_handler(const low_level::isr::interrupt_info *int_info)
{
  graphics::framebuffer::printf("PAGE FAULT: tried to access 0x%016lX\n",
                                int_info->registers.cr2);
  halt();
}

void fault_handler(const low_level::isr::interrupt_info *)
{
}
} // namespace
