/**
 * @file: kernel/cpu.c
 * @author: LinhengXilan
 * @data: 2025-9-26
 * @version: build15
 **/

#include <kernel/cpu.h>
#include <kernel/init.h>
#include <kernel/printk.h>

void init_cpu()
{
	u32 cpuFactoryName[4] = {0};
	char buffer[20] = {0};
	// 获取cpu厂商名
	get_cpuid(0, 0, &cpuFactoryName[0], &cpuFactoryName[1], &cpuFactoryName[2], &cpuFactoryName[3]);
	*(u32*)&buffer[0] = cpuFactoryName[1];
	*(u32*)&buffer[4] = cpuFactoryName[3];
	*(u32*)&buffer[8] = cpuFactoryName[2];
	buffer[12] = '\0';
	printk("%s\n", buffer);
	// 获取cpu型号
	for (int i = 0x80000002; i < 0x80000005; i++)
	{
		get_cpuid(i, 0, &cpuFactoryName[0], &cpuFactoryName[1], &cpuFactoryName[2], &cpuFactoryName[3]);
		*(u32*)&buffer[0] = cpuFactoryName[0];
		*(u32*)&buffer[4] = cpuFactoryName[1];
		*(u32*)&buffer[8] = cpuFactoryName[2];
		*(u32*)&buffer[12] = cpuFactoryName[3];
		buffer[16] = '\0';
		printk("%s", buffer);
	}
	printk("\n");
	// 获取cpu版本信息
	get_cpuid(1, 0, &cpuFactoryName[0], &cpuFactoryName[1], &cpuFactoryName[2], &cpuFactoryName[3]);
	printk(
		"Family :0x%4x, Extended Family:0x%4x, Model:0x%4x, Extended Model:0x%4x, Processor Type:0x%4x, Stepping ID:0x%4x\n",
		cpuFactoryName[0] >> 8 & 0xF,
		cpuFactoryName[0] >> 20 & 0xFF,
		cpuFactoryName[0] >> 4 & 0xF,
		cpuFactoryName[0] >> 16 & 0xF,
		cpuFactoryName[0] >> 12 & 0x3,
		cpuFactoryName[0] & 0xF
	);
	// 获取cpu功能
	printk("cpus:%d\n", cpuFactoryName[1] >> 8 & 0xFF);
	printk("ECX:%x, EDX:%x\n", cpuFactoryName[2], cpuFactoryName[3]);
	// 获取线性/物理地址大小
	get_cpuid(0x80000008, 0, &cpuFactoryName[0], &cpuFactoryName[1], &cpuFactoryName[2], &cpuFactoryName[3]);
	printk("Physical address size:%d, Linear address size:%d\n", cpuFactoryName[0] & 0xFF, cpuFactoryName[0] >> 8 & 0xFF);
	// 获取最大cpuid功能号
	get_cpuid(0, 0, &cpuFactoryName[0], &cpuFactoryName[1], &cpuFactoryName[2], &cpuFactoryName[3]);
	printk("Max basic operation code:0x%x\n", cpuFactoryName[0]);
	get_cpuid(0x80000000, 0, &cpuFactoryName[0], &cpuFactoryName[1], &cpuFactoryName[2], &cpuFactoryName[3]);
	printk("Max extended operation code:0x%x\n", cpuFactoryName[0]);
}
