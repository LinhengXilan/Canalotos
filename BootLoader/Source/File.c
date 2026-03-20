/**
 * @file File.c
 * @author LinhengXilan
 * @version 0.0.2.13
 * @date 2026-3-20
 */

#include <Library/UefiLib.h>
#include <Guid/FileInfo.h>

#include <File.h>
#include <Error.h>

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* g_EfiSimpleFileSystemProtocol = nullptr;

EFI_STATUS InitFile(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	EFI_STATUS status = EFI_SUCCESS;

	EFI_BOOT_SERVICES* efiBootServices = systemTable->BootServices;

	UINTN handleCount = 0;
	EFI_HANDLE* handles = nullptr;
	status = efiBootServices->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, nullptr, &handleCount, &handles);
	CANALOTOS_ERROR_MESSAGE(status, "[File]", "Failed to Located EfiSimpleFileSystemProtocol");

	status = efiBootServices->OpenProtocol(
		handles[0],
		&gEfiSimpleFileSystemProtocolGuid,
		(VOID**)&g_EfiSimpleFileSystemProtocol,
		imageHandle,
		nullptr,
		EFI_OPEN_PROTOCOL_GET_PROTOCOL
	);
	CANALOTOS_ERROR_MESSAGE(status, "[File]", "Failed to Open EfiSimpleFileSystemProtocol");

	return status;
}

EFI_STATUS GetFileHandle(CHAR16* fileName, EFI_FILE_PROTOCOL** file)
{
	EFI_STATUS status = EFI_SUCCESS;

	// 打开文件系统的卷
	EFI_FILE_PROTOCOL* root = nullptr;
	status = g_EfiSimpleFileSystemProtocol->OpenVolume(g_EfiSimpleFileSystemProtocol, &root);
	CANALOTOS_ERROR_MESSAGE(status, "[File]", "Failed to OpenVolume in GetFileHandle");

	status = root->Open(root, file, fileName, EFI_FILE_MODE_READ, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
	CANALOTOS_ERROR_MESSAGE(status, "[File]", "Failed to Open File \"%s\"", fileName);

	return status;
}

EFI_STATUS ReadFile(EFI_SYSTEM_TABLE* systemTable, EFI_FILE_PROTOCOL* file, EFI_PHYSICAL_ADDRESS* address)
{
	EFI_STATUS status = EFI_SUCCESS;
	EFI_BOOT_SERVICES* efiBootServices = systemTable->BootServices;
	// 分配内存空间
	EFI_FILE_INFO* fileInfo = nullptr;
	UINTN fileInfoSize = sizeof(EFI_FILE_INFO) + 16;	// +16是因为下面的读取信息不止读了EFI_FILE_INFO，还有文件名。
	status = efiBootServices->AllocatePool(EfiLoaderData, fileInfoSize, (VOID**)&fileInfo);
	CANALOTOS_ERROR_MESSAGE(status, "[File]", "Failed to AllocatePool in ReadFile");

	// 读取文件信息
	status = file->GetInfo(file, &gEfiFileInfoGuid, &fileInfoSize, fileInfo);
	CANALOTOS_ERROR_MESSAGE(status, "[File]", "Failed to GetInfo in ReadFile");

	// 分配存放文件的内存空间
	EFI_PHYSICAL_ADDRESS tempAddress;
	UINTN pageNum = (fileInfo->FileSize >> 12) + 1;
	status = efiBootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, pageNum, &tempAddress);
	CANALOTOS_ERROR_MESSAGE(status, "[File]", "Failed to AllocatePages in ReadFile");

	// 读取文件至指定内存地址
	status = file->Read(file, &fileInfo->FileSize, (VOID*)tempAddress);
	CANALOTOS_ERROR_MESSAGE(status, "[File]", "Failed to Read in ReadFile");

	*address = tempAddress;
	// 释放之前分配的内存空间
	status = efiBootServices->FreePool(fileInfo);
	CANALOTOS_ERROR_MESSAGE(status, "[File]", "Failed to FreePool in ReadFile");

	return status;
}