/**
 * @file Kernel.lds
 * @author LinhengXilan
 * @version 0.0.1.9
 * @date 2026-2-28
 */

OUTPUT_FORMAT("binary", "binary", "binary")
OUTPUT_ARCH(i386:x86-64)
ENTRY(Kernel)
SECTIONS
{
	. = 0;
	.text :
	{
		_Text = .;
		*(.text)
		_EndText = .;
	}
	. = ALIGN(8);
	.data :
	{
		_Data = .;
		*(.data)
		_EndData = .;
	}
	_End = .;
}