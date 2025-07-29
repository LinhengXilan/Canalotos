/**
 * @file: kernel/irq/8295A.c
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#include <kernel/lib/lib.h>
#include <kernel/init.h>
#include <kernel/const.h>
#include <kernel/global.h>
#include <kernel/printk.h>
#include <kernel/interrupt.h>

void init_8259A()
{
	_color_printk(ORANGE, BLACK, "init_8259A()\n");
	_port_write_byte(0x20, 0x11);	// master.ICW1
	_port_write_byte(0x21, 0x20);	// master.ICW2
	_port_write_byte(0x21, 0x4);	// master.ICW3
	_port_write_byte(0x21, 0x1);	// master.ICW4
	_port_write_byte(0xA0, 0x11);	// slaver.ICW1
	_port_write_byte(0xA1, 0x28);	// slaver.ICW2
	_port_write_byte(0xA1, 0x2);	// slaver.ICW3
	_port_write_byte(0xA1, 0x1);	// slaver.ICW4
	_port_write_byte(0x21, 0xFF);	// master.OCW1
	_port_write_byte(0xA1, 0xFF);	// slaver.OCW1
	for (int i = 0; i < NR_IRQ; i++)
	{
		irq_table[i] = default_irq_handler;
	}
}

void default_irq_handler()
{
	_printk("irq!\n");
}

void _put_irq_handler(int irq, irq_handler handler)
{
	_disable_irq(irq);
	irq_table[irq] = handler;
}