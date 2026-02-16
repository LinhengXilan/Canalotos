/**
 * @file Kernel/Kernel.cpp
 * @author LinhengXilan
 * @version 0.0.1.8
 * @date 2026-2-16
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
	shell.Print("MemoryMapKey: %d\n", efiData->Memory.MapKey);

	EFIMemoryMap* memoryMap = efiData->Memory.MemoryMap;
	shell.Print("Type\tVirtualStart\tNumberOfPages\tAttribute\n");
	for (int i = 0; i < 100; ++i, ++memoryMap)
	{
		shell.Print("0x%x\t\t0x%8lx\t\t0x%8lx\t\t0x%lx\n", memoryMap->Type, memoryMap->PhysicalStart, memoryMap->NumberOfPages, memoryMap->Attribute);
	}

	while (true)
	{

	}
	return 0;
}