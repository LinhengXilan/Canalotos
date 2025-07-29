/**
 * @file: include/process/process.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KERNEL_PROCESS_PROCESS_H_
#define _KERNEL_PROCESS_PROCESS_H_

#include <kernel/const.h>
#include <kernel/types.h>
#include <kernel/memory/pages.h>
#include <kernel/lib/list.h>

struct ProcessMemory
{
	pgt_t* pgt;
	u64 code_start;
	u64 code_end;
	u64 data_start;
	u64 data_end;
	u64 read_only_data_start;
	u64 read_only_data_end;
	u64 brk_start;
	u64 brk_end;
	u64 stack_start;
};

struct ProcessMemory i_process_memory = { 0 };

struct Thread
{
	u64 rsp0;
	u64 rip;
	u64 rsp;
	u64 fs;
	u64 gs;
	u64 cr2;
	u64 trap_nr;
	u64 error_code;
};

struct Context
{
	u64 r15;
	u64 r14;
	u64 r13;
	u64 r12;
	u64 r11;
	u64 r10;
	u64 r9;
	u64 r8;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;
	u64 rbp;
	u64 ds;
	u64 es;
	u64 rax;
	u64 func;
	u64 errcode;
	u64 rip;
	u64 cs;
	u64 rflags;
	u64 rsp;
	u64 ss;
};

enum ProcessFlags : u64
{
	KERNEL = 1 << 0
};

enum ProcessState : u64
{
	RUNNING			= 1 << 0,
	UNINTERRUPTIBLE = 1 << 1
};

struct Process_struct
{
	struct List list;
	struct ProcessMemory* memory;
	struct Context* context;
	struct Thread* thread;
	pid_t pid;
	u64	address_limit;
	u64 priority;
	enum ProcessFlags flags;
	enum ProcessState state;
};

union Process
{
	struct Process_struct process;
	u64 stack[PROCESS_STACK_SIZE / sizeof(u64)];
}__attribute__((aligned(8)));

union Process _init_process;

#define INIT_PROCESS_STRUCT(process)	\
{	\
	.state = UNINTERRUPTIBLE,	\
	.flags = KERNEL,	\
	.memory = &i_process_memory,	\
	.thread = &i_thread,	\
	.address_limit = 0xFFFF800000000000,	\
	.pid = 0,	\
	.priority = 0	\
}

struct Thread i_thread =
{
	.rsp0 = (u64)(_init_process.stack + PROCESS_STACK_SIZE / sizeof(u64)),
	.rsp = (u64)(_init_process.stack + PROCESS_STACK_SIZE / sizeof(u64)),
	.fs = SELECTOR_KERNEL_DS,
	.gs = SELECTOR_KERNEL_DS,
	.cr2 = 0,
	.trap_nr = 0,
	.error_code = 0
};

inline struct Process_struct* get_current_process()
{
	struct Process_struct* current = nullptr;
	__asm__ __volatile__(
		"	andq	%%rsp, %0	\n\t"
		: "=r"(current)
		: "0"(~32767ul)
	);
	return current;
}

union Process _init_process __attribute__((__section__(".data._init_process"))) = {INIT_PROCESS_STRUCT(_init_process.process)};
struct Process_struct* _init_process_struct[NR_CPUS] = { &_init_process.process, 0 };

#define current_process get_current_process()

#define switch_to(prev, next)	\
do {	\
	__asm__ __volatile__(	\
		"	pushq	%%rbp				\n\t"	\
		"	pushq	%%rax				\n\t"	\
		"	movq	%%rsp, %0			\n\t"	\
		"	movq	%2, %%rsp			\n\t"	\
		"	leaq	1f(%%rip), %%rax	\n\t"	\
		"	movq	%%rax, %1			\n\t"	\
		"	pushq	%3					\n\t"	\
		"	jmp		_switch_to			\n\t"	\
		"1:	popq	%%rax				\n\t"	\
		"	popq	%%rbp				\n\t"	\
		: "=m"(prev->thread->rsp), "=m"(prev->thread->rip)	\
		: "r"(next->thread->rsp), "r"(next->thread->rip), "D"(prev), "S"(next)	\
		: "memory"	\
	);	\
} while (0)

#endif