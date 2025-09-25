/**
 * @file: include/kernel/const.h
 * @author: LinhengXilan
 * @data: 2025-9-26
 * @version: build15
 **/

#ifndef _KERNEL_CONST_H_
#define _KERNEL_CONST_H_

#define PUBLIC
#define PRIVATE static

// boolean
#define TRUE 1
#define FALSE 0
#define NULL 0

#define LINEAR_MEMORY_BASE	0xFFFF800000000000

// color
#define BLACK	0x00000000
#define BLUE	0x000000FF
#define GREEN	0x0000FF00
#define INDIGO	0x004B0082
#define RED		0x00FF0000
#define ORANGE	0x00FF8000
#define WHITE	0x00FFFFFF

// irq
#define NR_IRQ	16
#define IRQ_KEYBOARD	1

// process
#define PROCESS_STACK_SIZE 32768
#define SELECTOR_KERNEL_CS 0x08
#define SELECTOR_KERNEL_DS 0x10
#define NR_PROCESSES 0x1

#endif