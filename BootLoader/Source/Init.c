/**
 * @file Init.c
 * @author LinhengXilan
 * @version 0.0.2.13
 * @date 2026-3-20
 */

#include <Uefi.h>

#include <Init.h>
#include <Graphics.h>
#include <File.h>
#include <Error.h>

EFI_STATUS Init(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;
	status = InitGraphics(imageHandle, systemTable);
	CANALOTOS_ERROR(status);

	status = InitFile(imageHandle, systemTable);
	CANALOTOS_ERROR(status);

	return status;
}
