#include "workers.h"
#define _CRT_SECURE_NO_WARNINGS

static int nextWorkerId = 1000;

Worker::Worker()
{
	id = 0;
	firstName[0] = '\0';
	lastName[0] = '\0';
	phone[0] = '\0';
	age = 0;
	password[0] = '\0';
	isApprovedV = false;
	isLoggedInV = false;
}

Worker::Worker(int workerId, const char* fName, const char* lName, const char* phoneNum, int workerAge, const char* pwd)
{
	id = workerId;
	strCopy(firstName, fName, sizeof(firstName));
	strCopy(lastName, lName, sizeof(lastName));
	strCopy(phone, phoneNum, sizeof(phone));
	strCopy(password, pwd, sizeof(password));
	age = workerAge;
	isApprovedV = false;
	isLoggedInV = false;
}

Worker::~Worker()
{

}

bool Worker::authenticate(const char* pwd) const
{
	return strCompare(password, pwd);
}

void Worker::saveToFile(std::ofstream &file) const
{
	file <<id << "\n";
	file << firstName << "\n";
	file << lastName << "\n";
	file << phone << "\n";
	file << age << "\n";
	file << password << "\n";
	file << isApprovedV << "\n";
	file << isLoggedInV << "\n";
}

void Worker::loadFromFile(std::ifstream& file)
{
	file >> id;
	file.ignore();
	file.getline(firstName, sizeof(firstName));
	file.getline(lastName, sizeof(lastName));
	file.getline(password, sizeof(password));
	file.getline(phone, sizeof(phone));
	file >> age;
	file.ignore();
	file >> isApprovedV;
	file.ignore();
	isLoggedInV = false;
}

void Worker::setPassword(const char* newPassword)
{
	strCopy(password, newPassword, sizeof(password));
}

void Worker::getFullName(char* fullName, int maxSize) const
{
	strCopy(fullName, firstName, maxSize);

	int i = 0;
	while (fullName[i] != '\0' && i < maxSize - 1)
	{
		i++;
	}
	if (i < maxSize - 1)
	{
		fullName[i] = ' ';
		i++;
		strCopy(fullName + i, lastName, maxSize - i);
	}
}

int Worker::generateNewId()
{
	return nextWorkerId++;
}

Warning::Warning()
{
	points = 0;
	reason[0] = '\0';
	date[0] = '/0';
}

Warning::Warning(int warningPoints,const char* warningReason)
{
	points = warningPoints;
	strCopy(reason, warningReason, sizeof(reason));
	getCurrentDate(date, sizeof(date));
}

void Warning::saveToFile(std::ofstream& file) const
{
	file << points << "\n";
	file << reason << "\n";
	file << date << "\n";
}

void Warning::loadFromFile(std::ifstream& file)
{
	file >> points;
	file.ignore();
	file.getline(reason, sizeof(reason));
	file.getline(date, sizeof(date));
}

Cashier::Cashier() : Worker()
{
	transactionCount = 0;
	warnings = nullptr;
	warningCount = 0;
	warningCapacity = 0;
	totalWarningPoints = 0;
}

Cashier::Cashier(int workerId, const char* fName, const char* lName, const char* phoneNum, int workerAge, const char* pwd)
	: Worker(workerId, fName, lName, phoneNum, workerAge, pwd)
{
	transactionCount = 0;
	warnings = nullptr;
	warningCount = 0;
	warningCapacity = 0;
	totalWarningPoints = 0;
}

Cashier::~Cashier()
{
	clearWarnings();
}

Cashier::Cashier(const Cashier& other) : Worker(other)
{
	transactionCount = other.transactionCount;
	warningCount = other.warningCount;
	warningCapacity = other.warningCapacity;
	totalWarningPoints = other.totalWarningPoints;

	warnings = nullptr;
	copyWarnings(other);
}

Cashier& Cashier::operator = (const Cashier& other)
{
	if (this != &other) {
		Worker::operator = (other);
		clearWarnings();

		transactionCount = other.transactionCount;
		warningCount = other.warningCount;
		warningCapacity = other.warningCapacity;
		totalWarningPoints = other.totalWarningPoints;

		copyWarnings(other);
	}
	return *this;
}

Cashier::Cashier(Cashier&& other) noexcept : Worker(std::move(other))
{
	warnings = other.warnings;
	warningCount = other.warningCount;
	warningCapacity = other.warningCapacity;
	totalWarningPoints = other.totalWarningPoints;
	transactionCount = other.transactionCount;

	other.warnings = nullptr;
	other.warningCount = 0;
	other.warningCapacity = 0;
	other.totalWarningPoints = 0;
	other.transactionCount = 0;
}

Cashier& Cashier::operator = (Cashier&& other) noexcept {
	if (this != &other) {
		clearWarnings();

		Worker::operator=(std::move(other));

		warnings = other.warnings;
		warningCount = other.warningCount;
		warningCapacity = other.warningCapacity;
		totalWarningPoints = other.totalWarningPoints;
		transactionCount = other.transactionCount;

		// Reset the source object:
		other.warnings = nullptr;
		other.warningCount = 0;
		other.warningCapacity = 0;
		other.totalWarningPoints = 0;
		other.transactionCount = 0;
	}
	return *this;
}

const char* Cashier::getRole() const
{
	return "Cashier";
}

void Cashier::displayInfo() const
{
	char fullName[100];
	getFullName(fullName, sizeof(fullName));

	std::cout << "CASHIER INFO: " << std::endl;
	std::cout << "ID: " << id << std::endl;
	std::cout << "Name: " << fullName << std::endl;
	std::cout << "Phone: " << phone << std::endl;
	std::cout << "Age: " << age << std::endl;
	std::cout << "Transactions: " << transactionCount << std::endl;
	std::cout << "Warning Points: " << totalWarningPoints << std::endl;
	std::cout << "Status: " << (isApprovedV ? "Approved" : "Pending") << std::endl;

	if (warningCount > 0)
	{
		std::cout << "Warnings:" << std::endl;
		for (int i = 0; i < warningCount; i++)
		{
			std::cout << warnings[i].getPoints() << "points, "
				<< warnings[i].getReason() << " (" << warnings[i].getDate() << ")" << std::endl;
		}
	}
}

bool Cashier::hasSpecialPrivileges() const
{
	return false;
}

void Cashier::saveToFile(std::ofstream& file) const
{
	file << "Cachier\n";
	Worker::saveToFile(file);
	file << transactionCount << "\n";
	file << warningCount << "\n";
	file << totalWarningPoints << "\n";

	for (int i = 0; i < warningCount; i++)
	{
		warnings[i].saveToFile(file);
	}
}

void Cashier::loadFromFile(std::ifstream& file)
{
	Worker::loadFromFile(file);
	file >> transactionCount;
	file >> warningCount;
	file >> totalWarningPoints;
	file.ignore();

	clearWarnings();
	if (warningCount > 0)
	{
		warningCapacity = warningCount;
		warnings = new Warning[warningCapacity];

		for (int i = 0; i < warningCount; i++) {
			warnings[i].loadFromFile(file);
		}
	}
}

void Cashier::addWarning(int points, const char* reason)
{
	if (warningCount >= warningCapacity)
	{
		resizeWarningArray();
	}

	warnings[warningCount] = Warning(points, reason);
	warningCount++;
	totalWarningPoints += points;
}

bool Cashier::canPromoteToManager() const
{
	return transactionCount >= 50 && totalWarningPoints < 100;
}

bool Cashier::shouldBeFired() const
{
	return totalWarningPoints >= 300;
}

void Cashier::resizeWarningArray()
{
	int newCapacity = (warningCapacity == 0) ? 2 : warningCapacity * 2;
	Warning* newWarnings = new Warning[newCapacity];

	for (int i = 0; i < warningCount; i++)
	{
		newWarnings[i] = warnings[i];
	}
	delete[] warnings;
	warnings = newWarnings;
	warningCapacity = newCapacity;
}

void Cashier::copyWarnings(const Cashier& other)
{
	if (other.warningCount > 0)
	{
		warningCapacity = other.warningCapacity;
		warnings = new Warning[warningCapacity];

		for (int i = 0; i < other.warningCount; i++)
		{
			warnings[i] = other.warnings[i];
		}
	}
}

void Cashier::clearWarnings()
{
	delete[] warnings;
	warnings = nullptr;
	warningCount = 0;
	warningCapacity = 0;
}

Manager::Manager() : Worker()
{
	specialCode[0] = '\0';
	codeIsValid = false;
}

Manager::Manager(int workerId, const char* fName, const char* lName,
	const char* phoneNum, int workerAge, const char* pwd)
	: Worker(workerId, fName, lName, phoneNum, workerAge, pwd) {
	specialCode[0] = '\0';
	codeIsValid = false;
	generateSpecialCode();
}

Manager::~Manager()
{

}

const char* Manager::getRole() const
{
	return "Manager";
}

void Manager::displayInfo() const
{
	char fullName[100];
	getFullName(fullName, sizeof(fullName));

	std::cout << "MANAGER INFO" << std::endl;
	std::cout << "ID: " << id << std::endl;
	std::cout << "Name: " << fullName << std::endl;
	std::cout << "Phone: " << phone << std::endl;
	std::cout << "Age: " << age << std::endl;
	std::cout << "Special Code: " << (codeIsValid ? specialCode : "INVALID") << std::endl;
	std::cout << "Status: " << (isApprovedV ? "Approved" : "Pending") << std::endl;
}

bool Manager::hasSpecialPrivileges() const
{
	return codeIsValid;
}

void Manager::saveToFile(std::ofstream& file) const
{
	file << "MANAGER\n";
	Worker::saveToFile(file);
	file << specialCode << "\n";
	file << codeIsValid << "\n";
}

void Manager::loadFromFile(std::ifstream& file)
{
	Worker::loadFromFile(file);
	file.getline(specialCode, sizeof(specialCode));
	file >> codeIsValid;
	file.ignore();
}

void Manager::generateSpecialCode()
{
	specialCode[0] = generateRandomDigit();
	specialCode[1] = generateRandomUppercase();
	specialCode[1] = generateRandomUppercase();
	specialCode[2] = generateRandomUppercase();
	specialCode[3] = generateRandomDigit();
	specialCode[4] = generateRandomDigit();
	specialCode[5] = generateRandomDigit();
	specialCode[6] = generateRandomLowercase();
	specialCode[7] = '\0';

	codeIsValid = true;
	saveSpecialCodeToFile();
}

bool Manager::validateSpecialCode(const char* code) const
{
	return codeIsValid && strCompare(specialCode, code);
}

void Manager::saveSpecialCodeToFile() const
{
	char filename[100];
	getSpecialCodeFile(filename, sizeof(filename));

	std::ofstream file(filename);
	if (file.is_open())
	{
		file << specialCode << std::endl;
		file.close();
	}
}

bool Manager::loadSpecialCodeFile()
{
	char filename[100];
	getSpecialCodeFile(filename, sizeof(filename));

	std::ifstream file(filename);
	if (file.is_open()) {
		file.getline(specialCode, sizeof(specialCode));
		file.close();
		codeIsValid = (getStrLength(specialCode) == 7);
		return codeIsValid;
	}

	codeIsValid = false;
	return false;
}

bool Manager::approveCashier(Cashier* cashier, const char* providedCode)
{
	if (!validateSpecialCode(providedCode))
	{
		return false;
	}

	if (!cashier)
	{
		return false;
	}

	cashier->setApproved(true);
	return true;
}

bool Manager::warnCashier(Cashier* cashier, int points, const char* reason, const char* providedCode)
{
	if (!validateSpecialCode(providedCode))
	{
		return false;
	}

	if (!cashier)
	{
		return false;
	}

	if (points != 100 && points != 200 && points != 300)
	{
		return false;
	}

	cashier->addWarning(points, reason);
	return true;
}

bool Manager::promoteCashier(Cashier* cashier, const char* providedCode)
{
	if (!validateSpecialCode(providedCode))
	{
		return false;
	}

	if (!cashier)
	{
		return false;
	}

	if (!cashier->canPromoteToManager())
	{
		return false;
	}

	return true; 
}

bool Manager::fireCashier(Cashier* cashier, const char* providedCode)
{
	if (!validateSpecialCode(providedCode))
	{
		return false;
	}

	if (!cashier)
	{
		return false;
	}

	cashier->setApproved(false);
	return true;
}

void Manager::getSpecialCodeFile(char* filename, int maxSize) const
{
	char idStr[20];
	intToString(id, idStr, sizeof(idStr));

	strCopy(filename, idStr, maxSize);

	int i = 0;
	while (filename[i] != '\0' && i < maxSize - 1)
	{
		i++;
	}

	const char* suffix = "_special_code.txt";
	strCopy(filename + i, suffix, maxSize - i);
}

char Manager::generateRandomDigit()
{
	return '0' + (rand() % 10);
}

char Manager::generateRandomUppercase()
{
	return 'A' + (rand() % 26);
}

char Manager::generateRandomLowercase()
{
	return 'a' + (rand() % 26);
}