/**
 * @file: include/kernel/interrupt.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KERNEL_TRAPS_H_
#define _KERNEL_TRAPS_H_

void _divide_error();
void _debug();
void _nmi();
void _breakpoint();
void _overflow();
void _bounds();
void _undefined_opcode();
void _device_not_available();
void _double_fault();
void _coprocessor_segment_overrun();
void _invalid_TSS();
void _segment_not_present();
void _stack_segment_fault();
void _general_protection();
void _page_fault();
void _x87_FPU_error();
void _alignment_check();
void _machine_check();
void _SIMD_exception();
void _virtualization_exception();

void _irq00();
void _irq01();
void _irq02();
void _irq03();
void _irq04();
void _irq05();
void _irq06();
void _irq07();
void _irq08();
void _irq09();
void _irq10();
void _irq11();
void _irq12();
void _irq13();
void _irq14();
void _irq15();

void default_irq_handler();
void _put_irq_handler(int irq, irq_handler handler);

#endif