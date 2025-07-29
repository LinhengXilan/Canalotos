/**
 * @file: include/kernel/init.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KERNEL_INIT_H
#define _KERNEL_INIT_H

void init_screen();
void init_IDT();
void init_memory();
void init_8259A();
void init_keyboard();
void init_process();

#endif