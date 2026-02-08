/**
 * @file Graphics.h
 * @version 0.0.0.10
 * @author LinhengXilan
 * @date 2026-2-9
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Uefi.h>
#include <KernelData.h>

EFI_STATUS InitGraphics(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);
EFI_STATUS QueryVideoMode();
EFI_STATUS SetVideoMode(UINT32 mode);
EFI_STATUS GetGraphicsModeInfo(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION** info);
EFI_STATUS GetEFIDataGraphics(EFI_DATA_GRAPHICS* efiDataGraphics);

#endif