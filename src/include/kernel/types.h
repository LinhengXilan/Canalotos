/**
 * @file: include/kernel/types.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KRENEL_TYPES_H_
#define _KRENEL_TYPES_H_

typedef unsigned char 		u8;
typedef unsigned short 		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef char 		s8;
typedef short 		s16;
typedef int			s32;
typedef long long	s64;

typedef unsigned long long size_t;
typedef unsigned long long ptr_t;
typedef unsigned long long pid_t;

typedef void (*int_handler)();
typedef void (*irq_handler)();

#endif