/**
 * @file Include/Type.h
 * @author LinhengXilan
 * @version 0.0.1.9
 * @date 2026-2-28
 */

#ifndef __TYPE_H__
#define __TYPE_H__

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef uint64_t size_t;
typedef uint64_t offset_t;
typedef uint64_t address_t;

struct Buffer
{
	address_t Base;
	size_t Size;
};

#endif