#pragma once

const int DATE_SIZE = 11, TIME_SIZE = 20;

void strCopy(char* dest, const char* str, int maxSize);

bool strCompare(const char* str1, const char* str2);

void trimStr(char* str);

bool isValidNumber(const char* str);

void getCurrentDate(char* dateStr, int maxSize);

void getCurrentTime(char* timeStr, int maxSize);

bool isValidEmail(const char* email);

bool isValidPassword(const char* password);

bool isValidPrice(const char* priceStr);

bool isValidFloat(const char* str);

int getStrLength(const char* str);

char toLowerCase(char c);

//char toUpperCase(char c);

bool isWhitespace(char c);

bool isDigitChar(char c);

void intToString(int number, char* str, int maxSize);

void toTwoDigits(int number, char* str);

bool isValidUsername(const char* username);

bool isValidProductName(const char* product);
