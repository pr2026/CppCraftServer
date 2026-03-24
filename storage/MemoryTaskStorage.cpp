#include "storage/MemoryTaskStorage.h"
#include <algorithm>

MemoryTaskStorage::MemoryTaskStorage(){
    Task t1;
    t1.id = nextId_++;
    t1.title = "Hello World";
    t1.description = "Write a program that prints 'Hello, World!'";
    t1.difficulty = "easy";
    t1.tests = {{"", "Hello, World!"}};
    tasks_[t1.id] = t1;

    Task t2;
    t2.id = nextId_++;
    t2.title = "Sum of two numbers";
    t2.description = "Read two integers and print their sum.";
    t2.difficulty = "easy";
    t2.tests = {
        {"2 3", "5"},
        {"10 20", "30"},
        {"-5 5", "0"},
        {"100 200", "300"}
    };
    tasks_[t2.id] = t2;
    
    Task t3;
    t3.id = nextId_++;
    t3.title = "Factorial";
    t3.description = "Compute the factorial of a given number.";
    t3.difficulty = "medium";
    t3.tests = {};
    tasks_[t3.id] = t3;
}

std::vector<Task> MemoryTaskStorage::getAllTasks(){
    std::lock_guard<std::mutex> lock(m);
    std::vector<Task> result;
    for (auto& [id, task]: tasks){
        result.push_back(task);
    }
    std::sort(result.begin(), result.end());
    return result;
}

std::optional<Task> MemoryTaskStorage::getTaskById(int id){
    std::lock_guard<std::mutex> lock(m);
    auto x = tasks.find(id);
    if (x == tasks.end()){
        return std::nullopt;
    }
    return x->second;
}

int MemoryTaskStorage::addTask(const Task& task){
    std::lock_guard<std::mutex> lock(m);
    int new_id = next_id++; // first we assign a value, then we increase next_id
    Task new_task = task;
    new_task.id = new_id;
    tasks[new_id] = new_task;
    return new_id;
}

bool MemoryTaskStorage::updateTask(int id, const Task& task){
    std::lock_guard<std::mutex> lock(m);
    auto x = tasks.find(id);
    if (x == tasks.end()){
        return false;
    }
    // next, we update the fields, except for the id, it does not change
    x->second.title = task.title;
    x->second.description = task.description;
    x->second.difficulty = task.difficulty;
    return true;
}

bool MemoryTaskStorage::deleteTask(int id){
    std::lock_guard<std::mutex> lock(m);
    return tasks.erase(id) > 0;
}