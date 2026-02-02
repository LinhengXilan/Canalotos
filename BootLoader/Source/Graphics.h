/**
 * @file Graphics.h
 * @version 0.0.0.2
 * @author LinhengXilan
 * @date 2026-2-3
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

EFI_STATUS InitGraphics(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);
EFI_STATUS QueryGraphicMode();
EFI_STATUS SetVideoMode(UINT32 mode);

#endif