/**
 * @file Shell/Shell.cpp
 * @author LinhengXilan
 * @version 0.0.1.10
 * @date 2026-3-22
 */

#include <Shell/Shell.h>
#include <Shell/Font.h>
#include <Lib/String.h>
#include <Lib/Print.h>
#include <Stdarg.h>

namespace Shell
{
	static Data shellData{};
	static DisplayData* displayData;
	static Graphics* graphicsData = nullptr;

	void Init(Graphics* graphics)
	{
		displayData = &shellData.displayData;
		graphicsData = graphics;
		displayData->printablePerRow = (graphics->GetResolution().Width - 2) / 8;
		displayData->printablePerColumn = (graphics->GetResolution().Height - 2) / 16;
		displayData->bufferSize = displayData->printablePerRow * displayData->printablePerColumn;
		displayData->cursorX = 0;
		displayData->cursorY = 0;
		graphicsData->WriteBlock(0, 0, graphicsData->GetResolution().Width, 1, 0xFFFFFFFF);
		graphicsData->WriteBlock(0, graphicsData->GetResolution().Height - 1, graphicsData->GetResolution().Width, 1, 0xFFFFFFFF);
		graphicsData->WriteBlock(0, 0, 1, graphicsData->GetResolution().Height, 0xFFFFFFFF);
		graphicsData->WriteBlock(graphicsData->GetResolution().Width - 1, 0, 1, graphicsData->GetResolution().Height, 0xFFFFFFFF);
		Write();
	}

	void Init(Graphics* graphics, uint32_t backgroundColor, uint32_t textColor)
	{
		Init(graphics);
		displayData->backgroundColor = backgroundColor;
		displayData->textColor = textColor;
		Write();
	}

	void SetBackgroundColor(uint32_t color)
	{
		displayData->backgroundColor = color;
	}

	void SetTextColor(uint32_t color)
	{
		displayData->textColor = color;
	}

	void PutChar(char ch)
	{
		displayData->buffer[displayData->index] = ch;
		displayData->index++;
		Write();
	}

	void PutString(const char* string)
	{
		for (uint16_t i = 0; i < Lib::StrLen(string); i++)
		{
			displayData->buffer[displayData->index] = string[i];
			++displayData->index;
		}
		Write();
	}

	void Print(const char* string, ...)
	{
		va_list args;
		va_start(args, string);
		uint8_t length = Lib::PrintfToBuffer(displayData->buffer + displayData->index, string, args);
		va_end(args);
		displayData->index += length;
		Write();
	}

	void Write()
	{
		displayData->cursorX = 0;
		displayData->cursorY = 0;
		for (uint16_t i = 0; i < displayData->bufferSize; ++i)
		{
			const uint8_t* ascii = Font[shellData.displayData.buffer[i]];
			if (displayData->cursorX >= displayData->printablePerRow)
			{
				displayData->cursorX = 0;
				++displayData->cursorY;
			}
			switch (displayData->buffer[i])
			{
			case '\n':
				displayData->cursorX = 0;
				++displayData->cursorY;
				break;
			case '\t':
				displayData->cursorX = displayData->cursorX + 4 & 0b11111100;
				break;
			case 32 ... 126:
			{
				uint16_t y = displayData->cursorY * 16 + 4;
				for (uint8_t j = 0; j < 16; j++)
				{
					uint16_t x = displayData->cursorX * 8 + 4;
					for (uint8_t k = 8; k > 0; k--)
					{
						if (ascii[j] & 1 << k)
						{
							graphicsData->WritePixel(x, y, displayData->textColor);
						}
						else
						{
							graphicsData->WritePixel(x, y, displayData->backgroundColor);
						}
						x++;
					}
					y++;
				}
				displayData->cursorX++;
				break;
			}
			default:
				break;
			}
		}
	}
}