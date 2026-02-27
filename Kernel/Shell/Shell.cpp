/**
 * @file Shell/Shell.cpp
 * @author LinhengXilan
 * @version 0.0.1.9
 * @date 2026-2-28
 */

#include <Shell/Shell.h>
#include <Shell/Font.h>
#include <Lib/String.h>

Shell::Shell(Graphics* graphics)
	: m_Graphics(graphics)
{
	m_PrintablePerRow = (graphics->GetResolution().Width - 2) / 8;
	m_PrintablePerColumn = (graphics->GetResolution().Height - 2) / 16;
	m_BufferSize = m_PrintablePerRow * m_PrintablePerColumn;

	m_Graphics->WriteBlock(0, 0, m_Graphics->GetResolution().Width, 1, 0xFFFFFFFF);
	m_Graphics->WriteBlock(0, m_Graphics->GetResolution().Height - 1, m_Graphics->GetResolution().Width, 1, 0xFFFFFFFF);
	m_Graphics->WriteBlock(0, 0, 1, m_Graphics->GetResolution().Height, 0xFFFFFFFF);
	m_Graphics->WriteBlock(m_Graphics->GetResolution().Width - 1, 0, 1, m_Graphics->GetResolution().Height, 0xFFFFFFFF);
	Write();
}

Shell::Shell(Graphics* graphics, uint32_t backgroundColor, uint32_t textColor)
	: Shell(graphics)
{
	m_BackgroundColor = backgroundColor;
	m_TextColor = textColor;
	Write();
}

void Shell::SetBackgroundColor(uint32_t color)
{
	m_BackgroundColor = color;
}

void Shell::SetTextColor(uint32_t color)
{
	m_TextColor = color;
}

void Shell::PutChar(char ch)
{
	m_Buffer[m_Index] = ch;
	m_Index++;
	Write();
}

void Shell::PutString(const char* string)
{
	for (uint16_t i = 0; i < Lib::StrLen(string); i++)
	{
		m_Buffer[m_Index] = string[i];
		m_Index++;
	}
	Write();
}

void Shell::Write()
{
	m_CursorX = 0;
	m_CursorY = 0;
	for (uint16_t i = 0; i < m_BufferSize; i++)
	{
		uint8_t* ascii = Font[m_Buffer[i]];
		if (m_CursorX >= m_PrintablePerRow)
		{
			m_CursorX = 0;
			m_CursorY++;
		}
		switch (m_Buffer[i])
		{
		case '\n':
			m_CursorX = 0;
			m_CursorY++;
			break;
		case '\t':
			m_CursorX = m_CursorX + 4 & 0b11111100;
			break;
		case 32 ... 126:
		{
			uint16_t y = m_CursorY * 16 + 4;
			for (uint8_t j = 0; j < 16; j++)
			{
				uint16_t x = m_CursorX * 8 + 4;
				for (uint8_t k = 8; k > 0; k--)
				{
					if (ascii[j] & 1 << k)
					{
						m_Graphics->WritePixel(x, y, m_TextColor);
					}
					else
					{
						m_Graphics->WritePixel(x, y, m_BackgroundColor);
					}
					x++;
				}
				y++;
			}
			m_CursorX++;
			break;
		}
		default:
			break;
		}
	}
}