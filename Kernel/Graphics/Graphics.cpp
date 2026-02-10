/**
 * @file Graphics/Graphics.cpp
 * @author LinhengXilan
 * @version 0.0.0.4
 * @date 2026-2-10
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

void Graphics::DrawPixel(uint16_t x, uint16_t y, uint32_t color)
{
	*reinterpret_cast<uint32_t*>(m_Buffer.Base + (m_Resolution.Width * y + x) * 4) = color;
}