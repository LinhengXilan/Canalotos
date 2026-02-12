/**
 * @file Graphics/Graphics.cpp
 * @author LinhengXilan
 * @version 0.0.0.6
 * @date 2026-2-13
 */

#include <Graphics/Graphics.h>

Graphics::Graphics(const EFIDataGraphics& data)
{
	m_Buffer.Base = data.FrameBufferBase;
	m_Buffer.Size = data.FrameBufferSize;
	m_Resolution.Width = data.HorizontalResolution;
	m_Resolution.Height = data.VerticalResolution;
}

Resolution Graphics::GetResolution() const
{
	return m_Resolution;
}

void Graphics::WritePixel(uint16_t x, uint16_t y, uint32_t color)
{
	*reinterpret_cast<uint32_t*>(m_Buffer.Base + (m_Resolution.Width * y + x) * 4) = color;
}

void Graphics::WriteBlock(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
	for (uint16_t i = 0; i < width; i++)
	{
		for (uint16_t j = 0; j < height; j++)
		{
			WritePixel(x + i, y + j, color);
		}
	}
}