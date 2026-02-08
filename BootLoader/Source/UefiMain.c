/**
 * @file UefiMain.c
 * @version 0.0.1.10
 * @author LinhengXilan
 * @date 2026-2-9
 */

#include <Uefi.h>
#include <Library/UefiLib.h>

#include <Config.h>
#include <Init.h>
#include <Graphics.h>
#include <File.h>
#include <KernelData.h>

typedef int(*Kernel)(EFI_DATA efiData);

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;

	// 初始化
	status = Init(imageHandle, systemTable);
	if (EFI_ERROR(status))
	{
		return status;
	}

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

	EFI_DATA efiData;
	status = GetEFIDataGraphics(&efiData.Graphics);
	if (EFI_ERROR(status))
	{
		return status;
	}

	Kernel main = (Kernel)kernelEntryPoint;
	int ret = main(efiData);
	Print(L"Address of Kernel: %x\n", kernelEntryPoint);
	Print(L"ret from Kernel: 0x%lx\n", ret);

	while (1)
	{

	}
	return status;
}