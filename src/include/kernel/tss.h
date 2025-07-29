/**
 * @file: include/kernel/tss.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KERNEL_TSS_H_
#define _KERNEL_TSS_H_

#include <kernel/types.h>
#include <kernel/cpu.h>
#include <kernel/process/process.h>

extern u32 TSS[26];

struct tss_struct
{
	u32 reserved0;
	u64 rsp0;
	u64 rsp1;
	u64 rsp2;
	u64 reserved1;
	u64 ist1;
	u64 ist2;
	u64 ist3;
	u64 ist4;
	u64 ist5;
	u64 ist6;
	u64 ist7;
	u64 reserved2;
	u16 reserved3;
	u16 iomap;
}__attribute__((packed));

#define INIT_TSS	\
{	\
    .reserved0 = 0,	\
    .rsp0 = (u64)(_init_process.stack + PROCESS_STACK_SIZE / sizeof(u64)),	\
    .rsp1 = (u64)(_init_process.stack + PROCESS_STACK_SIZE / sizeof(u64)),	\
    .rsp2 = (u64)(_init_process.stack + PROCESS_STACK_SIZE / sizeof(u64)),	\
    .reserved1 = 0,	\
    .ist1 = 0xFFFF800000007C00,	\
    .ist2 = 0xFFFF800000007C00,	\
    .ist3 = 0xFFFF800000007C00,	\
    .ist4 = 0xFFFF800000007C00,	\
    .ist5 = 0xFFFF800000007C00,	\
    .ist6 = 0xFFFF800000007C00,	\
    .ist7 = 0xFFFF800000007C00,	\
    .reserved2 = 0,	\
    .reserved3 = 0,	\
    .iomap = 0	\
}

struct tss_struct _init_tss[NR_CPUS] = { [0 ... NR_CPUS - 1] = INIT_TSS };

inline void set_tss(u64 rsp0, u64 rsp1, u64 rsp2, u64 ist1, u64 ist2, u64 ist3, u64 ist4, u64 ist5, u64 ist6, u64 ist7)
{
	*(u64*)(TSS + 1) = rsp0;
	*(u64*)(TSS + 3) = rsp1;
	*(u64*)(TSS + 5) = rsp2;
	*(u64*)(TSS + 9) = ist1;
	*(u64*)(TSS + 11) = ist2;
	*(u64*)(TSS + 13) = ist3;
	*(u64*)(TSS + 15) = ist4;
	*(u64*)(TSS + 17) = ist5;
	*(u64*)(TSS + 19) = ist6;
	*(u64*)(TSS + 21) = ist7;
}

inline void set_tss_struct(struct tss_struct* tss)
{
	*(u64*)(TSS + 1) = tss->rsp0;
	*(u64*)(TSS + 3) = tss->rsp1;
	*(u64*)(TSS + 5) = tss->rsp2;
	*(u64*)(TSS + 9) = tss->ist1;
	*(u64*)(TSS + 11) = tss->ist2;
	*(u64*)(TSS + 13) = tss->ist3;
	*(u64*)(TSS + 15) = tss->ist4;
	*(u64*)(TSS + 17) = tss->ist5;
	*(u64*)(TSS + 19) = tss->ist6;
	*(u64*)(TSS + 21) = tss->ist7;
}

#endif