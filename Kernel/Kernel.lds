/**
 * @file Kernel.lds
 * @author LinhengXilan
 * @version 0.0.0.2
 * @date 2026-2-8
 */

OUTPUT_FORMAT("binary", "binary", "binary")
OUTPUT_ARCH(i386:x86-64)
ENTRY(Kernel)
SECTIONS
{
	. = 0;
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
	_end = .;
}