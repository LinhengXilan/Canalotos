/**
 * @file: include/kernel/cpu.h
 * @author: LinhengXilan
 * @data: 2025-9-26
 * @version: build15
 **/

#ifndef _KERNEL_CPU_H_
#define _KERNEL_CPU_H_

#include <kernel/types.h>

#define NR_CPUS 8

// msr
#define IA32_EFER			0xC0000080
#define IA32_SYSENTER_CS	0x174
#define IA32_SYSENTER_ESP	0x175
#define IA32_SYSENTER_EIP	0x176

/**
 * @param[in] mainFunction 主功能号
 * @param[in] slaveFunction 子功能号
 * @param[out] out1 输出1
 * @param[out] out2 输出2
 * @param[out] out3 输出3
 * @param[out] out4 输出4
 * @brief 获取cpu信息
 */
inline void get_cpuid(u32 mainFunction, u32 slaveFunction, u32* out1, u32* out2, u32* out3, u32* out4)
{
	__asm__ __volatile__(
		"	cpuid	\n\t"
		: "=a"(*out1), "=b"(*out2), "=c"(*out3), "=d"(*out4)
		: "0"(mainFunction), "2"(slaveFunction)
	);
}

#endif