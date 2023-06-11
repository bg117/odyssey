#include "vmm.h"

#include "pmm.h"
#include "terminal.h"

#include <stdbool.h>

#define PAGE_SIZE 0x1000
#define REC_PM 510

#define PML4_OFFSET(x) (((x) >> 39) & 0x1ff)
#define PDPT_OFFSET(x) (((x) >> 30) & 0x1ff)
#define PD_OFFSET(x) (((x) >> 21) & 0x1ff)
#define PT_OFFSET(x) (((x) >> 12) & 0x1ff)

#define TABLE(pml4, pdpt, pd, pt)                                 \
  ((void *)(((uint64_t)(pml4) << 39) | ((uint64_t)(pdpt) << 30) | \
            ((uint64_t)(pd) << 21) | ((uint64_t)(pt) << 12)))

// check if PT is present; if not, allocate new page and set PT entry as present
#define ALLOCATE_PT(pt)                              \
  do                                                 \
  {                                                  \
    if (!((pt)->flags & PT_PRESENT))                 \
    {                                                \
      (pt)->flags     = PT_PRESENT | PT_WRITABLE;    \
      (pt)->base_addr = (uint64_t)pmm_alloc() >> 12; \
    }                                                \
  } while (0)

#define RECURSIVE 510 // index 511 where the PML4 recurses into itself
#define KERNEL_HIGHER_HALF 0xffffffff80000000

static struct page_table_entry *PML4;
static bool initialized;

extern uint64_t KERNEL_PHYS, KERNEL_SIZE, FRAMEBUFFERS_PHYS, FRAMEBUFFERS_SIZE,
    HIGHER_HALF_START;

void map_page(uint64_t phys, uint64_t virt);

void vmm_init()
{
  initialized = false;

  PML4                = pmm_alloc(); // allocate a brand new page for PML4
  PML4[510].flags     = PT_PRESENT | PT_WRITABLE;
  PML4[510].base_addr = (uint64_t)PML4 >> 12;

  const uint64_t rounded_kernel_size =
      (KERNEL_SIZE + PAGE_SIZE - 1) &
      -PAGE_SIZE; // -PAGE_SIZE is equivalent to ~(PAGE_SIZE - 1)
  const uint64_t rounded_fb_size =
      (FRAMEBUFFERS_SIZE + PAGE_SIZE - 1) & -PAGE_SIZE;

  struct page_table_entry *pml4e, *pdpte, *pde, *pte;

  // map each kernel page into directory
  for (uint64_t b = 0; b < rounded_kernel_size; b += PAGE_SIZE)
  {
    const uint64_t offset = KERNEL_HIGHER_HALF + b;

    pml4e = &PML4[PML4_OFFSET(offset)];
    ALLOCATE_PT(pml4e);

    pdpte = &((struct page_table_entry *)(pml4e->base_addr
                                          << 12))[PDPT_OFFSET(offset)];
    ALLOCATE_PT(pdpte);

    pde = &(
        (struct page_table_entry *)(pdpte->base_addr << 12))[PD_OFFSET(offset)];
    ALLOCATE_PT(pde);

    pte =
        &((struct page_table_entry *)(pde->base_addr << 12))[PT_OFFSET(offset)];

    pte->flags = PT_PRESENT | PT_WRITABLE;
    pte->base_addr =
        (KERNEL_PHYS + b) >> 12; // physical address of kernel + page offset
  }

  // map framebuffer into higher half
  for (uint64_t b = 0; b < rounded_fb_size; b += PAGE_SIZE)
  {
    const uint64_t offset = HIGHER_HALF_START + FRAMEBUFFERS_PHYS + b;

    pml4e = &PML4[PML4_OFFSET(offset)];
    ALLOCATE_PT(pml4e);

    pdpte = &((struct page_table_entry *)(pml4e->base_addr
                                          << 12))[PDPT_OFFSET(offset)];
    ALLOCATE_PT(pdpte);

    pde = &(
        (struct page_table_entry *)(pdpte->base_addr << 12))[PD_OFFSET(offset)];
    ALLOCATE_PT(pde);

    pte =
        &((struct page_table_entry *)(pde->base_addr << 12))[PT_OFFSET(offset)];

    pte->flags     = PT_PRESENT | PT_WRITABLE;
    pte->base_addr = (offset - HIGHER_HALF_START) >>
                     12; // physical address of framebuffer + page offset
  }

  // get bottom of stack (rbp)
  uint64_t rbp;
  asm volatile("movq %%rbp, %0;" : "=r"(rbp));

  // round rbp up to nearest page
  rbp = (rbp + PAGE_SIZE - 1) & -PAGE_SIZE;

  // map kernel stack into higher half
  for (uint64_t b = 0; b < 0x20000; b += PAGE_SIZE)
  {
    const uint64_t offset = (uint64_t)rbp - b; // x86 stack grows downwards

    pml4e = &PML4[PML4_OFFSET(offset)];
    ALLOCATE_PT(pml4e);

    pdpte = &((struct page_table_entry *)(pml4e->base_addr
                                          << 12))[PDPT_OFFSET(offset)];
    ALLOCATE_PT(pdpte);

    pde = &(
        (struct page_table_entry *)(pdpte->base_addr << 12))[PD_OFFSET(offset)];
    ALLOCATE_PT(pde);

    pte =
        &((struct page_table_entry *)(pde->base_addr << 12))[PT_OFFSET(offset)];

    pte->flags     = PT_PRESENT | PT_WRITABLE;
    pte->base_addr = (offset - HIGHER_HALF_START) >>
                     12; // physical address of framebuffer + page offset
  }

  initialized = true;

  // LOAD LEVEL 4 PAGE TABLE!!!!
  asm volatile("xchg %%bx, %%bx; mov %0, %%cr3" ::"r"(PML4));
}

void map_page(uint64_t phys, uint64_t virt)
{
  struct page_table_entry *pdpt = TABLE(RECURSIVE, RECURSIVE, RECURSIVE,
                                        PML4_OFFSET(virt)),
                          *pd   = TABLE(RECURSIVE, RECURSIVE, PML4_OFFSET(virt),
                                        PDPT_OFFSET(virt)),
                          *pt   = TABLE(RECURSIVE, PML4_OFFSET(virt),
                                        PDPT_OFFSET(virt), PD_OFFSET(virt));

  // check if each level is present
  ALLOCATE_PT(pdpt);
  ALLOCATE_PT(pd);
  ALLOCATE_PT(pt);

  // set page frame address in level 1 page table
  pt[PT_OFFSET(virt)].flags     = PT_PRESENT | PT_WRITABLE;
  pt[PT_OFFSET(virt)].base_addr = phys >> 12; // strip off lower 12 bits
}
