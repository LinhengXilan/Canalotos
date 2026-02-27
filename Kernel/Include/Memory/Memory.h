/**
 * @file Include/Memory/Memory.h
 * @author LinhengXilan
 * @version 0.0.1.9
 * @date 2026-2-28
 */

#ifndef __MEMORY_MEMORY_H__
#define __MEMORY_MEMORY_H__

#include <Type.h>
#include <Boot.h>

extern uint8_t _Text;
extern uint8_t _Data;

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

class MemoryDescriptor
{
public:
	MemoryDescriptor(const EFIDataMemory& efiMemory);
	~MemoryDescriptor() = default;

	MemoryPageDescriptor* GetPageDescriptor() const;
	MemoryPageDescriptor* GetPageDescriptor(uint32_t index) const;

private:
	MemoryPageDescriptor* m_MemoryPageDescriptor;
	address_t m_KernelCodeAddress = 0;
	address_t m_KernelDataAddress = 0;
	uint32_t m_PageCount = 0;
};

#endif