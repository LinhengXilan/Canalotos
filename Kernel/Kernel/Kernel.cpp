/**
 * @file Kernel/Kernel.cpp
 * @author LinhengXilan
 * @version 0.0.0.7
 * @date 2026-2-14
 */

#include <Boot.h>
#include <Graphics/Graphics.h>
#include <Shell/Shell.h>

extern "C" {
	uint64_t Kernel(const EFIData* efiData);
}

uint64_t Kernel(const EFIData* efiData)
{
	Graphics graphics{efiData->Graphics};
	Shell shell{&graphics, 0xFF0D1117, 0xFFFFCCDD};
	shell.PutString("Kernel");
	shell.Print(": %d", 10);

	while (true)
	{

	}
	return 0;
}