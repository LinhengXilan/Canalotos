/**
 * @file: include/kernel/printk.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KERNEL_PRINTK_H
#define _KERNEL_PRINTK_H

#include <stdarg.h>

int _printk(const char* format, ...);
int _color_printk(u32 frontColor, u32 backgroundColor, const char* format, ...);
int _vsprintf(char* buffer, const char* format, va_list args);
void _putchar(u32 frontColor, u32 backgroundColor, u8 character);

#endif
