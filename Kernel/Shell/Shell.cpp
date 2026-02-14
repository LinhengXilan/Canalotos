/**
 * @file Shell/Shell.cpp
 * @author LinhengXilan
 * @version 0.0.0.7
 * @date 2026-2-14
 */

#include <Shell/Shell.h>
#include <Shell/Font.h>
#include <Lib/CString.h>
#include <Lib/Print.h>

Shell::Shell(Graphics* graphics)
	: m_Graphics(graphics)
{
	m_PrintablePerRow = graphics->GetResolution().Width - 2 / 8;
	m_PrintablePerColumn = graphics->GetResolution().Height - 2 / 16;
	m_BufferSize = m_PrintablePerRow * m_PrintablePerColumn;
	m_Graphics->WriteBlock(0, 0, m_Graphics->GetResolution().Width, 1, 0xFFFFFFFF);
	m_Graphics->WriteBlock(0, m_Graphics->GetResolution().Height - 1, m_Graphics->GetResolution().Width, 1, 0xFFFFFFFF);
	m_Graphics->WriteBlock(0, 0, 1, m_Graphics->GetResolution().Height, 0xFFFFFFFF);
	m_Graphics->WriteBlock(m_Graphics->GetResolution().Width - 1, 0, 1, m_Graphics->GetResolution().Height, 0xFFFFFFFF);
}

Shell::Shell(Graphics* graphics, uint32_t backgroundColor, uint32_t textColor)
	: Shell(graphics)
{
	m_BackgroundColor = backgroundColor;
	m_TextColor = textColor;
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
	for (uint16_t i = 0; i < StrLen(string); i++)
	{
		m_Buffer[m_Index] = string[i];
		m_Index++;
	}
	Write();
}

void Shell::Write()
{
	uint16_t printableCount = 0;
	for (uint16_t i = 0; i < m_BufferSize; i++)
	{
		uint8_t* ascii = Font[m_Buffer[i]];
		switch (m_Buffer[i])
		{
		case 32 ... 126:
		{
			uint16_t y = printableCount / m_PrintablePerRow * 16 + 4;
			for (uint8_t j = 0; j < 16; j++)
			{
				uint16_t x = printableCount % m_PrintablePerRow * 8 + 4;
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
					x += 1;
				}
				y += 1;
			}
			printableCount++;
			break;
		}
		default:
			break;
		}
	}
}