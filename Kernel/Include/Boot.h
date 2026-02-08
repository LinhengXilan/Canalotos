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
	size_t FrameBufferSize;
	uint32_t HorizontalResolution;
	uint32_t VerticalResolution;
};

struct EFIData
{
	EFIDataGraphics Graphics;
};

#endif