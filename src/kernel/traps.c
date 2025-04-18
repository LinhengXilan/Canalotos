/**
 * @file: kernel/traps.c
 * @author: lhxl
 * @data: 2025-4-17
 * @version: build7
 **/

#include "kernel/function.h"
#include "kernel/types.h"
#include "kernel/global.h"
#include "kernel/printk.h"
#include "kernel/const.h"
#include "kernel/init.h"

void divide_error();
void debug();
void nmi();
void breakpoint();
void overflow();
void bounds();
void undefined_opcode();
void device_not_available();
void double_fault();
void coprocessor_segment_overrun();
void invalid_TSS();
void segment_not_present();
void stack_segment_fault();
void general_protection();
void page_fault();
void x87_FPU_error();
void alignment_check();
void machine_check();
void SIMD_exception();
void virtualization_exception();

PRIVATE void init_IDT_DESC(u8 IDTIndex, u8 gateType, u8 privilege, int_handler handler);

void init_IDT()
{
	init_IDT_DESC(0x00, DA_386IGate, DA_DPL0, divide_error);
	init_IDT_DESC(0x01, DA_386IGate, DA_DPL0, debug);
	init_IDT_DESC(0x02, DA_386IGate, DA_DPL0, nmi);
	init_IDT_DESC(0x03, DA_386IGate, DA_DPL3, breakpoint);
	init_IDT_DESC(0x04, DA_386IGate, DA_DPL3, overflow);
	init_IDT_DESC(0x05, DA_386IGate, DA_DPL3, bounds);
	init_IDT_DESC(0x06, DA_386IGate, DA_DPL0, undefined_opcode);
	init_IDT_DESC(0x07, DA_386IGate, DA_DPL0, device_not_available);
	init_IDT_DESC(0x08, DA_386IGate, DA_DPL0, double_fault);
	init_IDT_DESC(0x09, DA_386IGate, DA_DPL0, coprocessor_segment_overrun);
	init_IDT_DESC(0x0A, DA_386IGate, DA_DPL0, invalid_TSS);
	init_IDT_DESC(0x0B, DA_386IGate, DA_DPL0, segment_not_present);
	init_IDT_DESC(0x0C, DA_386IGate, DA_DPL0, stack_segment_fault);
	init_IDT_DESC(0x0D, DA_386IGate, DA_DPL0, general_protection);
	init_IDT_DESC(0x0E, DA_386IGate, DA_DPL0, page_fault);
	init_IDT_DESC(0x10, DA_386IGate, DA_DPL0, x87_FPU_error);
	init_IDT_DESC(0x11, DA_386IGate, DA_DPL0, alignment_check);
	init_IDT_DESC(0x12, DA_386IGate, DA_DPL0, machine_check);
	init_IDT_DESC(0x13, DA_386IGate, DA_DPL0, SIMD_exception);
	init_IDT_DESC(0x14, DA_386IGate, DA_DPL0, virtualization_exception);
}

void do_divide_error(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "divide_error, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_debug(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "debug, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_nmi(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "nmi, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_breakpoint(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "breakpoint, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_overflow(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "overflow, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_bounds(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "bounds, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_undefined_opcode(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "undefined_opcode, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_device_not_available(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "device_not_available, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_double_fault(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "double_fault, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_coprocessor_segment_overrun(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "coprocessor_segment_overrun, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_invalid_TSS(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "invalid_tss, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	if(error_code & 0x01)
	{
		__color_printk(RED, BLACK, "The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");
	}
	if(error_code & 0x02)
	{
		__color_printk(RED, BLACK, "Refers to a gate descriptor in the IDT;\n");
	}
	else
	{
		__color_printk(RED, BLACK, "Refers to a descriptor in the GDT or the current LDT;\n");
	}
	if((error_code & 0x02) == 0)
	{
		if(error_code & 0x04)
		{
			__color_printk(RED, BLACK, "Refers to a segment or gate descriptor in the LDT;\n");
		}
		else
		{
			__color_printk(RED, BLACK, "Refers to a descriptor in the current GDT;\n");
		}
	}
	__color_printk(RED, BLACK, "Segment Selector Index:%x\n", error_code & 0xfff8);
	while(1);
}

void do_segment_not_present(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "segment_not_present,ERROR_CODE:%lx,RSP:%lx,RIP:%lx\n", error_code, rsp, *p);

	if(error_code & 0x01)
	{
		__color_printk(RED, BLACK, "The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");
	}
	if(error_code & 0x02)
	{
		__color_printk(RED, BLACK, "Refers to a gate descriptor in the IDT;\n");
	}
	else
	{
		__color_printk(RED, BLACK, "Refers to a descriptor in the GDT or the current LDT;\n");
	}
	if((error_code & 0x02) == 0)
	{
		if(error_code & 0x04)
		{
			__color_printk(RED, BLACK, "Refers to a segment or gate descriptor in the LDT;\n");
		}
		else
		{
			__color_printk(RED, BLACK, "Refers to a descriptor in the current GDT;\n");
		}
	}
	__color_printk(RED, BLACK, "Segment Selector Index:%#010x\n",error_code & 0xfff8);
	while(1);
}

void do_stack_segment_fault(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "stack_segment_fault,ERROR_CODE:%lx,RSP:%lx,RIP:%lx\n", error_code, rsp, *p);

	if(error_code & 0x01)
	{
		__color_printk(RED, BLACK, "The exception occurred during delivery of an event external to the program.\n");
	}
	if(error_code & 0x02)
	{
		__color_printk(RED, BLACK, "Refers to a gate descriptor in the IDT;\n");
	}
	else
	{
		__color_printk(RED, BLACK, "Refers to a descriptor in the GDT or the current LDT;\n");
	}

	if((error_code & 0x02) == 0)
	{
		if(error_code & 0x04)
		{
			__color_printk(RED, BLACK, "Refers to a segment or gate descriptor in the LDT;\n");
		}
		else
		{
			__color_printk(RED, BLACK, "Refers to a descriptor in the current GDT;\n");
		}
	}
	__color_printk(RED, BLACK, "Segment Selector Index:%x\n", error_code & 0xfff8);
	while(1);
}

void do_general_protection(u64 rsp,u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "general_protection, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	if(error_code & 0x01)
	{
		__color_printk(RED, BLACK, "The exception occurred during delivery of an event external to the program.\n");
	}
	if(error_code & 0x02)
	{
		__color_printk(RED, BLACK, "Refers to a gate descriptor in the IDT;\n");
	}
	else
	{
		__color_printk(RED, BLACK, "Refers to a descriptor in the GDT or the current LDT;\n");
	}
	if((error_code & 0x02) == 0)
	{
		if(error_code & 0x04)
		{
			__color_printk(RED, BLACK, "Refers to a segment or gate descriptor in the LDT;\n");
		}
		else
		{
			__color_printk(RED, BLACK, "Refers to a descriptor in the current GDT;\n");
		}
	}
	__color_printk(RED, BLACK, "Segment Selector Index:%x\n",error_code & 0xfff8);
	while(1);
}

void do_page_fault(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	u64 cr2 = 0;
	__asm__	__volatile__("movq	%%cr2,	%0":"=r"(cr2)::"memory");
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "page_fault, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	if(!(error_code & 0x01))
	{
		__color_printk(RED, BLACK, "Page Not-Present,\t");
	}
	if(error_code & 0x02)
	{
		__color_printk(RED, BLACK, "Write Cause Fault,\t");
	}
	else
	{
		__color_printk(RED, BLACK, "Read Cause Fault,\t");
	}
	if(error_code & 0x04)
	{
		__color_printk(RED, BLACK, "Fault in user(3)\t");
	}
	else
	{
		__color_printk(RED, BLACK, "Fault in supervisor(0,1,2)\t");
	}
	if(error_code & 0x08)
	{
		__color_printk(RED, BLACK, ",Reserved Bit Cause Fault\t");
	}
	if(error_code & 0x10)
	{
		__color_printk(RED, BLACK, ",Instruction fetch Cause Fault");
	}
	__color_printk(RED, BLACK, "\n");
	__color_printk(RED, BLACK, "CR2:%lx\n",cr2);
	while(1);
}

void do_coprocessor_error(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "coprocessor_error, ERROR_CODE:%lx, RSP:%x, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_x87_FPU_error(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "x87_FPU_error, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_alignment_check(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "alignment_check, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_machine_check(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "machine_check, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_SIMD_exception(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "SIMD_exception, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

void do_virtualization_exception(u64 rsp, u64 error_code)
{
	u64* p = nullptr;
	p = (u64*)(rsp + 0x98);
	__color_printk(RED, BLACK, "virtualization_exception, ERROR_CODE:%lx, RSP:%lx, RIP:%lx\n", error_code, rsp, *p);
	while(1);
}

PRIVATE void init_IDT_DESC(u8 IDTIndex, u8 gateType, u8 privilege, int_handler handler)
{
	struct Gate* p_gate = &IDT[IDTIndex];
	u64 base = (u64)handler;
	p_gate->offsetLow = base & 0xFFFF;
	p_gate->selector = 0x8;
	p_gate->paramCount = 0;
	p_gate->attribute = gateType | privilege;
	p_gate->offsetMid = base >> 16 & 0xFFFF;
	p_gate->offsetHigh = base >> 32 & 0xFFFFFFFF;
	p_gate->zero = 0;
}