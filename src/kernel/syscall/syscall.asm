; @file: kernel/syscall/syscall.asm
; @author: LinhengXilan
; @data: 2025-8-18
; @version: build13

global system_call

extern do_syscall

[section .text]
[bits 64]
system_call:
	sti
	sub     rsp, 0x38
	cld
	push    rax
	mov     rax, es
	push    rax
	mov     rax, ds
	push    rax
	xor     rax, rax
	push    rbp
	push    rdi
	push    rsi
	push    rdx
	push    rcx
	push    rbx
	push    r8
	push    r9
	push    r10
	push    r11
	push    r12
	push    r13
	push    r14
	push    r15
	mov     rdx, 0x10
	mov     ds, rdx
	mov     es, rdx
	mov     rdi, rsp
	call    do_syscall
	mov     [rsp + 0x80], rax
	pop     r15
	pop     r14
	pop     r13
	pop     r12
	pop     r11
	pop     r10
	pop     r9
	pop     r8
	pop     rbx
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    pop     rbp
    pop     rax
    mov     ds, rax
    pop     rax
    mov     es, rax
    pop     rax
    add     rsp, 0x38
    db      0x48
    sysexit