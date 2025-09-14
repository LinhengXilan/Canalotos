; @file: boot/FAT12.inc
; @author: LinhengXilan
; @data: 2025-9-14
; @version: build14

rootDirSectors      equ 14
sectorNumOfRootDir  equ 25
sectorNumOfFAT1     equ 1

BS_OEMName      db 'Canaloto'
BPB_BytesPerSec dw 0x200
BPB_SecPerClus  db 0x4
BPB_RsvdSecCnt  dw 0x1
BPB_NumFATs     db 0x2
BPB_RootEntCnt  dw 0x200
BPB_TotSec16    dw 0x3E82
BPB_Media       db 0xF0
BPB_FATSz16     dw 0xC
BPB_SecPerTrk   dw 0x3F
BPB_NumHeads    dw 0xFF
BPB_HiddSec     dd 0
BPB_TotSec32    dd 0
BS_DrvNum       db 0x80
BS_Reserved1    db 0
BS_BootSig      db 0x29
BS_VolID        dd 0
BS_VolLab       db 'boot loader'
BS_FileSysType  db 'FAT12   '