/**
 * @file Include/Boot.h
 * @author LinhengXilan
 * @version 0.0.0.3
 * @date 2026-2-9
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

struct EFIMemoryMap
{
	uint32_t Type;
	uint32_t Reserved0;
	address_t PhysicalStart;
	address_t VirtualStart;
	uint64_t NumberOfPages;
	uint64_t Attribute;
	uint64_t Reserved1;
};

struct EFIDataMemory
{
	uint64_t Size;
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