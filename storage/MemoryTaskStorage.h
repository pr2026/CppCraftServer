#pragma once
#include <mutex>
#include <unordered_map>
#include "TaskStorage.h"

class MemoryTaskStorage : public TaskStorage {
public:
    MemoryTaskStorage();

    std::vector<Task> getAllTasks() override;
    std::optional<Task> getTaskById(int id) override;
    bool updateTask(int id, const Task &task, int user_id) override;
    bool deleteTask(int id, int user_id) override;
    int addTask(const Task& task);
    bool addTest(int taskId, const std::string& input, const std::string& expected_output);

private:
    std::unordered_map<int, Task> tasks;
    std::mutex m;
    int next_id = 1;
};