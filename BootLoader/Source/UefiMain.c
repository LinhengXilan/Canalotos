/**
 * @file UefiMain.c
 * @version 0.0.0.5
 * @author LinhengXilan
 * @date 2026-2-4
 */

#include <Uefi.h>

#include <Init.h>
#include <Graphics.h>

EFI_BOOT_SERVICES* g_EfiBootServices = nullptr;

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	g_EfiBootServices = systemTable->BootServices;
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

	while (1)
	{

	}
	return status;
}