#include "vmm.h"

#include "pmm.h"
#include "terminal.h"

#define PAGE_SIZE 0x1000

#define PML4_OFFSET(x) (((x) >> 39) & 0x1ff)
#define PDPT_OFFSET(x) (((x) >> 30) & 0x1ff)
#define PD_OFFSET(x) (((x) >> 21) & 0x1ff)
#define PT_OFFSET(x) (((x) >> 12) & 0x1ff)

#define KERNEL_START 0xffffffff80000000

static struct page_table_entry *pml4;

extern uint64_t HIGHER_HALF_START;
extern struct limine_memmap_entry **MEMMAP;
extern uint64_t MEMMAP_COUNT;
extern uint64_t KERNEL_PHYS;

void map_page(uint64_t phys, uint64_t virt);

void vmm_init()
{
  terminal_printf("KERNEL_PHYS: %016llp\n", KERNEL_PHYS);
  pml4 = pmm_alloc() + HIGHER_HALF_START;

  // map usable memory into HIGHER_HALF_START + offset
  for (uint64_t i = 0; i < MEMMAP_COUNT; i++)
  {
    // if (MEMMAP[i]->type == LIMINE_MEMMAP_USABLE)
    // {
    //   // guaranteed to be page aligned
    //   uint64_t base   = MEMMAP[i]->base;
    //   uint64_t length = MEMMAP[i]->length;

    //   // map the page
    //   for (uint64_t j = 0; j < length; j += PAGE_SIZE)
    //   {
    //     map_page(base + j, HIGHER_HALF_START + base + j);
    //   }
    // }

    // if kernel/modules, map them into the higher half
    if (MEMMAP[i]->type == LIMINE_MEMMAP_KERNEL_AND_MODULES)
    {
      // guaranteed to be page aligned
      uint64_t base   = MEMMAP[i]->base;
      uint64_t length = MEMMAP[i]->length;

      // map the page
      for (uint64_t j = 0; j < length; j += PAGE_SIZE)
      {
        map_page(base + j, HIGHER_HALF_START + base + j);
      }
    }
  }
}

void map_page(uint64_t phys, uint64_t virt)
{
  uint64_t pml4_offset = PML4_OFFSET(virt);
  uint64_t pdpt_offset = PDPT_OFFSET(virt);
  uint64_t pd_offset   = PD_OFFSET(virt);
  uint64_t pt_offset   = PT_OFFSET(virt);

  // if the pml4 entry is not present, allocate a new pdpt
  if (!(pml4[pml4_offset].flags & PT_PRESENT))
  {
    // allocate a new pdpt
    uint64_t pdpt_phys = (uint64_t)pmm_alloc();
    // set the pml4 entry
    pml4[pml4_offset].flags     = PT_PRESENT | PT_WRITABLE;
    pml4[pml4_offset].base_addr = pdpt_phys;
  }

  // if pdpt entry is not present, allocate a new pd
  struct page_table_entry *pdpt_entry =
      &((struct page_table_entry *)HIGHER_HALF_START +
        pml4[pml4_offset].base_addr)[pdpt_offset];
  if (!(pdpt_entry->flags & PT_PRESENT))
  {
    // allocate a new pd
    uint64_t pd_phys = (uint64_t)pmm_alloc();
    // set the pdpt entry
    pdpt_entry->flags     = PT_PRESENT | PT_WRITABLE;
    pdpt_entry->base_addr = pd_phys;
  }

  // if pd entry is not present, allocate a new pt
  struct page_table_entry *pd_entry =
      &((struct page_table_entry *)HIGHER_HALF_START +
        pdpt_entry->base_addr)[pd_offset];
  if (!(pd_entry->flags & PT_PRESENT))
  {
    // allocate a new pt
    uint64_t pt_phys = (uint64_t)pmm_alloc();
    // set the pd entry
    pd_entry->flags     = PT_PRESENT | PT_WRITABLE;
    pd_entry->base_addr = pt_phys;
  }

  // set the pt entry
  struct page_table_entry *pt_entry =
      &((struct page_table_entry *)HIGHER_HALF_START +
        pd_entry->base_addr)[pt_offset];
  pt_entry->flags     = PT_PRESENT | PT_WRITABLE;
  pt_entry->base_addr = phys;

  terminal_printf("PML4: %llu, PDPT: %llu, PD: %llu, PT: %llu\n", pml4_offset,
                  pdpt_offset, pd_offset, pt_offset);
}