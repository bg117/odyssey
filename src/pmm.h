/**
 * @file pmm.h
 * @author bg117 (bg117@sooosilly.xyz)
 * @brief Physical Memory Manager
 * @version 0.1
 * @date 2023-06-03
 *
 * This file contains code for managing physical memory. This includes
 * allocating and freeing physical memory in blocks of 4KB (page size).
 */

#ifndef ODYSSEY_PMM_H
#define ODYSSEY_PMM_H

#include "limine.h"

#include <stddef.h>

#define PMM_PAGE_SIZE 4096

/**
 * @brief Initializes the physical memory manager.
 *
 * This function initializes the physical memory manager. It should be called
 * before any other functions in this file are called.
 *
 * @param mmap The memory map to use for the physical memory manager.
 * @param mmap_count The number of entries in the memory map.
 */
void pmm_init(struct limine_memmap_entry **mmap, size_t mmap_count);

void *pmm_alloc(void);
void pmm_free(void *ptr);

#endif