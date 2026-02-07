/**
 * @file Graphics.c
 * @version 0.0.1.9
 * @author LinhengXilan
 * @date 2026-2-8
 */

#include <Uefi.h>
#include <Library/UefiLib.h>

#include <../Config.h>
#include <Graphics.h>

#include "Guid/FileInfo.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL* g_GraphicsOutputProtocol = nullptr;

EFI_STATUS InitGraphics(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;
	EFI_BOOT_SERVICES* efiBootServices = systemTable->BootServices;

	UINTN noHandles = 0;
	EFI_HANDLE* handles = nullptr;
	status = efiBootServices->LocateHandleBuffer(ByProtocol, &gEfiGraphicsOutputProtocolGuid, nullptr, &noHandles, &handles);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[Graphics]Error %d: Failed to Locate EfiGraphicsOutputProtocol!\n", status);
#endif
		return status;
	}

	status = efiBootServices->OpenProtocol(
		handles[0],
		&gEfiGraphicsOutputProtocolGuid,
		(void**)&g_GraphicsOutputProtocol,
		imageHandle,
		nullptr,
		EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
	);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[Graphics]Error %d: Failed to Open EfiGraphicsOutputProtocol!\n", status);
#endif
		return status;
	}
	return status;
}

EFI_STATUS QueryVideoMode()
{
	EFI_STATUS status = EFI_SUCCESS;

	UINTN sizeOfInfo = 0;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info = nullptr;
	for (UINTN i = 0; i < g_GraphicsOutputProtocol->Mode->MaxMode; i++)
	{
		status = g_GraphicsOutputProtocol->QueryMode(g_GraphicsOutputProtocol, i ,&sizeOfInfo, &info);
		if (EFI_ERROR(status))
		{
#ifdef DEBUG
			Print(L"[Graphics]Error %d: Failed to query video mode!\n", status);
#endif
			return status;
		}
	}

	return status;
}

EFI_STATUS SetVideoMode(UINT32 mode)
{
	EFI_STATUS status = EFI_SUCCESS;
	status = g_GraphicsOutputProtocol->SetMode(g_GraphicsOutputProtocol, mode);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[Graphics]Error %d: Failed to set video mode!\n", status);
#endif
		return status;
	}
	return status;
}