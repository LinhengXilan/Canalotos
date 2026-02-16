/**
 * @file Lib/Print.cpp
 * @author LinhengXilan
 * @version 0.0.1.8
 * @date 2026-2-16
 */

#include <Lib/Print.h>

namespace Lib
{
	uint8_t printf(char* buffer, const char* string, ...)
	{
		char lbuffer[256];
		va_list args;
		va_start(args, string);
		uint8_t length = format(lbuffer, string, args);
		va_end(args);
		for (int i = 0; i < length; i++)
		{
			buffer[i] = lbuffer[i];
		}
		return length;
	}

	uint8_t format(char* buffer, const char* format, va_list args)
	{
		char* l_Buffer = buffer;
		bool flagFormat = false;
		bool flagLong = false;
		uint8_t precision = 0;
		while (*format != '\0')
		{
			if (*format == '%' && flagFormat == false)
			{
				flagFormat = true;
				format++;
				continue;
			}
			if (flagFormat)
			{
				switch (*format)
				{
				case '0'...'9':
				    precision *= 10;
					precision += *format - '0';
					break;
				case 'b':
					if (flagLong)
					{
						l_Buffer = itoa(l_Buffer, va_arg(args, uint64_t), 2, precision);
					}
					else
					{
						l_Buffer = itoa(l_Buffer, static_cast<uint32_t>(va_arg(args, uint64_t)), 2, precision);
					}
					flagFormat = false;
					flagLong = false;
					precision = 0;
					break;
				case 'c':
					flagLong = false;
					*l_Buffer++ = static_cast<char>(va_arg(args, uint32_t));
					break;
				case 'd':
					if (flagLong)
					{
						int64_t n = va_arg(args, int64_t);
						if (n < 0)
						{
							*l_Buffer++ = '-';
							l_Buffer = itoa(l_Buffer, -n, 10, precision);
						}
						else
						{
							l_Buffer = itoa(l_Buffer, n, 10, precision);
						}
					}
					else
					{
						int32_t n = va_arg(args, int32_t);
						if (n < 0)
						{
							*l_Buffer++ = '-';
							l_Buffer = itoa(l_Buffer, -n, 10, precision);
						}
						else
						{
							l_Buffer = itoa(l_Buffer, n, 10, precision);
						}
					}
					flagFormat = false;
					flagLong = false;
					precision = 0;
					break;
				case 'l':
					flagLong = true;
					break;
				case 'o':
					if (flagLong)
					{
						l_Buffer = itoa(l_Buffer, va_arg(args, uint64_t), 8, precision);
					}
					else
					{
						l_Buffer = itoa(l_Buffer, va_arg(args, uint32_t), 8, precision);
					}
					flagFormat = false;
					flagLong = false;
					precision = 0;
					break;
				case 's':
					{
						flagFormat = false;
						char* str = reinterpret_cast<char*>(va_arg(args, address_t));
						while (*str)
						{
							*l_Buffer++ = *str;
							str++;
						}
						break;
					}
				case 'u':
					if (flagLong)
					{
						l_Buffer = itoa(l_Buffer, va_arg(args, uint64_t), 10, precision);
					}
					else
					{
						l_Buffer = itoa(l_Buffer, va_arg(args, uint32_t), 10, precision);
					}
					flagFormat = false;
					flagLong = false;
					precision = 0;
					break;
				case 'x':
					if (flagLong == true)
					{
						l_Buffer = itoa(l_Buffer, va_arg(args, uint64_t), 16, precision);
					}
					else
					{
						l_Buffer = itoa(l_Buffer, va_arg(args, uint32_t), 16, precision);
					}
					flagFormat = false;
					flagLong = false;
					precision = 0;
					break;
				case '%':
				default:
					flagFormat = false;
					*l_Buffer++ = '%';
					break;
				}
				format++;
				continue;
			}
			*l_Buffer++ = *format;
			format++;
		}
		return l_Buffer - buffer;
	}

	char* itoa(char* buffer, uint64_t number, uint8_t base, uint8_t precision)
	{
		char tempBuffer[64];
		uint64_t tempNumber = number;
		int n = 0;
		while (tempNumber > 0)
		{
			char mod = static_cast<char>(tempNumber % base);
			tempBuffer[n++] = mod < 10 ? '0' + mod : 'A' + mod - 10;
			tempNumber /= base;
		}
		while (precision > n)
		{
			tempBuffer[n++] = '0';
		}
		for (int i = 1; i <= n; i++)
		{
			*buffer = tempBuffer[n - i];
			buffer++;
		}
		return buffer;
	}
}
