/**
 * @file File.h
 * @version 0.0.0.6
 * @author LinhengXilan
 * @date 2026-2-6
 */

#ifndef FILE_H
#define FILE_H

#include <Uefi.h>
#include <Library/FileHandleLib.h>

EFI_STATUS InitFile(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);

/**
 *
 * @param fileName 文件名
 * @param fileProtocol 文件协议
 * @return 状态码
 * @brief 获取文件句柄
 */
EFI_STATUS GetFileHandle(CHAR16* fileName, EFI_FILE_PROTOCOL** file);

/**
 *
 * @param file 打开的文件
 * @param address 内存地址
 * @return 状态码
 * @brief 将文件读取到内存指定位置
 */
EFI_STATUS ReadFile(EFI_FILE_PROTOCOL* file, EFI_PHYSICAL_ADDRESS* address);

#endif