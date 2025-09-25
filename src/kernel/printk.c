/**
 * @file: kernel/Printk.c
 * @author: LinhengXilan
 * @data: 2025-9-26
 * @version: build15
 **/

#include <kernel/types.h>
#include <kernel/const.h>
#include <kernel/global.h>
#include <kernel/font.h>
#include <kernel/init.h>

#include <kernel/printk.h>

PRIVATE char* itoa_32(char* buffer, u32 number, u8 precision);
PRIVATE char* itoa(char* buffer, u64 number, u8 base, u8 precision);

void init_screen()
{
	screen.Resolution.x = 1440;
	screen.Resolution.y = 900;
	screen.Position.x = 0;
	screen.Position.y = 0;
	screen.CharSize.x = 8;
	screen.CharSize.y = 16;
	screen.bufferAddress = (u32*)0xFFFF800000A00000;
	screen.bufferSize = screen.Position.x * screen.Position.y * 4 + PAGE_4K - 1 & PAGE_4K_MASK;
}

int printk(const char* format, ...)
{
	char buffer[1024] = {0};
	va_list args;
	va_start(args, format);
	int i = vsprintf(buffer, format, args);
	va_end(args);
	for (int j = 0; j < i; j++)
	{
		switch (buffer[j])
		{
		case '\r':
		case '\n':
			screen.Position.x = 0;
			screen.Position.y = screen.Position.y + screen.CharSize.y + 1;
			continue;
		case '\t':
			do
			{
				putchar(WHITE, BLACK, ' ');
			} while (screen.Position.x % (screen.CharSize.x * 4) != 0);
			continue;
		case '\'':
			putchar(WHITE, BLACK, '\'');
			continue;
		case '\"':
			putchar(WHITE, BLACK, '\"');
			continue;
		case '\\':
			putchar(WHITE, BLACK, '\\');
			continue;
		default:
			break;
		}
		putchar(WHITE, BLACK, buffer[j]);
	}
	return i;
}

int color_printk(u32 frontColor, u32 backgroundColor, const char* format, ...)
{
	char buffer[1024] = {0};
	va_list args;
	va_start(args, format);
	int i = vsprintf(buffer, format, args);
	va_end(args);
	for (int j = 0; j < i; j++)
	{
		switch (buffer[j])
		{
		case '\r':
		case '\n':
			screen.Position.x = 0;
			screen.Position.y = screen.Position.y + screen.CharSize.y + 1;
			continue;
		case '\t':
			do
			{
				putchar(frontColor, backgroundColor, ' ');
			} while (screen.Position.x % (screen.CharSize.x * 4) != 0);
			continue;
		case '\'':
			putchar(frontColor, backgroundColor, '\'');
			continue;
		case '\"':
			putchar(frontColor, backgroundColor, '\"');
			continue;
		case '\\':
			putchar(frontColor, backgroundColor, '\\');
			continue;
		default:
			break;
		}
		putchar(frontColor, backgroundColor, buffer[j]);
	}
	return i;
}

int vsprintf(char* buffer, const char* format, va_list args)
{
	char* pbuffer = buffer;
	u8 flag_format = FALSE;
	u8 flag_long = FALSE;
	int precision = 0;
	while (*format != '\0')
	{
		if (*format == '%' && flag_format == FALSE)
		{
			flag_format = TRUE;
			format++;
			continue;
		}
		if (flag_format == TRUE)
		{
			switch (*format)
			{
			case '0'...'9':
			    precision *= 10;
				precision += *format - '0';
				break;
			case 'b':
				if (flag_long == TRUE)
				{
					pbuffer = itoa(pbuffer, va_arg(args, u64), 2, precision);
				}
				else
				{
					pbuffer = itoa(pbuffer, va_arg(args, u32), 2, precision);
				}
				flag_format = FALSE;
				flag_long = FALSE;
				precision = 0;
				break;

			case 'c':
				flag_format = FALSE;
				*pbuffer++ = (char)va_arg(args, u32);
				break;
			case 'd':
				if (flag_long == TRUE)
				{
					s64 n = va_arg(args, u64);
					if (n < 0)
					{
						*pbuffer++ = '-';
						pbuffer = itoa(pbuffer, -n, 10, precision);
					}
					else
					{
						pbuffer = itoa(pbuffer, n, 10, precision);
					}
				}
				else
				{
					s32 n = va_arg(args, u32);
					if (n < 0)
					{
						*pbuffer++ = '-';
						pbuffer = itoa_32(pbuffer, -n, precision);
					}
					else
					{
						pbuffer = itoa_32(pbuffer, n, precision);
					}
				}
				flag_format = FALSE;
				flag_long = FALSE;
				precision = 0;
				break;
			case 'l':
				flag_long = TRUE;
				break;
			case 'o':
				if (flag_long == TRUE)
				{
					pbuffer = itoa(pbuffer, va_arg(args, u64), 8, precision);
				}
				else
				{
					pbuffer = itoa(pbuffer, va_arg(args, u32), 8, precision);
				}
				flag_format = FALSE;
				flag_long = FALSE;
				precision = 0;
				break;
			case 's':
				flag_format = FALSE;
				char* str = (char*)va_arg(args, ptr_t);
				while (*str)
				{
					*pbuffer++ = *str;
					str++;
				}
				break;
			case 'u':
				if (flag_long == TRUE)
				{
					pbuffer = itoa(pbuffer, va_arg(args, u64), 10, precision);
				}
				else
				{
					pbuffer = itoa(pbuffer, va_arg(args, u32), 10, precision);
				}
				flag_format = FALSE;
				flag_long = FALSE;
				precision = 0;
				break;
			case 'x':
				if (flag_long == TRUE)
				{
					pbuffer = itoa(pbuffer, va_arg(args, u64), 16, precision);
				}
				else
				{
					pbuffer = itoa(pbuffer, va_arg(args, u32), 16, precision);
				}
				flag_format = FALSE;
				flag_long = FALSE;
				precision = 0;
				break;
			case '%':
			default:
				flag_format = FALSE;
				*pbuffer++ = '%';
				break;
			}
			format++;
			continue;
		}
		*pbuffer++ = *format;
		format++;
	}
	return pbuffer - buffer;
}

void putchar(u32 frontColor, u32 backgroundColor, u8 character)
{
	u32* base_addr = screen.bufferAddress;
	u16 x = screen.Position.x;
	u16 y = screen.Position.y;
	u16 res_x = screen.Resolution.x;
	u8* font = font_ascii[character];
	for (int i = 0; i < 16; i++)
	{
		u32* addr = base_addr + (y + i) * res_x + x;
		u8 mask = 0b10000000;
		for (int j = 0; j < 8; j++)
		{
			*(addr + j) = mask & font[i] ? frontColor : backgroundColor;
			mask >>= 1;
		}
	}
	screen.Position.x += 8;
	if (x == screen.Resolution.x)
	{
		screen.Position.y = screen.Position.y + screen.CharSize.y + 1;
		screen.Position.x = 0;
	}
}

/**
 * @param buffer 缓冲区指针
 * @param number 数字
 * @param base 基数
 * @param precision 精度
 * @param flags 标志
 * @return 转换后的字符串指针
 * @note PRIVATE
 */
PRIVATE char* itoa_32(char* buffer, u32 number, u8 precision)
{
	char temp_buffer[32];
	int n = 0;
	while (number > 0)
	{
		char temp = number % 10;
		temp_buffer[n] = temp < 10 ? '0' + temp : 'A' + temp - 10;
		n++;
		number /= 10;
	}
	while (precision > n)
	{
		temp_buffer[n] = '0';
		n++;
	}
	for (int i = 1; i <= n; i++)
	{
		*buffer = temp_buffer[n - i];
		buffer++;
	}
	return buffer;
}

/**
 * @param buffer 缓冲区指针
 * @param number 数字
 * @param base 基数
 * @param precision 精度
 * @param flags 标志
 * @return 转换后的字符串指针
 * @note PRIVATE
 */
PRIVATE char* itoa(char* buffer, u64 number, u8 base, u8 precision)
{
	char temp_buffer[64];
	int n = 0;
	while (number > 0)
	{
		char temp = number % base;
		temp_buffer[n] = temp < 10 ? '0' + temp : 'A' + temp - 10;
		n++;
		number /= base;
	}
	while (precision > n)
	{
		temp_buffer[n] = '0';
		n++;
	}
	for (int i = 1; i <= n; i++)
	{
		*buffer = temp_buffer[n - i];
		buffer++;
	}
	return buffer;
}