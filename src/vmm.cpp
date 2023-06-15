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

constexpr offset RECURSIVE_PSE_INDEX = 510;

void map_page(physical_address phys, virtual_address virt);
void map_page_initialized(physical_address phys, virtual_address virt);
void map_page_uninitialized(physical_address phys, virtual_address virt);

void allocate_paging_structure_entry(memory::paging_structure_entry *pse);

void reload_cr3();

memory::paging_structure_entry *find_first_free_pte(offset &pml4_idx,
                                                    offset &pdpt_idx,
                                                    offset &pd_idx,
                                                    offset &pt_idx);

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

void *allocate()
{
  // get a free PTE
  offset pml4i, pdpti, pdi, pti;
  paging_structure_entry *pte = find_first_free_pte(pml4i, pdpti, pdi, pti);

  // if NULL, return NULL
  if (pte == nullptr)
  {
    return nullptr;
  }

  // allocate phys mem for PTE
  void *block = pmm::allocate();

  // set members
  pte->flags     = 0x3;
  pte->base_addr = reinterpret_cast<physical_address>(block) >> 12;

  virtual_address addr = vaddr_from_indices(pml4i, pdpti, pdi, pti);
  return reinterpret_cast<void *>(addr);
}

void deallocate(void *page)
{
  uint64_t addr = (uint64_t)page;

  paging_structure_entry
      *pml4e = entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                  RECURSIVE_PSE_INDEX, pml4_offset(addr)),
      *pdpte = entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                  pml4_offset(addr), pdpt_offset(addr)),
      *pde   = entry_from_indices(RECURSIVE_PSE_INDEX, pml4_offset(addr),
                                  pdpt_offset(addr), pd_offset(addr)),
      *pte   = entry_from_indices(pml4_offset(addr), pdpt_offset(addr),
                                  pd_offset(addr), pt_offset(addr));
  // if any structure in the hierarchy is not actually present then abort
  if (!((pml4e->flags & 0x1) && (pdpte->flags & 0x1) && (pde->flags & 0x1) &&
        (pte->flags & 0x1)))
  {
    LOG("error: page 0x%016lX not allocated", addr);
    return; // false alarm ._.
  }

  // free physical page
  pmm::deallocate(reinterpret_cast<void *>(pte->base_addr << 12));

  // unset present bit
  pte->flags = 0;
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

memory::paging_structure_entry *find_first_free_pte(offset &pml4_idx,
                                                    offset &pdpt_idx,
                                                    offset &pd_idx,
                                                    offset &pt_idx)
{
  memory::paging_structure_entry *pml4e, *pdpte, *pde, *pte;

  for (pml4_idx = 256; pml4_idx < 512; pml4_idx++)
  {
    // get PML4 entry associated
    pml4e = entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                               RECURSIVE_PSE_INDEX, pml4_idx);
    allocate_paging_structure_entry(pml4e);

    for (pdpt_idx = 0; pdpt_idx < 512; pdpt_idx++)
    {
      pdpte = entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                 pml4_idx, pdpt_idx);
      allocate_paging_structure_entry(pdpte);

      for (pd_idx = 0; pd_idx < 512; pd_idx++)
      {
        pde =
            entry_from_indices(RECURSIVE_PSE_INDEX, pml4_idx, pdpt_idx, pd_idx);
        allocate_paging_structure_entry(pde);

        for (pt_idx = 0; pt_idx < 512; pt_idx++)
        {
          pte = entry_from_indices(pml4_idx, pdpt_idx, pd_idx, pt_idx);
          if (!(pte->flags & 0x1))
          {
            return pte;
          }
        }
      }
    }
  }

  LOG("critical: no more free virtual address (how does this even happen?)");
  return nullptr; // no free :(
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