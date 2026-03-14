#include "storage/MemoryTaskStorage.h"
#include <algorithm>

MemoryTaskStorage::MemoryTaskStorage(){
    Task t1{next_id++, "Hello World", "Write a program that prints 'Hello, World!'", "easy"};
    Task t2{next_id++, "Sum of two numbers", "Read two integers and print their sum.", "easy"};
    Task t3{next_id++, "Factorial", "Compute the factorial of a given number.", "medium"};
    tasks[t1.id] = t1;
    tasks[t2.id] = t2;
    tasks[t3.id] = t3;
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