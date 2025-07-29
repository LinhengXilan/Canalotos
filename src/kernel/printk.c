/**
 * @file: kernel/printk.c
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/


#include <kernel/types.h>
#include <kernel/const.h>
#include <kernel/global.h>
#include <kernel/font.h>
#include <kernel/init.h>

#include <kernel/printk.h>

PRIVATE char* _itoa(const char* buffer, u64 number, int base, u8 precision, u8 flags);

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

int _printk(const char* format, ...)
{
	char buffer[1024] = {0};
	va_list args;
	va_start(args, format);
	int i = _vsprintf(buffer, format, args);
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
				_putchar(WHITE, BLACK, ' ');
			} while (screen.Position.x % (screen.CharSize.x * 4) != 0);
			continue;
		case '\'':
			_putchar(WHITE, BLACK, '\'');
			continue;
		case '\"':
			_putchar(WHITE, BLACK, '\"');
			continue;
		case '\\':
			_putchar(WHITE, BLACK, '\\');
			continue;
		default:
			break;
		}
		_putchar(WHITE, BLACK, buffer[j]);
	}
	return i;
}

int _color_printk(u32 frontColor, u32 backgroundColor, const char* format, ...)
{
	char buffer[1024] = {0};
	va_list args;
	va_start(args, format);
	int i = _vsprintf(buffer, format, args);
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
				_putchar(frontColor, backgroundColor, ' ');
			} while (screen.Position.x % (screen.CharSize.x * 4) != 0);
			continue;
		case '\'':
			_putchar(frontColor, backgroundColor, '\'');
			continue;
		case '\"':
			_putchar(frontColor, backgroundColor, '\"');
			continue;
		case '\\':
			_putchar(frontColor, backgroundColor, '\\');
			continue;
		default:
			break;
		}
		_putchar(frontColor, backgroundColor, buffer[j]);
	}
	return i;
}

int _vsprintf(char* buffer, const char* format, va_list args)
{
	char* pbuffer = buffer;
	u8 flag_format = FALSE;
	u8 flag_long = FALSE;
	u8 flag = 0;
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
					pbuffer = _itoa(pbuffer, va_arg(args, u64), 2, precision, TRUE);
					flag_format = FALSE;
					flag_long = FALSE;
				    precision = 0;
					break;
				}
				pbuffer = _itoa(pbuffer, va_arg(args, u32), 2, precision, TRUE);
				flag_format = FALSE;
			    precision = 0;
				break;
			case 'c':
				flag_format = FALSE;
				*pbuffer++ = (char)va_arg(args, u32);
				break;
			case 'd':
				if (flag_long == TRUE)
				{
					pbuffer = _itoa(pbuffer, va_arg(args, u64), 10, precision, flag);
					flag_format = FALSE;
					flag_long = FALSE;
					flag = FALSE;
				    precision = 0;
					break;
				}
				pbuffer = _itoa(pbuffer, va_arg(args, u32), 10, precision, flag);
				flag_format = FALSE;
				flag = FALSE;
			    precision = 0;
				break;
			case 'l':
				flag_long = TRUE;
				break;
			case 'o':
				if (flag_long == TRUE)
				{
					pbuffer = _itoa(pbuffer, va_arg(args, u64), 8, precision, TRUE);
					flag_format = FALSE;
					flag_long = FALSE;
				    precision = 0;
					break;
				}
				pbuffer = _itoa(pbuffer, va_arg(args, u32), 8, precision, TRUE);
				flag_format = FALSE;
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
				flag |= 1;
				break;
			case 'x':
				if (flag_long == TRUE)
				{
					pbuffer = _itoa(pbuffer, va_arg(args, u64), 16, precision, TRUE);
					flag_format = FALSE;
					flag_long = FALSE;
				    precision = 0;
					break;
				}
				pbuffer = _itoa(pbuffer, va_arg(args, u32), 16, precision, TRUE);
				flag_format = FALSE;
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

void _putchar(u32 frontColor, u32 backgroundColor, u8 character)
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

PRIVATE char* _itoa(const char* buffer, u64 number, int base, u8 precision, u8 flags)
{
    u64 temp = number;
	s64 stemp = (s64)number;
	char temp_buffer[64] = {0};
	char* p_buffer = buffer;
	char* p_temp_buffer = temp_buffer;
	u8 size = 0;
    if (temp == 0)
    {
        *p_buffer++ = '0';
    }
	else if (stemp < 0 &&flags | 1 == 0 && base == 10)
	{
		*p_buffer++ = '-';
		stemp = -stemp;
		while (stemp > 0)
		{
			int rem = (int)(stemp % base);
			stemp /= base;
			*p_temp_buffer++ = rem < 10 ? rem + '0' : rem + 'A' - 10;
			size++;
		}
	}
	else
	{
		while (temp > 0)
		{
			int rem = (int)(temp % base);
			temp /= base;
			*p_temp_buffer++ = rem < 10 ? rem + '0' : rem + 'A' - 10;
			size++;
		}
	}
	if (precision > size)
	{
		precision -= size;
		while (precision > 0)
		{
			*p_temp_buffer++ = '0';
			precision--;
		}
	}
	while (p_temp_buffer > temp_buffer)
	{
		p_temp_buffer--;
		*p_buffer++ = *p_temp_buffer;
	}
	return p_buffer;
}