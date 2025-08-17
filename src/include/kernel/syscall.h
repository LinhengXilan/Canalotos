/**
 * @file: include/kernel/syscall.h
 * @author: LinhengXilan
 * @data: 2025-8-18
 * @version: build13
 **/

#ifndef _KERNEL_SYSCALL_H_
#define _KERNEL_SYSCALL_H_

#include <kernel/types.h>
#include <kernel/process/process.h>

#define SYSCALL_NR 16

typedef unsigned long long (*syscall_func)(struct Context* context);

u64 no_syscall_func(struct Context* context)
{
	_color_printk(RED, BLACK, "Syscall error: no syscall function: 0x%x\n", context->rax);
	return -1;
}

syscall_func syscall_table[SYSCALL_NR] = { [0 ... SYSCALL_NR - 1] = no_syscall_func };

#endif