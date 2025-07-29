; @file: kernel/lib/string.asm
; @author: LinhengXilan
; @data: 2025-7-29
; @version: build11

global memset
global memcpy

[section .text]
[bits 64]
; void memset(void* ptr, char ch, size_t size);
memset:
	push    rcx
	push    rax
	mov     rcx, rdx
    mov     rax, rsi
    cld
    rep     stosb
	pop     rax
	pop     rcx
	ret

memcpy:
	push	rdx
	mov     rcx, rdx
	mov     rdx, rsi
.1:
	cmp     rcx, 0
	jz      .2
	mov     al, [rsi]
	inc     rsi
	mov     byte [ds:rdi], al
	inc     rdi
	dec     rcx
	jmp     .1
.2:
	mov     rax, rdx
	pop     rdx
	ret