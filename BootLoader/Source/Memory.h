/**
 * @file Memory.h
 * @version 0.0.2.16
 * @author LinhengXilan
 * @date 2026-2-16
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <Uefi.h>
#include <KernelData.h>

EFI_STATUS GetEFIDataMemory(EFI_SYSTEM_TABLE* systemTable, EFI_DATA_MEMORY* memory);

#endif