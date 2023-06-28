#include "kernel/info.hpp"

#include <cstring>

kernel::info INFO = {};

extern char __kernel;
extern void kmain();

namespace
{
/* stop the CPU */
void halt()
{
  asm volatile("cli");
  while (true)
  {
    asm volatile("hlt");
  }
}

/* features */
volatile struct limine_framebuffer_request fb_request = {
    .id       = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};
volatile struct limine_memmap_request mmap_request = {.id =
                                                          LIMINE_MEMMAP_REQUEST,
                                                      .revision = 0};
volatile struct limine_hhdm_request hhdm_request   = {.id = LIMINE_HHDM_REQUEST,
                                                      .revision = 0};
[[maybe_unused]] volatile struct limine_stack_size_request ss_request = {
    .id         = LIMINE_STACK_SIZE_REQUEST,
    .revision   = 0,
    .stack_size = 0x20000};

volatile limine_rsdp_request rsdp_request = {.id       = LIMINE_RSDP_REQUEST,
                                             .revision = 0};
} // namespace

extern "C" void _start()
{
  asm volatile("mov %0, rsp" : "=m"(INFO.stack.location));
  // enable SSE
  asm volatile("mov rax, cr0\n;"
               "and ax, 0xFFFB\n"
               "or rax, 0x2\n"
               "mov cr0, rax\n"
               "mov rax, cr4\n"
               "or ax, 0x600\n"
               "mov cr4, rax\n");
  INFO.stack.size = ss_request.stack_size;

  // we got nowhere to write on
  if (fb_request.response == nullptr ||
      fb_request.response->framebuffer_count < 1)
  {
    halt();
  }

  // copy members of 1st framebuffer
  memcpy(&INFO.framebuffer.info, fb_request.response->framebuffers[0],
         sizeof(struct limine_framebuffer));

  // try to find where stuff is located in memory map
  limine_memmap_entry **mmap = mmap_request.response->entries;

  INFO.memory_map.map   = mmap;
  INFO.memory_map.count = mmap_request.response->entry_count;

  for (uint64_t i = 0; i < INFO.memory_map.count; i++)
  {
    switch (mmap[i]->type)
    {
    case LIMINE_MEMMAP_FRAMEBUFFER:
      INFO.framebuffer.location = mmap[i]->base;
      INFO.framebuffer.size     = mmap[i]->length;
      break;
    case LIMINE_MEMMAP_KERNEL_AND_MODULES:
      INFO.kernel.location = mmap[i]->base;
      INFO.kernel.size     = mmap[i]->length;
      break;
    }
  }

  INFO.higher_half_direct_offset = hhdm_request.response->offset;
  INFO.higher_half_kernel_offset = reinterpret_cast<uint64_t>(&__kernel);

  INFO.stack.location -= INFO.higher_half_direct_offset;

  INFO.rsdp.location = reinterpret_cast<uintptr_t>(rsdp_request.response->address);
  INFO.rsdp.size = sizeof(low_level::rsdp);
  memcpy(&INFO.rsdp.info, rsdp_request.response->address,
         sizeof(low_level::rsdp));

  kmain();

  halt();
}
