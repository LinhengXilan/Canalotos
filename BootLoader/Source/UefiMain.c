/**
 * @file UefiMain.c
 * @version 0.0.1.8
 * @author LinhengXilan
 * @date 2026-2-7
 */

#include <Uefi.h>

#include <Init.h>
#include <Graphics.h>
#include <File.h>

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;
	// 初始化
	status = Init(imageHandle, systemTable);
	if (EFI_ERROR(status))
	{
		return status;
	}
	// 查询显示模式
	status = QueryVideoMode();
	if (EFI_ERROR(status))
	{
		return status;
	}
	// 设置显示模式
	status = SetVideoMode(22);
	if (EFI_ERROR(status))
	{
		return status;
	}

	EFI_FILE_PROTOCOL* kernel = nullptr;
	status = GetFileHandle(L"Kernel\\Head.bin", &kernel);
	if (EFI_ERROR(status))
	{
		return status;
	}

	EFI_PHYSICAL_ADDRESS kernelAddress;
	status = ReadFile(systemTable, kernel, &kernelAddress);
	if (EFI_ERROR(status))
	{
		return status;
	}
	Print(L"1");
	asm(
		"jmp	%0"
		:
		: "m"(kernelAddress)
	);

	while (1)
	{

	}
	return status;
}