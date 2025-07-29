/**
 * @file: include/kernel/gate.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KERNEL_GATE_H_
#define _KERNEL_GATE_H_

#include <kernel/types.h>

struct Gate
{
	u16 offsetLow;
	u16 selector;
	u8 paramCount;
	u8 attribute;
	u16 offsetMid;
	u32 offsetHigh;
	u32 zero;
};

#define DA_DPL0 0x00    // DPL = 0
#define DA_DPL1 0x20    // DPL = 1
#define DA_DPL2 0x40    // DPL = 2
#define DA_DPL3 0x60    // DPL = 3

#define DA_LDT		0x82    // LDT
#define DA_TaskGate	0x85    // task gate
#define DA_386TSS	0x89    // TSS
#define DA_386CGate 0x8C    // call gate
#define DA_386IGate 0x8E    // interrupt gate
#define DA_386TGate 0x8F    // trap gate(unused)

#define LOAD_TR(n)	\
do{	\
	__asm__ __volatile(	\
		"	ltr		%%ax	\n\t"	\
		:	\
		: "a"(n << 3)	\
		: "memory"	\
	);	\
} while (0)

#endif