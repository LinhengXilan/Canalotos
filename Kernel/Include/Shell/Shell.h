/**
 * @file Include/Shell/Shell.h
 * @author LinhengXilan
 * @version 0.0.0.6
 * @date 2026-2-13
 */

#ifndef __SHELL_SHELL_H__
#define __SHELL_SHELL_H__

#include <Type.h>
#include <Graphics/Graphics.h>

class Shell
{
public:
	Shell(Graphics* graphics);
	Shell(Graphics* graphics, uint32_t backgroundColor, uint32_t textColor);
	~Shell() = default;

public:
	void SetBackgroundColor(uint32_t color);
	void SetTextColor(uint32_t color);

public:
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

private:
	void Write();

private:
	Graphics* m_Graphics;
	char m_Buffer[65536];
	uint16_t m_BufferSize;
	uint16_t m_Index = 0;
	uint8_t m_PrintablePerRow;
	uint8_t m_PrintablePerColumn;
	uint32_t m_BackgroundColor = 0x00000000;
	uint32_t m_TextColor = 0xFFFFFFFF;
};

#endif