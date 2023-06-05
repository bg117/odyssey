#include "vmm.h"

#include "pmm.h"

#define PAGE_SIZE 0x1000

#define PML4_OFFSET(x) (((x) >> 39) & 0x1ff)
#define PDPT_OFFSET(x) (((x) >> 30) & 0x1ff)
#define PD_OFFSET(x) (((x) >> 21) & 0x1ff)
#define PT_OFFSET(x) (((x) >> 12) & 0x1ff)

static struct page_table_entry *pml4;

extern uint64_t KERNEL_PHYS;
extern uint64_t KERNEL_SIZE;

void vmm_init()
{
  pml4 = pmm_alloc();
  // round kernel size up to the nearest page
  uint64_t kernel_size_round = (KERNEL_SIZE + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

  // map the kernel into the higher half (0xffffffff80000000)
  uint64_t base = 0xffffffff80000000;
  for (uint64_t i = 0; i < kernel_size_round; i += PAGE_SIZE)
  {
    struct page_table_entry *pml4_ent = &pml4[PML4_OFFSET(base + i)];
    // if the entry is not used, allocate a new page table
    if (!pml4_ent->used)
    {
      pml4_ent->used      = 1;
      pml4_ent->flags     = PT_PRESENT | PT_WRITABLE;
      pml4_ent->base_addr = (uint64_t)pmm_alloc() >> 12;
    }

    struct page_table_entry *pdpt_ent = (void*)(pml4_ent->base_addr << 12);

  }
}
