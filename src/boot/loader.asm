; @file: boot/loader.asm
; @author: LinhengXilan
; @data: 2025-9-26
; @version: build15

%include "macro.inc"

	org     0x10000
	jmp     Start
	nop
;%include "FAT12.inc"

ROOT_DIR_SECTORS      equ 14
SECTOR_NUM_OF_ROOT_DIR  equ 25
SECTOR_NUM_OF_FAT1     equ 1

KERNEL_BASE equ 0
KERNEL_OFFSET equ 0x100000
KERNEL_TEMP_BASE equ 0x9000
KERNEL_TEMP_OFFSET equ 0x0
MEMORY_STRUCT equ 0x7E00
BUFFER_BASE equ 0x0
BUFFER_OFFSET equ 0x8000

[section .gdt]
gdt:
	DESCRIPTOR32 0, 0, 0
gdtKernelCode:
	DESCRIPTOR32 0x00000000, 0xFFFFF, DA_32 | DA_LIMIT_4K | DA_CR
gdtKernelData:
	DESCRIPTOR32 0x00000000, 0xFFFFF, DA_32 | DA_LIMIT_4K | DA_DRW
GDT_LEN equ $ - gdt
gdtPtr:
	dw GDT_LEN - 1
	dd gdt
SELECTOR_KERNEL_CODE equ gdtKernelCode - gdt
SELECTOR_KERNEL_DATA equ gdtKernelData - gdt

[section .gdt64]
gdt64:
	DESCRIPTOR 0
gdtKernelCode64:
	DESCRIPTOR DA_64 | DA_CR
gdtKernelData64:
	DESCRIPTOR DA_DRW
GDT_LEN64 equ $ - gdt64
gdtPtr64:
	dw GDT_LEN64 - 1
	dd gdt64
SELECTOR_KERNEL_CODE64 equ gdtKernelCode64 - gdt64
SELECTOR_KERNEL_DATA64 equ gdtKernelData64 - gdt64

[section .text16]
[bits 16]
Start:
	mov     ax, cs
	mov     ds, ax
	mov     es, ax
	mov     ax, 0
	mov     ss, ax
	mov     sp, 0x7C00
; Open address A20.
	push    ax
	in      al, 0x92
	or      al, 0b00000010
	out     0x92, al
	pop     ax
	cli
; 强制使fs寄存器获得32位寻址能力
	lgdt    [gdtPtr]
	mov     eax, cr0
	or      eax, 1
	mov     cr0, eax
	mov     ax, SELECTOR_KERNEL_DATA
	mov     fs, ax
	mov     eax, cr0
	and     al, 0b11111110
	mov     cr0, eax
	sti
; 搜索kernel.bin.
	mov     word [sectorNo], SECTOR_NUM_OF_ROOT_DIR
SearchInRootDir:
	cmp     word [rootDirSize], 0
	jz      .NoKernel
	mov     ax, BUFFER_BASE
	mov     es, ax
	dec     word [rootDirSize]
	mov     eax, [sectorNo]
	mov     cl, 1
	mov     bx, BUFFER_BASE
	mov     dx, BUFFER_OFFSET
	call    ReadDisk
	mov     si, kernelFileName
	mov     di, BUFFER_OFFSET
	cld
	mov     dx, 0x10
.SearchKernel:
	cmp     dx, 0
	jz      .NextSector
	dec     dx
	mov     cx, 11
.CmpFileName:
	cmp     cx, 0
	jz      .Found
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
	mov     si, kernelFileName
	jmp     .SearchKernel
.NextSector:
	add     word [sectorNo], 1
	jmp     SearchInRootDir
.NoKernel:
	mov     ax, 0x1301
	mov     bx, 0x8C
	mov     dx, 0x300
	mov     cx, 21
	push    ax
	mov     ax, ds
	mov     es, ax
	pop     ax
	mov     bp, msgNoKernel
	int     0x10
	jmp     $
.Found:
	and     di, 0xFFE0  ; 定位到此目录项起始地址
	add     di, 0x1A    ; 文件起始簇号
	mov     ax, word[es:di]
	mov     word [clusterNo], ax
	mov     eax, SECTOR_NUM_OF_FAT1
    mov     bx, BUFFER_BASE
    mov     dx, BUFFER_OFFSET
    mov     cx, word 0xC
    call    ReadDisk
.LoadFile:
	mov     ax, word [clusterNo]
	sub     ax, 2
	mov     bl, 4
	mul     bl
	add     ax, 57
	mov     cx, 4
	mov     bx, KERNEL_TEMP_BASE
	mov     dx, KERNEL_TEMP_OFFSET
	call    ReadDisk
	push	cx
	push	eax
	push	edi
	push	ds
	push	esi
	mov     cx,	0x800
	mov     edi, dword [kernelByteCount]
	mov     ax,	KERNEL_TEMP_BASE
	mov     ds,	ax
	mov     esi, KERNEL_TEMP_OFFSET
MoveKernel:
	mov     al,	byte [ds:esi]
	mov     byte [fs:edi], al
	inc     esi
	inc     edi
	loop    MoveKernel
	mov     ax, 0x1000  ; 0x1014c
	mov     ds,	ax
	mov     dword [kernelByteCount], edi
	pop     esi
	pop     ds
	pop     edi
	pop     eax
	pop     cx
	mov     ax, word [clusterNo]
    call    GetNextFAT
	cmp     ax,	0FFFh
	jz      .FileLoaded
	jmp     SearchInRootDir.LoadFile
.FileLoaded:
	mov     ax, 0xB800
	mov     gs, ax
	push    dx
	mov     dx, 0x3F2
	mov     al, 0
	out     dx, al
	pop     dx
	mov     ebx, 0
	mov     ax, 0
	mov     es, ax
	mov     di, MEMORY_STRUCT
GetMemoryStruct:
	mov     eax, 0xE820
	mov     ecx, 20
	mov     edx, 0x534D4150
	int     0x15
	jc      .Fail
	add     di, 20
	cmp     ebx, 0
	jne     GetMemoryStruct
	jmp     .Success
.Fail:
	mov     ax, 0x1301
	mov     bx, 0x8C
	mov     dx, 0x500
	mov     cx, 23
	push    ax
	mov     ax, ds
	mov     es, ax
	pop     ax
	mov     bp, msgGetMemStructError
	int     0x10
.Success:
; 获取VBE信息
GetSVGAInfo:
	xor     ax, ax  ; 0x101cc
	mov     es, ax
	mov     di, 0x8000
	mov     ax, 0x4F00  ; 0b01001000'00000000
	int     0x10
	cmp     ax, 0x4F
	jz      .Success
; 获取VBE信息失败
	mov     ax, 0x1301
	mov     bx, 0x8C
	mov     dx, 0x900
	mov     cx, 23
	push    ax
	mov     ax, ds
	mov     es, ax
	pop     ax
	mov     bp, msgGetSVGAInfoError
	int     0x10
	jmp     $
.Success:
	mov     ax, 0
	mov     es, ax
	mov     si, 0x800E
	mov     esi, dword [es:si]
	mov     edi, 0x8200
GetSVGAMode:
	mov     cx, word [es:esi]
.DisplaySVGAInfo:


	cmp     cx, 0xFFFF
	jz      SetSVGAMode
	mov     ax, 0x4F01
	int     0x10
	cmp     ax, 0x4F
	jnz     .Fail
	inc     dword [SVGAModeCounter]
	add     esi, 2
	add     edi, 0x100
	jmp     GetSVGAMode
.Fail:
	mov     ax, 0x1301
	mov     bx, 0x8C
	mov     dx, 0xD00
	mov     cx, 24
	push    ax
	mov     ax, ds
	mov     es, ax
	pop     ax
	mov     bp, msgGetSVGAModeError
	int     0x10
SetSVGAMode:
	mov     ax, 0x4F02
	mov     bx, 0x4180
	int     0x10
	cmp     ax, 004Fh
	jnz     .Fail
	jmp     SetProtectMode
.Fail:
	jmp     $
; Enter temp protect mode.
SetProtectMode:
	cli
	lgdt    [gdtPtr]
	mov     eax, cr0
	or      eax, 1
	mov     cr0, eax
	jmp     dword SELECTOR_KERNEL_CODE64:GoToTempProtect

[SECTION .s32]
[BITS 32]
GoToTempProtect:
; Enter temp long mode.
	mov     ax, 0x10
	mov     ds, ax
	mov     es, ax
	mov     fs, ax
	mov     ss, ax
	mov     esp, 0x7E00
	call    SupportLongMode
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

	lgdt    [gdtPtr64]
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
	jmp     SELECTOR_KERNEL_CODE64:KERNEL_OFFSET  ;0x1036b

[section .lib16]
[bits 16]
; void ReadDisk(eax lba, cx nr_sectors, bx buffer_base, dx buffer_offset);
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

GetNextFAT:
	xor     dx, dx
	mov     bx, 2
	div     bx
	mov     word [odd], dx
	mov     bl, 3
	mul     bl
	mov     si, BUFFER_OFFSET
	add     si, ax
	cmp     byte [odd], 1
	push    ds
	mov     ax, BUFFER_BASE
	mov     ds, ax
	jz      .1
	lodsw
	jmp     .2
.1:
	add     si, 1
	lodsw
	shr     ax, 4
.2:
	pop     ds
	and     ax, 0xFFF
	mov     word [clusterNo], ax
	ret

[section .lib32]
[bits 32]
SupportLongMode:
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

rootDirSize	dw ROOT_DIR_SECTORS
sectorNo dw 0
odd db 0
kernelByteCount dd KERNEL_OFFSET
MemStructNumber dd 0
SVGAModeCounter dd 0
clusterNo dw 0

diskPack:
	db      0x10
	db      0
diskNrSectors:
	dw      0
diskBufferOffset:
	dw      0
diskBufferBase:
	dw      0
diskLba:
	dd      0
	dd      0

msgNoKernel:
	db "ERROR:No KERNEL Found"
kernelFileName:
	db "KERNEL  BIN", 0
msgGetMemStructError:
	db "Get Memory Struct ERROR"
msgGetSVGAInfoError:
	db "Get SVGA VBE Info ERROR"
msgGetSVGAModeError:
	db "Get SVGA Mode Info ERROR"