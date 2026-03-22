/**
 * @file Kernel/Kernel.cpp
 * @author LinhengXilan
 * @version 0.0.1.10
 * @date 2026-3-22
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
	Shell::Init(&graphics, 0xFF0D1117, 0xFFFFCCDD);
	Memory::MemoryDescriptor::Init(efiData->Memory);
	Memory::MemoryPageDescriptor* page = Memory::MemoryDescriptor::GetPageDescriptor();
	Shell::Print("0x%lx\n", page);
	Shell::Print("Address\t\tType\tAttitude\n");
	for (int i = 2200; i < 2280; ++i)
	{
		Shell::Print("%lx\t\t%d\t\t%d\n", page[i].Address, page[i].Type, page[i].Attitude);
	}

	while (true)
	{

	}
	return 0;
}