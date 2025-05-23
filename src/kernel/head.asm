; @file: kernel/head.asm
; @author: lhxl
; @data: 2025-5-13
; @version: build10

%include "macro.inc"

extern __kernel_main
extern __color_printk

[section .text]
global __start
__start:
	mov     ax, Selector_Kernel_Data
	mov     ds, ax
	mov     es, ax
	mov     fs, ax
	mov     ss, ax
	mov     esp, 0x7E00
	lgdt    [rel GDT_Pointer]
	lidt    [rel IDT_Pointer]
	mov     ax, Selector_Kernel_Data
	mov     ds, ax
	mov     es, ax
	mov     fs, ax
	mov     gs, ax
	mov     ss, ax
	mov     rsp, 0x7E00
	mov     rax, 0x101000
	mov     cr3, rax
	mov     rax, [rel IEntry]
	push    Selector_Kernel_Code
	push    rax
	db      0x48
	retf
IEntry:
	dq      Entry
Entry:
	mov     rax, Selector_Kernel_Data
	mov     ds, rax
	mov     es, rax
	mov     gs, rax
	mov     ss, rax
	mov     rsp, 0xFFFF800000007E00

; Init the IDT = { 0, offset:16~63, attribute, segment selector, offset:0~15 }.
Init_IDT:
	lea     rdx, [rel ignore_int]
	mov     rax, Selector_Kernel_Code << 16     ; segment selector
	mov     ax, dx                              ; offset 0~15
	mov     rcx, DA_386IGate << 40              ; attribute
	add     rax, rcx
	mov     ecx, edx
	shr     ecx, 16
	shl     rcx, 48
	add     rax, rcx                            ; offset 16~31
	shr     rdx, 32                             ; offset 32~63
	lea     rdi, [rel IDT]
	mov     rcx, 256
; Write IDT into IDT_Table
Load_IDT:
	mov     [rdi], rax
	mov     [rdi + 8], rdx
	add     rdi, 0x10
	dec     rcx
	jne     Load_IDT

; Init the TSS
Init_TSS:
	lea     rdx, [rel TSS]
	xor     rax, rax
	xor     rcx, rcx
	mov     rax, DA_386TSS
	shl     rax, 40             ; attribute 0~3
	mov     ecx, edx
	shr     ecx, 24
	shl     rcx, 56
	add     rax, rcx            ; segment base 24~31
	xor     rcx, rcx
	mov     ecx, edx
	and     ecx, 0xFFFFFF
	shl     rcx, 16
	add     rax, rcx            ; segment base 16~23
	add     rax, 103            ; segment limit(length of TSS)
	mov     qword [rel GDT_TSS0], rax
	shr     rdx, 32
	mov     qword [rel GDT_TSS1], rdx
	; jump to kernel
	mov     rax, [rel IStartKernel]
	push    Selector_Kernel_Code
	push    rax
	db      0x48
	retf

IStartKernel:
	dq      __kernel_main

; The default interrupt handler save all general registers and print message "Unknown Interrupt".
ignore_int:
    cld
    push    rax
    push    rbx
    push    rcx
    push    rdx
    push    rbp
    push    rdi
    push    rsi
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15
    mov     rax, es
    push    rax
    mov     rax, ds
    push    rax
    mov     rax, 0x10
    mov     ds, rax
    mov     es, rax
    lea     rax, [rel MSG_Interrupt]
    push    rax
    mov     rdx, rax
    mov     rsi, 0
    mov     rdi, 0x00FF0000
    mov     rax, 0
    call    __color_printk
    add     rsp, 0x8
Loop:
    jmp     Loop
    pop     rax
    mov     ds, rax
    pop     rax
    mov     es, rax
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rsi
    pop     rdi
    pop     rbp
    pop     rdx
    pop     rcx
    pop     rbx
    pop     rax
    iret

align 8
	times   0x1000 - ($ - $$) db 0
__PML4E:
	dq      0x102007
	resq    255
	dq      0x102007
	resq    255

	times   0x2000 - ($ - $$) db 0
__PDPTE:
	dq      0x103003
	resq    511

	times   0x3000 - ($ - $$) db 0
__PDE:
	dq      0x83
	dq      0x200083
	dq      0x400083
	dq      0x600083
	dq      0x800083
	dq      0xE0000083
	dq      0xE0200083
	dq      0xE0400083
	dq      0xE0600083
	dq      0xE0800083
	dq      0xE0A00083
	dq      0xE0C00083
	dq      0xE0E00083
	resq    499

[section .data]
global GDT
GDT:                ; 0
	Descriptor 0
GDT_Kernel_Code:    ; 1
	Descriptor DA_64 | DA_C
GDT_Kernel_Data:    ; 2
	Descriptor DA_DRW
GDT_User_Code:      ; 3
	Descriptor DA_64 | DA_C | DA_DPL3
GDT_User_Data:      ; 4
	Descriptor DA_DRW | DA_DPL3
GDT_Kernel_Code32:  ; 5
	Descriptor32 0x00000000, 0xFFFFF, DA_32 | DA_LIMIT_4K | DA_CR
GDT_Kernel_Data32:  ; 6
	Descriptor32 0x00000000, 0xFFFFF, DA_32 | DA_LIMIT_4K | DA_DRW
GDT_TSS0:           ; 7
	dq      0
GDT_TSS1:
	dq      0
GDT_NULL:
	Descriptor 0
	Descriptor 0

GDT_Length equ $ - GDT
GDT_Pointer:
	dw      GDT_Length - 1
	dq      GDT
Selector_Kernel_Code equ GDT_Kernel_Code - GDT
Selector_Kernel_Data equ GDT_Kernel_Data - GDT
Selector_User_Code equ GDT_User_Code - GDT
Selector_User_Data equ GDT_User_Data - GDT
Selector_Kernel_Code32 equ GDT_Kernel_Code32 - GDT
Selector_Kernel_Data32 equ GDT_Kernel_Data32 - GDT
Selector_TSS equ GDT_TSS0 - GDT

global IDT
IDT:
	resq    512
IDT_Length equ $ - IDT
IDT_Pointer:
	dw      IDT_Length - 1
	dq      IDT

global TSS
TSS:
	resq    13
TSS_Length equ $ - TSS
TSS_Pointer:
	dw      TSS_Length - 1
	dq      TSS

MSG_Interrupt:
	db      "Unknown Interrupt", 0