; @file: kernel/process/switch.asm
; @author: LinhengXilan
; @data: 2025-7-29
; @version: build11

extern do_process_exit

global kernel_thread_func

[section .text]
[bits 64]
kernel_thread_func:
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
	mov     rdi, rdx
	call    rbx
	mov     rdi, rax
	call    do_process_exit