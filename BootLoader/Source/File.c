/**
 * @file File.c
 * @version 0.0.0.6
 * @author LinhengXilan
 * @date 2026-2-6
 */

#include <Library/UefiLib.h>

#include <File.h>
#include <Global.h>
#include <Config.h>

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* g_EfiSimpleFileSystemProtocol = nullptr;

EFI_STATUS InitFile(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;

	UINTN handleCount = 0;
	EFI_HANDLE* handles = nullptr;
	status = g_EfiBootServices->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, nullptr, &handleCount, &handles);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to Located EfiSimpleFileSystemProtocol.", status);
#endif
		return status;
	}

	status = g_EfiBootServices->OpenProtocol(
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
		Print(L"[File]ERROR %d: Failed to Open EfiSimpleFileSystemProtocol.", status);
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
		Print(L"[File]ERROR %d: Failed to OpenVolume in GetFileHandle.", status);
#endif
		return status;
	}

	// 从根开始打开文件，直至打开所需文件
	status = root->Open(root, file, fileName, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to Open File.", status);
#endif
	}
	return status;
}

EFI_STATUS ReadFile(EFI_FILE_PROTOCOL* file, EFI_PHYSICAL_ADDRESS* address)
{
	EFI_STATUS status = EFI_SUCCESS;

	// 分配内存空间
	EFI_FILE_INFO* fileInfo = nullptr;
	UINTN fileInfoSize = sizeof(EFI_FILE_INFO);
	status = g_EfiBootServices->AllocatePool(EfiLoaderData, fileInfoSize, (VOID**)&fileInfo);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to AllocatePool in ReadFile.", status);
#endif
		return status;
	}

	// 读取文件信息
	status = file->GetInfo(file, gEfiFileInfoGuid, fileInfoSize, &fileInfo);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to GetInfo in ReadFile.", status);
#endif
		return status;
	}

	// 分配存放文件的内存空间
	UINTN pageNum = (fileInfo->FileSize >> 12) + 1;
	status = g_EfiBootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, pageNum, address);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to AllocatePages in ReadFile.", status);
#endif
		return status;
	}

	// 读取文件至指定内存地址
	UINTN fileSize = fileInfo->FileSize;
	status = file->Read(file, &fileSize, (VOID*)address);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[File]ERROR %d: Failed to Read FileSize in ReadFile.", status);
#endif
		return status;
	}

	// 释放之前分配的内存空间
	g_EfiBootServices->FreePool(fileInfo);

	return status;
}