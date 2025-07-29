/**
 * @file: kernel/lib/lib.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
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
// #define container_of(ptr, type, member)	\
// ({	\
// 	typeof(((type*)0)->member)* p = (ptr);	\
// 	(type*)((u64)p - (u64)&(((type*)0)->member));	\
// })

#define container_of(ptr,type,member)							\
({											\
typeof(((type *)0)->member) * p = (ptr);					\
(type *)((unsigned long)p - (unsigned long)&(((type *)0)->member));		\
})

#endif