#include <iostream>
#include "utils.h"

int getStrLength(const char* str)
{
	if (str == nullptr)
	{
		return 0;
	}

	int length = 0;
	while (*str != '\0')
	{
		length++;
		str++;
	}
	
	return length;
}

char toLowerCase(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		c = c + ('a' - 'A');
	}

	return c;
}

bool isWhitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

bool isDigitChar(char c)
{
	return (c >= '0' && c <= '9');
}

void strCopy(char* dest, const char* str, int maxSize)
{
	if (str == nullptr || dest == nullptr || maxSize <= 0)
	{
		return;
	}

	int tempMax = 0;
	while (tempMax < maxSize - 1 && *str != '\0')
	{
		*dest = *str;
		dest++;
		str++;
		tempMax++;
	}
	*dest = '\0';
}

bool strCompareNoCase(const char* str1, const char* str2)
{
	if (str1 == nullptr || str2 == nullptr)
	{
		return false;
	}

	while (*str1 != '\0' && *str2 != '\0')
	{
		if (toLowerCase(*str1) != toLowerCase(*str2))
		{
			return false;
		}
		str1++;
		str2++;
	}
	return *str1 == '\0' && *str2 == '0';
}

void trimStr(char* str)
{
	if (str == nullptr)
	{
		return;
	}

	int start = 0;
	while (*str != '\0' && isWhitespace(*str))
	{
		str++;
		start++;
	}

	int end = getStrLength(str) - 1;
	while (end >= start && isWhitespace(str[end]))
	{
		str--;
		end--;
	}

	int temp = 0;
	for (int i = start; i <= end; i++)
	{
		str[temp] = str[i];
		temp++;
	}
	str[temp] = '\0';
}

bool isValidNUmber(const char* str)
{
	if (str == nullptr || *str == '\0')
	{
		return false;
	}

	if (*str == '+' || *str == '-')
	{
		str++;
	}

	if (*str == '\0')
	{
		return false;
	}

	while (*str != '\0')
	{
		if (!isDigitChar(*str))
		{
			return false;
		}
		str++;
	}

	return true;
}

bool isValidFloat(const char* str)
{
	if (str == nullptr || *str == '\0')
	{
		return false;
	}

	if (*str == '+' || *str == '-')
	{
		str++;
	}

	if (*str == '\0')
	{
		return false;
	}

	bool hasDecimal = false;
	bool hasNumber = false;
	bool hasDigitAfter = false;
	while (*str != '\0')
	{
		if (isDigitChar(*str))
		{
			hasNumber = true;
			str++;
			if (hasDecimal)
			{
				hasDigitAfter = true;
			}
		}
		else if (*str == '.' && !hasDecimal && hasNumber)
		{	
			hasDecimal = true;
			str++;
		}
		else
		{
			return false;
		}
	}

	return (hasDigitAfter || !hasDecimal);
}

