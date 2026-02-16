/**
 * @file Include/Shell/Shell.inl
 * @author LinhengXilan
 * @version 0.0.1.8
 * @date 2026-2-16
 */

#ifndef __SHELL_SHELL_INL_
#define __SHELL_SHELL_INL_

#include <Shell/Shell.h>

template<typename... Args>
void Shell::Print(const char* string, Args... args)
{
	uint8_t length = Lib::printf(m_Buffer + m_Index, string, args...);
	m_Index += length;
	Write();
}

#endif