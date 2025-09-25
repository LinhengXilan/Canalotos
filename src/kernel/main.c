/**
 * @file: kernel/main.c
 * @author: LinhengXilan
 * @data: 2025-9-26
 * @version: build15
 **/

#include <kernel/tss.h>
#include <kernel/gate.h>
#include <kernel/init.h>
#include <kernel/global.h>
#include <kernel/memory.h>

/**
 * @note 地址: 0x104000
 */
void kernel_main()
{
	init_screen();
	init_IDT();
	init_cpu();
	LOAD_TR(10);
	set_tss(stack_start, stack_start, stack_start, 0xFFFF800000007C00, 0xFFFF800000007C00, 0xFFFF800000007C00, 0xFFFF800000007C00, 0xFFFF800000007C00, 0xFFFF800000007C00, 0xFFFF800000007C00);
	init_memory();
	init_8259A();
	init_process();	// 0x1040B8
    while (1)
    {

    }
}
