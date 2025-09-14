; @file: boot/boot.asm
; @author: LinhengXilan
; @data: 2025-9-14
; @version: build14

STACK_BASE      equ 0x7C00
TEMP_BUFFER_OFFSET   equ 0x8000
LOADER_BASE     equ 0x1000
LOADER_OFFSET   equ 0

	org     0x7C00
	jmp     short Start
	nop
%include "FAT12.inc"

Start:
	mov	    ax,	cs
	mov	    ds,	ax
	mov	    es,	ax
	mov	    ss,	ax
	mov	    sp,	STACK_BASE
; Clear screen.
	mov     ah,	0x6     ; int 0x10 function 0x6: clear screen
	mov     bx,	0x700
	mov     cx,	0
	mov     dx,	0x184F
	int     0x10
; Set cursor.
	mov     ah,	0x2     ; int 0x10 function 0x2: set cursor
	mov     bx, 0
	mov     dx, 0
	int     0x10
; Search for loader.
	mov     word [sectorNo], sectorNumOfRootDir
SearchFile:
	cmp     word [rootDirSize], 0
	jz      .FileNotFound
	dec     word [rootDirSize]
	mov     eax, [sectorNo]
	mov     cx, 1
	mov     bx, 0
	mov     dx, TEMP_BUFFER_OFFSET
	call    ReadDisk
	mov     si, loaderFileName
	mov     di, TEMP_BUFFER_OFFSET
	cld
	mov     dx, 0x10
.SearchLoader:
	cmp     dx, 0
	jz      .NextSector
	dec     dx
	mov     cx, 11
.CmpFileName:
	cmp     cx, 0
	jz      .FileFound
	dec     cx
	lodsb
	cmp     al, byte [es:di]
	jz      .GoOn
	jmp     .Different
.GoOn:
	inc     di
	jmp     .CmpFileName
.Different:
	and     di, 0xFFE0
	add     di, 0x20
	mov     si, loaderFileName
	jmp     .SearchLoader
.NextSector:
	add     word [sectorNo], 1
	jmp     SearchFile
.FileNotFound:
	mov     ax, 0x1301
	mov     bx, 0x8C
	mov     dx, 0x100
	mov     cx, 27
	push    ax
	mov     ax, ds
	mov     es, ax
	pop     ax
	mov     bp, msg_NoLoader
	int     0x10
	jmp     $
.FileFound:
	and     di, 0xFFE0  ; 定位到此目录项起始地址
	add     di, 0x1A    ; 文件起始簇号
	mov     ax, word[es:di]
	mov     word [clusterNo], ax
	mov     eax, sectorNumOfFAT1
    mov     bx, 0
    mov     dx, TEMP_BUFFER_OFFSET
    mov     cx, word [BPB_FATSz16]
    call    ReadDisk
.LoadFile:
	mov     ax, word [clusterNo]
	sub     ax, 2
	mov     bl, 4
	mul     bl
	add     ax, 57
	mov     cx, 4
	mov     bx, LOADER_BASE
	mov     dx, LOADER_OFFSET
	call    ReadDisk
	mov     ax, word [clusterNo]
	call    GetNextFAT
	cmp     ax, 0xFFF
	jnz     .LoadFile
.FileLoaded:
	jmp     LOADER_BASE:LOADER_OFFSET

; void ReadDisk(eax lba, cx nr_sectors, bx buffer_base, dx TEMP_BUFFER_OFFSET);
; Read disk.
ReadDisk:
	mov     dword [diskLba], eax
	mov     word [diskNrSectors], cx
	mov     word [diskBufferBase], bx      ; base of buffer
	mov     word [diskBufferOffset], dx      ; base of buffer
	mov     si, diskPack
	mov     ah, 0x42    ; function number: read disk
	mov     dl, 0x80    ; disk drive number
	int     0x13
	ret

; ax GetFATEntry(ax cluster);
; Get Next.
GetNextFAT:
	xor     dx, dx
	mov     bx, 2
	div     bx
	mov     word [odd], dx
	mov     bl, 3
	mul     bl
	mov     si, TEMP_BUFFER_OFFSET
	add     si, ax
	cmp     byte [odd], 1
	jz      .1
	lodsw
	jmp     .2
.1:
	add     si, 1
	lodsw
	shr     ax, 4
.2:
	and     ax, 0xFFF
	mov     word [clusterNo], ax
	ret

rootDirSize dw rootDirSectors
sectorNo    dw 0
clusterNo   dw 0
odd db 0

loaderFileName:
	db "LOADER  BIN", 0
msg_NoLoader:
	db "cannot find file loader.bin"

diskPack:
	db      0x10
	db      0
diskNrSectors:
	dw      0x1
diskBufferOffset:
	dw      0x8000
diskBufferBase:
	dw      0
diskLba:
	dd      0
	dd      0

	resb    510 - ($ - $$)
	dw      0xAA55