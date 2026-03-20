/**
 * @file Graphics.c
 * @author LinhengXilan
 * @version 0.0.2.13
 * @date 2026-3-20
 */

#include <Uefi.h>
#include <Library/UefiLib.h>

#include <Graphics.h>
#include <Error.h>

UINT32 g_GraphicsMode = 0;

EFI_GRAPHICS_OUTPUT_PROTOCOL* g_GraphicsOutputProtocol = nullptr;

EFI_STATUS InitGraphics(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;
	EFI_BOOT_SERVICES* efiBootServices = systemTable->BootServices;

	UINTN noHandles = 0;
	EFI_HANDLE* handles = nullptr;
	status = efiBootServices->LocateHandleBuffer(ByProtocol, &gEfiGraphicsOutputProtocolGuid, nullptr, &noHandles, &handles);
	CANALOTOS_ERROR_MESSAGE(status, "[Graphics]", "Failed to Locate EfiGraphicsOutputProtocol");

	status = efiBootServices->OpenProtocol(
		handles[0],
		&gEfiGraphicsOutputProtocolGuid,
		(void**)&g_GraphicsOutputProtocol,
		imageHandle,
		nullptr,
		EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
	);
	CANALOTOS_ERROR_MESSAGE(status, "[Graphics]", "Failed to Open EfiGraphicsOutputProtocol");

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
		CANALOTOS_ERROR_MESSAGE(status, "[Graphics]", "Failed to query video mode");
	}

	return status;
}

EFI_STATUS SetVideoMode(UINT32 mode)
{
	EFI_STATUS status = EFI_SUCCESS;
	status = g_GraphicsOutputProtocol->SetMode(g_GraphicsOutputProtocol, mode);
	CANALOTOS_ERROR_MESSAGE(status, "[Graphics]", "Failed to set video mode");

	g_GraphicsMode = mode;

	return status;
}

EFI_STATUS GetGraphicsModeInfo(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION** info)
{
	EFI_STATUS status = EFI_SUCCESS;

	UINTN infoSize = 0;
	status = g_GraphicsOutputProtocol->QueryMode(g_GraphicsOutputProtocol, g_GraphicsMode, &infoSize, info);
	CANALOTOS_ERROR_MESSAGE(status, "[Graphics]", "Failed to GetGraphicsModeInfo");

	return status;
}

EFI_STATUS GetEFIDataGraphics(EFI_DATA_GRAPHICS* efiDataGraphics)
{
	EFI_STATUS status = EFI_SUCCESS;

	efiDataGraphics->FrameBufferBase = g_GraphicsOutputProtocol->Mode->FrameBufferBase;
	efiDataGraphics->FrameBufferSize = g_GraphicsOutputProtocol->Mode->FrameBufferSize;

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info = nullptr;
	status = GetGraphicsModeInfo(&info);
	CANALOTOS_ERROR(status);

	efiDataGraphics->HorizontalResolution = info->HorizontalResolution;
	efiDataGraphics->VerticalResolution = info->VerticalResolution;

	return status;
}