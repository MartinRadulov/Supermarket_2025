#pragma once

#include "utils.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

class Warning;

class Worker
{
protected:
	int id;
	char firstName[50];
	char lastName[50];
	char phone[20];
	int age;
	char password[50];
	bool isApprovedV;
	bool isLoggedInV;

public:
	Worker();
	Worker(int workerId, const char* fName, const char* lName, const char* phoneNum, int workerAge, const char* pwd);

	virtual ~Worker();

	virtual const char* getRole() const = 0;
	virtual void displayInfo() const = 0;
	virtual bool hasSpecialPrivileges() const = 0;
	
	virtual bool authenticate(const char* password) const;
	virtual void saveToFile(std::ofstream& file) const;
	virtual void loadFromFile(std::ifstream& file);

	int getId() const
	{
		return id;
	}
	const char* getFirstName() const
	{
		return firstName;
	}
	const char* getLastName() const
	{
		return lastName;
	}
	const char* getPhone() const
	{
		return firstName;
	}
	int getAge() const
	{
		return age;
	}
	const char* getPassword() const
	{
		return password;
	}
	bool isApproved() const
	{
		return isApproved;
	}
	bool isLoggedIn() const
	{
		return isLoggedIn;
	}

	void setApproved(bool approved)
	{
		isApprovedV = approved;
	}
	void setLoggedIn(bool loggedIn)
	{
		isLoggedInV = loggedIn;
	}
	void setPassword(const char* newPassword);

	void getFullName(char* fullName, int maxSize) const;
	static int generateNewId();
};

class Warning
{
private:
	int points;
	char reason[200];
	char date[20];

public:
	Warning();
	Warning(int warningPoints, const char* warningReason);

	int getPoints() const
	{
		return points;
	}
	const char* getReason() const
	{
		return reason;
	}
	const char* getDate() const
	{
		return date;
	}

	void saveToFile(std::ofstream&) const;
	void loadFromFile(std::ifstream&);
};

class Cashier : public Worker
{
private:
	int transactionCount;
	Warning* warnings;
	int warningCount;
	int warningCapacity;
	int totalWarningPoints;

public:
	Cashier();
	Cashier(int workerId, const char* fName, const char* lName, const char* phoneNum, int workerAge, const char* psw);

	virtual ~Cashier();

	Cashier(const Cashier& other);
	Cashier& operator=(const Cashier& other);
	Cashier(Cashier&& other) noexcept;
	Cashier& operator=(Cashier&& other) noexcept;

	virtual const char* getRole() const override;
	virtual void displayInfo() const override;
	virtual bool hasSpecialPrivileges() const override;
	virtual void saveToFile(std::ofstream& file) const override;
	virtual void loadFromFile(std::ifstream& file) override;

	void addWarning(int points, const char* reason);
	void incrementTransactionCount()
	{
		transactionCount++;
	}
	bool canPromoteToManager() const;
	bool shouldBeFired() const;

	int getTransactionPoints() const
	{
		return transactionCount;
	}
	const Warning* getWarnings() const
	{
		return warnings;
	}
	int getWarningCount() const
	{
		return warningCount;
	}
	int getWarningPoints() const
	{
		return totalWarningPoints;
	}

private:
	void resizeWarningArray();
	void copyWarnings(const Cashier& other);
	void clearWarnings();
};

class Manager : public Worker
{
private:
	char specialCode[8];
	bool codeIsValid;

public:
	Manager();
	Manager(int workerId, const char* fName, const char* lName, const char* phoneNum, int workerAge, const char* pwd);

	virtual ~Manager();

	virtual const char* getRole() const override;
	virtual void displayInfo() const override;
	virtual bool hasSpecialPrivileges() const override;
	virtual void saveToFile(std::ofstream& file) const override;
	virtual void loadFromFile(std::ifstream& file) override;

	void generateSpecialCode();
	bool validateSpecialCode(const char* code) const;
	void saveSpecialCode() const;
	bool loadSpecialCode();

	bool approveCashier(Cashier* cashier, const char* providedCode);
	bool warnCashier(Cashier* cashier, int points, const char* reason, const char* providedCode);;
	bool promoteCashier(Cashier* chasier, const char* providedCode);
	bool fireCashier(Cashier* chasier, const char* providedCode);

	const char* getSpecialCode() const
	{
		return specialCode;
	}
	bool hasValidCode() const {
		return codeIsValid;
	}

private:
	void getSpecialCodeFile(char* filename, int maxSize) const;
	static char generateRandomDigit();
	static char generateRandomLowercase();
	static char generateRandomUppercase();
	void saveSpecialCodeToFile() const;
	bool loadSpecialCodeFile();
};
//class WorkerManager
//{
//private:
//	Worker** workers;
//	int workerCount;
//	int workerCapacity;
//
//public:
//	WorkerManager();
//	~WorkerManager();
//
//	bool addWoker(Worker* worker);
//	Worker* findWorkerById(int id);
//	bool removeWorker(int id);
//
//	Worker authenticateWorker(int id, const char* password);
//	bool approveWorker(int id, int managerId, const char* specialCode);
//
//	void saveAllWorkers(const char* filename) const;
//	void loadAllWorkerks(const char* filename);
//
//	void listAllWorkers() const;
//	void listPendingWorkers() const;
//
//	int getWorkerCount() const
//	{
//		return workerCount;
//	}
//
//private:
//	void resizeWorkersArray();
//	void clearWorkers();
//};

