#include "memory/vmm.hpp"

#include "kernel/info.hpp"
#include "memory/paging_structure.hpp"
#include "memory/pmm.hpp"
#include "misc/flag.hpp"
#include "misc/log.hpp"
#include "misc/round.hpp"

extern kernel::info INFO;

namespace
{
memory::paging_structure_entry *pml4;
bool initialized = false;

constexpr int RECURSIVE_PSE_INDEX = 510;

void map_page(uintptr_t phys, uintptr_t virt);
void map_page_initialized(uintptr_t phys, uintptr_t virt);
void map_page_uninitialized(uintptr_t phys, uintptr_t virt);

void allocate_paging_structure_entry(memory::paging_structure_entry *pse);

void reload_cr3();
void invalidate_page(uintptr_t addr);

uintptr_t find_first_free_region(uint64_t count = 1);

memory::paging_structure_entry *entry_from_indices(int pdpt_idx, int pd_idx,
                                                   int pt_idx, int pte_idx);
constexpr uintptr_t vaddr_from_indices(int pml4_idx, int pdpt_idx, int pd_idx,
                                       int pt_idx);

constexpr int pml4_offset(uintptr_t addr);
constexpr int pdpt_offset(uintptr_t addr);
constexpr int pd_offset(uintptr_t addr);
constexpr int pt_offset(uintptr_t addr);
} // namespace

namespace memory
{
namespace vmm
{
void initialize()
{
  LOG("allocating PML4");
  pml4 = static_cast<paging_structure_entry *>(pmm::allocate());

  LOG("mapping PML4[%d] to itself", RECURSIVE_PSE_INDEX);
  pml4[RECURSIVE_PSE_INDEX].flags     = 0x3;
  pml4[RECURSIVE_PSE_INDEX].base_addr = reinterpret_cast<uintptr_t>(pml4) >> 12;

  // get some info about stuff
  const uint64_t rounded_kernel_size = round::up(INFO.kernel.size, PAGE_SIZE);
  const uint64_t rounded_fb_size = round::up(INFO.framebuffer.size, PAGE_SIZE);
  const uint64_t rounded_stack_loc = round::up(INFO.stack.location, PAGE_SIZE);

  LOG("mapping kernel into higher half");
  for (uint64_t i = 0; i < rounded_kernel_size; i += PAGE_SIZE)
  {
    map_page(INFO.kernel.location + i, INFO.higher_half_kernel_offset + i);
  }

  LOG("mapping framebuffer into higher half");
  for (uint64_t i = 0; i < rounded_fb_size; i += PAGE_SIZE)
  {
    const auto fb_loc = INFO.framebuffer.location + i;
    map_page(fb_loc, INFO.higher_half_direct_offset + fb_loc);
  }

  LOG("mapping kernel stack into higher half");
  for (uint64_t i = 0; i < INFO.stack.size; i += PAGE_SIZE)
  {
    const auto stack_loc = rounded_stack_loc - i;
    map_page(stack_loc, INFO.higher_half_direct_offset + stack_loc);
  }

  LOG("mapping PMM bitmap into higher half");
  for (uint64_t i = 0; i < INFO.bitmap.size; i += PAGE_SIZE)
  {
    const auto bitmap_loc = INFO.bitmap.location + i;
    map_page(bitmap_loc, INFO.higher_half_direct_offset + bitmap_loc);
  }

  initialized = true;

  LOG("reloading CR3 with %p", pml4);
  reload_cr3();
}

void *allocate(const uint64_t count)
{
  if (count == 0)
  {
    LOG("error: cannot find 0 consecutive pages of virtual addresses");
    return nullptr;
  }

  // get a free region
  const uintptr_t region = find_first_free_region(count);

  // if no memory, return NULL
  if (region == 0)
  {
    LOG("critical: no more free virtual addresses");
    return nullptr;
  }

  // allocate phys mem for PTE
  for (uint64_t i = 0; i < count; i++)
  {
    const auto block      = pmm::allocate();
    const auto region_off = region + i * PAGE_SIZE;
    map_page(reinterpret_cast<uintptr_t>(block), region_off);

    const auto pte =
        entry_from_indices(pml4_offset(region_off), pdpt_offset(region_off),
                           pd_offset(region_off), pt_offset(region_off));

    // fill lower 3 available bits with 0 if not end of chain, else 0x7
    pte->available_0 = i < count - 1 ? 0 : 0x7;
  }

  return reinterpret_cast<void *>(region);
}

void deallocate(void *page)
{
  const auto addr = reinterpret_cast<uintptr_t>(page);
  const auto pml4e =
                 entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                    RECURSIVE_PSE_INDEX, pml4_offset(addr)),
             pdpte =
                 entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                    pml4_offset(addr), pdpt_offset(addr)),
             pde = entry_from_indices(RECURSIVE_PSE_INDEX, pml4_offset(addr),
                                      pdpt_offset(addr), pd_offset(addr));
  auto pte       = entry_from_indices(pml4_offset(addr), pdpt_offset(addr),
                                      pd_offset(addr), pt_offset(addr));
  // if any structure in the hierarchy is not actually present then abort
  if (!(flag::is_set(pml4e->flags, 1) && flag::is_set(pdpte->flags, 1) &&
        flag::is_set(pde->flags, 1) && flag::is_set(pte->flags, 1)))
  {
    LOG("error: page 0x%016lX not allocated", addr);
    return; // false alarm ._.
  }

  uint64_t i = 0;
  while (true)
  {
    const auto addr_off = addr + i * PAGE_SIZE;
    pte = entry_from_indices(pml4_offset(addr_off), pdpt_offset(addr_off),
                             pd_offset(addr_off), pt_offset(addr_off));
    // free physical page
    pmm::deallocate(reinterpret_cast<void *>(pte->base_addr << 12));

    // unset flags
    pte->flags     = 0;
    pte->base_addr = 0;

    // end of chain
    if (pte->available_0 == 0x7)
    {
      pte->available_0 = 0;
      return;
    }

    i++;
  }
}
} // namespace vmm
} // namespace memory

namespace
{
void map_page(const uintptr_t phys, const uintptr_t virt)
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

void map_page_initialized(const uintptr_t phys, const uintptr_t virt)
{
  // obtain page structure entries
  const auto pml4e =
                 entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                    RECURSIVE_PSE_INDEX, pml4_offset(virt)),
             pdpte =
                 entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                                    pml4_offset(virt), pdpt_offset(virt)),
             pde = entry_from_indices(RECURSIVE_PSE_INDEX, pml4_offset(virt),
                                      pdpt_offset(virt), pd_offset(virt)),
             pte = entry_from_indices(pml4_offset(virt), pdpt_offset(virt),
                                      pd_offset(virt), pt_offset(virt));

  // allocate the missing structures (if any)
  allocate_paging_structure_entry(pml4e);
  allocate_paging_structure_entry(pdpte);
  allocate_paging_structure_entry(pde);

  // set page address
  pte->flags     = 0x3;
  pte->base_addr = phys >> 12;

  invalidate_page(virt);
}

void map_page_uninitialized(const uintptr_t phys, const uintptr_t virt)
{
  // get PML4 entry
  const auto pml4e = pml4 + pml4_offset(virt);
  // if PML4 entry is not present, point to blank PDPT
  allocate_paging_structure_entry(pml4e);

  // rinse and repeat
  const auto pdpte = reinterpret_cast<memory::paging_structure_entry *>(
                         pml4e->base_addr << 12) +
                     pdpt_offset(virt);
  allocate_paging_structure_entry(pdpte);

  const auto pde = reinterpret_cast<memory::paging_structure_entry *>(
                       pdpte->base_addr << 12) +
                   pd_offset(virt);
  allocate_paging_structure_entry(pde);

  const auto pte =
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
  if (!flag::is_set(pse->flags, 1))
  {
    pse->flags = 0x03; // 0b01 | 0b10 == 0b11 (0x3) (PRESENT | WRITABLE)
    // if struct.base_addr is not set, allocate new
    if (pse->base_addr == 0)
    {
      pse->base_addr =
          reinterpret_cast<uintptr_t>(memory::pmm::allocate()) >> 12;
    }

    if (initialized)
    {
      invalidate_page(reinterpret_cast<uintptr_t>(pse));
    }
  }
}

memory::paging_structure_entry *entry_from_indices(const int pdpt_idx,
                                                   const int pd_idx,
                                                   const int pt_idx,
                                                   const int pte_idx)
{
  const auto table_addr = vaddr_from_indices(510, pdpt_idx, pd_idx, pt_idx);
  return reinterpret_cast<memory::paging_structure_entry *>(table_addr) +
         pte_idx;
}

void reload_cr3()
{
  asm volatile("mov cr3, %0" ::"r"(
      pml4)); // load CR3 with memory address of PML4 (load into register first)
}

uintptr_t find_first_free_region(const uint64_t count)
{
  uint64_t consec_count                           = 0;
  uintptr_t consec_base                           = 0;
  memory::paging_structure_entry *consec_base_pse = nullptr;

  for (int pml4_idx = 256; pml4_idx < 512; pml4_idx++)
  { // get PML4 entry associated
    const auto pml4e =
        entry_from_indices(RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX,
                           RECURSIVE_PSE_INDEX, pml4_idx);
    allocate_paging_structure_entry(pml4e);

    for (int pdpt_idx = 0; pdpt_idx < 512; pdpt_idx++)
    {
      const auto pdpte = entry_from_indices(
          RECURSIVE_PSE_INDEX, RECURSIVE_PSE_INDEX, pml4_idx, pdpt_idx);
      allocate_paging_structure_entry(pdpte);

      for (int pd_idx = 0; pd_idx < 512; pd_idx++)
      {
        const auto pde =
            entry_from_indices(RECURSIVE_PSE_INDEX, pml4_idx, pdpt_idx, pd_idx);
        allocate_paging_structure_entry(pde);

        for (int pt_idx = 0; pt_idx < 512; pt_idx++)
        {
          const auto pte =
              entry_from_indices(pml4_idx, pdpt_idx, pd_idx, pt_idx);
          if (!flag::is_set(pte->flags, 1))
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
            consec_count = 0; // resent uint64_t
          }
        }
      }
    }
  }

  return 0; // no free :(
}

void invalidate_page(uintptr_t addr)
{
  asm volatile("invlpg [%0]" ::"r"(addr) : "memory");
}

constexpr uintptr_t vaddr_from_indices(const int pml4_idx, const int pdpt_idx,
                                       const int pd_idx, const int pt_idx)
{
  const int64_t p1 = pml4_idx, p2 = pdpt_idx, p3 = pd_idx, p4 = pt_idx;
  return 0xffff000000000000 | p1 << 39 | p2 << 30 | p3 << 21 | p4 << 12;
}

constexpr int pml4_offset(const uintptr_t addr)
{
  return addr >> 39 & 0x1ff; // mask up to 511 only
}

constexpr int pdpt_offset(const uintptr_t addr)
{
  return addr >> 30 & 0x1ff;
}

constexpr int pd_offset(const uintptr_t addr)
{
  return addr >> 21 & 0x1ff;
}

constexpr int pt_offset(const uintptr_t addr)
{
  return addr >> 12 & 0x1ff;
}
} // namespace
