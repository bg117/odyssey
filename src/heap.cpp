#include "memory/heap.hpp"

#include "kernel/info.hpp"
#include "memory/vmm.hpp"
#include "misc/log.hpp"

#include <cstddef>
#include <cstring>

struct heap_entity_list
{
  void *entity_list;
  uint64_t page_count;
};

struct __attribute__((aligned(alignof(max_align_t)))) heap_entity
{
  uint64_t size;
  bool used;
};

namespace
{
heap_entity_list *entity_lists;
uint64_t entity_list_count;
uint64_t entity_lists_page_count;

constexpr uint64_t ENTITY_SIZE = sizeof(heap_entity);

bool add_new_entity_list(uint64_t page_count);
void split_entity(heap_entity *entity, uint64_t size);
} // namespace

namespace memory
{
namespace heap
{
void initialize()
{
  // can confidently store 256 of these
  entity_lists      = reinterpret_cast<heap_entity_list *>(vmm::allocate());
  entity_list_count = 0;
  entity_lists_page_count = 1;
}

void *allocate(const uint64_t bytes)
{
  const uint64_t rounded_bytes = (bytes + ENTITY_SIZE - 1) & -ENTITY_SIZE;
  const uint64_t bytes_page =
      ((rounded_bytes + PAGE_SIZE - 1) & -PAGE_SIZE) / PAGE_SIZE;

  bool done  = false;
  bool found = false;
  heap_entity *entity;

  while (!done)
  {
    for (uint64_t i = 0; i < entity_list_count; i++)
    {
      auto start = reinterpret_cast<heap_entity *>(entity_lists[i].entity_list);
      auto current = start;

      while (true)
      {
        // if current entity is not used and can fit rounded_bytes (or more)
        // bytes of data
        if (current->size >= rounded_bytes && !current->used)
        {
          found  = true;
          entity = current;
          break;
        }

        // move to next entity
        auto next = reinterpret_cast<virtual_address>(current) + current->size +
                    ENTITY_SIZE;
        current = reinterpret_cast<heap_entity *>(next);

        // if at end of entity list, move to next entity list
        offset loc = reinterpret_cast<virtual_address>(current) -
                     reinterpret_cast<virtual_address>(start);
        if (loc >= entity_lists[i].page_count * PAGE_SIZE)
        {
          break;
        }
      }

      if (found)
      {
        break;
      }
    }

    // try to add new page if entries are full
    if (!found)
    {
      bool result = add_new_entity_list(bytes_page);
      if (!result)
      {
        done = true;
      }
    }
    else
    {
      done = true;
    }
  }

  if (done && !found)
  {
    // no available memory
    LOG("critical: no available memory");
    return nullptr;
  }

  entity->used = true;
  if (entity->size > rounded_bytes + ENTITY_SIZE)
  {
    split_entity(entity, rounded_bytes);
  }

  // return space after (pointer arithmetic)
  return reinterpret_cast<void *>(entity + 1);
}

void deallocate(void *block)
{
  LOG("deallocate %p", block);
}
} // namespace heap
} // namespace memory

namespace
{
bool add_new_entity_list(uint64_t page_count)
{
  entity_list_count++;
  const uint64_t new_entity_list_page_count =
      ((entity_list_count + PAGE_SIZE - 1) & -PAGE_SIZE) / PAGE_SIZE;

  if (new_entity_list_page_count > entity_lists_page_count)
  {
    void *reloc = memory::vmm::allocate(new_entity_list_page_count);
    if (reloc == nullptr)
    {
      return false;
    }

    entity_lists_page_count = new_entity_list_page_count;

    // resize heap entity list
    memcpy(reloc, entity_lists, entity_list_count * sizeof(heap_entity_list));
    entity_lists = reinterpret_cast<heap_entity_list *>(reloc);
  }

  void *entity_list = memory::vmm::allocate(page_count);
  if (entity_list == nullptr)
  {
    return false;
  }

  entity_lists[entity_list_count - 1].entity_list = entity_list;
  entity_lists[entity_list_count - 1].page_count  = page_count;

  auto first_entity  = reinterpret_cast<heap_entity *>(entity_list);
  first_entity->used = 0;
  first_entity->size = page_count * PAGE_SIZE - ENTITY_SIZE;

  return true;
}

void split_entity(heap_entity *entity, uint64_t size)
{
  auto b2_addr = reinterpret_cast<virtual_address>(entity + 1) + size;

  auto b1 = entity;
  auto b2 = reinterpret_cast<heap_entity *>(b2_addr);

  b2->size = b1->size - size - ENTITY_SIZE;
  b2->used = false;
  b1->size = size;
}
} // namespace
