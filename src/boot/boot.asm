; @file: boot/boot.asm
; @author: LinhengXilan
; @data: 2025-7-29
; @version: build11

BaseOfStack         equ	0x7C00
BaseOfLoader        equ 0x1000
OffsetOfLoader      equ 0

	org     0x7C00
	jmp     short _Start
	nop
%include "FAT12.inc"

_Start:
	mov	    ax,	cs
	mov	    ds,	ax
	mov	    es,	ax
	mov	    ss,	ax
	mov	    sp,	BaseOfStack
; Clear screen.
	mov     ah,	0x6     ; int 0x10 function: clear screen
	mov     bx,	0x700
	mov     cx,	0
	mov     dx,	0x184F
	int     0x10
; Set cursor.
	mov     ah,	0x2     ; int 0x10 function: set cursor
	mov     bx, 0
	mov     dx, 0
	int     0x10
; Reset floppy.
	xor     ah,	ah
	xor     dl,	dl
	int     0x13

; Search for loader.
	mov     word [sectorNo], SectorNumOfRootDir
_SearchFile:
	cmp     word [rootDirSize], 0
	jz      .FileNotFound
	dec     word [rootDirSize]
	mov     ax, 0
	mov     es, ax
	mov     bx, 0x8000
	mov     ax, [sectorNo]
	mov     cl, 1
	call    _ReadSector
	mov     si, LoaderFileName
	mov     di, 0x8000
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
	mov     si, LoaderFileName
	jmp     .SearchLoader
.NextSector:
	add     word [sectorNo], 1
	jmp     _SearchFile
.FileNotFound:
	mov     ax, 0x1301
	mov     bx, 0x8C
	mov     dx, 0x100
	mov     cx, 23
	push    ax
	mov     ax, ds
	mov     es, ax
	pop     ax
	mov     bp, MSG_NoLoader
	int     0x10
	jmp     $
.FileFound:
	mov     ax, RootDirSectors
	and     di, 0xFFE0
	add     di, 0x1A
	mov     cx, word [es:di]
	push    cx
	add     cx, ax
	add     cx, SectorBalance
	mov     ax, BaseOfLoader
	mov     es, ax
	mov     bx, OffsetOfLoader
	mov     ax, cx
; Get the FATs of loader.bin and call ReadSector to load it to memory until FAT=0xFFF.
.LoadFile:
	mov     cl, 1
	call    _ReadSector
	pop     ax
	call    _GetFATEntry
	cmp     ax, 0xFFF
	jz      .FileLoaded
	push    ax
	mov     dx, RootDirSectors
	add     ax, dx
	add     ax, SectorBalance
	add     bx, [BPB_BytesPerSec]
	jmp     .LoadFile
.FileLoaded:
	jmp     BaseOfLoader:OffsetOfLoader

; void _ReadSector(ax=sectorToRead, cl=numOfSectors, es:bx=buffer);
; Read sector.
_ReadSector:
	push    bp
	mov     bp, sp
	sub     esp, 2
	mov     byte [bp - 2], cl
	push    bx
	mov     bl, [BPB_SecPerTrk]
	div     bl
	inc     ah
	mov     cl, ah
	mov     dh, al
	shr     al, 1
	mov     ch, al
	and     dh, 1
	pop     bx
	mov     dl, [BS_DrvNum]
.Reading:
	mov     ah, 2
	mov     al, byte [bp - 2]
	int     0x13
	jc      .Reading
	add     esp, 2
	pop     bp
	ret

; bool _GetFATEntry(ah=numOfFAT) -> [odd];
; Get FAt Entry.
_GetFATEntry:
	push    es
	push    bx
	push    ax
	mov     ax, 0
	mov     es, ax
	pop     ax
	mov     byte [odd], 0
	mov     bx, 3
	mul     bx
	mov     bx, 2
	div     bx
	cmp     dx, 0
	jz      .Even
	mov     byte [odd], 1
.Even:
	xor     dx, dx
	mov     bx, [BPB_BytesPerSec]
	div     bx
	push    dx
	mov     bx, 0x8000
	add     ax, SectorNumOfFAT1
	mov     cl, 2
	call    _ReadSector
	pop     dx
	add     bx, dx
	mov     ax, [es:bx]
	cmp     byte [odd], 1
	jnz     .Even_2
	shr     ax, 4
.Even_2:
	and     ax, 0xFFF
	pop     bx
	pop     es
	ret

rootDirSize dw RootDirSectors
sectorNo    dw 0
odd db 0
LoaderFileName:
	db "LOADER  BIN", 0
MSG_NoLoader:
	db "Missing file loader.bin"

	resb    510 - ($ - $$)
	dw      0xaa55