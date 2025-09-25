/**
 * @file: include/kernel/printk.h
 * @author: LinhengXilan
 * @data: 2025-9-26
 * @version: build15
 **/

#ifndef _KERNEL_PRINTK_H
#define _KERNEL_PRINTK_H

#include <stdarg.h>

int printk(const char* format, ...);
int color_printk(u32 frontColor, u32 backgroundColor, const char* format, ...);
int vsprintf(char* buffer, const char* format, va_list args);
void putchar(u32 frontColor, u32 backgroundColor, u8 character);

#endif
