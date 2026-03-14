#pragma once
#include <drogon/drogon.h>
#include "storage/TaskStorage.h"
#include "storage/MemoryTaskStorage.h"
#include <memory>

class TasksController: public drogon::HttpController<TasksController>
{
  public:
    TasksController() : taskStorage(std::make_unique<MemoryTaskStorage>()) {}
    void getTasks(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback);

    METHOD_LIST_BEGIN
      ADD_METHOD_TO(TasksController::getTasks, "/tasks", drogon::Get);
    METHOD_LIST_END

  private:
    std::unique_ptr<TaskStorage> taskStorage;
};