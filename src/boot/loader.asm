; @file: boot/loader.asm
; @author: LinhengXilan
; @data: 2025-7-29
; @version: build11

%include "macro.inc"

	org     0x10000
	jmp     _Start

%include "FAT12.inc"

BaseOfKernel equ 0
OffsetOfKernel equ 0x100000
BaseTmpOfKernel equ 0x00
OffsetTmpOfKernel equ 0x7E00
MemoryStructBuffer equ 0x7E00

[section .gdt]
GDT32:              Descriptor32 0,          0,       0
GDT_Kernel_Code32:  Descriptor32 0x00000000, 0xFFFFF, DA_32 | DA_LIMIT_4K | DA_CR
GDT_Kernel_Data32:  Descriptor32 0x00000000, 0xFFFFF, DA_32 | DA_LIMIT_4K | DA_DRW
GdtLen32 equ $ - GDT32
GdtPtr32    dw GdtLen32 - 1
			dd GDT32
GDTSelector_Kernel_Code32 equ GDT_Kernel_Code32 - GDT32
GDTSelector_Kernel_Data32 equ GDT_Kernel_Data32 - GDT32

[section .gdt64]
GDT:		        Descriptor 0
GDT_Kernel_Code:	Descriptor DA_64 | DA_CR
GDT_Kernel_Data:	Descriptor DA_DRW
GdtLen equ $ - GDT
GdtPtr  dw GdtLen - 1
		dd GDT
GDTSelector_Kernel_Code	equ	GDT_Kernel_Code - GDT
GDTSelector_Kernel_Data	equ	GDT_Kernel_Data - GDT

[section .s16]
[bits 16]
_Start:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ax, 0
	mov	ss, ax
	mov	sp, 0x7C00
; Open address A20.
	push    ax
	in      al, 0x92
	or      al, 0b00000010
	out     0x92, al
	pop     ax
	cli
	lgdt    [GdtPtr32]
	mov     eax, cr0
	or      eax, 1
	mov     cr0, eax
	mov     ax, GDTSelector_Kernel_Data32
	mov     fs, ax
	mov     eax, cr0
	and     al, 0b11111110
	mov     cr0, eax
	sti
; Reset floppy.
	xor     ah, ah
	xor     dl, dl
	int     0x13
; Search kernel.bin.
	mov     word [SectorNo], SectorNumOfRootDir
_SearchInRootDir:
	cmp     word [RootDirSize], 0
	jz      .NoKernel
	dec     word [RootDirSize]
	mov     ax, 0
	mov     es, ax
	mov     bx, 0x8000
	mov     ax, [SectorNo]
	mov     cl, 1
	call    _ReadSector
	mov     si, KernelFile
	mov     di, 0x8000
	cld
	mov     dx, 0x10
.SearchForKernel:
	cmp     dx, 0
	jz      .GotoNextSector
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
	inc	di
	jmp	.CmpFileName
.Different:
	and     di, 0xFFE0
	add     di, 0x20
	mov     si, KernelFile
	jmp     .SearchForKernel
.GotoNextSector:
	add     word [SectorNo], 1
	jmp     _SearchInRootDir
.NoKernel:
	mov     ax, 0x1301
	mov     bx, 0x8C
	mov     dx, 0x300
	mov     cx, 21
	push    ax
	mov     ax, ds
	mov     es, ax
	pop     ax
	mov     bp, MSG_NoKernel
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
	mov     eax, BaseTmpOfKernel
	mov     es, eax
	mov     bx, OffsetTmpOfKernel
	mov     ax, cx
.GoOnLoadingFile:
	mov	    cl, 1
	call    _ReadSector
	pop     ax
	push    cx
	push    eax
	push    fs
	push    edi
	push    ds
	push    esi
	mov     cx, 0x200
	mov     ax, BaseOfKernel
	mov     fs, ax
	mov     edi, dword [OffsetOfKernelCount]
	mov     ax, BaseTmpOfKernel
	mov     ds, ax
	mov     esi, OffsetTmpOfKernel
_MoveKernel:
	mov     al, byte [ds:esi]
	mov     byte [fs:edi], al
	inc     esi
	inc     edi
	loop	_MoveKernel
	mov     eax, 0x1000
	mov     ds, eax
	mov     dword [OffsetOfKernelCount], edi
	pop     esi
	pop     ds
	pop     edi
	pop     fs
	pop     eax
	pop     cx
	call	_GetFATEntry
	cmp     ax, 0xFFF
	jz      .FileLoaded
	push    ax
	mov     dx, RootDirSectors
	add     ax, dx
	add     ax, SectorBalance
	jmp	    _SearchInRootDir.GoOnLoadingFile
.FileLoaded:
	mov     ax, 0xB800
	mov     gs, ax

_KillMotor:
	push    dx
	mov     dx, 0x3F2
	mov     al, 0
	out     dx, al
	pop     dx

	mov     ebx, 0
	mov     ax, 0
	mov     es, ax
	mov     di, MemoryStructBuffer
_GetMemoryStruct:
	mov     eax, 0xE820
	mov     ecx, 20
	mov     edx, 0x534D4150
	int     0x15
	jc      .GetMemoryFail
	add     di, 20
	inc     dword [MemStructNumber]
	cmp     ebx, 0
	jne     _GetMemoryStruct
	jmp     .GetMemorySuccess
.GetMemoryFail:
	mov	dword [MemStructNumber], 0
	mov     ax, 0x1301
	mov     bx, 0x8C
	mov     dx, 0x500
	mov     cx, 23
	push    ax
	mov     ax, ds
	mov     es, ax
	pop     ax
	mov     bp, MSG_GetMemStructError
	int     0x10
.GetMemorySuccess:
_GetSVGAInfo:
	mov     ax, 0
	mov     es, ax
	mov     di, 0x8000
	mov     ax, 0x4F00
	int     0x10
	cmp     ax, 0x4F
	jz      .KO
; Fail.
	mov     ax, 0x1301
	mov     bx, 0x8C
	mov     dx, 0x900
	mov     cx, 23
	push    ax
	mov     ax, ds
	mov     es, ax
	pop     ax
	mov     bp, MSG_GetSVGAInfoError
	int     0x10
	jmp     $

.KO:
	mov     ax, 0
	mov     es, ax
	mov     si, 0x800E
	mov     esi, dword [es:si]
	mov     edi, 0x8200
_GetSVGAMode:
	mov     cx, word [es:esi]
	cmp     cx, 0xFFFF
	jz      .GetSVGAModeFinish
	mov     ax, 0x4F01
	int     0x10
	cmp     ax, 0x4F
	jnz     .GetSVGAModeFail
	inc     dword [SVGAModeCounter]
	add     esi, 2
	add     edi, 0x100
	jmp     _GetSVGAMode
.GetSVGAModeFail:
	mov     ax, 0x1301
	mov     bx, 0x8C
	mov     dx, 0xD00
	mov     cx, 24
	push    ax
	mov     ax, ds
	mov     es, ax
	pop     ax
	mov     bp, MSG_GetSVGAModeError
	int     0x10
.SetSVGAModeFail:
	jmp     $
.GetSVGAModeFinish:
	mov     ax, 0x4F02
	mov     bx, 0x4180
	int     0x10
	cmp     ax, 004Fh
	jnz     .SetSVGAModeFail
; Enter temp protect mode.
	cli
	lgdt    [GdtPtr32]
	mov     eax, cr0
	or      eax, 1
	mov     cr0, eax
	jmp     dword GDTSelector_Kernel_Code32:_GoToTempProtect

[SECTION .s32]
[BITS 32]
_GoToTempProtect:
; Enter temp long mode.
	mov     ax, 0x10
	mov     ds, ax
	mov     es, ax
	mov     fs, ax
	mov     ss, ax
	mov     esp, 0x7E00
	call    _Support_long_mode
	test    eax, eax
	jz	no_support
; Setup temporary page table.
	mov dword [0x90000], 0x91007
	mov dword [0x90004], 0x00000
	mov dword [0x90800], 0x91007
	mov dword [0x90804], 0x00000
	mov dword [0x91000], 0x92007
	mov dword [0x91004], 0x00000
	mov dword [0x92000], 0x000083
	mov dword [0x92004], 0x000000
	mov dword [0x92008], 0x200083
	mov dword [0x9200c], 0x000000
	mov dword [0x92010], 0x400083
	mov dword [0x92014], 0x000000
	mov dword [0x92018], 0x600083
	mov dword [0x9201c], 0x000000
	mov dword [0x92020], 0x800083
	mov dword [0x92024], 0x000000
	mov dword [0x92028], 0xa00083
	mov dword [0x9202c], 0x000000

	lgdt    [GdtPtr]
	mov     ax, 0x10
	mov     ds, ax
	mov     es, ax
	mov     fs, ax
	mov     gs, ax
	mov     ss, ax
	mov     esp, 0x7E00
; Open PAE.
	mov     eax, cr4
	bts     eax, 5
	mov     cr4, eax
; Load cr3.
	mov     eax, 0x90000
	mov     cr3, eax
; Enable long mode
	mov     ecx, 0xC0000080
	rdmsr
	bts     eax, 8
	wrmsr
; Open PE and Paging
	mov     eax, cr0
	bts     eax, 0
	bts     eax, 31
	mov     cr0, eax
	jmp     GDTSelector_Kernel_Code:OffsetOfKernel  ;0x1036b

[section .lib16]
[bits 16]
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
.GoOnReading:
	mov     ah, 2
	mov     al, byte [bp - 2]
	int     0x13
	jc      .GoOnReading
	add     esp, 2
	pop     bp
	ret

_GetFATEntry:
	push    es
	push    bx
	push    ax
	mov     ax, 0
	mov     es, ax
	pop     ax
	mov     byte [Odd], 0
	mov     bx, 3
	mul     bx
	mov     bx, 2
	div     bx
	cmp     dx, 0
	jz      .Even
	mov     byte [Odd], 1
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
	cmp     byte [Odd], 1
	jnz     .Even2
	shr     ax, 4
.Even2:
	and     ax, 0xFFF
	pop     bx
	pop     es
	ret

[section .lib32]
[bits 32]
_Support_long_mode:
	mov     eax, 0x80000000
	cpuid
	cmp     eax, 0x80000001
	setnb   al
	jb      .done
	mov     eax, 0x80000001
	cpuid
	bt      edx, 29
	setc    al
.done:
	movzx   eax, al
	ret
no_support:
	jmp     $

RootDirSize	dw RootDirSectors
SectorNo dw 0
Odd db 0
OffsetOfKernelCount dd OffsetOfKernel
MemStructNumber dd 0
SVGAModeCounter dd 0

MSG_NoKernel: db "ERROR:No KERNEL Found"
KernelFile: db "KERNEL  BIN", 0
MSG_GetMemStructError: db "Get Memory Struct ERROR"
MSG_GetSVGAInfoError: db "Get SVGA VBE Info ERROR"
MSG_GetSVGAModeError: db "Get SVGA Mode Info ERROR"