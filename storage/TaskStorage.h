#pragma once
#include <string>
#include <vector>
#include <optional>

struct Task{
    int id;
    std::string title;
    std::string description;
    std::string difficulty;

    bool operator<(const Task& other) const { return id < other.id; }
};

class TaskStorage {
public:
    virtual ~TaskStorage() = default;
    virtual std::vector<Task> getAllTasks() = 0;
    virtual std::optional<Task> getTaskById(int id) = 0;
    virtual int addTask(const Task& task) = 0;
    virtual bool updateTask(int id, const Task& task) = 0;
    virtual bool deleteTask(int id) = 0;
};