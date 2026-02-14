/**
 * @file Include/Lib/Print.h
 * @author LinhengXilan
 * @version 0.0.0.7
 * @date 2026-2-14
 */

#ifndef __LIB_PRINT_H__
#define __LIB_PRINT_H__

#include <Lib/String.h>
#include <stdarg.h>

namespace Lib
{
	void printf(char* buffer, const char* string, ...);

	uint8_t format(char* buffer, const char* format, va_list args);
	char* itoa(char* buffer, uint64_t number, uint8_t base, uint8_t precision);
}

#endif