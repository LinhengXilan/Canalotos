/**
 * @file Lib/String.cpp
 * @author LinhengXilan
 * @version 0.0.0.6
 * @date 2026-2-13
 */

#include <Lib/String.h>

namespace Lib
{
	size_t StrLen(const char* str)
	{
		size_t size = 0;
		while (*str++ != '\0')
		{
			++size;
		}
		return size;
	}
}