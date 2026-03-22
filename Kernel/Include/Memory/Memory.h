/**
 * @file Include/Memory/Memory.h
 * @author LinhengXilan
 * @version 0.0.1.10
 * @date 2026-3-22
 */

#ifndef __MEMORY_MEMORY_H__
#define __MEMORY_MEMORY_H__

#include <Type.h>
#include <Boot.h>

extern uint8_t _Text;
extern uint8_t _Data;

namespace Memory
{
	enum class MemoryPageType : uint8_t
	{
		Unusable,
		System, User,
		UEFI
	};

	enum class MemoryPageAttitude : uint8_t
	{
		None
	};

	/**
	 * @brief 4KB内存页
	 */
	struct MemoryPageDescriptor
	{
		address_t Address = 0;
		MemoryPageType Type = MemoryPageType::Unusable;
		MemoryPageAttitude Attitude = MemoryPageAttitude::None;
	};

	/**
	 * @brief 整个可用内存
	 */

	struct MemoryDescriptorData
	{
		MemoryPageDescriptor* memoryPageDescriptor;
		address_t kernelCodeAddress = 0;
		address_t kernelDataAddress = 0;
		uint32_t pageCount = 0;
	};

	namespace MemoryDescriptor
	{
		void Init(const EFIDataMemory& efiMemory);
		MemoryPageDescriptor* GetPageDescriptor();
		MemoryPageDescriptor* GetPageDescriptor(uint32_t index);
	}
}

#endif