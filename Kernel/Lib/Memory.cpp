/**
 * @file Lib/Memory.cpp
 * @author LinhengXilan
 * @version 0.0.1.9
 * @date 2026-2-28
 */

#include <Lib/Memory.h>

namespace Lib
{
	void* MemSet(void* dst, uint8_t data, uint64_t count)
	{
		for (uint64_t i = 0; i < count; i++)
		{
			static_cast<uint8_t*>(dst)[i] = data;
		}
		return dst;
	}
}
