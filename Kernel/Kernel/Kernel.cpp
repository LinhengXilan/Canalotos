/**
 * @file Kernel/Kernel.cpp
 * @author LinhengXilan
 * @version 0.0.1.9
 * @date 2026-2-28
 */

#include <Boot.h>
#include <Graphics/Graphics.h>
#include <Shell/Shell.h>
#include <Memory/Memory.h>

extern "C" {
	uint64_t Kernel(const EFIData* efiData);
}

uint64_t Kernel(const EFIData* efiData)
{
	Graphics graphics{efiData->Graphics};
	Shell shell{&graphics, 0xFF0D1117, 0xFFFFCCDD};
	MemoryDescriptor memoryDescriptor{efiData->Memory};
	MemoryPageDescriptor* page = memoryDescriptor.GetPageDescriptor();

	shell.Print("0x%lx\n", page);
	shell.Print("Address\tType\tAttitude\n");
	for (int i = 2200; i < 2280; ++i)
	{
		shell.Print("%lx\t%d\t%d\n", page[i].Address, page[i].Type, page[i].Attitude);
	}

	while (true)
	{

	}
	return 0;
}