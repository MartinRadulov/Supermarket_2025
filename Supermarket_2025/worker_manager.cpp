#include "worker_manager.h"
#include "workers.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

WorkerManager::WorkerManager()
{
    workers = nullptr;
    workerCount = 0;
    workerCapacity = 0;
    srand(time(nullptr));
}

WorkerManager::~WorkerManager()
{
    clearWorkers();
}

bool WorkerManager::addWorker(Worker* worker)
{
    if (!worker)
    {
        return false;
    }

    if (workerCount >= workerCapacity)
    {
        resizeWorkersArray();
    }

    workers[workerCount] = worker;
    workerCount++;
    return true;
}

Worker* WorkerManager::findWorkerById(int id)
{
    for (int i = 0; i < workerCount; i++)
    {
        if (workers[i] && workers[i]->getId() == id)
        {
            return workers[i];
        }
    }
    return nullptr;
}

bool WorkerManager::removeWorker(int id) {
    for (int i = 0; i < workerCount; i++) {
        if (workers[i] && workers[i]->getId() == id) {
            delete workers[i];
            for (int j = i; j < workerCount - 1; j++) {
                workers[j] = workers[j + 1];
            }

            workerCount--;
            return true;
        }
    }
    return false;
}

Worker* WorkerManager::authenticateWorker(int id, const char* password)
{
    Worker* worker = findWorkerById(id);
    if (worker && worker->authenticate(password) && worker->isApproved())
    {
        worker->setLoggedIn(true);
        return worker;
    }
    return nullptr;
}

bool WorkerManager::approveWorker(int id, int managerId, const char* specialCode)
{
    Worker* worker = findWorkerById(id);
    Worker* manager = findWorkerById(managerId);

    if (!worker || !manager)
    {
        return false;
    }

    Manager* mgr = dynamic_cast<Manager*>(manager);
    if (!mgr || !mgr->validateSpecialCode(specialCode)) return false;

    worker->setApproved(true);
    return true;
}

void WorkerManager::saveAllWorkers(const char* filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << workerCount << "\n";

    for (int i = 0; i < workerCount; i++)
    {
        if (workers[i])
        {
            workers[i]->saveToFile(file);
        }
    }

    file.close();
}

void WorkerManager::loadAllWorkers(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return;

    clearWorkers();

    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++)
    {
        char role[20];
        file.getline(role, sizeof(role));

        Worker* worker = nullptr;
        if (strCompare(role, "CASHIER"))
        {
            worker = new Cashier();
        }
        else if (strCompare(role, "MANAGER"))
        {
            worker = new Manager();
        }

        if (worker)
        {
            worker->loadFromFile(file);
            addWorker(worker);
        }
    }

    file.close();
}

void WorkerManager::listAllWorkers() const
{
    std::cout << "=== ALL WORKERS ===" << std::endl;
    for (int i = 0; i < workerCount; i++)
    {
        if (workers[i])
        {
            workers[i]->displayInfo();
            std::cout << std::endl;
        }
    }
}

void WorkerManager::listPendingWorkers() const
{
    std::cout << "PENDING APPROVAL" << std::endl;
    bool foundPending = false;

    for (int i = 0; i < workerCount; i++)
    {
        if (workers[i] && !workers[i]->isApproved())
        {
            workers[i]->displayInfo();
            std::cout << std::endl;
            foundPending = true;
        }
    }

    if (!foundPending)
    {
        std::cout << "No workers pending approval." << std::endl;
    }
}

void WorkerManager::resizeWorkersArray()
{
    int newCapacity = (workerCapacity == 0) ? 2 : workerCapacity * 2;
    Worker** newWorkers = new Worker * [newCapacity];

    for (int i = 0; i < workerCount; i++)
    {
        newWorkers[i] = workers[i];
    }

    delete[] workers;
    workers = newWorkers;
    workerCapacity = newCapacity;
}

void WorkerManager::clearWorkers()
{
    for (int i = 0; i < workerCount; i++)
    {
        delete workers[i];
    }
    delete[] workers;
    workers = nullptr;
    workerCount = 0;
    workerCapacity = 0;
}