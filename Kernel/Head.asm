; @file Head.asm
; @author LinhengXilan
; @version 0.0.0.1
; @date 2026-2-7

BITS 64

	mov     ecx, 0x001FA400 ; 1920*1080
	mov     eax, 0xFF557799
	xor     rdi, rdi
	mov     rdi, 0xC0000000

Write:
	mov     [rdi], eax
	add     rdi, 4
	loop    Write

	jmp     $