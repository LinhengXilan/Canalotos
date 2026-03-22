/**
 * @file Include/Lib/Memory.h
 * @author LinhengXilan
 * @version 0.0.1.8
 * @date 2026-2-16
 */

#ifndef __LIB_MEMORY_H__
#define __LIB_MEMORY_H__

#include <Type.h>

namespace Lib
{
	void* MemSet(void* dst, uint8_t data, uint64_t count);
}

#endif