﻿#include "auth.h"
#include "workers.h"
#include "worker_manager.h"
#include <iostream>

AuthSystem::AuthSystem()
    : currentUser(nullptr), isLoggedInV(false), workerManager(nullptr)
{

}

AuthSystem::AuthSystem(WorkerManager* mgr)
    : currentUser(nullptr), isLoggedInV(false), workerManager(mgr)
{

}

AuthSystem::~AuthSystem()
{
    clearCurrentUser();
}

bool AuthSystem::login(int worker_id, const char* password)
{
    if (!isValidWorkerId(worker_id) || !isValidPassword(password))
    {
        std::cout << "Invalid login credentials format." << std::endl;
        return false;
    }

    if (isLoggedInV) 
    {
        std::cout << "A user is already logged in. Please logout first." << std::endl;
        return false;
    }

    if (!workerManager) 
    {
        std::cout << "Authentication system not properly initialized." << std::endl;
        return false;
    }

    Worker* foundWorker = workerManager->findWorkerById(worker_id);
    if (!foundWorker) 
    {
        std::cout << "Invalid login credentials." << std::endl;
        return false;
    }

    if (!foundWorker->authenticate(password))
    {
        std::cout << "Invalid login credentials." << std::endl;
        return false;
    }

    Worker* authenticatedWorker = foundWorker;

    if (strCompare(authenticatedWorker->getRole(), "Cashier"))
    {
        Cashier* cashier = static_cast<Cashier*>(authenticatedWorker);
        if (!cashier->isApproved())
        {
            std::cout << "Cashier registration is pending manager approval." << std::endl;
            return false;
        }
    }

    currentUser = authenticatedWorker;
    isLoggedInV = true;

    char nameBuffer[100];
    authenticatedWorker->getFullName(nameBuffer, sizeof(nameBuffer));
    std::cout << "User " << nameBuffer << " with ID: " << worker_id
        << " has been logged into the system!" << std::endl;

    return true;
}

void AuthSystem::logout()
{
    if (!isLoggedInV || !currentUser) 
    {
        std::cout << "No user is currently logged in." << std::endl;
        return;
    }

    char nameBuffer[100];
    currentUser->getFullName(nameBuffer, sizeof(nameBuffer));
    std::cout << "User " << nameBuffer << " has been logged out." << std::endl;

    clearCurrentUser();
}

bool AuthSystem::isLoggedIn() const 
{
    return isLoggedInV && currentUser != nullptr;
}

bool AuthSystem::isManager() const
{
    if (!isLoggedIn())
    {
        return false;
    }
    return strCompare(currentUser->getRole(), "Manager");
}

bool AuthSystem::isCashier() const 
{
    if (!isLoggedIn()) 
    {
        return false;
    }
    return strCompare(currentUser->getRole(), "Cashier");
}

bool AuthSystem::hasPendingApproval() const 
{
    if (!isCashier())
    {
        return false;
    }

    Cashier* cashier = static_cast<Cashier*>(currentUser);
    return !cashier->isApproved();
}

Worker* AuthSystem::getCurrentUser() const
{
    return currentUser;
}

int AuthSystem::getCurrentUserId() const
{
    if (!isLoggedIn())
    {
        return -1;
    }
    return currentUser->getId();
}

const char* AuthSystem::getCurrentUserRole() const
{
    if (!isLoggedIn()) 
    {
        return "Not logged in";
    }
    return currentUser->getRole();
}

void AuthSystem::displayCurrentUserInfo() const
{
    if (!isLoggedIn())
    {
        std::cout << "No user is currently logged in." << std::endl;
        return;
    }

    currentUser->displayInfo();
}

bool AuthSystem::canExecuteManagerCommand() const
{
    return isManager();
}

bool AuthSystem::canExecuteCashierCommand() const
{
    return isCashier() && !hasPendingApproval();
}

bool AuthSystem::canExecuteCommand(const char* command) const
{
    if (!isLoggedIn())
    {
        return false;
    }

    if (isManager())
    {
        if (strCompare(command, "approve") || strCompare(command, "decline") ||
            strCompare(command, "promote-cashier") || strCompare(command, "fire-cashier") ||
            strCompare(command, "warn-cashier") || strCompare(command, "add-category") ||
            strCompare(command, "delete-category") || strCompare(command, "add-product") ||
            strCompare(command, "delete-product") || strCompare(command, "load-products") ||
            strCompare(command, "load-gift-cards") || strCompare(command, "create-single-voucher") ||
            strCompare(command, "create-multiple-voucher") || strCompare(command, "create-all-voucher") ||
            strCompare(command, "save-workers") || strCompare(command, "load-workers") ||
            strCompare(command, "save-vouchers"))
        {
            return true;
        }
    }

    if (isCashier() && !hasPendingApproval())
    {
        if (strCompare(command, "sell"))
        {
            return true;
        }
    }

    if (strCompare(command, "list-user-data") || strCompare(command, "list-workers") ||
        strCompare(command, "list-products") || strCompare(command, "list-feed") ||
        strCompare(command, "list-transactions") || strCompare(command, "list-vouchers") ||
        strCompare(command, "logout") || strCompare(command, "leave"))
    {
        return true;
    }

    return false;
}


bool AuthSystem::validateManagerCode(const char* special_code) const
{
    if (!isManager())
    {
        std::cout << "Only managers can use special authentication codes." << std::endl;
        return false;
    }

    Manager* manager = static_cast<Manager*>(currentUser);
    return manager->validateSpecialCode(special_code);
}

void AuthSystem::setWorkerManager(WorkerManager* mgr) 
{
    workerManager = mgr;
}

WorkerManager* AuthSystem::getWorkerManager() const
{
    return workerManager;
}

void AuthSystem::displayLoginPrompt() const 
{
    std::cout << std::endl;
    std::cout << "=== FMI Supermarket Management System ===" << std::endl;
    std::cout << "Please login to continue." << std::endl;
    std::cout << "Usage: login <worker_id> <password>" << std::endl;
    std::cout << std::endl;
}

void AuthSystem::displayAccessDeniedMessage(const char* command) const
{
    std::cout << "Access denied. ";

    if (!isLoggedIn())
    {
        std::cout << "Please login first." << std::endl;
        return;
    }

    if (isCashier() && hasPendingApproval()) 
    {
        std::cout << "Cashier account is pending manager approval." << std::endl;
        return;
    }

    for (int i = 0; i < AuthConstants::MANAGER_COMMAND_COUNT; i++) 
    {
        if (strCompare(command, AuthConstants::MANAGER_COMMANDS[i]))
        {
            std::cout << "Manager privileges required for command: " << command << std::endl;
            return;
        }
    }

    for (int i = 0; i < AuthConstants::CASHIER_COMMAND_COUNT; i++) 
    {
        if (strCompare(command, AuthConstants::CASHIER_COMMANDS[i]))
        {
            std::cout << "Cashier privileges required for command: " << command << std::endl;
            return;
        }
    }

    std::cout << "Command not recognized or insufficient privileges." << std::endl;
}

void AuthSystem::displayNotLoggedInMessage() const 
{
    std::cout << "Please login first to access system features." << std::endl;
    std::cout << "Usage: login <worker_id> <password>" << std::endl;
}

void AuthSystem::clearCurrentUser() 
{
    currentUser = nullptr;
    isLoggedInV = false;
}

bool AuthSystem::isValidWorkerId(int id) const
{
    return id >= AuthConstants::MIN_WORKER_ID && id <= AuthConstants::MAX_WORKER_ID;
}

bool AuthSystem::isValidPassword(const char* password) const
{
    if (!password) 
    {
        return false;
    }

    int length = getStrLength(password);
    return length >= AuthConstants::MIN_PASSWORD_LENGTH &&
        length <= AuthConstants::MAX_PASSWORD_LENGTH;
}