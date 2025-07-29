/**
 * @file: kernel/process/process.c
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/
#include <kernel/global.h>
#include <kernel/process/process.h>
#include <kernel/tss.h>
#include <kernel/lib/string.h>
#include <kernel/memory.h>
#include <kernel/lib/list.h>
#include <kernel/lib/lib.h>
#include <kernel/printk.h>

void init_process_memory();
u64 init_context(u64 (*func)(u64), u64 arg, u64 flags);
u64 init(u64 arg);
u64 create_process(struct Context* regs, u64 flags, u64 stack_start, u64 stack_size);
void kernel_thread_func();
void ret_system_call();

/**
 * @brief 初始化进程
 * @note 地址：0x107433
 */
void init_process()
{
	_color_printk(ORANGE, BLACK, "init_process()\n");
	struct Process_struct* process = nullptr;
	init_process_memory();
	list_init(&_init_process.process.list);
	init_context(init, 10, NULL);
	_init_process.process.state = RUNNING;
	process = container_of(list_next(&get_current_process()->list), struct Process_struct, list);
	switch_to(get_current_process(), process);
}

/**
 * @brief 初始化进程内存信息
 */
void init_process_memory()
{
	i_process_memory.pgt = (pgt_t*)Global_CR3;
	i_process_memory.code_start = memory_desc.kernel_code_start;
	i_process_memory.code_end = memory_desc.kernel_code_end;
	i_process_memory.data_start = (u64)&_data;
	i_process_memory.data_end = memory_desc.kernel_data_end;
	i_process_memory.read_only_data_start = (u64)&_rodata;
	i_process_memory.read_only_data_end = (u64)&_erodata;
	i_process_memory.brk_start = 0;
	i_process_memory.brk_end = memory_desc.kernel_end;
	i_process_memory.stack_start = stack_start;
	set_tss(
		i_thread.rsp0, _init_tss[0].rsp1, _init_tss[0].rsp2,
		_init_tss[0].ist1, _init_tss[0].ist2, _init_tss[0].ist3, _init_tss[0].ist4, _init_tss[0].ist5, _init_tss[0].ist6, _init_tss[0].ist7
	);
	_init_tss[0].rsp0 = i_thread.rsp0;
}

/**
 * @param func 进程主函数
 * @param arg 进程参数
 * @param flags 标志
 * @brief 初始化进程上下文信息
 * @note PRIVATE
 */
u64 init_context(u64 (*func)(u64), u64 arg, u64 flags)
{
	struct Context context;
	memset(&context, 0, sizeof(context));
	context.rbx = (u64)func;
	context.rdx = arg;
	context.ds = SELECTOR_KERNEL_DS;
	context.es = SELECTOR_KERNEL_DS;
	context.cs = SELECTOR_KERNEL_CS;
	context.ss = SELECTOR_KERNEL_DS;
	context.rflags = 1 << 9;
	context.rip = (u64)kernel_thread_func;
	return create_process(&context, flags, NULL, NULL);
}

/**
 * @return 0
 */
u64 create_process(struct Context* regs, u64 flags, u64 stack_start, u64 stack_size)
{
	_color_printk(GREEN, BLACK, "alloc_pages, bitmap:%16x\n", *memory_desc.bits_map);
	struct Page* page = alloc_page(ZONE_NORMAL,1,PAGE_TABLE_MAPPED | PAGE_ACTIVE | PAGE_KERNEL);
	_color_printk(GREEN, BLACK, "alloc_pages, bitmap:%16x\n", *memory_desc.bits_map);
	struct Process_struct* process = (struct Process_struct*)phy2vir(page->address);
	_color_printk(GREEN, BLACK, "process address:%lx\n", (u64)process);
	memset(process, 0, sizeof(*process));
	*process = *current_process;
	list_init(&process->list);
	list_push_front(&_init_process.process.list, &process->list);
	process->pid++;
	process->state = UNINTERRUPTIBLE;
	struct Thread* thread = (struct Thread*)(process + 1);
	process->thread = thread;
	memcpy((void*)((u64)process + PROCESS_STACK_SIZE - sizeof(struct Context)), regs, sizeof(struct Context));
	thread->rsp0 = (u64)process + PROCESS_STACK_SIZE;
	thread->rip = regs->rip;
	thread->rsp = (u64)process + PROCESS_STACK_SIZE - sizeof(struct Context);
	if (!(process->flags & KERNEL))
	{
		thread->rip = regs->rip = (u64)ret_system_call;
	}
	process->state = RUNNING;
	return 0;
}

u64 init(u64 arg)
{
	_color_printk(BLUE, BLACK, "init process is running, arg = 0x%lx\n", arg);
	return 1;
}

void do_process_exit(u64 code)
{
	_color_printk(BLUE, BLACK, "exit process is running, code = 0x%lx\n", code);
	while(1);
}

void ret_system_call()
{

}

void _switch_to(struct Process_struct* prev, struct Process_struct* next)
{
	_init_tss[0].rsp0 = next->thread->rsp0;
	set_tss(
		_init_tss[0].rsp0, _init_tss[0].rsp1, _init_tss[0].rsp2,
		_init_tss[0].ist1, _init_tss[0].ist2, _init_tss[0].ist3, _init_tss[0].ist4, _init_tss[0].ist5, _init_tss[0].ist6, _init_tss[0].ist7
	);
	__asm__ __volatile__(
		"	movq	%%fs, %0	\n\t"
		: "=a"(prev->thread->fs)
	);
	__asm__ __volatile__(
		"	movq	%%gs, %0	\n\t"
		: "=a"(prev->thread->gs)
	);
	__asm__ __volatile__(
		"	movq	%0, %%fs	\n\t"
		:
		: "a"(next->thread->fs)
	);
	__asm__ __volatile__(
		"	movq	%0, %%gs	\n\t"
		:
		: "a"(next->thread->gs)
	);
	_color_printk(GREEN, BLACK, "prev->thread->rsp0:%x\n", prev->thread->rsp0);
	_color_printk(GREEN, BLACK, "next->thread->rsp0:%x\n", next->thread->rsp0);
}