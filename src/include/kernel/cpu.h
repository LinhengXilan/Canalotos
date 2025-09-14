/**
 * @file: include/kernel/cpu.h
 * @author: LinhengXilan
 * @data: 2025-9-14
 * @version: build14
 **/

#ifndef _KERNEL_CPU_H_
#define _KERNEL_CPU_H_

#define NR_CPUS 8

// msr
#define IA32_EFER			0xC0000080
#define IA32_SYSENTER_CS	0x174
#define IA32_SYSENTER_ESP	0x175
#define IA32_SYSENTER_EIP	0x176

#endif