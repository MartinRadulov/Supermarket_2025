#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "utils.h"

using namespace std;

//void testCopy()
//{
//	std::cout << "testing copy:" << std::endl;
//
//	char dest[20];
//	strCopy(dest, "Hello world12345678911121314", 20);
//	std::cout << dest << std::endl;
//}
//
//void testStrEquals()
//{
//	std::cout << "testing equals:" << std::endl;
//
//	std::cout << "Hello world / Hello world :" << strCompare("Hello world", "Hello world") << std::endl;
//	std::cout << "Hello world / Hello world :" << strCompare("hEllO world", "Hello world") << std::endl;
//	std::cout << "Hello wolrd1 / Hello world2 :" << strCompare("Hello world1", "Hello world2") << std::endl;
//}
//
//void testTrimString()
//{
//	std::cout << "testing trim:" << std::endl;
//
//	char str[20] = " monkey   ";
//	char str2[20];
//	std::cout << str << std::endl;
//	trimStr(str);
//	std::cout << str << std::endl;
//}
//
//void testIsValidNumber()
//{
//	std::cout << "testing number:" << std::endl;
//
//	std::cout << "15 :" << isValidNumber("15") << std::endl;
//	std::cout << "-15 :" << isValidNumber("-15") << std::endl;
//	std::cout << "- :" << isValidNumber("-") << std::endl;
//	std::cout << "0.15 :" << isValidNumber("0.15") << std::endl;
//	std::cout << "hehe :" << isValidNumber("hehe") << std::endl;
//}
//
//void testIsValidFloat()
//{
//	std::cout << "testing float:" << std::endl;
//
//	std::cout << "15 :" << isValidFloat("15") << std::endl;
//	std::cout << "-15 :" << isValidFloat("-15") << std::endl;
//	std::cout << "- :" << isValidFloat("-") << std::endl;
//	std::cout << "0.15 :" << isValidFloat("0.15") << std::endl;
//	std::cout << "hehe :" << isValidFloat("hehe") << std::endl;
//	std::cout << ". :" << isValidFloat(".") << std::endl;
//	std::cout << ".1234 :" << isValidFloat(".1234") << std::endl;
//	std::cout << "5. :" << isValidFloat("5.") << std::endl;
//}

void testIsValidEmail() {
    cout << "=== Testing isValidEmail() ===" << endl;

    cout << "Valid emails:" << endl;
    cout << "  'user@domain.com': " << (isValidEmail("user@domain.com") ? "valid" : "invalid") << endl;
    cout << "  'test@gmail.bg': " << (isValidEmail("test@gmail.bg") ? "valid" : "invalid") << endl;
    cout << "  'a@b.co': " << (isValidEmail("a@b.co") ? "valid" : "invalid") << endl;

    cout << "Invalid emails:" << endl;
    cout << "  'notanemail': " << (isValidEmail("notanemail") ? "valid" : "invalid") << endl;
    cout << "  '@domain.com': " << (isValidEmail("@domain.com") ? "valid" : "invalid") << endl;
    cout << "  'user@': " << (isValidEmail("user@") ? "valid" : "invalid") << endl;
    cout << "  'user@@domain.com': " << (isValidEmail("user@@domain.com") ? "valid" : "invalid") << endl;
    cout << "  'user@domain': " << (isValidEmail("user@domain") ? "valid" : "invalid") << endl;

    cout << endl;
}

void testPasswordValidation() {
    cout << "=== Testing isValidPassword() ===" << endl;

    cout << "Valid passwords:" << endl;
    cout << "  'password123': " << (isValidPassword("password123") ? "valid" : "invalid") << endl;
    cout << "  'MyPass!': " << (isValidPassword("MyPass!") ? "valid" : "invalid") << endl;
    cout << "  'verylongpassword': " << (isValidPassword("verylongpassword") ? "valid" : "invalid") << endl;

    cout << "Invalid passwords:" << endl;
    cout << "  'short': " << (isValidPassword("short") ? "valid" : "invalid") << endl;
    cout << "  'pass word': " << (isValidPassword("pass word") ? "valid" : "invalid") << endl;
    cout << "  '': " << (isValidPassword("") ? "valid" : "invalid") << endl;
    cout << "  'password\\ttab': " << (isValidPassword("password\ttab") ? "valid" : "invalid") << endl;

    cout << endl;
}

void testUsernameValidation() {
    cout << "=== Testing isValidUsername() ===" << endl;

    cout << "Valid usernames:" << endl;
    cout << "  'user123': " << (isValidUsername("user123") ? "valid" : "invalid") << endl;
    cout << "  'test_user': " << (isValidUsername("test_user") ? "valid" : "invalid") << endl;
    cout << "  'UserName': " << (isValidUsername("UserName") ? "valid" : "invalid") << endl;
    cout << "  'abc': " << (isValidUsername("abc") ? "valid" : "invalid") << endl;

    cout << "Invalid usernames:" << endl;
    cout << "  '123user': " << (isValidUsername("123user") ? "valid" : "invalid") << endl;
    cout << "  'ab': " << (isValidUsername("ab") ? "valid" : "invalid") << endl;
    cout << "  'user-name': " << (isValidUsername("user-name") ? "valid" : "invalid") << endl;
    cout << "  'user name': " << (isValidUsername("user name") ? "valid" : "invalid") << endl;
    cout << "  'user@domain': " << (isValidUsername("user@domain") ? "valid" : "invalid") << endl;

    cout << endl;
}

void testProductNameValidation() {
    cout << "=== Testing isValidProductName() ===" << endl;

    cout << "Valid product names:" << endl;
    cout << "  'Bread': " << (isValidProductName("Bread") ? "valid" : "invalid") << endl;
    cout << "  'Coca Cola': " << (isValidProductName("Coca Cola") ? "valid" : "invalid") << endl;
    cout << "  'Product-123': " << (isValidProductName("Product-123") ? "valid" : "invalid") << endl;
    cout << "  'Milk 3.5': " << (isValidProductName("Milk 3.5") ? "valid" : "invalid") << endl;

    cout << "Invalid product names:" << endl;
    cout << "  'A': " << (isValidProductName("A") ? "valid" : "invalid") << endl;
    cout << "  '   ': " << (isValidProductName("   ") ? "valid" : "invalid") << endl;
    cout << "  '': " << (isValidProductName("") ? "valid" : "invalid") << endl;
    cout << "  'Product@special': " << (isValidProductName("Product@special") ? "valid" : "invalid") << endl;

    cout << endl;
}

void testPriceValidation() {
    cout << "=== Testing isValidPrice() ===" << endl;

    cout << "Valid prices:" << endl;
    cout << "  '12.50': " << (isValidPrice("12.50") ? "valid" : "invalid") << endl;
    cout << "  '0.99': " << (isValidPrice("0.99") ? "valid" : "invalid") << endl;
    cout << "  '150': " << (isValidPrice("150") ? "valid" : "invalid") << endl;
    cout << "  '5.5': " << (isValidPrice("5.5") ? "valid" : "invalid") << endl;
    cout << "  '0.01': " << (isValidPrice("0.01") ? "valid" : "invalid") << endl;

    cout << "Invalid prices:" << endl;
    cout << "  '12.555': " << (isValidPrice("12.555") ? "valid" : "invalid") << endl;
    cout << "  '01.50': " << (isValidPrice("01.50") ? "valid" : "invalid") << endl;
    cout << "  '.50': " << (isValidPrice(".50") ? "valid" : "invalid") << endl;
    cout << "  '12.': " << (isValidPrice("12.") ? "valid" : "invalid") << endl;
    cout << "  '-5.50': " << (isValidPrice("-5.50") ? "valid" : "invalid") << endl;
    cout << "  'abc': " << (isValidPrice("abc") ? "valid" : "invalid") << endl;

    cout << endl;
}

void testRealWorldScenarios() {
    cout << "=== Real world scenarios from supermarket ===" << endl;

    // Scenario 1: Cashier registration
    cout << "Scenario 1 - Cashier registration:" << endl;
    const char* username = "ivan_cashier";
    const char* email = "ivan@supermarket.bg";
    const char* password = "mypassword123";

    cout << "  Username '" << username << "': " <<
        (isValidUsername(username) ? "?" : "?") << endl;
    cout << "  Email '" << email << "': " <<
        (isValidEmail(email) ? "?" : "?") << endl;
    cout << "  Password '" << password << "': " <<
        (isValidPassword(password) ? "?" : "?") << endl;

    // Scenario 2: Adding product
    cout << "Scenario 2 - Adding product:" << endl;
    const char* productName = "Milk Vereia 3.6%";
    const char* price = "2.89";

    cout << "  Product name '" << productName << "': " <<
        (isValidProductName(productName) ? "?" : "?") << endl;
    cout << "  Price '" << price << "': " <<
        (isValidPrice(price) ? "?" : "?") << endl;

    // Scenario 3: Common mistakes
    cout << "Scenario 3 - Common mistakes:" << endl;
    const char* badUsername = "123ivan";  // Starts with digit
    const char* badEmail = "ivan@";       // No domain
    const char* badPrice = "12.345";     // Too many digits after dot

    cout << "  Bad username '" << badUsername << "': " <<
        (isValidUsername(badUsername) ? "?" : "?") << endl;
    cout << "  Bad email '" << badEmail << "': " <<
        (isValidEmail(badEmail) ? "?" : "?") << endl;
    cout << "  Bad price '" << badPrice << "': " <<
        (isValidPrice(badPrice) ? "?" : "?") << endl;

    cout << endl;
}

int main()
{
	/*testCopy();

	testStrEquals();

	testTrimString();

	testIsValidNumber();

	testIsValidFloat();*/

    testIsValidEmail();
    testPasswordValidation();
    testUsernameValidation();
    testProductNameValidation();
    testPriceValidation();
    testRealWorldScenarios();

}