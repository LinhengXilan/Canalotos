/**
 * @file Include/Graphics/Graphics.h
 * @author LinhengXilan
 * @version 0.0.0.6
 * @date 2026-2-13
 */

#ifndef __GRAPHICS_GRAPHICS_H__
#define __GRAPHICS_GRAPHICS_H__

#include <Type.h>
#include <Boot.h>

struct Resolution
{
	uint16_t Width;
	uint16_t Height;
};

class Graphics
{
public:
	Graphics(const EFIDataGraphics& data);
	~Graphics() = default;

public:
	Resolution GetResolution() const;

public:
	/**
	 * @param x uint16_t | x坐标，从0开始
	 * @param y uint16_t | y坐标，从0开始
	 * @param color uint32_t | 颜色
	 * @brief 绘制坐标处1像素的颜色
	 */
	void WritePixel(uint16_t x, uint16_t y, uint32_t color);

	/**
	 *
	 * @param x uint16_t | x坐标，从0开始
	 * @param y uint16_t | y坐标，从0开始
	 * @param width uint16_t | x方向长度
	 * @param height uint16_t | y方向长度
	 * @param color uint32_t | 颜色
	 */
	void WriteBlock(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

private:
	Buffer m_Buffer;
	Resolution m_Resolution;
};

#endif