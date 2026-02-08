/**
 * @file Boot.h
 * @version 0.0.1.10
 * @author LinhengXilan
 * @date 2026-2-9
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
	EFI_DATA_GRAPHICS Graphics;
}EFI_DATA;

#endif