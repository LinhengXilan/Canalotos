/**
 * @file Error.h
 * @author LinhengXilan
 * @version 0.0.2.13
 * @date 2026-3-20
 */

#ifndef ERROR_H
#define ERROR_H

#include <Config.h>

#define CANALOTOS_ERROR(x)	\
	do						\
	{						\
		if (EFI_ERROR((x))) \
		{					\
			return (x);		\
		}					\
	} while (0)

#ifdef CANALOTOS_DEBUG
/*
	Print(L"[%s] ERROR %d: %s", msg1, x, msg2);
	示例：CANALOTOS_ERROR_MESSAGE(status, "[UEFI]", "Failed to ExitBootServices");
	输出：Print(L"[UEFI] ERROR %d: Failed to ExitBootServices\n", status);

	示例：CANALOTOS_ERROR_MESSAGE(status, "[File]", "Failed to Open File \"%s\"", fileName);
	输出：Print(L"[UEFI] ERROR %d: Failed to Open File \"%s\"\n", status, fileName);
*/
#define CANALOTOS_ERROR_MESSAGE(x, msg1, msg2, ...) \
	do												\
	{												\
		if (EFI_ERROR((x)))							\
		{											\
			Print(L##msg1);							\
			Print(L" ERROR %d: ", (x));				\
			Print(L##msg2, ##__VA_ARGS__);			\
			Print(L"\n");			\
			return (x);								\
		}											\
	} while(0)
#else
#define CANALOTOS_ERROR_MESSAGE(x, msg1, msg2, ...) \
	do 												\
	{ 												\
		if (EFI_ERROR((x))) 						\
		{ 											\
			return (x); 							\
		} 											\
	} while (0)
#endif

#endif