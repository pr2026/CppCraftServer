#pragma once
#include <mutex>
#include <unordered_map>
#include "TaskStorage.h"

class MemoryTaskStorage : public TaskStorage {
public:
    MemoryTaskStorage();  // constructor

    std::vector<Task> getAllTasks() override;
    std::optional<Task> getTaskById(int id) override;
    bool updateTask(int id, const Task &task) override; // for teachers
    bool deleteTask(int id) override; // for teachers
    int addTask(const Task& task); // for teachers

private:
    std::unordered_map<int, Task> tasks;
    std::mutex m;
    int next_id = 1;  // for generation id
};