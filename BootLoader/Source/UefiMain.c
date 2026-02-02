/**
 * @file UefiMain.c
 * @version 0.0.0.1
 * @author LinhengXilan
 * @date 2026-2-3
 */

#include <Uefi.h>
#include <Library/UefiLib.h>

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	Print(L"CanalotosBoot.\n");

	EFI_STATUS status = EFI_SUCCESS;
	EFI_BOOT_SERVICES* efiBootServices = systemTable->BootServices;

	// 获取显卡句柄
	UINTN noHandles = 0;
	EFI_HANDLE* handles = nullptr;
	status = efiBootServices->LocateHandleBuffer(ByProtocol, &gEfiGraphicsOutputProtocolGuid, nullptr, &noHandles, &handles);
	if (EFI_ERROR(status))
	{
		Print(L"Error Code: %d, Cannot find EFI GraphicsOutput protocol!\n", status);
		return status;
	}
	Print(L"No handles: %d\n", noHandles);

	// 打开显卡句柄
	EFI_GRAPHICS_OUTPUT_PROTOCOL* graphicsOutputProtocol;
	status = efiBootServices->OpenProtocol(
		handles[0],
		&gEfiGraphicsOutputProtocolGuid,
		(void**)&graphicsOutputProtocol,
		imageHandle,
		nullptr,
		EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
	);
	if (EFI_ERROR(status))
	{
		Print(L"Error Code: %d, Failed to Open graphicsOutputProtocol!\n", status);
		return status;
	}

	// 查询显示模式
	UINTN sizeOfInfo = 0;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;
	for (UINTN i = 0; i < graphicsOutputProtocol->Mode->MaxMode; i++)
	{
		status = graphicsOutputProtocol->QueryMode(graphicsOutputProtocol, i ,&sizeOfInfo, &info);
		if (EFI_ERROR(status))
		{
			Print(L"Error Code: %d, Cannot query mode!\n", status);
			return status;
		}
		Print(L"Mode Size: %d\n", sizeOfInfo);
		Print(L"Mode: %d, Width: %d, Height: %d\n", i, info->HorizontalResolution, info->VerticalResolution);
	}

	// 设置显示模式
	status = graphicsOutputProtocol->SetMode(graphicsOutputProtocol, 22);
	if (EFI_ERROR(status))
	{
		Print(L"Error Code: %d, Cannot set mode!\n", status);
		return status;
	}

	while (1)
	{

	}
	return EFI_SUCCESS;
}