/**
 * @file Include/Shell/Shell.h
 * @author LinhengXilan
 * @version 0.0.1.9
 * @date 2026-2-28
 */

#ifndef __SHELL_SHELL_H__
#define __SHELL_SHELL_H__

#include <Type.h>
#include <Graphics/Graphics.h>

namespace Shell
{
	struct DisplayData
	{
		Graphics* graphics = nullptr;
		char buffer[65536];
		uint16_t bufferSize = 0;
		uint16_t index = 0;
		uint8_t printablePerRow = 0;
		uint8_t printablePerColumn = 0;
		uint8_t cursorX = 0;
		uint8_t cursorY = 0;
		uint32_t backgroundColor = 0x00000000;
		uint32_t textColor = 0xFFFFFFFF;
	};

	struct Data
	{
		DisplayData displayData;
	};

	void Init(Graphics* graphics);
	void Init(Graphics* graphics, uint32_t backgroundColor, uint32_t textColor);

	void SetBackgroundColor(uint32_t color);
	void SetTextColor(uint32_t color);

	/**
	 * @param ch ascii字符
	 *
	 * @brief 打印一个ascii字符
	 */
	void PutChar(char ch);
	/**
	 * @param string const char* | 字符串指针
	 *
	 * @note 请保证字符串大小小于65535
	 */
	void PutString(const char* string);
	void Print(const char* string, ...);
	void Write();
}

//#include <Shell/Shell.inl>

#endif