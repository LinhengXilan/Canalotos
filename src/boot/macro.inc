; @file: boot/macro.inc
; @author: LinhengXilan
; @data: 2025-9-26
; @version: build15

; segment attribute
DA_64           equ 0x2000  ; 64-bit code segment
DA_32           equ 0x4000  ; 32-bit segment
DA_LIMIT_4K     equ 0x8000  ; granularity = 4KB

DA_DPL0         equ 0x00    ; DPL = 0
DA_DPL1         equ 0x20    ; DPL = 1
DA_DPL2         equ 0x40    ; DPL = 2
DA_DPL3         equ 0x60    ; DPL = 3

DA_DR           equ 0x90    ; read-only data
DA_DRW          equ 0x92    ; read&write data
DA_DRWA         equ 0x93    ; read&write accessed data
DA_C            equ 0x98    ; executable-only code
DA_CR           equ 0x9A    ; executable&read code
DA_CCO          equ 0x9C    ; executable-only confirming code
DA_CCOR         equ 0x9E    ; executable&read confirming code

DA_LDT          equ 0x82    ;
DA_TaskGate     equ 0x85    ; task gate
DA_386TSS       equ 0x89    ; tss
DA_386CGate     equ 0x8C    ; 调用门
DA_386IGate     equ 0x8E    ; interrupt gate
DA_386TGate     equ 0x8F    ; 陷阱门

; segment selector attribute
SA_RPL0         equ 0x00    ; RPL = 0
SA_RPL1         equ 0x01    ; RPL = 1
SA_RPL2         equ 0x02    ; RPL = 2
SA_RPL3         equ 0x03    ; RPL = 3
SA_TIG          equ 0x00    ;
SA_TIL          equ 0x04    ;

; page attribute
PG_P            equ 0x01    ; 存在
PG_RWR          equ 0x00    ; 可读、可执行
PG_RWW          equ 0x02    ; 可读写、可执行
PG_USS          equ 0x00    ; 系统级
PG_USU          equ 0x04    ; 用户级

; 用法：DESCRIPTOR32 base, limit, attribute
%macro DESCRIPTOR32 3
	dw %2 & 0xFFFF
	dw %1 & 0xFFFF
	db (%1 >> 16) & 0xFF
	dw ((%2 >> 8) & 0x0F00) | (%3 & 0xF0FF)
	db (%1 >> 24) & 0xFF
%endmacro

; 用法：DESCRIPTOR attribute
%macro DESCRIPTOR 1
	dd 0
	db 0
	dw %1 & 0xF0FF
	db 0
%endmacro

; 用法：Gate selector, offset, param count, attribute
%macro Gate 4
	dw (%2 & 0xFFFF)
	dw %1
	dw (%3 & 0x1F) | ((%4 << 8) & 0xFF00)
	dw ((%2 >> 16) & 0xFFFF)
%endmacro