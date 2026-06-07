#pragma once
#include <drogon/drogon.h>
#include <memory>
#include "storage/MemoryTaskStorage.h"
#include "storage/TaskStorage.h"

class TasksController : public drogon::HttpController<TasksController> {
public:
    TasksController() : taskStorage(std::make_unique<MemoryTaskStorage>()) {
    }

    void getTasks(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback
    );
    void getTask(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        int taskId
    );

    void createTask(const drogon::HttpRequestPtr& req,
                std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    void deleteTask(const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        int taskId);

    void updateTask(const drogon::HttpRequestPtr& req,
                std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                int taskId);

    METHOD_LIST_BEGIN
    ADD_METHOD_TO(TasksController::getTasks, "/tasks", drogon::Get);
    ADD_METHOD_TO(TasksController::getTask, "/tasks/{task-id}", drogon::Get);
    ADD_METHOD_TO(TasksController::createTask, "/tasks", drogon::Post);
    ADD_METHOD_TO(TasksController::deleteTask, "/tasks/{task-id}", drogon::Delete);
    ADD_METHOD_TO(TasksController::updateTask, "/tasks/{task-id}", drogon::Put);
    METHOD_LIST_END

private:
    std::unique_ptr<TaskStorage> taskStorage;
};