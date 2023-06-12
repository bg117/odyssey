#include "vmm.h"

#include "log.h"
#include "pmm.h"
#include "terminal.h"

#include <stdbool.h>

#define PAGE_SIZE 0x1000
#define REC_PM 510

#define PML4_OFFSET(x) (((x) >> 39) & 0x1ff)
#define PDPT_OFFSET(x) (((x) >> 30) & 0x1ff)
#define PD_OFFSET(x) (((x) >> 21) & 0x1ff)
#define PT_OFFSET(x) (((x) >> 12) & 0x1ff)

#define ENTRY(pdpt, pd, pt, offset)                                   \
  &((struct page_table_entry *)(0xffff000000000000 | (510ull << 39) | \
                                ((uint64_t)(pdpt) << 30) |            \
                                ((uint64_t)(pd) << 21) |              \
                                ((uint64_t)(pt) << 12)))[offset]
#define VIRTUALADDR(pml4, pdpt, pd, pt)                                       \
  (0xffff000000000000 | ((uint64_t)(pml4) << 39) | ((uint64_t)(pdpt) << 30) | \
   ((uint64_t)(pd) << 21) | ((uint64_t)(pt) << 12))

// check if PT is present; if not, allocate new page and set PT entry as present
#define ALLOCATE_PTU(pt)                             \
  do                                                 \
  {                                                  \
    if (!((pt)->flags & PT_PRESENT))                 \
    {                                                \
      (pt)->flags     = PT_PRESENT | PT_WRITABLE;    \
      (pt)->base_addr = (uint64_t)pmm_alloc() >> 12; \
    }                                                \
  } while (0)
#define ALLOCATE_PT(pt) \
  ALLOCATE_PTU(pt);     \
  RELOAD_CR3()
#define RELOAD_CR3() asm volatile("mov %0, %%cr3" ::"r"(PML4))

#define RECURSIVE 510 // index 511 where the PML4 recurses into itself
#define KERNEL_HIGHER_HALF 0xffffffff80000000

static struct page_table_entry *PML4;
static bool initialized;

extern uint64_t KERNEL_PHYS, KERNEL_SIZE, FRAMEBUFFERS_PHYS, FRAMEBUFFERS_SIZE,
    HIGHER_HALF_START, *BITMAP, BITMAP_SIZE;
extern void *STACK_TOP;

void map_page(uint64_t phys, uint64_t virt);
struct page_table_entry *find_first_free_pte(uint64_t *pml4_idx,
                                             uint64_t *pdpt_idx,
                                             uint64_t *pd_idx,
                                             uint64_t *pt_idx);

void vmm_init()
{
  initialized = false;

  LOG("Creating new PML4...");

  PML4                = pmm_alloc(); // allocate a brand new page for PML4
  PML4[510].flags     = PT_PRESENT | PT_WRITABLE;
  PML4[510].base_addr = (uint64_t)PML4 >> 12;

  const uint64_t rounded_kernel_size =
      (KERNEL_SIZE + PAGE_SIZE - 1) &
      -PAGE_SIZE; // -PAGE_SIZE is equivalent to ~(PAGE_SIZE - 1)
  const uint64_t rounded_fb_size =
      (FRAMEBUFFERS_SIZE + PAGE_SIZE - 1) & -PAGE_SIZE;

  struct page_table_entry *pml4e, *pdpte, *pde, *pte;

  LOG("Mapping kernel into higher half...");

  // map each kernel page into directory
  for (uint64_t b = 0; b < rounded_kernel_size; b += PAGE_SIZE)
  {
    const uint64_t offset = KERNEL_HIGHER_HALF + b;

    pml4e = &PML4[PML4_OFFSET(offset)];
    ALLOCATE_PTU(pml4e);

    pdpte = &((struct page_table_entry *)(pml4e->base_addr
                                          << 12))[PDPT_OFFSET(offset)];
    ALLOCATE_PTU(pdpte);

    pde = &(
        (struct page_table_entry *)(pdpte->base_addr << 12))[PD_OFFSET(offset)];
    ALLOCATE_PTU(pde);

    pte =
        &((struct page_table_entry *)(pde->base_addr << 12))[PT_OFFSET(offset)];

    pte->flags = PT_PRESENT | PT_WRITABLE;
    pte->base_addr =
        (KERNEL_PHYS + b) >> 12; // physical address of kernel + page offset
  }

  LOG("Mapping framebuffer into higher half...");

  // map framebuffer into higher half
  for (uint64_t b = 0; b < rounded_fb_size; b += PAGE_SIZE)
  {
    const uint64_t offset = HIGHER_HALF_START + FRAMEBUFFERS_PHYS + b;

    pml4e = &PML4[PML4_OFFSET(offset)];
    ALLOCATE_PTU(pml4e);

    pdpte = &((struct page_table_entry *)(pml4e->base_addr
                                          << 12))[PDPT_OFFSET(offset)];
    ALLOCATE_PTU(pdpte);

    pde = &(
        (struct page_table_entry *)(pdpte->base_addr << 12))[PD_OFFSET(offset)];
    ALLOCATE_PTU(pde);

    pte =
        &((struct page_table_entry *)(pde->base_addr << 12))[PT_OFFSET(offset)];

    pte->flags     = PT_PRESENT | PT_WRITABLE;
    pte->base_addr = (offset - HIGHER_HALF_START) >>
                     12; // physical address of framebuffer + page offset
  }

  // get bottom of stack (rbp)
  uint64_t rbp = (uint64_t)STACK_TOP;
  // round rbp up to nearest page
  rbp = (rbp + PAGE_SIZE - 1) & -PAGE_SIZE;

  LOG("Stack top at %016llp, max size %llp", rbp, 0x20000);
  LOG("Mapping stack into higher half...");

  // map kernel stack into higher half
  for (uint64_t b = 0; b < 0x20000; b += PAGE_SIZE)
  {
    const uint64_t offset = (uint64_t)rbp - b; // x86 stack grows downwards

    pml4e = &PML4[PML4_OFFSET(offset)];
    ALLOCATE_PTU(pml4e);

    pdpte = &((struct page_table_entry *)(pml4e->base_addr
                                          << 12))[PDPT_OFFSET(offset)];
    ALLOCATE_PTU(pdpte);

    pde = &(
        (struct page_table_entry *)(pdpte->base_addr << 12))[PD_OFFSET(offset)];
    ALLOCATE_PTU(pde);

    pte =
        &((struct page_table_entry *)(pde->base_addr << 12))[PT_OFFSET(offset)];

    pte->flags     = PT_PRESENT | PT_WRITABLE;
    pte->base_addr = (offset - HIGHER_HALF_START) >>
                     12; // physical address of framebuffer + page offset
  }

  LOG("Mapping bitmap into higher half...");

  // map PMM bitmap into higher half
  for (uint64_t b = 0; b < BITMAP_SIZE; b += PAGE_SIZE)
  {
    const uint64_t offset = (uint64_t)BITMAP + b;

    pml4e = &PML4[PML4_OFFSET(offset)];
    ALLOCATE_PTU(pml4e);

    pdpte = &((struct page_table_entry *)(pml4e->base_addr
                                          << 12))[PDPT_OFFSET(offset)];
    ALLOCATE_PTU(pdpte);

    pde = &(
        (struct page_table_entry *)(pdpte->base_addr << 12))[PD_OFFSET(offset)];
    ALLOCATE_PTU(pde);

    pte =
        &((struct page_table_entry *)(pde->base_addr << 12))[PT_OFFSET(offset)];

    pte->flags     = PT_PRESENT | PT_WRITABLE;
    pte->base_addr = (offset - HIGHER_HALF_START) >>
                     12; // physical address of framebuffer + page offset
  }

  initialized = true;

  LOG("Loading CR3 with PML4...");

  // LOAD LEVEL 4 PAGE TABLE!!!!
  RELOAD_CR3();
}

void map_page(uint64_t phys, uint64_t virt)
{
  LOG("Mapping phys %016llp to virt %016llp...", phys, virt);

  // obtain page structure entries
  struct page_table_entry *pml4e = ENTRY(RECURSIVE, RECURSIVE, RECURSIVE,
                                         PML4_OFFSET(virt)),
                          *pdpte = ENTRY(RECURSIVE, RECURSIVE,
                                         PML4_OFFSET(virt), PDPT_OFFSET(virt)),
                          *pde   = ENTRY(RECURSIVE, PML4_OFFSET(virt),
                                         PDPT_OFFSET(virt), PD_OFFSET(virt)),
                          *pte   = ENTRY(PML4_OFFSET(virt), PDPT_OFFSET(virt),
                                         PD_OFFSET(virt), PT_OFFSET(virt));

  // allocate the missing structures (if any)
  ALLOCATE_PT(pml4e);
  ALLOCATE_PT(pdpte);
  ALLOCATE_PT(pde);

  // set page address
  pte->flags     = PT_PRESENT | PT_WRITABLE;
  pte->base_addr = phys >> 12;
}

struct page_table_entry *find_first_free_pte(uint64_t *pml4_idx,
                                             uint64_t *pdpt_idx,
                                             uint64_t *pd_idx, uint64_t *pt_idx)
{
  LOG("Trying to find free page table entry...");

  uint64_t pml4i, pdpti, pdi, pti;
  struct page_table_entry *pml4e, *pdpte, *pde, *pte;

  for (pml4i = 256; pml4i < 512; pml4i++)
  {
    // get PML4 entry associated
    pml4e = ENTRY(RECURSIVE, RECURSIVE, RECURSIVE, pml4i);
    ALLOCATE_PT(pml4e);

    for (pdpti = 0; pdpti < 512; pdpti++)
    {
      pdpte = ENTRY(RECURSIVE, RECURSIVE, pml4i, pdpti);
      ALLOCATE_PT(pdpte);

      for (pdi = 0; pdi < 512; pdi++)
      {
        pde = ENTRY(RECURSIVE, pml4i, pdpti, pdi);
        ALLOCATE_PT(pde);

        for (pti = 0; pti < 512; pti++)
        {
          pte = ENTRY(pml4i, pdpti, pdi, pti);
          if (!(pte->flags & PT_PRESENT))
          {
            LOG("First free virtual address: "
                "PML4[%llu]->PDPT[%llu]->PD[%llu]->PT[%llu]",
                pml4i, pdpti, pdi, pti);
            *pml4_idx = pml4i;
            *pdpt_idx = pdpti;
            *pd_idx   = pdi;
            *pt_idx   = pti;
            return pte;
          }
        }
      }
    }
  }

  LOG("Critical: no more free virtual address (how does this even happen?)");
  return NULL; // no free :(
}

void *vmm_alloc(void)
{
  // get a free PTE
  uint64_t pml4i, pdpti, pdi, pti;
  struct page_table_entry *pte =
      find_first_free_pte(&pml4i, &pdpti, &pdi, &pti);

  // if NULL, return NULL
  if (pte == NULL)
  {
    return NULL;
  }

  // allocate phys mem for PTE
  void *block = pmm_alloc();

  // set members
  pte->flags     = PT_PRESENT | PT_WRITABLE;
  pte->base_addr = (uint64_t)block >> 12;

  uint64_t addr = VIRTUALADDR(pml4i, pdpti, pdi, pti);
  LOG("Allocated virtual %016llp", addr);
  return (void *)addr;
}

void vmm_free(void *page)
{
  uint64_t addr = (uint64_t)page;

  struct page_table_entry *pml4e = ENTRY(RECURSIVE, RECURSIVE, RECURSIVE,
                                         PML4_OFFSET(addr)),
                          *pdpte = ENTRY(RECURSIVE, RECURSIVE,
                                         PML4_OFFSET(addr), PDPT_OFFSET(addr)),
                          *pde   = ENTRY(RECURSIVE, PML4_OFFSET(addr),
                                         PDPT_OFFSET(addr), PD_OFFSET(addr)),
                          *pte   = ENTRY(PML4_OFFSET(addr), PDPT_OFFSET(addr),
                                         PD_OFFSET(addr), PT_OFFSET(addr));
  // if any structure in the hierarchy is not actually present then abort
  if (!((pml4e->flags & PT_PRESENT) && (pdpte->flags & PT_PRESENT) &&
        (pde->flags & PT_PRESENT) && (pte->flags & PT_PRESENT)))
  {
    LOG("Error: page %016llp not allocated", addr);
    return; // false alarm ._.
  }

  // free physical page
  pmm_free((void *)((uint64_t)pte->base_addr << 12));

  // unset present bit
  pte->flags = 0;

  LOG("Freed virtual %016llp", addr);
}
