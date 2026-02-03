/**
 * @file Graphics.c
 * @version 0.0.0.3
 * @author LinhengXilan
 * @date 2026-2-4
 */

#include <Uefi.h>
#include <Library/UefiLib.h>

#include <Graphics.h>
#include <Config.h>
#include <Global.h>

EFI_GRAPHICS_OUTPUT_PROTOCOL* g_GraphicsOutputProtocol;

EFI_STATUS InitGraphics(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;
	UINTN noHandles = 0;
	EFI_HANDLE* handles = nullptr;
	status = g_EfiBootServices->LocateHandleBuffer(ByProtocol, &gEfiGraphicsOutputProtocolGuid, nullptr, &noHandles, &handles);
#ifdef DEBUG
	if (EFI_ERROR(status))
	{
		Print(L"Error Code: %d, Cannot find EFI GraphicsOutput protocol!\n", status);
		return status;
	}
#endif

#ifdef LOG
	Print(L"No handles: %d\n", noHandles);
#endif

	status = g_EfiBootServices->OpenProtocol(
		handles[0],
		&gEfiGraphicsOutputProtocolGuid,
		(void**)&g_GraphicsOutputProtocol,
		imageHandle,
		nullptr,
		EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
	);
#ifdef DEBUG
	if (EFI_ERROR(status))
	{
		Print(L"Error Code: %d, Failed to Open graphicsOutputProtocol!\n", status);
		return status;
	}
#endif
	return EFI_SUCCESS;
}

EFI_STATUS QueryGraphicMode()
{
	EFI_STATUS status = EFI_SUCCESS;
	UINTN sizeOfInfo = 0;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;
	for (UINTN i = 0; i < g_GraphicsOutputProtocol->Mode->MaxMode; i++)
	{
		status = g_GraphicsOutputProtocol->QueryMode(g_GraphicsOutputProtocol, i ,&sizeOfInfo, &info);
#ifdef DEBUG
		if (EFI_ERROR(status))
		{
			Print(L"Error Code: %d, Cannot query mode!\n", status);
			return status;
		}
#endif
		Print(L"Mode Size: %d\n", sizeOfInfo);
		Print(L"Mode: %d, Width: %d, Height: %d\n", i, info->HorizontalResolution, info->VerticalResolution);
	}
	return EFI_SUCCESS;
}

EFI_STATUS SetVideoMode(UINT32 mode)
{
	EFI_STATUS status = EFI_SUCCESS;
	status = g_GraphicsOutputProtocol->SetMode(g_GraphicsOutputProtocol, mode);
#ifdef DEBUG
	if (EFI_ERROR(status))
	{
		Print(L"Error Code: %d, Cannot set mode!\n", status);
		return status;
	}
#endif
	return EFI_SUCCESS;
}