#pragma once
#include "TaskStorage.h"
#include <unordered_map>
#include <mutex>

class MemoryTaskStorage : public TaskStorage {
public:
    MemoryTaskStorage(); // constructor

    std::vector<Task> getAllTasks() override;
    std::optional<Task> getTaskById(int id) override;
    int addTask(const Task& task) override;
    bool updateTask(int id, const Task& task) override;
    bool deleteTask(int id) override;

private:
    std::unordered_map<int, Task> tasks;
    std::mutex m;
    int next_id = 1; // for generation id
};