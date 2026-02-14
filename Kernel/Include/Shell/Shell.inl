/**
 * @file Include/Shell/Shell.inl
 * @author LinhengXilan
 * @version 0.0.0.7
 * @date 2026-2-14
 */

#ifndef __SHELL_SHELL_INL_
#define __SHELL_SHELL_INL_

#include <Shell/Shell.h>

template<typename... Args>
	void Shell::Print(const char* string, Args... args)
{
	Lib::printf(m_Buffer + m_Index, string, args...);
	Write();
}

#endif