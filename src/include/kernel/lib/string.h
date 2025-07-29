/**
 * @file: kernel/lib/string.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _LIB_STRING_H_
#define _LIB_STRING_H_

size_t strlen(const char* str);
void* memcpy(void* p_dst, void* p_src, size_t size);
void memset(void* ptr, char ch, size_t size);
char* strcpy(char* p_dst, char* p_src);
int memcmp(void* p_dst, void* p_src, size_t size);

#endif