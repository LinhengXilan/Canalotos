/**
 * @file Kernel/Kernel.cpp
 * @author LinhengXilan
 * @version 0.0.0.4
 * @date 2026-2-10
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
	shell.PutChar('K');
	shell.PutChar('e');
	shell.PutChar('r');
	shell.PutChar('n');
	shell.PutChar('e');
	shell.PutChar('l');


	while (true)
	{

	}
	return 0;
}