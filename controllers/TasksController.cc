#include "TasksController.h"

void TasksController::getTasks(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback){
    auto tasks = taskStorage->getAllTasks();

    Json::Value tasksJson(Json::arrayValue);

    for (auto& task : tasks) {
        Json::Value t;
        t["id"] = task.id;
        t["title"] = task.title;
        t["description"] = task.description;
        t["difficulty"] = task.difficulty;
        tasksJson.append(t);
    }

    Json::Value result;
    result["tasks"] = tasksJson;
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    callback(result_callback);
}

void TasksController::getTask(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback, int taskId){
    auto task = taskStorage->getTaskById(taskId);

    if (!task.has_value()){
        Json::Value result;
        result["error"] = "Task not found";
        auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
        result_callback->setStatusCode(drogon::k404NotFound);
        callback(result_callback);
        return;
    }

    auto& task_res = task.value();
    Json::Value result;
    result["id"] = task_res.id;
    result["title"] = task_res.title;
    result["description"] = task_res.description;
    result["difficulty"] = task_res.difficulty;

    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    callback(result_callback);
}