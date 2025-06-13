#pragma once

void strcpy(char* dest, const char* src, int maxSize);

void strCompareNoCase(const char* str1, const char* str2);

void trimString(char* str);

bool isValidNUmber(const char* str);

void getCurrentDate(char* dateStr, int maxSize);

void formatDate(char* foramtDate, int maxSize);

bool isValidEmail(const char* email);

bool isValidPassword(const char* password);

bool isValidPrice(const char* priceStr);

bool isValidFloat(const char* str);

int getStrLength(const char* str);

char toLowerCase(char c);

char toUpperCase(char c);

bool isWhitespace(char c);

bool isDigitChar(char c);
