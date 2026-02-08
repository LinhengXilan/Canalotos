/**
 * @file Kernel.cpp
 * @author LinhengXilan
 * @version 0.0.0.3
 * @date 2026-2-9
 */

#include <Boot.h>

extern "C" {
	uint64_t Kernel(EFIData efiData);
}

uint64_t Kernel(EFIData efiData)
{
	return efiData.Graphics.HorizontalResolution;
}