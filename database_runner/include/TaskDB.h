#pragma once
#include <optional>
#include <vector>
#include "Database.h"
#include "storage/TaskStorage.h"

class TaskDB : public Database, public TaskStorage {
public:
    using Database::Database;

    std::vector<Task> getAllTasks() override;
    std::optional<Task> getTaskById(int id) override;

    bool updateTask(int id, const Task &task) override;
    bool deleteTask(int id) override;

    int addTask(
        const std::string &title,
        const std::string &description,
        const std::string &difficulty = "easy"
    ) ;
    std::optional<Task> get_task_by_id(int id);
};