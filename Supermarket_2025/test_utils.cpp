#include <iostream>
#include "utils.h"


void testCopy()
{
	std::cout << "testing copy:" << std::endl;

	char dest[20];
	strCopy(dest, "Hello world12345678911121314", 20);
	std::cout << dest << std::endl;
}

void testStrEquals()
{
	std::cout << "testing equals:" << std::endl;

	std::cout << "Hello world / Hello world :" << strCompare("Hello world", "Hello world") << std::endl;
	std::cout << "Hello world / Hello world :" << strCompare("hEllO world", "Hello world") << std::endl;
	std::cout << "Hello wolrd1 / Hello world2 :" << strCompare("Hello world1", "Hello world2") << std::endl;
}

void testTrimString()
{
	std::cout << "testing trim:" << std::endl;

	char str[20] = " monkey   ";
	char str2[20];
	std::cout << str << std::endl;
	trimStr(str);
	std::cout << str << std::endl;
}

void testIsValidNumber()
{
	std::cout << "testing number:" << std::endl;

	std::cout << "15 :" << isValidNumber("15") << std::endl;
	std::cout << "-15 :" << isValidNumber("-15") << std::endl;
	std::cout << "- :" << isValidNumber("-") << std::endl;
	std::cout << "0.15 :" << isValidNumber("0.15") << std::endl;
	std::cout << "hehe :" << isValidNumber("hehe") << std::endl;
}

void testIsValidFloat()
{
	std::cout << "testing float:" << std::endl;

	std::cout << "15 :" << isValidFloat("15") << std::endl;
	std::cout << "-15 :" << isValidFloat("-15") << std::endl;
	std::cout << "- :" << isValidFloat("-") << std::endl;
	std::cout << "0.15 :" << isValidFloat("0.15") << std::endl;
	std::cout << "hehe :" << isValidFloat("hehe") << std::endl;
	std::cout << ". :" << isValidFloat(".") << std::endl;
	std::cout << ".1234 :" << isValidFloat(".1234") << std::endl;
	std::cout << "5. :" << isValidFloat("5.") << std::endl;
}

int main()
{
	testCopy();

	testStrEquals();

	testTrimString();

	testIsValidNumber();

	testIsValidFloat();

}