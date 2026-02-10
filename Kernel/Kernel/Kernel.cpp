/**
 * @file Kernel/Kernel.cpp
 * @author LinhengXilan
 * @version 0.0.0.4
 * @date 2026-2-10
 */

#include <Boot.h>
#include <Graphics/Graphics.h>

extern "C" {
	uint64_t Kernel(const EFIData* efiData);
}

uint64_t Kernel(const EFIData* efiData)
{
	Graphics graphics{efiData->Graphics};

	for (uint16_t i = 0; i < 100; i++)
	{
		for (uint16_t j = 0; j < 100; j++)
		{
			graphics.DrawPixel(i, j, 0xFFFFCCEE);
		}
	}

	while (true)
	{

	}
	return 0;
}