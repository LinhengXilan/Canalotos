/**
 * @file: kernel/lib/lib.h
 * @author: LinhengXilan
 * @data: 2025-8-18
 * @version: build13
 **/

#ifndef _KERNEL_LIB_H_
#define _KERNEL_LIB_H_

#include <kernel/types.h>

void _port_write_byte(u16 port, u8 data);
u8 _port_read_byte(u16 port);

void _enable_irq(int irq);
void _disable_irq(int irq);

/**
 * @param ptr 成员变量基地址
 * @param type 成员变量所在结构体
 * @param member 成员变量名
 * @brief 获取成员变量父结构基地址
 */
#define container_of(ptr, type, member)	\
({	\
	typeof(((type*)0)->member)* p = (ptr);	\
	(type*)((u64)p - (u64)&(((type*)0)->member));	\
})

/**
 * @param msr msr寄存器地址
 * @param value 值
 * @brief 向msr寄存器写入值
 */
inline void wrmsr(u64 msr, u64 value)
{
	__asm__ __volatile__(
		"	wrmsr	\n\t"
		:
		: "d"(value >> 32), "a"(value & 0xFFFFFFFF), "c"(msr)
		: "memory"
	);
}

void system_call();

#endif