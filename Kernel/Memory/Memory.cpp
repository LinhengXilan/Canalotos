/**
 * @file Include/Memory/Memory.cpp
 * @author LinhengXilan
 * @version 0.0.1.9
 * @date 2026-2-28
 */

#include <Memory/Memory.h>
#include <Boot.h>
#include <Shell/Shell.h>

MemoryDescriptor::MemoryDescriptor(const EFIDataMemory& efiMemory)
	: m_KernelCodeAddress(reinterpret_cast<address_t>(&_Text)), m_KernelDataAddress(reinterpret_cast<address_t>(&_Data))
{
	uint16_t efiMemoryZoneCount = efiMemory.MemoryMapSize / efiMemory.DescriptorSize;

	// 总分页数
	uint32_t PageCount = 0;
	for (uint16_t i = 0; i < efiMemoryZoneCount; ++i)
	{
		PageCount += efiMemory.MemoryMap[i].NumberOfPages;
	}
	m_PageCount = PageCount;

	// 寻找足以容纳页描述符的空间
	uint32_t memoryPageCount = 0;
	EFIMemoryType type = EFIMemoryType::EfiReservedMemoryType;
	address_t startAddress = 0x100000;
	for (uint16_t i = 0; i < efiMemoryZoneCount; ++i)
	{
		// 前1MB内存不用
		if (efiMemory.MemoryMap[i].PhysicalStart + efiMemory.MemoryMap[i].NumberOfPages * 4096 < 0x100000)
		{
			continue;
		}

		type = efiMemory.MemoryMap[i].Type;
		if (type == EFIMemoryType::EfiBootServicesCode || type == EFIMemoryType::EfiBootServicesData || type == EFIMemoryType::EfiConventionalMemory)
		{
		 	if (efiMemory.MemoryMap[i].PhysicalStart < 0x100000)
		 	{
		 		memoryPageCount += efiMemory.MemoryMap[i].NumberOfPages - (0x100000 - efiMemory.MemoryMap[i].PhysicalStart) / 4096;
		 	}
		 	else
		 	{
		 		memoryPageCount += efiMemory.MemoryMap[i].NumberOfPages;
		 	}
		 	if (memoryPageCount >= PageCount * sizeof(MemoryPageDescriptor) / 4096 + 1)
		 	{
		 		break;
		 	}
		}
		else
		{
		 	memoryPageCount = 0;
		 	startAddress = efiMemory.MemoryMap[i + 1].PhysicalStart;
		}
	}

	m_MemoryPageDescriptor = reinterpret_cast<MemoryPageDescriptor*>(startAddress);
	MemoryPageDescriptor* memoryPage = m_MemoryPageDescriptor;

	// 设置每个页的描述符
	for (uint16_t i = 0; i < efiMemoryZoneCount; ++i)
	{
		uint8_t flag = 0;
		switch (efiMemory.MemoryMap[i].Type)
		{
		case EFIMemoryType::EfiReservedMemoryType:
		case EFIMemoryType::EfiUnusableMemory:
		case EFIMemoryType::EfiUnacceptedMemoryType:
		case EFIMemoryType::EfiLoaderCode:
		case EFIMemoryType::EfiLoaderData:
			flag = 0;	// 不可用
			break;
		case EFIMemoryType::EfiBootServicesCode:
		case EFIMemoryType::EfiBootServicesData:
		case EFIMemoryType::EfiConventionalMemory:
		case EFIMemoryType::EfiPersistentMemory:
			flag = 1;	// 可用
			break;
		case EFIMemoryType::EfiRuntimeServicesCode:
		case EFIMemoryType::EfiRuntimeServicesData:
		case EFIMemoryType::EfiACPIReclaimMemory:
		case EFIMemoryType::EfiACPIMemoryNVS:
		case EFIMemoryType::EfiMemoryMappedIO:
		case EFIMemoryType::EfiMemoryMappedIOPortSpace:
		case EFIMemoryType::EfiPalCode:
			flag = 2;	// UEFI
			break;
		default:
			flag = 0;
			break;
		}

		for (uint32_t j = 0; j < efiMemory.MemoryMap[i].NumberOfPages; ++j, ++memoryPage)
		{
			memoryPage->Address = efiMemory.MemoryMap[i].PhysicalStart + j * 4096;
			memoryPage->Type =
				  flag == 0 || efiMemory.MemoryMap[i].PhysicalStart + j * 4096 < 0x100000 ? MemoryPageType::Unusable
				: flag == 1 ? MemoryPageType::System
				: flag == 2 ? MemoryPageType::UEFI
				: MemoryPageType::Unusable;
			memoryPage->Attitude = MemoryPageAttitude::None;
		}
	}
}

MemoryPageDescriptor* MemoryDescriptor::GetPageDescriptor() const
{
	return m_MemoryPageDescriptor;
}

MemoryPageDescriptor* MemoryDescriptor::GetPageDescriptor(uint32_t index) const
{
	if (index > m_PageCount)
	{
		return nullptr;
	}
	return m_MemoryPageDescriptor + index * sizeof(MemoryPageDescriptor);
}