/**
 * @file Include/Boot.h
 * @author LinhengXilan
 * @version 0.0.1.9
 * @date 2026-2-28
 */

#ifndef __BOOT_H__
#define __BOOT_H__

#include <Type.h>

struct EFIDataGraphics
{
	address_t FrameBufferBase;
	uint64_t FrameBufferSize;
	uint32_t HorizontalResolution;
	uint32_t VerticalResolution;
};

enum class EFIMemoryType : uint32_t
{
	EfiReservedMemoryType,
	EfiLoaderCode,
	EfiLoaderData,
	EfiBootServicesCode,	// 可用
	EfiBootServicesData,	// 可用
	EfiRuntimeServicesCode,	// UEFI和ACPI S1~3
	EfiRuntimeServicesData,	// UEFI和ACPI S1~3
	EfiConventionalMemory,	// 可用
	EfiUnusableMemory,	// 错误内存
	EfiACPIReclaimMemory,	// ACPI
	EfiACPIMemoryNVS,	// ACPI S1~3
	EfiMemoryMappedIO,	// IO
	EfiMemoryMappedIOPortSpace,	// IO
	EfiPalCode,	// UEFI和ACPI S1~S4
	EfiPersistentMemory,	// 可用，特殊
	EfiUnacceptedMemoryType,
	EfiMaxMemoryType
};

struct EFIMemoryMap
{
	EFIMemoryType Type;
	uint32_t Reserved0;
	address_t PhysicalStart;
	address_t VirtualStart;
	uint64_t NumberOfPages;
	uint64_t Attribute;
	uint64_t Reserved1;
};

struct EFIDataMemory
{
	uint64_t MemoryMapSize;
	EFIMemoryMap* MemoryMap;
	uint64_t MapKey;
	uint64_t DescriptorSize;
	uint32_t DescriptorVersion;
};

struct EFIData
{
	EFIDataGraphics Graphics;
	EFIDataMemory Memory;
};

#endif