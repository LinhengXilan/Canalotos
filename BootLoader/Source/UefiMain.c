/**
 * @file UefiMain.c
 * @author LinhengXilan
 * @version 0.0.2.13
 * @date 2026-3-20
 */

#include <Uefi.h>
#include <Library/UefiLib.h>

#include <Config.h>
#include <Init.h>
#include <Graphics.h>
#include <File.h>
#include <KernelData.h>
#include <Memory.h>
#include <Error.h>

typedef int(*Kernel)(const EFI_DATA* efiData);

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;
	EFI_BOOT_SERVICES* bootServices = systemTable->BootServices;

	// 初始化
	status = Init(imageHandle, systemTable);
	CANALOTOS_ERROR(status);

	// 设置显示模式
#ifdef CANALOTOS_VIRTUAL_MACHINE
	status = SetVideoMode(22);
#else
	status = SetVideoMode(0);
#endif
	CANALOTOS_ERROR(status);

	EFI_FILE_PROTOCOL* kernel = nullptr;
	status = GetFileHandle(L"Canalotos\\Kernel", &kernel);
	CANALOTOS_ERROR(status);

	EFI_PHYSICAL_ADDRESS kernelEntryPoint;
	status = ReadFile(systemTable, kernel, &kernelEntryPoint);
	CANALOTOS_ERROR(status);

	EFI_DATA efiData;
	status = GetEFIDataGraphics(&efiData.Graphics);
	CANALOTOS_ERROR(status);

	status = GetEFIDataMemory(systemTable, &efiData.Memory);
	CANALOTOS_ERROR(status);

	status = bootServices->ExitBootServices(imageHandle, efiData.Memory.MapKey);
	CANALOTOS_ERROR_MESSAGE(status, "[UEFI]", "Failed to ExitBootServices");

	Kernel main = (Kernel)kernelEntryPoint;
	main(&efiData);

	return status;
}