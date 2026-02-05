/**
 * @file Init.c
 * @version 0.0.0.2
 * @author LinhengXilan
 * @date 2026-2-3
 */

#include <Uefi.h>

#include <Init.h>
#include <Graphics.h>
#include <File.h>

EFI_STATUS Init(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;
	status = InitGraphics(imageHandle, systemTable);
	if (EFI_ERROR(status))
	{
		return status;
	}
	status = InitFile(imageHandle, systemTable);
	if (EFI_ERROR(status))
	{
		return status;
	}
	return status;
}
