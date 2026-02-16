/**
 * @file Memory.c
 * @version 0.0.2.12
 * @author LinhengXilan
 * @date 2026-2-16
 */

#include <Memory.h>
#include <Config.h>

EFI_STATUS GetEFIDataMemory(EFI_SYSTEM_TABLE* systemTable, EFI_DATA_MEMORY* memory)
{
	EFI_STATUS status = EFI_SUCCESS;
	EFI_BOOT_SERVICES* bootServices = systemTable->BootServices;

	memory->Size = 0;
	memory->Buffer = nullptr;
	memory->MapKey = 0;
	memory->DescriptorSize = 0;
	memory->DescriptorVersion = 0;

	status = bootServices->GetMemoryMap(&memory->Size, memory->Buffer, &memory->MapKey, &memory->DescriptorSize, &memory->DescriptorVersion);
	if (status == EFI_BUFFER_TOO_SMALL)
	{
#ifdef DEBUG
		Print(L"[Memory]Error %d: Failed to GetMemoryMap because EFI_BUFFER_TOO_SMALL\n", status);
#endif
	}
	status = bootServices->AllocatePool(EfiLoaderData, memory->Size + 1, (VOID**)&memory->Buffer);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[Memory]Error %d: Failed to AllocatePage\n", status);
#endif
		return status;
	}

	status = bootServices->GetMemoryMap(&memory->Size, memory->Buffer, &memory->MapKey, &memory->DescriptorSize, &memory->DescriptorVersion);
	if (EFI_ERROR(status))
	{
#ifdef DEBUG
		Print(L"[Memory]Error %d: Failed to GetMemoryMap\n", status);
#endif
		return status;
	}

	return status;
}
