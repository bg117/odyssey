#include "memory/vmm.hpp"

#include "kernel/info.hpp"
#include "memory/paging_structure.hpp"
#include "memory/pmm.hpp"
#include "misc/log.hpp"
#include "misc/types.hpp"

extern kernel::info INFO;

namespace
{
memory::paging_structure_entry *pml4;
bool initialized = false;

constexpr offset RECURSIVE_PSE_INDEX      = 510;
constexpr uint32_t CONSECUTIVE_ADDR_LIMIT = 0x3ffff;

void map_page(physical_address phys, virtual_address virt);
void map_page_initialized(physical_address phys, virtual_address virt);
void map_page_uninitialized(physical_address phys, virtual_address virt);

void allocate_paging_structure_entry(memory::paging_structure_entry *pse);

void reload_cr3();

virtual_address find_first_free_region(uint64_t count = 1);

memory::paging_structure_entry *entry_from_indices(offset pdpt, offset pd,
                                                   offset pt, offset pte);
constexpr virtual_address vaddr_from_indices(offset pml4, offset pdpt,
                                             offset pd, offset pt);

constexpr offset pml4_offset(virtual_address addr);
constexpr offset pdpt_offset(virtual_address addr);
constexpr offset pd_offset(virtual_address addr);
constexpr offset pt_offset(virtual_address addr);
} // namespace

namespace memory
{
namespace vmm
{
void initialize()
{
  LOG("allocating PML4");
  pml4 = reinterpret_cast<paging_structure_entry *>(pmm::allocate());

  LOG("mapping PML4[%lu] to itself", RECURSIVE_PSE_INDEX);
  pml4[RECURSIVE_PSE_INDEX].flags = 0x3;
  pml4[RECURSIVE_PSE_INDEX].base_addr =
      reinterpret_cast<physical_address>(pml4) >> 12;

  // get some info about stuff
  const uint64_t rounded_kernel_size =
      (INFO.kernel.size + PAGE_SIZE - 1) &
      -PAGE_SIZE; // -PAGE_SIZE is equivalent to ~(PAGE_SIZE - 1)
  const uint64_t rounded_fb_size =
      (INFO.framebuffer.size + PAGE_SIZE - 1) & -PAGE_SIZE;
  const uint64_t rounded_stack_loc =
      (INFO.stack.location + PAGE_SIZE - 1) & -PAGE_SIZE;

  LOG("mapping kernel into higher half");
  for (counter i = 0; i < rounded_kernel_size; i += PAGE_SIZE)
  {
    map_page(INFO.kernel.location + i, INFO.higher_half_kernel_offset + i);
  }

  LOG("mapping framebuffer into higher half");
  for (counter i = 0; i < rounded_fb_size; i += PAGE_SIZE)
  {
    auto fb_loc = INFO.framebuffer.location + i;
    map_page(fb_loc, INFO.higher_half_direct_offset + fb_loc);
  }

  LOG("mapping kernel stack into higher half");
  for (counter i = 0; i < INFO.stack.size; i += PAGE_SIZE)
  {
    auto stack_loc = rounded_stack_loc - i;
    map_page(stack_loc, INFO.higher_half_direct_offset + stack_loc);
  }

  LOG("mapping PMM bitmap into higher half");
  for (counter i = 0; i < INFO.bitmap.size; i += PAGE_SIZE)
  {
    auto bitmap_loc = INFO.bitmap.location + i;
    map_page(bitmap_loc, INFO.higher_half_direct_offset + bitmap_loc);
  }

  initialized = true;

  LOG("reloading CR3 with %p", pml4);
  reload_cr3();
}

void *allocate(uint32_t count)
{
  if (count == 0)
  {
    LOG("error: cannot find 0 consecutive pages of virtual addresses");
    return 0;
  }

  // max 2^18 consec. pages (available bits)
  if (count > CONSECUTIVE_ADDR_LIMIT)
  {
    LOG("warning: will not allocate more than %u consecutive pages of virtual "
        "addresses",
        CONSECUTIVE_ADDR_LIMIT);
    count = CONSECUTIVE_ADDR_LIMIT;
  }

  // get a free region
  virtual_address region = find_first_free_region(count);

  // if no memory, return NULL
  if (region == 0)
  {
    LOG("critical: no more free virtual addresses");
    return nullptr;
  }

  // allocate phys mem for PTE
  for (counter i = 0; i < count; i++)
  {
    void *block           = pmm::allocate();
    const auto region_off = region + i * PAGE_SIZE;
    map_page(reinterpret_cast<virtual_address>(block), region_off);

    // if base of region, set count of consecutive virtual addresses
    if (i == 0)
    {
      auto pte =
          entry_from_indices(pml4_offset(region_off), pdpt_offset(region_off),
                             pd_offset(region_off), pt_offset(region_off));
      pte->available_1 = count & 0x7FFF;      // lower 15 bits
      pte->available_0 = (count >> 15) & 0x7; // upper 3 bits
    }
  }

  return reinterpret_cast<void *>(region);
}

void deallocate(void *page)
{
  auto addr  = reinterpret_cast<virtual_address>(page);
  auto pml4e = entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                  RECURSIVE_PSE_INDEX, pml4_offset(addr)),
       pdpte = entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                  pml4_offset(addr), pdpt_offset(addr)),
       pde   = entry_from_indices(RECURSIVE_PSE_INDEX, pml4_offset(addr),
                                  pdpt_offset(addr), pd_offset(addr)),
       pte   = entry_from_indices(pml4_offset(addr), pdpt_offset(addr),
                                  pd_offset(addr), pt_offset(addr));
  // if any structure in the hierarchy is not actually present then abort
  if (!((pml4e->flags & 0x1) && (pdpte->flags & 0x1) && (pde->flags & 0x1) &&
        (pte->flags & 0x1)))
  {
    LOG("error: page 0x%016lX not allocated", addr);
    return; // false alarm ._.
  }

  uint32_t consec_count = pte->available_0 << 15 | pte->available_1;
  if (consec_count == 0)
  {
    LOG("warning: PTE.available_0 | PTE.available_1 of 0x%016lu is 0; freeing "
        "1 page only",
        addr);
    consec_count = 1;
  }

  for (counter i = 0; i < consec_count; i++)
  {
    const auto addr_off = addr + i * PAGE_SIZE;
    pte = entry_from_indices(pml4_offset(addr_off), pdpt_offset(addr_off),
                             pd_offset(addr_off), pt_offset(addr_off));
    // free physical page
    pmm::deallocate(reinterpret_cast<void *>(pte->base_addr << 12));
    // unset members
    pte->available_0 = pte->available_1 = 0;
    pte->flags                          = 0;
    pte->base_addr                      = 0;
  }
}
} // namespace vmm
} // namespace memory

namespace
{
void map_page(physical_address phys, virtual_address virt)
{
  if (initialized)
  {
    map_page_initialized(phys, virt);
  }
  else
  {
    map_page_uninitialized(phys, virt);
  }

  // LOG("mapped 0x%016lX -> 0x%016lX", phys, virt);
}

void map_page_initialized(physical_address phys, virtual_address virt)
{
  // obtain page structure entries
  auto pml4e = entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                  RECURSIVE_PSE_INDEX, pml4_offset(virt)),
       pdpte = entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                  pml4_offset(virt), pdpt_offset(virt)),
       pde   = entry_from_indices(RECURSIVE_PSE_INDEX, pml4_offset(virt),
                                  pdpt_offset(virt), pd_offset(virt)),
       pte   = entry_from_indices(pml4_offset(virt), pdpt_offset(virt),
                                  pd_offset(virt), pt_offset(virt));

  // allocate the missing structures (if any)
  allocate_paging_structure_entry(pml4e);
  allocate_paging_structure_entry(pdpte);
  allocate_paging_structure_entry(pde);

  // set page address
  pte->flags     = 0x3;
  pte->base_addr = phys >> 12;
}

void map_page_uninitialized(physical_address phys, virtual_address virt)
{
  // get PML4 entry
  auto pml4e = pml4 + pml4_offset(virt);
  // if PML4 entry is not present, point to blank PDPT
  allocate_paging_structure_entry(pml4e);

  // rinse and repeat
  auto pdpte = reinterpret_cast<memory::paging_structure_entry *>(
                   pml4e->base_addr << 12) +
               pdpt_offset(virt);
  allocate_paging_structure_entry(pdpte);

  auto pde = reinterpret_cast<memory::paging_structure_entry *>(pdpte->base_addr
                                                                << 12) +
             pd_offset(virt);
  allocate_paging_structure_entry(pde);

  auto pte =
      reinterpret_cast<memory::paging_structure_entry *>(pde->base_addr << 12) +
      pt_offset(virt);

  // manually set flags
  pte->flags = 0x3;
  // deallocate previous page
  if (pte->base_addr != 0)
  {
    memory::pmm::deallocate(reinterpret_cast<void *>(pte->base_addr << 12));
  }
  // set physical address
  pte->base_addr = phys >> 12;
}

void allocate_paging_structure_entry(memory::paging_structure_entry *pse)
{
  // 0x1 is PRESENT bit
  if (!(pse->flags & 0x01))
  {
    pse->flags = 0x03; // 0b01 | 0b10 == 0b11 (0x3) (PRESENT | WRITABLE)
    // if struct.base_addr is not set, allocate new
    if (pse->base_addr == 0)
    {
      pse->base_addr =
          reinterpret_cast<physical_address>(memory::pmm::allocate()) >> 12;
      if (initialized)
      {
        reload_cr3();
      }
    }
  }
}

memory::paging_structure_entry *entry_from_indices(offset pdpt, offset pd,
                                                   offset pt, offset pte)
{
  const virtual_address table_addr =
      0xffff000000000000 | 510ull << 39 | pdpt << 30 | pd << 21 | pt << 12;
  return reinterpret_cast<memory::paging_structure_entry *>(table_addr) + pte;
}

void reload_cr3()
{
  asm volatile("mov %0, %%cr3" ::"r"(
      pml4)); // load CR3 with memory address of PML4 (load into register first)
  LOG("reloaded CR3");
}

virtual_address find_first_free_region(uint64_t count)
{
  counter consec_count                            = 0;
  virtual_address consec_base                     = 0;
  memory::paging_structure_entry *consec_base_pse = 0;

  for (offset pml4_idx = 256; pml4_idx < 512; pml4_idx++)
  { // get PML4 entry associated
    auto pml4e = entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                    RECURSIVE_PSE_INDEX, pml4_idx);
    allocate_paging_structure_entry(pml4e);

    for (offset pdpt_idx = 0; pdpt_idx < 512; pdpt_idx++)
    {
      auto pdpte = entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                      pml4_idx, pdpt_idx);
      allocate_paging_structure_entry(pdpte);

      for (offset pd_idx = 0; pd_idx < 512; pd_idx++)
      {
        auto pde =
            entry_from_indices(RECURSIVE_PSE_INDEX, pml4_idx, pdpt_idx, pd_idx);
        allocate_paging_structure_entry(pde);

        for (offset pt_idx = 0; pt_idx < 512; pt_idx++)
        {
          auto pte = entry_from_indices(pml4_idx, pdpt_idx, pd_idx, pt_idx);
          if (!(pte->flags & 0x1))
          {
            // if 0 consecutive virtual addresses and encounter a free entry,
            // set base
            if (consec_count == 0)
            {
              consec_base =
                  vaddr_from_indices(pml4_idx, pdpt_idx, pd_idx, pt_idx);
              consec_base_pse = pte;
            }

            consec_count++;

            // enough consecutive virtual addresses
            if (consec_count == count)
            {
              // set number of consecutive pages
              consec_base_pse->available_1 = count & 0x7FFF;      // low 15 bits
              consec_base_pse->available_0 = (count >> 15) & 0x7; // high 3 bits
              return consec_base;
            }
          }
          else
          {
            consec_count = 0; // resent counter
          }
        }
      }
    }
  }

  return 0; // no free :(
}

constexpr virtual_address vaddr_from_indices(offset pml4, offset pdpt,
                                             offset pd, offset pt)
{
  return 0xffff000000000000 | pml4 << 39 | pdpt << 30 | pd << 21 | pt << 12;
}

constexpr offset pml4_offset(virtual_address addr)
{
  return addr >> 39 & 0x1ff; // mask up to 511 only
}

constexpr offset pdpt_offset(virtual_address addr)
{
  return addr >> 30 & 0x1ff;
}

constexpr offset pd_offset(virtual_address addr)
{
  return addr >> 21 & 0x1ff;
}

constexpr offset pt_offset(virtual_address addr)
{
  return addr >> 12 & 0x1ff;
}
} // namespace
