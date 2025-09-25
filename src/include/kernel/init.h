/**
 * @file: include/kernel/init.h
 * @author: LinhengXilan
 * @data: 2025-9-26
 * @version: build15
 **/

#ifndef _KERNEL_INIT_H
#define _KERNEL_INIT_H

void init_IDT();
void init_cpu();
void init_screen();
void init_memory();
void init_8259A();
void init_keyboard();
void init_process();

#endif