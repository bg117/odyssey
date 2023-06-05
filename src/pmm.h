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
 */
void pmm_init(void);

void *pmm_alloc(void);
void pmm_free(void *ptr);

#endif