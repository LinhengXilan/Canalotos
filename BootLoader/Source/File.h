/**
 * @file File.h
 * @version 0.0.1.8
 * @author LinhengXilan
 * @date 2026-2-7
 */

#ifndef FILE_H
#define FILE_H

#include <Uefi.h>
#include <Protocol/SimpleFileSystem.h>

EFI_STATUS InitFile(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);

/**
 *
 * @param fileName 文件名
 * @param file [out] 文件
 * @return 状态码
 * @brief 获取文件句柄
 */
EFI_STATUS GetFileHandle(CHAR16* fileName, EFI_FILE_PROTOCOL** file);

/**
 *
 * @param systemTable 系统表
 * @param file 打开的文件
 * @param address [out] 内存地址
 * @return 状态码
 * @brief 将文件读取到内存位置
 */
EFI_STATUS ReadFile(EFI_SYSTEM_TABLE* systemTable, EFI_FILE_PROTOCOL* file, EFI_PHYSICAL_ADDRESS* address);

#endif