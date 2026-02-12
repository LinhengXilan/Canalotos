/**
 * @file Shell/Shell.cpp
 * @author LinhengXilan
 * @version 0.0.0.3
 * @date 2026-2-9
 */

#include <Shell/Shell.h>
#include <Shell/Font.h>

Shell::Shell(Graphics* graphics)
	: m_Graphics(graphics)
{
	m_PrintablePerRow = graphics->GetResolution().Width / 8;
	m_PrintablePerColumn = graphics->GetResolution().Height / 16;
	m_BufferSize = m_PrintablePerRow * m_PrintablePerColumn;
	Write();
}

Shell::Shell(Graphics* graphics, uint32_t backgroundColor, uint32_t textColor)
	: m_Graphics(graphics)
{
	m_PrintablePerRow = graphics->GetResolution().Width / 8;
	m_PrintablePerColumn = graphics->GetResolution().Height / 16;
	m_BufferSize = m_PrintablePerRow * m_PrintablePerColumn;
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

void Shell::PutChar(char c)
{
	m_Buffer[m_Index] = c;
	m_Index++;
	Write();
}

void Shell::Write()
{
	uint16_t printableCount = 0;
	for (uint16_t i = 0; i < m_BufferSize; i++)
	{
		uint8_t* ascii = Font[m_Buffer[i]];
		if (m_Buffer[i] <= 32 || m_Buffer[i] >= 127)
		{
			break;
		}
		uint16_t y = printableCount / m_PrintablePerRow * 16;
		for (uint8_t j = 0; j < 16; j++)
		{
			uint16_t x = printableCount % m_PrintablePerRow * 8;
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
	}
}
