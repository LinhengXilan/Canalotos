/**
 * @file KernelData.h
 * @version 0.0.2.12
 * @author LinhengXilan
 * @date 2026-2-16
 *
 * @brief 用于定义向内核传递的数据
 */

#ifndef BOOT_H
#define BOOT_H

#include <Uefi.h>
#include <Library/UefiLib.h>

typedef struct
{
	EFI_PHYSICAL_ADDRESS FrameBufferBase;
	UINT64 FrameBufferSize;
	UINT32 HorizontalResolution;
	UINT32 VerticalResolution;
}EFI_DATA_GRAPHICS;

typedef struct
{
	UINTN Size;
	EFI_MEMORY_DESCRIPTOR* Buffer;
	UINTN MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
}EFI_DATA_MEMORY;

typedef struct
{
	EFI_DATA_GRAPHICS Graphics;
	EFI_DATA_MEMORY Memory;
}EFI_DATA;

#endif