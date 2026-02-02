/**
 * @file UefiMain.c
 * @version 0.0.0.2
 * @author LinhengXilan
 * @date 2026-2-3
 */

#include <Uefi.h>
#include <Library/UefiLib.h>

#include <Config.h>
#include <Init.h>
#include <Graphics.h>

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	Print(L"CanalotosBoot.\n");
	EFI_STATUS status = EFI_SUCCESS;
	// 初始化
	status = Init(imageHandle, systemTable);
	// 查询显示模式
	status = QueryGraphicMode();
	// 设置显示模式
	status = SetVideoMode(22);

	while (1)
	{

	}
	return status;
}