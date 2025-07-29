/**
 * @file: include/kernel/global.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KERNEL_GLOBAL_H_
#define _KERNEL_GLOBAL_H_

#include <kernel/screen.h>
#include <kernel/gate.h>
#include <kernel/memory.h>
#include <kernel/types.h>
#include <kernel/tss.h>

extern struct Screen screen;

// interrupt
extern struct Gate IDT[256];
extern u32 TSS[26];

// memory
extern struct MemoryDescriptor memory_desc;
extern char _text;
extern char _etext;
extern char _data;
extern char _edata;
extern char _rodata;
extern char _erodata;
extern char _bss;
extern char _ebss;
extern char _end;
extern u32 ZONE_DMA_INDEX;
extern u32 ZONE_NORMAL_INDEX;
extern u32 ZONE_UNMAPPED_INDEX;
extern u64* Global_CR3;

extern u64 stack_start;

// irq
extern irq_handler irq_table[];

// process
extern struct Process_struct* p_current_process;

#endif