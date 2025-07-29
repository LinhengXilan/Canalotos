/**
* @file: include/memory/pages.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KERNEL_MEMORY_PAGES_H_
#define _KERNEL_MEMORY_PAGES_H_

#include <kernel/types.h>

typedef struct { u64 pt; } pt_t;
typedef struct { u64 pdt; } pdt_t;
typedef struct { u64 pet; } pet_t;
typedef struct { u64 pgt; } pgt_t;

#endif