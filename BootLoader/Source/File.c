/**
 * @file File.c
 * @version 0.0.1.8
 * @author LinhengXilan
 * @date 2026-2-7
 */

#include <Library/UefiLib.h>
#include <Guid/FileInfo.h>

#include <File.h>
#include <Config.h>

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* g_EfiSimpleFileSystemProtocol = nullptr;

EFI_STATUS InitFile(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;

	EFI_BOOT_SERVICES* efiBootServices = systemTable->BootServices;

	UINTN handleCount = 0;
	EFI_HANDLE* handles = nullptr;
	status = efiBootServices->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, nullptr, &handleCount, &handles);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to Located EfiSimpleFileSystemProtocol\n", status);
#endif
		return status;
	}

	status = efiBootServices->OpenProtocol(
		handles[0],
		&gEfiSimpleFileSystemProtocolGuid,
		(VOID**)&g_EfiSimpleFileSystemProtocol,
		imageHandle,
		nullptr,
		EFI_OPEN_PROTOCOL_GET_PROTOCOL
	);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to Open EfiSimpleFileSystemProtocol\n", status);
#endif
		return status;
	}

	return status;
}

EFI_STATUS GetFileHandle(CHAR16* fileName, EFI_FILE_PROTOCOL** file)
{
	EFI_STATUS status = EFI_SUCCESS;

	// 打开文件系统的卷
	EFI_FILE_PROTOCOL* root = nullptr;
	status = g_EfiSimpleFileSystemProtocol->OpenVolume(g_EfiSimpleFileSystemProtocol, &root);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to OpenVolume in GetFileHandle\n", status);
#endif
		return status;
	}

	status = root->Open(root, file, fileName, EFI_FILE_MODE_READ, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to Open File \"%s\"\n", status, fileName);
#endif
	}
	return status;
}

EFI_STATUS ReadFile(EFI_SYSTEM_TABLE* systemTable, EFI_FILE_PROTOCOL* file, EFI_PHYSICAL_ADDRESS* address)
{
	EFI_STATUS status = EFI_SUCCESS;
	EFI_BOOT_SERVICES* efiBootServices = systemTable->BootServices;
	// 分配内存空间
	EFI_FILE_INFO* fileInfo = nullptr;
	UINTN fileInfoSize = sizeof(EFI_FILE_INFO) + 16;	// 下面的读取信息不止读了EFI_FILE_INFO，还有文件名。
	status = efiBootServices->AllocatePool(EfiLoaderData, fileInfoSize, (VOID**)&fileInfo);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to AllocatePool in ReadFile\n", status);
#endif
		return status;
	}

	// 读取文件信息
	status = file->GetInfo(file, &gEfiFileInfoGuid, &fileInfoSize, fileInfo);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to GetInfo in ReadFile\n", status);
#endif
		return status;
	}
	// 分配存放文件的内存空间
	EFI_PHYSICAL_ADDRESS tempAddress;
	UINTN pageNum = (fileInfo->FileSize >> 12) + 1;
	status = efiBootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, pageNum, &tempAddress);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to AllocatePages in ReadFile\n", status);
#endif
		return status;
	}

	// 读取文件至指定内存地址
	status = file->Read(file, &fileInfo->FileSize, (VOID*)tempAddress);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to Read in ReadFile.", status);
#endif
		return status;
	}

	*address = tempAddress;
	// 释放之前分配的内存空间
	efiBootServices->FreePool(fileInfo);

	return status;
}