/**
 * @file Memory.c
 * @author LinhengXilan
 * @version 0.0.2.13
 * @date 2026-3-20
 */

#include <Memory.h>
#include <Config.h>
#include <Error.h>

EFI_STATUS GetEFIDataMemory(EFI_SYSTEM_TABLE* systemTable, EFI_DATA_MEMORY* memory)
{
	EFI_STATUS status = EFI_SUCCESS;
	EFI_BOOT_SERVICES* bootServices = systemTable->BootServices;

	memory->Size = 0;
	memory->Buffer = nullptr;
	memory->MapKey = 0;
	memory->DescriptorSize = 0;
	memory->DescriptorVersion = 0;

	// 第一次调用GetMemoryMap获取所需大小
	status = bootServices->GetMemoryMap(&memory->Size, memory->Buffer, &memory->MapKey, &memory->DescriptorSize, &memory->DescriptorVersion);
	status = bootServices->AllocatePool(EfiLoaderData, memory->Size + 1, (VOID**)&memory->Buffer);
	CANALOTOS_ERROR_MESSAGE(status, "[Memory]", "Failed to AllocatePool");

	status = bootServices->GetMemoryMap(&memory->Size, memory->Buffer, &memory->MapKey, &memory->DescriptorSize, &memory->DescriptorVersion);
	CANALOTOS_ERROR_MESSAGE(status, "[Memory]", "Failed to GetMemoryMap");

	return status;
}
