/**
 * @file: include/kernel/screen.h
 * @author: lhxl
 * @data: 2025-5-1
 * @version: build8
 **/

#ifndef _KERNEL_PRINTK_H_
#define _KERNEL_PRINTK_H_

#include <kernel/types.h>

struct Screen
{
	struct
	{
		u16 x;
		u16 y;
	} Resolution;
	struct			// The first position of next character
	{
		u16	x;
		u16	y;
	} Position;
	struct
	{
		u16 x;
		u16 y;
	} CharSize;
	u32* bufferAddress;
	u64 bufferSize;
};

#endif