#pragma once
#include <optional>
#include <vector>
#include "Database.h"
#include "Test.h"
#include "storage/TaskStorage.h"

class TaskDB : public Database, public TaskStorage {
public:
    using Database::Database;

    std::vector<Task> getAllTasks() override;
    std::optional<Task> getTaskById(int id) override;

    bool updateTask(int id, const Task &task, int user_id) override;
    bool deleteTask(int id, int user_id) override;

    int addTask(const Task &task) override;
    int addTask(
        const std::string &title,
        const std::string &description,
        int created_by,
        const std::string &difficulty = "easy"
    );
    std::vector<Test> getTestsForTask(int task_id);
    bool addTest(int task_id, const std::string &input, const std::string &expected) override;
    std::optional<int> getTaskOwner(int task_id);
};