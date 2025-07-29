/**
 * @file: kernel/kernel.lds
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)
SECTIONS
{
	. = 0xFFFF800000100000;
	.text :
	{
		_text = .;
		*(.text)
		_etext = .;
	}
	. = ALIGN(8);
	.data :
	{
		_data = .;
		*(.data)
		_edata = .;
	}
	.rodata :
	{
		_rodata = .;
		*(.rodata)
		_erodata = .;
	}
	. = ALIGN(32768);
	.data._init_process :
	{
		*(.data._init_process)
	}
	.bss :
	{
		_bss = .;
		*(.bss)
		_ebss = .;
	}
	_end = .;
}
