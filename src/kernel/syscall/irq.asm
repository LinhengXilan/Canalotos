; @file: kernel/syscall/irq.asm
; @author: LinhengXilan
; @data: 2025-7-29
; @version: build11

%macro irq_master 1
	call    _save
	in      al, 0x21
	or      al, (1 << %1)
	out     0x21, al
	mov     al, 0x20
	out     0x20, al
	sti
	push    %1
	lea     rax, [rel irq_table + 8 * %1]
	call    rax
	pop     rcx
	cli
	in      al, 0x21
    or      al, ~(1 << %1)
    out     0x21, al
%endmacro

%macro	irq_slave	1
	call    _save
	in      al, 0xA1
	or      al, (1 << (%1 - 8))
	out     0xA1, al
	mov     al, 0x20
	out     0x20, al
	nop
	out     0xA0, al
	sti
	push    %1
	lea     rax, [rel irq_table + 8 * %1]
	call    rax
	pop     rcx
	cli
	in      al, 0xA1
	and     al, ~(1 << (%1 - 8))
	out     0xA1, al
	ret
%endmacro

INT_M_CTLMASK equ 0x21
INT_S_CTLMASK equ 0xA1
EOI equ 0x20

extern irq_table

global _irq00
global _irq01
global _irq02
global _irq03
global _irq04
global _irq05
global _irq06
global _irq07
global _irq08
global _irq09
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

global _enable_irq
global _disable_irq

[section .text]
[bits 64]
_save:
	cld
	push    rax
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

_irq00:  ;
	irq_master 0
_irq01:  ;
	irq_master 1
_irq02:  ; cascade
	irq_master 2
_irq03:  ;
	irq_master 3
_irq04:  ;
	irq_master 4
_irq05:  ;
	irq_master 5
_irq06:  ;
	irq_master 6
_irq07:  ;
	irq_master 7
_irq08:  ;
	irq_slave 8
_irq09:  ; irq 2 redirected
	irq_slave 9
_irq10:  ;
	irq_slave 10
_irq11:  ;
	irq_slave 11
_irq12:  ;
	irq_slave 12
_irq13:  ;
	irq_slave 13
_irq14:  ;
	irq_slave 14
_irq15:  ;
	irq_slave 15

_enable_irq:
	mov     ecx, edi
	push    rax
	cli
	mov     ah, ~1
	rol     ah, cl
	cmp     cl, 8
	jae     .enable_8
.enable_0:
	in      al, INT_M_CTLMASK
	and     al, ah
	out     INT_M_CTLMASK, al
	pop     rax
	ret
.enable_8:
	in      al, INT_S_CTLMASK
	and     al, ah
	out     INT_S_CTLMASK, al
	pop     rax
	ret

_disable_irq:
	mov     cx, di
	push    rax
	cli
	mov     ah, 1
	rol     ah, cl
	cmp     cl, 8
	jae     .disable_8
.disable_0:
	in      al, INT_M_CTLMASK
	or      al, ah
	out     INT_M_CTLMASK, al
	pop     rax
	ret
.disable_8:
	in      al, INT_S_CTLMASK
	or      al, ah
	out     INT_S_CTLMASK, al
	pop     rax
	ret