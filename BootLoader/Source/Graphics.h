/**
 * @file Graphics.h
 * @version 0.0.0.9
 * @author LinhengXilan
 * @date 2026-2-8
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Uefi.h>

EFI_STATUS InitGraphics(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);
EFI_STATUS QueryVideoMode();
EFI_STATUS SetVideoMode(UINT32 mode);

#endif