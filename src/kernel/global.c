/**
 * @file: kernel/global.c
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#include <kernel/const.h>
#include <kernel/global.h>

struct Screen screen;

// memory
struct MemoryDescriptor memory_desc;
u64 zone_unmapped_index;
char _text;
char _etext;
char _data;
char _edata;
char _rodata;
char _erodata;
char _bss;
char _ebss;
char _end;
u32 ZONE_DMA_INDEX = 0;
u32 ZONE_NORMAL_INDEX = 0;
u32 ZONE_UNMAPPED_INDEX = 0;
u64* Global_CR3 = nullptr;

// irq
irq_handler irq_table[NR_IRQ];

// process
