/**
 * @file Graphics.c
 * @version 0.0.0.2
 * @author LinhengXilan
 * @date 2026-2-3
 */

#include <Uefi.h>
#include <Library/UefiLib.h>

#include <Graphics.h>
#include <Config.h>

EFI_GRAPHICS_OUTPUT_PROTOCOL* graphicsOutputProtocol;

EFI_STATUS InitGraphics(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;
	EFI_BOOT_SERVICES* efiBootServices = systemTable->BootServices;
	UINTN noHandles = 0;
	EFI_HANDLE* handles = nullptr;
	status = efiBootServices->LocateHandleBuffer(ByProtocol, &gEfiGraphicsOutputProtocolGuid, nullptr, &noHandles, &handles);
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

	status = efiBootServices->OpenProtocol(
		handles[0],
		&gEfiGraphicsOutputProtocolGuid,
		(void**)&graphicsOutputProtocol,
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
	for (UINTN i = 0; i < graphicsOutputProtocol->Mode->MaxMode; i++)
	{
		status = graphicsOutputProtocol->QueryMode(graphicsOutputProtocol, i ,&sizeOfInfo, &info);
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
	status = graphicsOutputProtocol->SetMode(graphicsOutputProtocol, mode);
#ifdef DEBUG
	if (EFI_ERROR(status))
	{
		Print(L"Error Code: %d, Cannot set mode!\n", status);
		return status;
	}
#endif
	return EFI_SUCCESS;
}