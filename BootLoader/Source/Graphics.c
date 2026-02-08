/**
 * @file Graphics.c
 * @version 0.0.1.10
 * @author LinhengXilan
 * @date 2026-2-9
 */

#include <Uefi.h>
#include <Library/UefiLib.h>

#include <Config.h>
#include <Graphics.h>

#include "Guid/FileInfo.h"

UINT32 g_GraphicsMode = 0;

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

	UINTN infoSize = 0;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info = nullptr;
	for (UINTN i = 0; i < g_GraphicsOutputProtocol->Mode->MaxMode; i++)
	{
		status = g_GraphicsOutputProtocol->QueryMode(g_GraphicsOutputProtocol, i ,&infoSize, &info);
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

	g_GraphicsMode = mode;

	return status;
}

EFI_STATUS GetGraphicsModeInfo(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION** info)
{
	EFI_STATUS status = EFI_SUCCESS;

	UINTN infoSize = 0;
	status = g_GraphicsOutputProtocol->QueryMode(g_GraphicsOutputProtocol, g_GraphicsMode, &infoSize, info);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[Graphics]Error %d: GetGraphicsModeInfo!\n", status);
#endif
		return status;
	}

	return status;
}

EFI_STATUS GetEFIDataGraphics(EFI_DATA_GRAPHICS* efiDataGraphics)
{
	EFI_STATUS status = EFI_SUCCESS;

	efiDataGraphics->FrameBufferBase = g_GraphicsOutputProtocol->Mode->FrameBufferBase;
	efiDataGraphics->FrameBufferSize = g_GraphicsOutputProtocol->Mode->FrameBufferSize;

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info = nullptr;
	status = GetGraphicsModeInfo(&info);
	if (EFI_ERROR(status))
	{
		return status;
	}
	efiDataGraphics->HorizontalResolution = info->HorizontalResolution;
	efiDataGraphics->VerticalResolution = info->VerticalResolution;

	return status;
}