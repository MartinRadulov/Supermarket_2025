#include <iostream>
#include <ctime>
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

bool strCompare(const char* str1, const char* str2)
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
	return *str1 == '\0' && *str2 == '\0';
}

void trimStr(char* str)
{
	if (str == nullptr)
	{
		return;
	}

	int start = 0;
	while (*str != '\0' && isWhitespace(str[start]))
	{
		start++;
	}

	int end = getStrLength(str) - 1;
	while (end >= start && isWhitespace(str[end]))
	{
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

bool isValidNumber(const char* str)
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

void intToString(int number, char* str, int maxSize)
{
	if (str == nullptr || maxSize <= 0)
	{
		return;
	}

	int startPos = 0;
	bool isNegative = false;
	if (number < 0)
	{
		isNegative = true;
		str[0] = '-';
		startPos++;
		number = -number;
	}

	if (number == 0)
	{
		if (maxSize >= 2)
		{
			str[0] = '0';
			str[1] = '\0';
		}
		return;
	}

	char temp[20]; 
	int tempIndex;
	while (number > 0 && tempIndex < 19)
	{
		temp[tempIndex] = '0' + number % 10;
		number  = number / 10;
		tempIndex++;
	}

	for (int j = tempIndex - 1; j >= 0, startPos < maxSize - 1; j--, startPos++)
	{
		str[startPos] = temp[j];
	}

	str[startPos] = '\0';
}

void toTwoDigits(int number, char* str)
{
	if (str == nullptr)
	{
		return;
	}

	if (number < 0)
	{
		number = 0;
	}
	if (number > 99)
	{
		number = 99;
	}

	str[0] = '0' + number / 10;
	str[1] = '0' + number % 10;
	str[2] = '\0';
}

void getCurrentDate(char* dateStr, int maxSize)
{
	if (dateStr == nullptr || maxSize < DATE_SIZE)
	{
		return;
	}

	time_t currentTime = time(nullptr);
	struct tm* localTime = localtime(&currentTime);
	if (localTime == nullptr)
	{
		strCopy(dateStr, "00/00/0000");
		return;
	}

	char day[3], month[3], year[5];
	dateStr[0] = '\0';
	
	toTwoDigits(localTime->tm_mday, day);
	toTwoDigits(localTime->tm_mon + 1, month);
	intToString(localTime->tm_year + 1900, year, 5);

	int startPos = 0;
	dateStr[startPos++] = day[0];
	dateStr[startPos++] = day[1];
	dateStr[startPos++] = '/';

	dateStr[startPos++] = month[0];
	dateStr[startPos++] = month[1];
	dateStr[startPos++] = '/';

	for (int i = 0; year[i] != '\0' && dateStr[startPos] < maxSize - 1; i++, startPos++)
	{
		dateStr[startPos] = year[i];
	}

	dateStr[startPos] = '\0';
}

void getCurrentTime(char* timeStr, int maxSize)
{
	if (timeStr == nullptr || maxSize < TIME_SIZE)
	{
		return;
	}

	time_t currentTime = time(nullptr);
	struct tm* localTime = localtime(&currentTime);
	if (localTime == nullptr)
	{
		strCopy(dateStr, "00/00/0000 00:00:00");
		return;
	}

	char day[3], month[3], year[5];
	char sec[3], min[3], hour[3];
	timeStr[0] = '\0';

	toTwoDigits(localTime->tm_mday, day);
	toTwoDigits(localTime->tm_mon + 1, month);
	intToString(localTime->tm_year + 1900, year, 5);
	toTwoDigits(localTime->tm_sec, sec);
	toTwoDigits(localTime->tm_min, min);
	toTwoDigits(localTime->tm_hour, hour);

	int startPos = 0;
	timeStr[startPos++] = day[0];
	timeStr[startPos++] = day[1];
	timeStr[startPos++] = '/';

	timeStr[startPos++] = month[0];
	timeStr[startPos++] = month[1];
	timeStr[startPos++] = '/';

	for (int i = 0; year[i] != '\0' && timeStr[startPos] < maxSize - 1; i++, startPos++)
	{
		timeStr[startPos] = year[i];
	}

	timeStr[startPos++] = sec[0];
	timeStr[startPos++] = sec[1];
	timeStr[startPos++] = ':';

	timeStr[startPos++] = min[0];
	timeStr[startPos++] = min[1];
	timeStr[startPos++] = ':';

	timeStr[startPos++] = hour[0];
	timeStr[startPos++] = hour[1];

	timeStr[startPos] = '\0';
}

bool isValidEmail(const char* email)
{
	if (email == nullptr || getStrLength(email) < 5)
	{
		return false;
	}

	int emailSize = getStrLength(email);

	if (email[0] == '.' || email[0] == '@' || email[emailSize - 1] == '@' || email[emailSize - 1] == '.')
	{
		return false;
	}

	size_t atCount = 0, afterAt = 0;
	size_t atPos = 0;
	bool atFound = false;
	for (int i = 0; i < emailSize; i++)
	{
		if (email[i] == '@')
		{
			if (email[i - 1] == '.')
			{
				return false;
			}
			atFound = true;
			atCount++;
			atPos = i + 1;
		}

		char c = email[i];
		if (c == ' ' || c == '\r' || c == '\t' || c == '\n')
		{
			return false;
		}

		if (c == '.' && email[i - 1] == '.')
		{
			return false;
		}
	}

	if (email[atPos] == '.')
	{
		return false;
	}

	bool dotFound = false;
	for (int i = atPos; i < emailSize; i++)
	{
		if (email[i] == '.')
		{
			dotFound = true;
		
		}
		afterAt++;
	}

	if (!atFound || !dotFound)
	{
		return false;
	}

	if (atCount != 1  || afterAt < 3)
	{
		return false;
	}

	return true;
}
