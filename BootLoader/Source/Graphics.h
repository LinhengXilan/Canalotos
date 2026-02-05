/**
 * @file Graphics.h
 * @version 0.0.0.3
 * @author LinhengXilan
 * @date 2026-2-4
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Uefi.h>
#include <Library/UefiLib.h>

extern EFI_GRAPHICS_OUTPUT_PROTOCOL* g_GraphicsOutputProtocol;

EFI_STATUS InitGraphics(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);
EFI_STATUS QueryGraphicMode();
EFI_STATUS SetVideoMode(UINT32 mode);

#endif