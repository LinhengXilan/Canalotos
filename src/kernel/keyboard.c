/**
 * @file: kernel/keyboard.c
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#include <kernel/init.h>
#include <kernel/lib/lib.h>
#include <kernel/const.h>
#include <kernel/interrupt.h>
#include <kernel/keyboard.h>
#include <kernel/keymap.h>
#include <kernel/printk.h>

#define KBD_DATA	0x60
#define KBD_CMD		0x64

PRIVATE struct Keyboard kbd;

PRIVATE u8 E0;
PRIVATE u8 shift_l;
PRIVATE u8 shift_r;
PRIVATE u8 alt_l;
PRIVATE u8 alt_r;
PRIVATE u8 ctrl_l;
PRIVATE u8 ctrl_r;
PRIVATE u8 caps_lock;
PRIVATE u8 num_lock;
PRIVATE u8 scroll_lock;
PRIVATE u8 column;

void init_keyboard()
{
	_color_printk(ORANGE, BLACK, "init_keyboard()\n");
	_put_irq_handler(IRQ_KEYBOARD, keyboard_handler);
	_enable_irq(IRQ_KEYBOARD);

	E0 = 0;
	shift_l = shift_r = 0;
	alt_l = alt_r = 0;
	ctrl_l = ctrl_r = 0;
	caps_lock = num_lock = scroll_lock = 0;
	column = 0;
	kbd.count = 0;
	kbd.p_head = kbd.p_tail = kbd.buffer;
}

void keyboard_handler()
{
	const u8 scan_code = _port_read_byte(KBD_DATA);
	if (kbd.count < KBD_BUFFER_SIZE)
	{
		*kbd.p_head = scan_code;
		kbd.p_head++;
		if (kbd.p_head == kbd.buffer + KBD_BUFFER_SIZE)
		{
			kbd.p_head = kbd.buffer;
		}
		kbd.count++;
	}
}

void keyboard_read()
{

}