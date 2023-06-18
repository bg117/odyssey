#include "kernel/info.hpp"
#include "misc/types.hpp"

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
  while (1)
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
} // namespace

extern "C" void _start()
{
  asm volatile("movq %%rsp, %0" : "=m"(INFO.stack.location));
  asm volatile("mov %cr0, %rax;"
               "and $0xFFFB, %ax;"
               "or $0x2, %rax;"
               "mov %rax, %cr0;"
               "mov %cr4, %rax;"
               "or $0x600, %ax;"
               "mov %rax, %cr4;");
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

  for (counter i = 0; i < INFO.memory_map.count; i++)
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
  INFO.higher_half_kernel_offset = reinterpret_cast<offset>(&__kernel);

  INFO.stack.location -= INFO.higher_half_direct_offset;

  kmain();

  halt();
}
