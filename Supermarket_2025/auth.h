#pragma once

#include "utils.h"

class Worker;
class WorkerManager;

class AuthSystem {
private:
    Worker* currentUser;
    bool isLoggedInV;  
    WorkerManager* workerManager;

public:
    AuthSystem();
    explicit AuthSystem(WorkerManager* mgr);
    ~AuthSystem();

    AuthSystem(const AuthSystem& other) = delete;
    AuthSystem& operator=(const AuthSystem& other) = delete;

    bool login(int worker_id, const char* password);
    void logout();

    bool isLoggedIn() const;
    bool isManager() const;
    bool isCashier() const;
    bool hasPendingApproval() const;

    Worker* getCurrentUser() const;
    int getCurrentUserId() const;
    const char* getCurrentUserRole() const;
    void displayCurrentUserInfo() const;

    bool canExecuteManagerCommand() const;
    bool canExecuteCashierCommand() const;
    bool canExecuteCommand(const char* command) const;

    bool validateManagerCode(const char* special_code) const;

    void setWorkerManager(WorkerManager* mgr);
    WorkerManager* getWorkerManager() const;

    void displayLoginPrompt() const;
    void displayAccessDeniedMessage(const char* command) const;
    void displayNotLoggedInMessage() const;

private:
    void clearCurrentUser();
    bool isValidWorkerId(int id) const;
    bool isValidPassword(const char* password) const;
};

namespace AuthConstants {
    const int MIN_WORKER_ID = 100;
    const int MAX_WORKER_ID = 999999;
    const int MIN_PASSWORD_LENGTH = 6;
    const int MAX_PASSWORD_LENGTH = 50;

    const char* const MANAGER_COMMANDS[] = {
        "approve",
        "decline",
        "promote-cashier",
        "fire-cashier",
        "warn-cashier",
        "add-category",
        "delete-category",
        "add-product",
        "delete-product",
        "load-products",
        "load-gift-cards"
    };

    const int MANAGER_COMMAND_COUNT = 11;

    const char* const COMMON_COMMANDS[] = {
        "list-user-data",
        "list-workers",
        "list-products",
        "list-feed",
        "list-transactions",
        "logout",
        "leave"
    };

    const int COMMON_COMMAND_COUNT = 7;

    const char* const CASHIER_COMMANDS[] = {
        "sell"
    };

    const int CASHIER_COMMAND_COUNT = 1;
}
