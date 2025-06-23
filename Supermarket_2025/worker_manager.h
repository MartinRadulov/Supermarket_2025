#pragma once

#include <fstream>

class Worker;
class Manager;
class Cashier;

class WorkerManager
{
private:
	Worker** workers;
	int workerCount;
	int workerCapacity;

public:
	WorkerManager();
	~WorkerManager();

	bool addWorker(Worker* worker);
	Worker* findWorkerById(int id);
	bool removeWorker(int id);

	Worker* authenticateWorker(int id, const char* password);
	bool approveWorker(int id, int managerId, const char* specialCode);

	void saveAllWorkers(const char* filename) const;
	void loadAllWorkers(const char* filename);

	void listAllWorkers() const;
	void listPendingWorkers() const;

	int getWorkerCount() const
	{
		return workerCount;
	}

private:
	void resizeWorkersArray();
	void clearWorkers();
};