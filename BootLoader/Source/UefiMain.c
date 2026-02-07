/**
 * @file UefiMain.c
 * @version 0.0.1.9
 * @author LinhengXilan
 * @date 2026-2-8
 */

#include <Uefi.h>
#include <Library/UefiLib.h>

#include <../Config.h>
#include <Init.h>
#include <Graphics.h>
#include <File.h>

typedef int(*Kernel)(int);

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;

	// 初始化
	status = Init(imageHandle, systemTable);
	if (EFI_ERROR(status))
	{
		return status;
	}

	// // 查询显示模式
	// EFI_GRAPHICS_OUTPUT_MODE_INFORMATION graphicsMode;
	// UINT32 graphicsIndex;
	// status = QueryVideoMode(&graphicsMode, &graphicsIndex);
	// if (EFI_ERROR(status))
	// {
	// 	return status;
	// }

	// 设置显示模式
#ifdef VIRTUAL_MACHINE
	status = SetVideoMode(22);
#else
	status = SetVideoMode(0);
#endif
	if (EFI_ERROR(status))
	{
		return status;
	}

	EFI_FILE_PROTOCOL* kernel = nullptr;
	status = GetFileHandle(L"Kernel\\Kernel.bin", &kernel);
	if (EFI_ERROR(status))
	{
		return status;
	}

	EFI_PHYSICAL_ADDRESS kernelEntryPoint;
	status = ReadFile(systemTable, kernel, &kernelEntryPoint);
	if (EFI_ERROR(status))
	{
		return status;
	}

	Kernel main = (Kernel)kernelEntryPoint;
	int ret = main(666);
	Print(L"ret from Kernel: %d", ret);

	while (1)
	{

	}
	return status;
}