; @file: kernel/lib/lib.asm
; @author: LinhengXilan
; @data: 2025-7-29
; @version: build11

global _port_write_byte
global _port_read_byte

[section .text]
[bits 64]
_port_write_byte:
	mov dx, di
	mov ax, si
	out dx, al
	nop
	nop
	ret

_port_read_byte:
	mov     dx, di
	xor     al, al
	in      al, dx
	nop
	nop
	ret