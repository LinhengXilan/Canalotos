/**
 * @file Include/Lib/Print.h
 * @author LinhengXilan
 * @version 0.0.1.8
 * @date 2026-2-16
 */

#ifndef __LIB_PRINT_H__
#define __LIB_PRINT_H__

#include <Lib/String.h>
#include <Stdarg.h>

namespace Lib
{
	/**
	 *
	 * @param buffer 缓冲区
	 * @param string 未格式化的字符串
	 * @param args 字符串占位符参数
	 * @return 实际打印的字符串长度
	 */
	uint8_t PrintfToBuffer(char* buffer, const char* string, va_list args);
	uint8_t FormatParse(char* buffer, const char* format, va_list args);
	uint8_t itoa(char* buffer, uint64_t number, uint8_t base, uint8_t precision);
}

#endif