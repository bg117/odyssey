#include "memory/heap.hpp"

#include "kernel/info.hpp"
#include "memory/vmm.hpp"
#include "misc/log.hpp"
#include "misc/round.hpp"

#include <cstddef>
#include <cstring>

struct __attribute__((aligned(alignof(max_align_t)))) heap_entity
{
  heap_entity *next, *prev;
  uint64_t size;
  bool used;
};

struct heap_entity_list
{
  heap_entity *first;
  uint64_t page_count;
};

namespace
{
heap_entity_list *entity_lists;
uint64_t entity_list_count;
uint64_t entity_lists_page_count;

constexpr uint64_t ALIGN       = alignof(max_align_t);
constexpr uint64_t ENTITY_SIZE = sizeof(heap_entity);

bool add_new_entity_list(uint64_t page_count);
void split_entity(heap_entity *entity, uint64_t size);
void merge_free_entities(heap_entity *start);
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
  if (bytes == 0)
  {
    return nullptr;
  }

  const uint64_t rounded_bytes = round::up(bytes, ALIGN);
  const uint64_t bytes_page = round::up(rounded_bytes + ENTITY_SIZE, PAGE_SIZE) / PAGE_SIZE;

  bool done  = false;
  bool found = false;
  heap_entity *entity;

  while (!done)
  {
    for (uint64_t i = 0; i < entity_list_count || found; i++)
    {
      auto start   = entity_lists[i].first;
      auto current = start;

      while (current != nullptr)
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
        current = current->next;
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
  if (entity->size > rounded_bytes + ENTITY_SIZE && entity->size % ALIGN == 0)
  {
    split_entity(entity, rounded_bytes);
  }

  // return space after (pointer arithmetic)
  return reinterpret_cast<void *>(entity + 1);
}

void deallocate(void *block)
{
  auto entity  = reinterpret_cast<heap_entity *>(block) - 1;
  entity->used = 0;
  merge_free_entities(entity);
}
} // namespace heap
} // namespace memory

namespace
{
bool add_new_entity_list(uint64_t page_count)
{
  entity_list_count++;
  const uint64_t new_entity_list_page_count =
      round::up(entity_list_count, PAGE_SIZE) / PAGE_SIZE;

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

  auto first_entity =
      reinterpret_cast<heap_entity *>(memory::vmm::allocate(page_count));
  if (first_entity == nullptr)
  {
    return false;
  }

  first_entity->used = 0;
  first_entity->size = page_count * PAGE_SIZE - ENTITY_SIZE;
  first_entity->next = first_entity->prev = nullptr;

  entity_lists[entity_list_count - 1].first      = first_entity;
  entity_lists[entity_list_count - 1].page_count = page_count;

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

  // make b2->prev point to b1, b2->next point to what b1->next was pointing
  b2->prev = b1;
  b2->next = b1->next;
  b1->next = b2;
}

void merge_free_entities(heap_entity *start)
{
  if (start == nullptr)
  {
    return;
  }

  auto p = start;
  // merge free blocks after
  while (p->next != nullptr && !p->next->used)
  {
    auto next = p->next;
    p->next   = next->next;
    p->size += next->size + ENTITY_SIZE;

    // we don't need to assign p = next since it merges the block after it
  }

  // merge free blocks before
  while (p->prev != nullptr && !p->prev->used)
  {
    auto prev  = p->prev;
    prev->next = p->next;
    prev->size += p->size + ENTITY_SIZE;
    p = prev;
  }
}
} // namespace
