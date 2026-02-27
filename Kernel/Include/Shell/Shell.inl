/**
 * @file Include/Shell/Shell.inl
 * @author LinhengXilan
 * @version 0.0.1.9
 * @date 2026-2-28
 */

#ifndef __SHELL_SHELL_INL_
#define __SHELL_SHELL_INL_

#include <Lib/Print.h>

template<typename... Args>
void Shell::Print(const char* string, Args... args)
{
	uint8_t length = Lib::printf(m_Buffer + m_Index, string, args...);
	m_Index += length;
	Write();
}

#endif